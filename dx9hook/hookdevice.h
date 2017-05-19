#pragma once
#include "d3d9.h"
#include "d3dx9.h"
#include <future> 


extern d3d9hook* hook;

void hookDevice();

bool vaildTech(int tech)
{
	return tech == 1 || tech == 2;
}


static HRESULT WINAPI BeginStateBlock(IDirect3DDevice9 *device)
{
	HRESULT res = hook->deviceVirtualFunctions.BeginStateBlock(device);
	hookDevice();
	return res;
}

static HRESULT WINAPI EndStateBlock(IDirect3DDevice9 *device, IDirect3DStateBlock9 **ppSB)
{
	HRESULT res = hook->deviceVirtualFunctions.EndStateBlock(device, ppSB);
	hookDevice();
	return res;
}

static HRESULT WINAPI Present(
	IDirect3DDevice9 *device,
	const RECT * pSourceRect,
	const RECT * pDestRect,
	HWND hDestWindowOverride,
	const RGNDATA * pDirtyRegion)
{
	HRESULT res = hook->deviceVirtualFunctions.Present(device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	return res;
}

namespace d3dx9hookFunc
{
	HRESULT WINAPI D3DXCreateTextureFromFileExW(
		LPDIRECT3DDEVICE9		 pDevice,
		LPCWSTR				pSrcFile,
		UINT				   Width,
		UINT				   Height,
		UINT				   MipLevels,
		DWORD				  Usage,
		D3DFORMAT				 Format,
		D3DPOOL				Pool,
		DWORD				  Filter,
		DWORD				  MipFilter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo,
		PALETTEENTRY*		  pPalette,
		LPDIRECT3DTEXTURE9*       ppTexture)
	{
		using funcType = std::add_pointer<decltype(D3DXCreateTextureFromFileExW)>::type;
		auto oFunc =
			reinterpret_cast<funcType>(hook->d3dx9->oriFunction(__func__));

		HRESULT res =  oFunc(pDevice, pSrcFile, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo,
			pPalette, ppTexture);
		hook->mmf.updateTexture(*ppTexture, pSrcFile);
		return res;
	}
}

static HRESULT WINAPI CreateDevice(
	IDirect3D9 *direct3d,
	UINT adapter,
	D3DDEVTYPE type,
	HWND window,
	DWORD flag,
	D3DPRESENT_PARAMETERS *param,
	IDirect3DDevice9 **device)
{
	HRESULT res = (*(hook->mCreateDevice))(direct3d, adapter, type, window, flag, param, device);
	hook->device = (*device);

	if (res >= 0) {
		hook->bakupDevice();
	}
	hookDevice();
	hook->havestMMDExp();
	hook->d3dx9->hookFunction("D3DXCreateTextureFromFileExW", (void*)d3dx9hookFunc::D3DXCreateTextureFromFileExW);
	return res;
}


static HRESULT WINAPI SetTexture(
	IDirect3DDevice9* device,
	DWORD sampler,
	IDirect3DBaseTexture9 * pTexture)
{

	if (sampler == 1) {
		int currentTech = hook->mmd.ExpGetCurrentTechnic();
		int currentObj = hook->mmd.ExpGetCurrentObject();
		if (vaildTech(currentTech) && hook->mmf.renderType(currentObj) == RenderModelType::PMD)
		{
			IDirect3DTexture9* texture = reinterpret_cast<IDirect3DTexture9*>(pTexture);
			hook->mmf.updateMaterial(texture);
		}
	}

	return hook->deviceVirtualFunctions.SetTexture(device, sampler, pTexture);
}

static HRESULT WINAPI DrawIndexedPrimitive(
	IDirect3DDevice9 *device,
	D3DPRIMITIVETYPE type,
	INT baseVertexIndex,
	UINT minIndex,
	UINT numVertices,
	UINT startIndex,
	UINT primitiveCount)
{
	int currentTech = hook->mmd.ExpGetCurrentTechnic();
	int currentObj = hook->mmd.ExpGetCurrentObject();
	if (vaildTech(currentTech) && hook->mmf.renderType(currentObj) == RenderModelType::PMD)
	{
		hook->mmf.updateModel(currentObj);
	}
	return hook->deviceVirtualFunctions.DrawIndexedPrimitive(device, type,
		baseVertexIndex, minIndex, numVertices, startIndex, primitiveCount);
}


static HRESULT WINAPI CreateVertexBuffer(
	IDirect3DDevice9* device,
	UINT length,
	DWORD usage,
	DWORD fvf,
	D3DPOOL pool,
	IDirect3DVertexBuffer9** ppVertexBuffer,
	HANDLE* pHandle)
{
	int currentTech = hook->mmd.ExpGetCurrentTechnic();
	int currentObj = hook->mmd.ExpGetCurrentObject();
	if (vaildTech(currentTech) && hook->mmf.renderType(currentObj) == RenderModelType::PMD)
	{
		hook->mmf.updateVertexBuffer(ppVertexBuffer, length);
	}
	HRESULT res = hook->deviceVirtualFunctions.CreateVertexBuffer(device, length, usage, fvf, pool, ppVertexBuffer, pHandle);

	return res;
}


static HRESULT WINAPI SetStreamSource(
	IDirect3DDevice9 *device,
	UINT streamNumber,
	IDirect3DVertexBuffer9 *pStreamData,
	UINT offsetInBytes,
	UINT stride)
{
	int currentTech = hook->mmd.ExpGetCurrentTechnic();
	int currentObj = hook->mmd.ExpGetCurrentObject();
	if (vaildTech(currentTech) && hook->mmf.renderType(currentObj) == RenderModelType::PMD)
	{
		hook->mmf.updateVertexStream(pStreamData, stride);
	}
	HRESULT res = hook->deviceVirtualFunctions.SetStreamSource(device, streamNumber, pStreamData, offsetInBytes, stride);
	
	return res;
}


static HRESULT WINAPI SetFVF(IDirect3DDevice9 *device, DWORD fvf)
{
	hook->mmf.updateFVF(fvf);
	HRESULT res = hook->deviceVirtualFunctions.SetFVF(device, fvf);
	return res;
}

static HRESULT WINAPI BeginScene(IDirect3DDevice9 *device)
{
	hook->update();
	HRESULT res = hook->deviceVirtualFunctions.BeginScene(device);
	return res;
}

void hookDevice()
{
	hook->device->lpVtbl->BeginScene = BeginScene;
	hook->device->lpVtbl->Present = Present;
	hook->device->lpVtbl->BeginStateBlock = BeginStateBlock;
	hook->device->lpVtbl->EndStateBlock = EndStateBlock;
	hook->device->lpVtbl->SetTexture = SetTexture;
	hook->device->lpVtbl->SetStreamSource = SetStreamSource;
	hook->device->lpVtbl->CreateVertexBuffer = CreateVertexBuffer;
	hook->device->lpVtbl->SetFVF = SetFVF;
	hook->device->lpVtbl->DrawIndexedPrimitive = DrawIndexedPrimitive;

}