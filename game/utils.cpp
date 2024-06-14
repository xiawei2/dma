//
// Created by 17129 on 2024/6/14.
//

#include "utils.h"
wstring UnicodeToAnsi(vector<BYTE> bytes)
{
    DWORD size = bytes.size();
    CHAR* Unicode = new CHAR[size];
    for (size_t i = 0; i < size; i++)
    {
        Unicode[i] = bytes[i];
    }
    //UniCode到Ansi
    DWORD Count = WideCharToMultiByte(936, 512, (LPCWCH)Unicode, -1, 0, 0, 0, FALSE);
    CHAR* Buffer = new CHAR[Count];
    WideCharToMultiByte(936, 512, (LPCWCH)Unicode, -1, Buffer, Count, NULL, NULL);

    // string转wstring
    DWORD nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Buffer, -1, NULL, 0);
    if (nLen == 0)
    {
        return NULL;
    }
    WCHAR* pResult = new WCHAR[nLen];
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Buffer, -1, pResult, nLen);

    return pResult;
}