cmake_minimum_required(VERSION 3.28)
project(dma)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

link_directories(libs)
add_executable(dma
        Memory/InputManager.cpp
        Memory/InputManager.h
        Memory/Memory.cpp
        Memory/Memory.h
        Memory/Registry.cpp
        Memory/Registry.h
        Memory/Shellcode.cpp
        Memory/Shellcode.h
        nt/structs.h
        Example.cpp
        framework.h
        pch.cpp
        pch.h
        vmmdll_example.c
        game/stat.cpp
        main.cpp
        main.h
        main.cpp
        game/Position.h
        game/stat.cpp
        game/Position.h
        game/Position.cpp
        game/MapData.cpp
        game/MapData.h
        game/utils.cpp
        game/utils.h)
target_link_libraries(dma
leechcore.lib
vmm.lib)

