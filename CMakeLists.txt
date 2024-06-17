cmake_minimum_required(VERSION 3.28)
project(dma)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

link_directories(libs)
include_directories(include)
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
        main.cpp
        main.h
        main.cpp
        include/Coordinate.h
        game/MapData.cpp
        include/MapData.h
        game/utils.cpp
        include/utils.h
        game/GameCall.cpp
        include/GameCall.h
        game/GameMap.cpp
        include/GameMap.h
        game/Auto.cpp
        include/Auto.h

        backchannel.c
        qt5vncclient.cpp
        include/qt5vncclient.h
        include/GameMove.h
        game/GameMove.cpp
        include/allHeader.h
        include/allHeader.cpp
        game/Coordinate.cpp
)
target_link_libraries(dma
leechcore.lib
vmm.lib
vncclient.lib)


