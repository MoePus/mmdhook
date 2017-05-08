#define CINTERFACE
#define NOMINMAX 
#include "d3d9.h"
#include "d3dx9.h"
#include <windows.h>
#include "dx9hook.h"
#include "hookdevice.h"
#include "mmdexport.h"
#pragma comment(lib, "d3dx9.lib")

d3d9hook* hook;

BOOL APIENTRY DllMain(HINSTANCE hinst, DWORD reason, LPVOID)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
	{
		hook = new d3d9hook(hinst);
		break;
	}
	case DLL_PROCESS_DETACH:
		delete hook;
		break;
	}
	return TRUE;
}



IDirect3D9 * WINAPI Direct3DCreate9(UINT SDKVersion) {
#pragma comment(linker, "/EXPORT:Direct3DCreate9=Direct3DCreate9")
	IDirect3D9 *direct3d = (*(hook->mDirect3DCreate9))(SDKVersion);
	hook->mCreateDevice = direct3d->lpVtbl->CreateDevice;

	DWORD old_protect;
	VirtualProtect(reinterpret_cast<void *>(direct3d->lpVtbl), sizeof(direct3d->lpVtbl), PAGE_EXECUTE_READWRITE, &old_protect);

	direct3d->lpVtbl->CreateDevice = CreateDevice;

	VirtualProtect(reinterpret_cast<void *>(direct3d->lpVtbl), sizeof(direct3d->lpVtbl), old_protect, &old_protect);

	return direct3d;
}

