#pragma once

namespace mmdExportType
{
	using FLOAT_VOID = float(*)(void);
	using INT_VOID = int(*)(void);
	using LPCHAR_INT = char*(*)(int);
	using INT_INT = int(*)(int);
	using INT_INTINT = int(*)(int,int);
	using D3DMATERIAL9_INTINT = D3DMATERIAL9(*)(int,int);
	using LPCHAR_INTINT = char*(*)(int,int);
	using D3DMATRIX_INTINT = D3DMATRIX(*)(int, int);
	using FLOAT_INTINT = float(*)(int, int);
	using BOOL_INT = bool(*)(int);
	using D3DMATRIX_INT = D3DMATRIX(*)(int);
	using VOID_INT = void(*)(int);
	using FLOAT_INT = float(*)(int);
};

struct MMDEXP{
	mmdExportType::FLOAT_VOID ExpGetFrameTime;
	mmdExportType::INT_VOID ExpGetPmdNum;
	mmdExportType::LPCHAR_INT ExpGetPmdFilename;
	mmdExportType::INT_INT ExpGetPmdOrder;
	mmdExportType::INT_INT ExpGetPmdMatNum;
	mmdExportType::D3DMATERIAL9_INTINT ExpGetPmdMaterial;
	mmdExportType::INT_INT ExpGetPmdBoneNum;
	mmdExportType::LPCHAR_INTINT ExpGetPmdBoneName;
	mmdExportType::D3DMATRIX_INTINT ExpGetPmdBoneWorldMat;
	mmdExportType::INT_INT ExpGetPmdMorphNum;
	mmdExportType::LPCHAR_INTINT ExpGetPmdMorphName;
	mmdExportType::FLOAT_INTINT ExpGetPmdMorphValue;
	mmdExportType::BOOL_INT ExpGetPmdDisp;
	mmdExportType::INT_INT ExpGetPmdID;
	mmdExportType::INT_VOID ExpGetAcsNum;
	mmdExportType::INT_VOID ExpGetPreAcsNum;
	mmdExportType::LPCHAR_INT ExpGetAcsFilename;
	mmdExportType::INT_INT ExpGetAcsOrder;
	mmdExportType::D3DMATRIX_INT ExpGetAcsWorldMat;
	mmdExportType::FLOAT_INT ExpGetAcsX;
	mmdExportType::FLOAT_INT ExpGetAcsY;
	mmdExportType::FLOAT_INT ExpGetAcsZ;
	mmdExportType::FLOAT_INT ExpGetAcsRx;
	mmdExportType::FLOAT_INT ExpGetAcsRy;
	mmdExportType::FLOAT_INT ExpGetAcsRz;
	mmdExportType::FLOAT_INT ExpGetAcsSi;
	mmdExportType::FLOAT_INT ExpGetAcsTr;
	mmdExportType::BOOL_INT ExpGetAcsDisp;
	mmdExportType::INT_INT ExpGetAcsID;
	mmdExportType::INT_INT ExpGetAcsMatNum;
	mmdExportType::D3DMATERIAL9_INTINT ExpGetAcsMaterial;
	mmdExportType::INT_VOID ExpGetCurrentObject;
	mmdExportType::INT_VOID ExpGetCurrentMaterial;
	mmdExportType::INT_VOID ExpGetCurrentTechnic;
	mmdExportType::VOID_INT ExpSetRenderRepeatCount;
	mmdExportType::INT_VOID ExpGetRenderRepeatCount;
};


MMDEXP initMMDEXP(HMODULE mmd);