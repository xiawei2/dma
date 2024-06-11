cmake_minimum_required(VERSION 3.28)
project(dma)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXE_LINKER_FLAGS "-static")


link_directories(libs)
add_executable(dma main.cpp
        leechcore.h
        vmmdll.h
)

target_link_libraries(dma
#        leechcore.dll
#        vmm.dll
vmm.lib
leechcore.lib)




