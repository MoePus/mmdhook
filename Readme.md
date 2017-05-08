##Prepare
###Patch d3dx9core.h:

from line 347:

    #ifdef __cplusplus
    #ifndef CINTERFACE
    #ifdef UNICODE
        HRESULT GetDesc(D3DXFONT_DESCW *pDesc) { return GetDescW(pDesc); }
        HRESULT PreloadText(LPCWSTR pString, INT Count) { return PreloadTextW(pString, Count); }
    #else
        HRESULT GetDesc(D3DXFONT_DESCA *pDesc) { return GetDescA(pDesc); }
        HRESULT PreloadText(LPCSTR pString, INT Count) { return PreloadTextA(pString, Count); }
    #endif
    #endif
    #endif //__cplusplus


___
##Usage
###in hookdevice.h
* apply hook->device->lpVtbl->sth to hook d3d9

* use hook->d3dx9->hookFunction() to hook d3dx9

* use hook->mmd.sth to get information from MMD
