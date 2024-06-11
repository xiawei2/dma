#include <iostream>
#include "vmmdll.h"

#include <windows.h>
#include <iostream>
int main()
{
    const char* tmp_str[] = { "","-device","vmware" };
    VMM_HANDLE handle = VMMDLL_Initialize(3, tmp_str);
    PDWORD pid;
    VMMDLL_PidGetFromName(handle, "DNF.exe", pid);
    ULONG64 pid_addr = 0x14B4B3F20;
    PBYTE pb;
    DWORD cb = 4;
    char magic[4] = { 0 };
//    VMMDLL_MemRead(handle, (DWORD) pid,pid_addr,  (PBYTE)magic, sizeof(magic));
    SIZE_T pcPIDS;
    VMMDLL_PidList(handle, nullptr, &pcPIDS);
    DWORD* pids = new DWORD[pcPIDS];
    VMMDLL_PidList(handle, pids, &pcPIDS);
    for (int i = 0; i < pcPIDS; i++) {

        VMMDLL_PROCESS_INFORMATION processInformation = { 0 };
        processInformation.magic = VMMDLL_PROCESS_INFORMATION_MAGIC;
        processInformation.wVersion = VMMDLL_PROCESS_INFORMATION_VERSION;
        SIZE_T pcbprocessInformation = sizeof(processInformation);
        VMMDLL_ProcessGetInformation(handle, pids[i], &processInformation, &pcbprocessInformation);
        std::cout << pids[i] << "-----" << processInformation.szName << std::endl;

        //VMMDLL_MAP_MODULE* MODULE = nullptr;
        //VMMDLL_Map_GetModuleU(handle, pids[i], &MODULE, VMMDLL_MODULE_FLAG_NORMAL);
        //for (int j = 0; j < MODULE->cMap;j++) {
        //    std::cout << "-----" << MODULE->pMap[j].uszFullName << std::endl;
        //}

        VMMDLL_MAP_MODULEENTRY* ppModuleMapEntry = nullptr;
        VMMDLL_Map_GetModuleFromNameU(handle, pids[i], processInformation.szName, &ppModuleMapEntry, VMMDLL_MODULE_FLAG_NORMAL);
        if (ppModuleMapEntry) {
            if (processInformation.szName == std::string("TestMem.exe")) {
                VMMDLL_MAP_VAD* va = NULL;
                VMMDLL_Map_GetVadU(handle, pids[i], false, &va);
                for (int j = 0; j < va->cMap; j++) {
                    std::cout << "-----" << std::hex << va->pMap[j].vaStart << "\t";
                    std::cout << "-----" << std::hex << va->pMap[j].vaEnd << "\t";
                    std::cout << "-----" << std::hex << va->pMap[j].vaEnd - va->pMap[j].vaStart + 1 << "\t";
                    //std::cout << "-----" << va->pMap[j].Protection << "\t";
                    //std::cout << "-----" << va->pMap[j].MemCommit << "\t";
                    //std::cout << "-----" << va->pMap[j].vaSubsection << "\t";
                    std::cout <<   std::endl;
                }
                // ppModuleMapEntry->vaBase 不要操作这个，程序会打不开
                std::cout << "-----" << ppModuleMapEntry->uszFullName << std::endl;
                char magic[4] = { 0 };
                ULONG64 Base = 0x00007FF7187C4074;
                VMMDLL_MemRead(handle, pids[i], Base, (PBYTE)magic, sizeof(magic));
                for (int i = 0; i < sizeof(magic); i++) {
                    std::cout << std::hex << static_cast<int>(magic[i]) << " ";
                }
                std::cout << std::endl;
                memset(magic, 66, sizeof(magic));
                VMMDLL_MemWrite(handle, pids[i], Base, (PBYTE)magic, sizeof(magic));
                VMMDLL_MemRead(handle, pids[i], Base, (PBYTE)magic, sizeof(magic));
                for (int i = 0; i < sizeof(magic); i++) {
                    std::cout << std::hex << static_cast<int>(magic[i]) << " ";
                }
                std::cout << std::endl;
            }
        }

    }
    delete[] pids;
    return 0;
}
class VMMDLL {
public:

};
