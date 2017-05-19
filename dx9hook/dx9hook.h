#ifdef DX9HOOK_EXPORTS
#define DX9HOOK_API __declspec(dllexport)
#else
#define DX9HOOK_API __declspec(dllimport)
#endif

#include "windows.h"
#include "iostream"
#include "string"
#include "mmdexport.h"
#include <tchar.h>
#include <future> 
#include "d3dx9hook.h"
#include "mmf.h"

class d3d9hook
{
public:
	using Direct3DCreate9 = IDirect3D9 *(WINAPI*)(UINT);
	using Direct3DCreate9Ex = HRESULT(WINAPI*)(UINT, IDirect3D9Ex**);

	using vCreateDevice = HRESULT(WINAPI *)(IDirect3D9*, UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS*, IDirect3DDevice9**);
	using vCreateDeviceEx = HRESULT(WINAPI *)(IDirect3D9Ex*, UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS*, D3DDISPLAYMODEEX*, IDirect3DDevice9Ex**);
private:
	bool init()
	{
		TCHAR buff[1024];
		int bufflen = GetSystemDirectory(buff, MAX_PATH);
		std::wstring dx9Path = std::wstring(buff) + L"\\D3D9.DLL";
		d3d9 = LoadLibrary(dx9Path.c_str());

		if (!d3d9) {
			return FALSE;
		}

		mDirect3DCreate9 = reinterpret_cast<Direct3DCreate9>(GetProcAddress(d3d9, "Direct3DCreate9"));
		if (!mDirect3DCreate9) {
			return FALSE;
		}
		mDirect3DCreate9Ex = reinterpret_cast<Direct3DCreate9Ex>(GetProcAddress(d3d9, "Direct3DCreate9Ex"));
		if (!mDirect3DCreate9Ex) {
			return FALSE;
		}
		return TRUE;
	}
	HMODULE d3d9;
public:
	d3d9hook(HINSTANCE _hinst):hinst(_hinst)
	{
		mmdExpFuture = std::async(initMMDEXP, GetModuleHandle(NULL));
		d3dx9 = new d3dx9hook;
		init();
	}
	~d3d9hook()
	{
		delete d3dx9;
	}
	void havestMMDExp()
	{
		mmd = mmdExpFuture.get();
	}
	void bakupDevice()
	{
		memcpy(&deviceVirtualFunctions, device->lpVtbl, sizeof(deviceVirtualFunctions));
	}
	void update()
	{
		int modelNums = mmd.ExpGetPmdNum();
		int* modelMaterialNums = new int[modelNums];
		int* modelOrders = new int[modelNums];
		vector<string> modelNames; modelNames.resize(modelNums);
		
		for (int i = 0; i < modelNums; i++)
		{
			modelMaterialNums[i] = mmd.ExpGetPmdMatNum(i);
			modelNames[i] = mmd.ExpGetPmdFilename(i);
			modelOrders[i] = mmd.ExpGetPmdOrder(i);
		}
		mmf.updateFrame(modelNums, modelOrders, modelMaterialNums, modelNames);
		delete modelMaterialNums;
		delete modelOrders;
	}

	std::future<MMDEXP> mmdExpFuture;
	d3dx9hook* d3dx9;
	HINSTANCE hinst;
	MMDEXP mmd;
	MMF mmf;
	/*Original DX Function*/
	Direct3DCreate9 mDirect3DCreate9;
	Direct3DCreate9Ex mDirect3DCreate9Ex;

	vCreateDevice mCreateDevice;
	vCreateDeviceEx mCreateDeviceEx;

	IDirect3DDevice9* device;
	std::remove_pointer<decltype(IDirect3DDevice9::lpVtbl)>::type deviceVirtualFunctions;
};