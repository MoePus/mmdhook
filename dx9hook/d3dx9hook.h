#pragma once
#include "windows.h"
#include <tchar.h>
#include "MinHook.h"

#if defined _M_X64
#pragma comment(lib, "MinHook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "MinHook.x86.lib")
#endif
#include <unordered_map>
#include <string>
class d3dx9hook
{
private:
	std::unordered_map<std::string, void*> funcMap;
public:
	d3dx9hook()
	{
		MH_Initialize();
	}
	~d3dx9hook()
	{
		MH_Uninitialize();
	}
	auto hookFunction(const char* funcName,void* hfunc)
	{
		void* ofunc;
		void* ptarget;
		auto res = MH_CreateHookApiEx(L"d3dx9_43.dll", funcName, hfunc, &ofunc,&ptarget);
		res = MH_EnableHook(ptarget);
		if (res == MH_OK)
		{
			funcMap[funcName] = ofunc;
		}
		return res;
	}
	void* oriFunction(const char* funcName)
	{
		std::string sfuncName{ funcName };
		auto it = funcMap.find(sfuncName);
		if (it == funcMap.end())
			return 0;
		else
		{
			return it->second;
		}
	}
};
