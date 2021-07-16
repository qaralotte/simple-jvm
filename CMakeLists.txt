cmake_minimum_required(VERSION 3.19)
project(jvm)
include_directories(.)
link_directories(${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_CXX_STANDARD 17)
set(JVM_SRC include/classfile/classfile.h include/classfile/constant.h include/classfile/std.h include/std.h include/log.h src/classfile/constant.cpp include/classfile/loader.h src/classfile/loader.cpp src/log.cpp include/classfile/field.h include/classfile/method.h include/classfile/attribute.h src/classfile/attribute.cpp include/finder.h src/finder.cpp include/config.h include/cmd.h src/cmd.cpp include/runtime/vartable_table.h include/runtime/thread.h include/runtime/operand_stack.h src/runtime/thread.cpp src/runtime/vartable_table.cpp include/runtime/slot.h src/runtime/operand_stack.cpp include/opcode/reader.h src/opcode/reader.cpp include/opcode/interpreter.h src/opcode/interpreter.cpp include/accessflags.h include/runtime/metaspace/clazz.h include/runtime/metaspace/field.h include/runtime/metaspace/method.h src/runtime/metaspace/clazz.cpp src/runtime/metaspace/field.cpp src/runtime/metaspace/method.cpp include/runtime/metaspace/constant.h src/runtime/metaspace/constant.cpp include/runtime/metaspace/ref/clazz.h src/runtime/metaspace/ref/clazz.cpp include/runtime/metaspace/ref/field.h src/runtime/metaspace/ref/field.cpp include/runtime/metaspace/ref/method.h src/runtime/metaspace/ref/method.cpp include/runtime/metaspace/ref/interface_method.h src/runtime/metaspace/ref/interface_method.cpp include/runtime/loader.h include/classpath.h src/classpath.cpp src/runtime/loader.cpp include/runtime/metaspace/object.h include/descriptor.h src/runtime/metaspace/object.cpp src/runtime/slot.cpp)

add_executable(jvm src/main.cpp ${JVM_LIB} ${JVM_SRC})
target_link_libraries(jvm minizip)