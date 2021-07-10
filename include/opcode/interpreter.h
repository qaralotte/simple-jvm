#ifndef JVM_OPCODE_INTERPRETER_H
#define JVM_OPCODE_INTERPRETER_H

#include <tuple>
#include <functional>

#include "include/std.h"

#include "include/runtime/thread.h"
#include "reader.h"

namespace opcode {
    class Interpreter {
    private:
        runtime::JVMFrame &frame;
        Reader &reader;
    private:
        /* 常量 */
        void nop();
        template<typename T> void constn(T);
        template<typename T> void push();
        void ldc(bool = false, bool = false);

        /* 加载 */
        template<typename T> void load();
        template<typename T> void loadn(uint);

        /* 储存 */
        template<typename T> void store();
        template<typename T> void storen(uint);

        /* 栈 */
        void pop(uint);
        void dup(uint);
        void swap();

        /* 数学 */
        template<typename T> void add();
        template<typename T> void sub();
        template<typename T> void mul();
        template<typename T> void div();
        template<typename T> void rem();
        template<typename T> void neg();
        template<typename T> void shl();
        template<typename T> void shr();
        template<typename T> void ushr();
        template<typename T> void _and();
        template<typename T> void _or();
        template<typename T> void _xor();
        template<typename T> void inc();

        /* 转换 */
        template<typename N, typename M> void n2m();

        /* 比较 */
        template<typename T> void cmp(jint = 0);
        template<typename T> void _if(function<bool(T)>);
        template<typename T> void if_cmp(function<bool(T, T)>);

        /* 控制 */
        void _goto();

        /* 引用 */
        void getstatic();
        void putstatic();
        void getfield();
        void putfield();

        void invokevirtual();
        void invokespecial();

        void _new();
        void checkcast();
        void instanceof();
    public:
        Interpreter(runtime::JVMFrame &_frame, Reader &_reader) : frame(_frame), reader(_reader) {};
        void execute(uint);
    };
}

#endif //JVM_OPCODE_INTERPRETER_H
