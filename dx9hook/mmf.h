#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include "d3d9.h"
#include "d3dx9.h"
#include "MMDFormats/MikuMikuFormats/pmx.h"

enum RenderModelType
{
	PMD,
	ACS
};

class VertexAgent 
{
public:
	VertexAgent(void* _stream,UINT _length,UINT _stride)
	{
		length = _length;
		stride = _stride;

		stream = new BYTE[length];
		memcpy(stream, _stream, length);
	}  
	~VertexAgent()
	{
		delete stream; 
	}
	void* stream;
	UINT length;
	UINT stride;
};

namespace 
{
	using namespace std;
	struct Texture
	{
		UINT width;
		UINT height;
		UINT levels;
		D3DFORMAT format;
		IDirect3DTexture9* abledo;
	};

	struct Material
	{
		wstring name;
		wstring texturePath;
		IDirect3DTexture9* abledo;
		shared_ptr<VertexAgent> vertex;
		DWORD fvf;
	};

	struct Model
	{
		string name;
		vector<Material> mats;
	};

	class MMF
	{
	private:
		using modTuple = tuple<Model, int/*no*/, int/*order*/>;
		unordered_map<int,modTuple> mods;
		struct
		{
			DWORD fvf;
			IDirect3DTexture9* currentTex;
			unordered_map<IDirect3DVertexBuffer9*, UINT> vertexBufferLengthMap;
			shared_ptr<VertexAgent> vertex = 0;
		}renderStatus;
		
		struct modTupleHasher : public std::unary_function<modTuple, std::size_t>
		{
			std::size_t operator()(const modTuple& k) const
			{
				return hash<string>()(get<0>(k).name) ^ hash<int>()(get<1>(k));
			}
		};
		unordered_map<modTuple, string, modTupleHasher> effectMap;
		unordered_map<IDirect3DTexture9*, wstring> textureNameMap;
		unordered_map<string, vector<wstring>> modelMaterialNamesMap;
		vector<int> modelOrderLst;

		void loadModelMaterialNames(string modelName)
		{
			//TODO:检查pmx是否被修改
			auto it = modelMaterialNamesMap.find(modelName);
			if (it == modelMaterialNamesMap.end())
			{
				pmx::PmxModel model;
				std::ifstream stream = std::ifstream(modelName, std::ios_base::binary);
				model.Read(&stream);
				stream.close();
				vector<wstring> materialNames;
				materialNames.reserve(model.material_count);
				for (int i = 0; i < model.material_count; i++)
				{
					materialNames.push_back(model.materials[i].material_name);
				}
				modelMaterialNamesMap[modelName] = materialNames;
			}
		}
	public:
		RenderModelType renderType(int currentObject)
		{
			bool isModel = std::find(modelOrderLst.begin(), modelOrderLst.end(), currentObject) != modelOrderLst.end();
			if(isModel)
				return RenderModelType::PMD;
			else
				return RenderModelType::ACS;
		}
		void updateFrame(int modelNums, int* modelOrder,int* modelMatNums, vector<string> modelNames)
		{
			modelOrderLst.clear();
			mods.clear();

			renderStatus.vertexBufferLengthMap.clear();

			for (int i = 0; i < modelNums; i++)
			{
				int order = modelOrder[i];
				modelOrderLst.push_back(order);
				auto& mod = mods[order];
				loadModelMaterialNames(modelNames[i]);
				get<0>(mod).mats.reserve(modelMatNums[i]);
				get<0>(mod).name = modelNames[i];
				get<1>(mod) = i;
			}
		}
		void updateModel(int modelOrder)
		{
			auto& model = mods[modelOrder];
			auto it = textureNameMap.find(renderStatus.currentTex);
			auto mat = modelMaterialNamesMap[get<0>(model).name];
			auto matName = mat[get<0>(model).mats.size()];
			get<0>(model).mats.push_back(
			{
				matName,
				it == textureNameMap.end() ? wstring() : it->second,
				renderStatus.currentTex,
				renderStatus.vertex,
				renderStatus.fvf
			});
			get<2>(model) = modelOrder;

			renderStatus.currentTex = 0;
			renderStatus.fvf = 0;
		}
		void updateMaterial(IDirect3DTexture9* albedo)
		{
			renderStatus.currentTex = albedo;
		}
		void updateTexture(IDirect3DTexture9* albedo, wstring name)
		{
			textureNameMap[albedo] = name;
		}
		void updateVertexBuffer(IDirect3DVertexBuffer9** ppVertexBuffer, UINT length)
		{
			renderStatus.vertexBufferLengthMap[*ppVertexBuffer] = length;
		}
		void updateVertexStream(IDirect3DVertexBuffer9* pStreamData, UINT stride)
		{
			UINT length = renderStatus.vertexBufferLengthMap[pStreamData];
			void* buffer;
			pStreamData->lpVtbl->Lock(pStreamData, 0, 0, &buffer, D3DLOCK_READONLY);

			auto vertex = make_shared<VertexAgent>(buffer, length, stride);

			renderStatus.vertex = vertex;

			pStreamData->lpVtbl->Unlock(pStreamData);
		}
		void updateFVF(DWORD fvf)
		{
			renderStatus.fvf = fvf;
		}
	};
}
