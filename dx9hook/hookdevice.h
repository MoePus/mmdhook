#pragma once
#include "d3d9.h"
#include "d3dx9.h"
#include <future> 

extern d3d9hook* hook;

void hookDevice();

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

		OutputDebugStringA("\nModel:\n");
		char* modelName = hook->mmd.ExpGetPmdFilename(hook->mmd.ExpGetCurrentObject());
		OutputDebugStringA(modelName);
		OutputDebugStringA("\nloads texture:\n");
		OutputDebugStringW(pSrcFile);
		return oFunc(pDevice, pSrcFile, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo,
			pPalette, ppTexture);
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

void hookDevice()
{
	hook->device->lpVtbl->Present = Present;
	hook->device->lpVtbl->BeginStateBlock = BeginStateBlock;
	hook->device->lpVtbl->EndStateBlock = EndStateBlock;
}