cmake_minimum_required(VERSION 3.19)
project(jvm)
include_directories(.)

set(CMAKE_CXX_STANDARD 17)
set(JVM_SRC include/classfile/classfile.h include/classfile/constant.h include/classfile/std.h include/std.h include/log.h src/classfile/constant.cpp include/classfile/reader.h src/classfile/reader.cpp src/log.cpp include/classfile/field.h include/classfile/method.h include/classfile/attribute.h src/classfile/attribute.cpp include/finder.h src/finder.cpp include/config.h include/cmd.h src/cmd.cpp)
set(JVM_LIB lib/zip_file.hpp )

add_executable(jvm src/engine.cpp ${JVM_SRC} ${JVM_LIB})