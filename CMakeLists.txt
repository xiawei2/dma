cmake_minimum_required(VERSION 3.28)
project(dma)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static")
#add_executable(dma1 main.cpp)

link_directories(libs)
#include_directories(include)
file(GLOB_RECURSE SRC_DIR_LIST "game/*.cpp" "Memory/*.cpp" "")	#遍历获取source/路径下所有的*.cpp和*.c文件列表

#将DIR_LIB_SRCS中文件列表以静态库的方式加入工程
add_executable(
        ${PROJECT_NAME}
        ${SRC_DIR_LIST}
        main.cpp
        include/fastCall.h#将DIR_LIB_SRCS中文件列表以静态库的方式加入工程
)

message(STATUS "SRC_DIR_LIST = ${SRC_DIR_LIST}")			#打印SRC_DIR_LIST中列表信息
macro(FIND_INCLUDE_DIR result curdir)
    file(GLOB_RECURSE children "${curdir}/*.cpp" "${curdir}/*.h" )
    set(dirlist "")
    foreach(child ${children})
        string(REGEX REPLACE "(.*)/.*" "\\1" LIB_NAME ${child})
        if(IS_DIRECTORY ${LIB_NAME})
            list (FIND dirlist ${LIB_NAME} list_index)
            if(${list_index} LESS 0)
                LIST(APPEND dirlist ${LIB_NAME})
            else()
            endif()
        endif()
    endforeach()
    set(${result} ${dirlist})
    message(STATUS "SRC_DIR_LIST = ${dirlist}")
endmacro()
#查找include目录下的所有*.hpp,*.h头文件,并路径列表保存到 INCLUDE_DIR_LIST 变量中
FIND_INCLUDE_DIR(INCLUDE_DIR_LIST ${PROJECT_SOURCE_DIR}/include)			#调用函数，指定参数

#将INCLUDE_DIR_LIST中路径列表加入工程
include_directories(   													#INCLUDE_DIR_LIST路径列表加入工程
        ${INCLUDE_DIR_LIST}
)
target_link_libraries(dma
        leechcore.lib
        vmm.lib
        vncclient.lib)


