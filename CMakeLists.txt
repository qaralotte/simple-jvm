cmake_minimum_required(VERSION 3.10)
include_directories(.)
project(simple_jvm)
set(CMAKE_CXX_STANDARD 11)

find_package(Java 1.8 REQUIRED)

add_definitions(-D PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR})

set(JVM_SRC include/jvm/classfile/classfile.h include/type.h include/jvm/engine/loader.h src/jvm/engine/loader.cpp include/log.h src/log.cpp include/jvm/engine/engine.h src/jvm/engine/engine.cpp include/jvm/classfile/constant.h include/jvm/classfile/field.h)
add_executable(simple_jvm src/main.cpp ${JVM_SRC})