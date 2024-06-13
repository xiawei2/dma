//// Example.cpp : This file contains the 'main' function. Program execution begins and ends there.
////
//
//#include <iostream>
//#include <cstdint>
//#include "Memory/Memory.h"
//
//int main()
//{
//    system("chcp 65001");
//
//    // Initialize DMA for memory operations, using "explorer.exe" as the target process.
//    // DMA的初始化对于后续的内存操作是必要的。
//    if (!mem.Init("smss.exe", true, true))
//    {
//        std::cout << "Failed to initilize DMA" << std::endl;
//        return 1;
//    }
//
//    std::cout << "DMA initilized" << std::endl;
//
//    // Initialize keyboard hotkeys through the kernel.
//    // 这一步是为了能够通过内核级别操作键盘。
//    if (!mem.GetKeyboard()->InitKeyboard())
//    {
//        std::cout << "Failed to initialize keyboard hotkeys through kernel." << std::endl;
//        return 1;
//    }
//    mem.GetKeyboard()->UpdateKeys();
//    //example keyboard usage.
//    std::cout << "Continueing once 'A' has been pressed." << std::endl;
//    // 等待用户按下'A'键，以便继续执行。
//    while (!mem.GetKeyboard()->IsKeyDown(0x41))
//    {
//        Sleep(100);
//    }
//
//    // Fix CR3 value to ensure memory operations' correctness.
//    // 修复CR3值是为了确保内存操作的准确性。
//    if (!mem.FixCr3())
//        std::cout << "Failed to fix CR3" << std::endl;
//    else
//        std::cout << "CR3 fixed" << std::endl;
//
//    // 获取"explorer.exe"进程的基地址。
//    uintptr_t base = mem.GetBaseDaddy("smss.exe");
//
//    // 从内存中读取并修改值。
//    std::cout << "Value: " << mem.Read<int>(base + 0x66) << std::endl;
//    mem.Write<int>(base + 0x66, 0x69);
//    std::cout << "Value: " << mem.Read<int>(base + 0x66) << std::endl;
//
//    // 使用scatter/gather方式读取内存。
//    int value = 0;
//    if (mem.Read(base + 0x66, &value, sizeof(value)))
//        std::cout << "Read Value" << std::endl;
//    else
//        std::cout << "Failed to read Value" << std::endl;
//    std::cout << "Value: " << value << std::endl;
//
//    // 创建一个scatter/gather句柄用于后续的内存操作。
//    auto handle = mem.CreateScatterHandle();
//
//    // 添加scatter读请求到句柄中。
//    value = 0;
//    mem.AddScatterReadRequest(handle, base + 0x66, &value, sizeof(value));
//    // 执行scatter读请求。
//    //You have to execute the read requests before you can read the values.
//    mem.ExecuteReadScatter(handle);
//    std::cout << "Value: " << value << std::endl;
//
//    // 添加scatter写请求到句柄中。
////    You can also write to memory using scatter requests.
//    value = 500;
//    mem.AddScatterWriteRequest(handle, base + 0x66, &value, sizeof(value));
//    // 执行scatter写请求。
//    mem.ExecuteWriteScatter(handle);
//
//    // 关闭scatter/gather句柄，释放资源。
////    Always make sure to clean up the handle, otherwise you'll end up with a memory leak.
//    mem.CloseScatterHandle(handle);
//
//    std::vector<uint64_t> codecaves = mem.GetShellcode().find_all_codecave(3172, "DNF.exe");
//    if (codecaves.size()== 0)
//        std::cout << "Failed to find codecave" << std::endl;
//    else
//        std::cout << "Codecave found at: " << std::hex << codecaves[0] << std::endl;
//    std::cout << "Hello World!\n";
//    //pause();
////    Sleep(10000);
////    uint64_t codecave = codecaves[0];
////    unsigned char shellcode[] = {72, 131, 236, 100, 0, 0, 0, 255, 21, 2, 0, 0, 0, 235, 8, 248, 224, 74, 75, 1, 0, 0, 0, 72, 163, 248, 224, 74, 75, 1, 0, 0, 0, 72, 131, 196, 100};
////    if (!mem.Write(codecave, shellcode, sizeof(shellcode)))
////    {
////        printf("Failed to write shellcode\n");
////        return 1;
////    }
////    long prt = mem.Read<long>(0x13FDC0000);
////    std::cout << "Value: " << prt << std::endl;
////    uint64_t call=   0x13FDC0000ULL;
////    mem.GetShellcode().call_function(reinterpret_cast<void*>(codecave),  (void*)call, "smss.exe");
////    prt = mem.Read<long>(0x13FDC0000ULL);
////    std::cout << "Value: " << prt << std::endl;
//    return 0;
//}
//
