#define CINTERFACE
#define NOMINMAX 
#include "d3d9.h"
#include "d3dx9.h"
#include "mmdexport.h"

MMDEXP initMMDEXP(HMODULE mmd)
{
	MMDEXP exp;
	exp.ExpGetFrameTime = reinterpret_cast<mmdExportType::FLOAT_VOID>(GetProcAddress(mmd, "ExpGetFrameTime"));
	exp.ExpGetPmdNum = reinterpret_cast<mmdExportType::INT_VOID>(GetProcAddress(mmd, "ExpGetPmdNum"));
	exp.ExpGetPmdFilename = reinterpret_cast<mmdExportType::LPCHAR_INT>(GetProcAddress(mmd, "ExpGetPmdFilename"));
	exp.ExpGetPmdOrder = reinterpret_cast<mmdExportType::INT_INT>(GetProcAddress(mmd, "ExpGetPmdOrder"));
	exp.ExpGetPmdMatNum = reinterpret_cast<mmdExportType::INT_INT>(GetProcAddress(mmd, "ExpGetPmdMatNum"));
	exp.ExpGetPmdMaterial = reinterpret_cast<mmdExportType::D3DMATERIAL9_INTINT>(GetProcAddress(mmd, "ExpGetPmdMaterial"));
	exp.ExpGetPmdBoneNum = reinterpret_cast<mmdExportType::INT_INT>(GetProcAddress(mmd, "ExpGetPmdBoneNum"));
	exp.ExpGetPmdBoneName = reinterpret_cast<mmdExportType::LPCHAR_INTINT>(GetProcAddress(mmd, "ExpGetPmdBoneName"));
	exp.ExpGetPmdBoneWorldMat = reinterpret_cast<mmdExportType::D3DMATRIX_INTINT>(GetProcAddress(mmd, "ExpGetPmdBoneWorldMat"));
	exp.ExpGetPmdMorphNum = reinterpret_cast<mmdExportType::INT_INT>(GetProcAddress(mmd, "ExpGetPmdMorphNum"));
	exp.ExpGetPmdMorphName = reinterpret_cast<mmdExportType::LPCHAR_INTINT>(GetProcAddress(mmd, "ExpGetPmdMorphName"));
	exp.ExpGetPmdMorphValue = reinterpret_cast<mmdExportType::FLOAT_INTINT>(GetProcAddress(mmd, "ExpGetPmdMorphValue"));
	exp.ExpGetPmdDisp = reinterpret_cast<mmdExportType::BOOL_INT>(GetProcAddress(mmd, "ExpGetPmdDisp"));
	exp.ExpGetPmdID = reinterpret_cast<mmdExportType::INT_INT>(GetProcAddress(mmd, "ExpGetPmdID"));
	exp.ExpGetAcsNum = reinterpret_cast<mmdExportType::INT_VOID>(GetProcAddress(mmd, "ExpGetAcsNum"));
	exp.ExpGetPreAcsNum = reinterpret_cast<mmdExportType::INT_VOID>(GetProcAddress(mmd, "ExpGetPreAcsNum"));
	exp.ExpGetAcsFilename = reinterpret_cast<mmdExportType::LPCHAR_INT>(GetProcAddress(mmd, "ExpGetAcsFilename"));
	exp.ExpGetAcsOrder = reinterpret_cast<mmdExportType::INT_INT>(GetProcAddress(mmd, "ExpGetAcsOrder"));
	exp.ExpGetAcsWorldMat = reinterpret_cast<mmdExportType::D3DMATRIX_INT>(GetProcAddress(mmd, "ExpGetAcsWorldMat"));
	exp.ExpGetAcsX = reinterpret_cast<mmdExportType::FLOAT_INT>(GetProcAddress(mmd, "ExpGetAcsX"));
	exp.ExpGetAcsY = reinterpret_cast<mmdExportType::FLOAT_INT>(GetProcAddress(mmd, "ExpGetAcsY"));
	exp.ExpGetAcsZ = reinterpret_cast<mmdExportType::FLOAT_INT>(GetProcAddress(mmd, "ExpGetAcsZ"));
	exp.ExpGetAcsRx = reinterpret_cast<mmdExportType::FLOAT_INT>(GetProcAddress(mmd, "ExpGetAcsRx"));
	exp.ExpGetAcsRy = reinterpret_cast<mmdExportType::FLOAT_INT>(GetProcAddress(mmd, "ExpGetAcsRy"));
	exp.ExpGetAcsRz = reinterpret_cast<mmdExportType::FLOAT_INT>(GetProcAddress(mmd, "ExpGetAcsRz"));
	exp.ExpGetAcsSi = reinterpret_cast<mmdExportType::FLOAT_INT>(GetProcAddress(mmd, "ExpGetAcsSi"));
	exp.ExpGetAcsTr = reinterpret_cast<mmdExportType::FLOAT_INT>(GetProcAddress(mmd, "ExpGetAcsTr"));
	exp.ExpGetAcsDisp = reinterpret_cast<mmdExportType::BOOL_INT>(GetProcAddress(mmd, "ExpGetAcsDisp"));
	exp.ExpGetAcsID = reinterpret_cast<mmdExportType::INT_INT>(GetProcAddress(mmd, "ExpGetAcsID"));
	exp.ExpGetAcsMatNum = reinterpret_cast<mmdExportType::INT_INT>(GetProcAddress(mmd, "ExpGetAcsMatNum"));
	exp.ExpGetAcsMaterial = reinterpret_cast<mmdExportType::D3DMATERIAL9_INTINT>(GetProcAddress(mmd, "ExpGetAcsMaterial"));
	exp.ExpGetCurrentObject = reinterpret_cast<mmdExportType::INT_VOID>(GetProcAddress(mmd, "ExpGetCurrentObject"));
	exp.ExpGetCurrentMaterial = reinterpret_cast<mmdExportType::INT_VOID>(GetProcAddress(mmd, "ExpGetCurrentMaterial"));
	exp.ExpGetCurrentTechnic = reinterpret_cast<mmdExportType::INT_VOID>(GetProcAddress(mmd, "ExpGetCurrentTechnic"));
	exp.ExpSetRenderRepeatCount = reinterpret_cast<mmdExportType::VOID_INT>(GetProcAddress(mmd, "ExpSetRenderRepeatCount"));
	exp.ExpGetRenderRepeatCount = reinterpret_cast<mmdExportType::INT_VOID>(GetProcAddress(mmd, "ExpGetRenderRepeatCount"));
	return exp;
}