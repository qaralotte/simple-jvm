cmake_minimum_required(VERSION 3.19)
project(jvm)
include_directories(.)

set(CMAKE_CXX_STANDARD 17)
set(JVM_LIB lib/zip_file.hpp)
set(JVM_SRC include/classfile/classfile.h include/classfile/constant.h include/classfile/std.h include/std.h include/log.h src/classfile/constant.cpp include/classfile/loader.h src/classfile/loader.cpp src/log.cpp include/classfile/field.h include/classfile/method.h include/classfile/attribute.h src/classfile/attribute.cpp include/finder.h src/finder.cpp include/config.h include/cmd.h src/cmd.cpp include/runtime/local_vartable.h include/runtime/thread.h include/runtime/operand_stack.h src/runtime/thread.cpp src/runtime/local_vartable.cpp include/runtime/heap/object.h include/runtime/slot.h src/runtime/operand_stack.cpp include/opcode/reader.h src/opcode/reader.cpp include/opcode/interpreter.h src/opcode/interpreter.cpp)

add_executable(jvm src/engine.cpp ${JVM_LIB} ${JVM_SRC})