#include <cmath>

#include "include/opcode/interpreter.h"
#include "include/log.h"
#include "include/accessflags.h"
#include "include/descriptor.h"

#include "include/runtime/metaspace/ref/clazz.h"
#include "include/runtime/metaspace/ref/field.h"
#include "include/runtime/metaspace/ref/method.h"

using namespace opcode;

/* 操作码实现方法 */

/* 常量 */
void Interpreter::nop() {
    // do nothing
}

template<typename T> void Interpreter::constn(T value) {
    frame.stack.push<T>(value);
}

template<typename T> void Interpreter::push() {
    ERROR("impossible reach");
    exit(0);
}

template<> void Interpreter::push<jbyte>() {
    frame.stack.push<jbyte>(reader.readByte());
}

template<> void Interpreter::push<jshort>() {
    frame.stack.push<jshort>(reader.readShort());
}

void Interpreter::ldc(bool wide, bool two) {
    jshort index = wide ? reader.readShort() : reader.readByte();
    auto constant_pool = frame.method -> clazz -> constant_pool -> constants;
    if (!two) {
        if (constant_pool[index] -> tag == runtime::INTEGER) {
            auto val = static_pointer_cast<runtime::IntegerConstant>(constant_pool[index]) -> value;
            frame.stack.push<jint>(val);
        } else if (constant_pool[index] -> tag == runtime::FLOAT) {
            auto val = static_pointer_cast<runtime::FloatConstant>(constant_pool[index]) -> value;
            frame.stack.push<jfloat>(val);
        } else {
            ERROR("todo: ldc功能未做完"); // todo
        }
    } else {
        if (constant_pool[index] -> tag == runtime::LONG) {
            auto val = static_pointer_cast<runtime::LongConstant>(constant_pool[index]) -> value;
            frame.stack.push<jlong>(val);
        } else if (constant_pool[index] -> tag == runtime::DOUBLE) {
            auto val = static_pointer_cast<runtime::DoubleConstant>(constant_pool[index]) -> value;
            frame.stack.push<jdouble>(val);
        } else {
            ERROR("todo: ldc功能未做完"); // todo
        }
    }
}

/* 加载 */
template<typename T> void Interpreter::load() {
    auto value = frame.locals.get<T>(reader.readByte());
    frame.stack.push<T>(value);
}

template<typename T> void Interpreter::loadn(uint index) {
    auto value = frame.locals.get<T>(index);
    frame.stack.push<T>(value);
}

/* 储存 */
template<typename T> void Interpreter::store() {
    auto value = frame.stack.pop<T>();
    frame.locals.set<T>(reader.readByte(), value);
}

template<typename T> void Interpreter::storen(uint index) {
    auto value = frame.stack.pop<T>();
    frame.locals.set<T>(index, value);
}

/* 栈 */
void Interpreter::pop(uint count) {
    for (int i = 0; i < count; ++i) {
        frame.stack.pop<runtime::Slot>();
    }
}

void Interpreter::dup(uint count) {
    vector<runtime::Slot> slots;
    for (int i = 0; i < count; ++i) {
        slots.push_back(frame.stack.pop<runtime::Slot>());
    }
    for (int i = 0; i < count * 2; ++i) {
        frame.stack.push<runtime::Slot>(slots[i % count]);
    }
}

void Interpreter::swap() {
    auto slot1 = frame.stack.pop<runtime::Slot>();
    auto slot2 = frame.stack.pop<runtime::Slot>();
    frame.stack.push(slot1);
    frame.stack.push(slot2);
}

/* 数学 */
template<typename T> void Interpreter::add() {
    auto v2 = frame.stack.pop<T>();
    auto v1 = frame.stack.pop<T>();
    frame.stack.push<T>(v1 + v2);
}

template<typename T> void Interpreter::sub() {
    auto v2 = frame.stack.pop<T>();
    auto v1 = frame.stack.pop<T>();
    frame.stack.push<T>(v1 - v2);
}

template<typename T> void Interpreter::mul() {
    auto v2 = frame.stack.pop<T>();
    auto v1 = frame.stack.pop<T>();
    frame.stack.push<T>(v1 * v2);
}

template<typename T> void Interpreter::div() {
    auto v2 = frame.stack.pop<T>();
    auto v1 = frame.stack.pop<T>();
    if (v2 == 0) {
        ERROR("div by zero");
    }
    frame.stack.push<T>(v1 / v2);
}

template<typename T> void Interpreter::rem() {
    auto v2 = frame.stack.pop<T>();
    auto v1 = frame.stack.pop<T>();
    if (v2 == 0) {
        ERROR("div by zero");
    }
    frame.stack.push<T>(v1 % v2);
}

// Float rem
template<> void Interpreter::rem<jfloat>() {
    auto v2 = frame.stack.pop<jfloat>();
    auto v1 = frame.stack.pop<jfloat>();
    if (v2 == 0) {
        ERROR("div by zero");
    }
    frame.stack.push<jfloat>(fmod(v1, v2));
}

// Double rem
template<> void Interpreter::rem<jdouble>() {
    auto v2 = frame.stack.pop<jdouble>();
    auto v1 = frame.stack.pop<jdouble>();
    if (v2 == 0) {
        ERROR("div by zero");
    }
    frame.stack.push<jdouble>(fmod(v1, v2));
}

template<typename T> void Interpreter::neg() {
    auto value = frame.stack.pop<T>();
    frame.stack.push<T>(-value);
}

template<typename T> void Interpreter::shl() {
    auto v2 = frame.stack.pop<T>();
    auto v1 = frame.stack.pop<T>();
    frame.stack.push<T>(v1 << v2);
}

template<typename T> void Interpreter::shr() {
    auto v2 = frame.stack.pop<T>();
    auto v1 = frame.stack.pop<T>();
    frame.stack.push<T>(v1 >> v2);
}

template<typename T> void Interpreter::ushr() {
    ERROR("impossible reach");
    exit(0);
}

template<> void Interpreter::ushr<jint>() {
    uint v2 = frame.stack.pop<jint>();
    uint v1 = frame.stack.pop<jint>();
    uint v = v2 & 0x1F;
    frame.stack.push<jint>(v1 >> v);
}

template<> void Interpreter::ushr<jlong>() {
    ulong v2 = frame.stack.pop<jlong>();
    ulong v1 = frame.stack.pop<jlong>();
    ulong v = v2 & 0x3F;
    frame.stack.push<jlong>(v1 >> v);
}

template<typename T> void Interpreter::_and() {
    auto v2 = frame.stack.pop<T>();
    auto v1 = frame.stack.pop<T>();
    frame.stack.push<T>(v1 & v2);
}

template<typename T> void Interpreter::_or() {
    auto v2 = frame.stack.pop<T>();
    auto v1 = frame.stack.pop<T>();
    frame.stack.push<T>(v1 | v2);
}

template<typename T> void Interpreter::_xor() {
    auto v2 = frame.stack.pop<T>();
    auto v1 = frame.stack.pop<T>();
    frame.stack.push<T>(v1 ^ v2);
}

template<typename T> void Interpreter::inc() {
    auto index = reader.readByte();
    auto inc = reader.readByte();
    auto value = frame.locals.get<T>(index);
    value += inc;
    frame.locals.set<T>(index, value);
}

/* 转换 */
template<typename N, typename M> void Interpreter::n2m() {
    auto value = frame.stack.pop<N>();
    frame.stack.push<M>((M) value);
}

/* 比较 */
template<typename T> void Interpreter::cmp(jint value) {
    auto v2 = frame.stack.pop<T>();
    auto v1 = frame.stack.pop<T>();
    if (v1 > v2) frame.stack.push<jint>(1);
    else if (v1 == v2) frame.stack.push<jint>(0);
    else if (v1 < v2) frame.stack.push<jint>(-1);
    else { // 其中一个是NAN
        if (value != 0) {
            frame.stack.push<jint>(value);
        }
    }
}

template<typename T> void Interpreter::_if(function<bool(T)> expr) {
    auto value = frame.stack.pop<T>();
    auto offset = reader.readShort();
    if (expr(value)) {
        reader.jumpPC(offset);
    }
}

template<typename T> void Interpreter::if_cmp(function<bool(T, T)> expr) {
    auto v2 = frame.stack.pop<T>();
    auto v1 = frame.stack.pop<T>();
    auto offset = reader.readShort();
    if (expr(v1, v2)) {
        reader.jumpPC(offset);
    }
}

/* 跳转 */
void Interpreter::_goto() {
    jshort offset = reader.readShort();
    reader.jumpPC(offset);
}

/* 引用 */
void Interpreter::getstatic() {
    auto index = reader.readShort();
    auto constant_pool = frame.method -> clazz -> constant_pool -> constants;
    auto field_ref = static_pointer_cast<runtime::FieldRef>(constant_pool[index]);
    auto field = field_ref -> resolvedField();
    auto clazz = field -> clazz;
    if (!field -> haveAccess(ACCESS_STATIC)) {
        ERROR("java.lang.IncompatibleClassChangeError: %s", clazz -> this_name.c_str());
        exit(0);
    }
    auto descriptor = field -> descriptor;
    auto slot_id = field -> slot_id;
    auto slots = clazz -> static_vars;
    if (descriptor == DESC_BOOLEAN ||
        descriptor == DESC_BYTE ||
        descriptor == DESC_CHAR ||
        descriptor == DESC_SHORT ||
        descriptor == DESC_INT) {
        frame.stack.push<jint>(slots.get<jint>(slot_id));
    } else if (descriptor == DESC_FLOAT) {
        frame.stack.push<jfloat>(slots.get<jfloat>(slot_id));
    } else if (descriptor == DESC_LONG) {
        frame.stack.push<jlong>(slots.get<jlong>(slot_id));
    } else if (descriptor == DESC_DOUBLE) {
        frame.stack.push<jdouble>(slots.get<jdouble>(slot_id));
    } else if (descriptor[0] == DESC_ARRAY_HEAD || descriptor[0] == DESC_CLASS_HEAD) {
        frame.stack.push<runtime::jobject>(slots.get<runtime::jobject>(slot_id));
    } else {
        ERROR("未知的 descriptor: %s", descriptor.c_str());
        exit(0);
    }
}

void Interpreter::putstatic() {
    auto index = reader.readShort();
    auto cur_method = frame.method;
    auto cur_clazz = cur_method -> clazz;
    auto constant_pool = cur_clazz -> constant_pool -> constants;
    auto field_ref = static_pointer_cast<runtime::FieldRef>(constant_pool[index]);
    auto field = field_ref -> resolvedField();
    auto clazz = field -> clazz;
    if (!field -> haveAccess(ACCESS_STATIC)) {
        ERROR("java.lang.IncompatibleClassChangeError: %s", clazz -> this_name.c_str());
        exit(0);
    }
    if (field -> haveAccess(ACCESS_FINAL)) {
        if (*cur_clazz != *clazz || cur_method -> name != "<clinit>") {
            ERROR("java.lang.IllegalAccessError: %s", clazz -> this_name.c_str());
            exit(0);
        }
    }
    auto descriptor = field -> descriptor;
    auto slot_id = field -> slot_id;
    auto &slots = clazz -> static_vars;
    if (descriptor == DESC_BOOLEAN ||
        descriptor == DESC_BYTE ||
        descriptor == DESC_CHAR ||
        descriptor == DESC_SHORT ||
        descriptor == DESC_INT) {
        slots.set<jint>(slot_id, frame.stack.pop<jint>());
    } else if (descriptor == DESC_FLOAT) {
        slots.set<jfloat>(slot_id, frame.stack.pop<jfloat>());
    } else if (descriptor == DESC_LONG) {
        slots.set<jlong>(slot_id, frame.stack.pop<jlong>());
    } else if (descriptor == DESC_DOUBLE) {
        slots.set<jdouble>(slot_id, frame.stack.pop<jdouble>());
    } else if (descriptor[0] == DESC_ARRAY_HEAD || descriptor[0] == DESC_CLASS_HEAD) {
        slots.set<runtime::jobject>(slot_id, frame.stack.pop<runtime::jobject>());
    } else {
        ERROR("未知的 descriptor: %s", descriptor.c_str());
        exit(0);
    }
}

void Interpreter::getfield() {
    auto index = reader.readShort();
    auto constant_pool = frame.method -> clazz -> constant_pool -> constants;
    auto field_ref = static_pointer_cast<runtime::FieldRef>(constant_pool[index]);
    auto field = field_ref -> resolvedField();
    auto clazz = field -> clazz;
    if (field -> haveAccess(ACCESS_STATIC)) {
        ERROR("java.lang.IncompatibleClassChangeError: %s", clazz -> this_name.c_str());
        exit(0);
    }
    auto ref = frame.stack.pop<runtime::jobject>();
    if (ref == nullptr) {
        ERROR("java.lang.NullPointerException: %s", ref -> clazz.this_name.c_str());
    }
    auto descriptor = field -> descriptor;
    auto slot_id = field -> slot_id;
    auto slots = clazz -> static_vars;
    if (descriptor == DESC_BOOLEAN ||
        descriptor == DESC_BYTE ||
        descriptor == DESC_CHAR ||
        descriptor == DESC_SHORT ||
        descriptor == DESC_INT) {
        frame.stack.push<jint>(slots.get<jint>(slot_id));
    } else if (descriptor == DESC_FLOAT) {
        frame.stack.push<jfloat>(slots.get<jfloat>(slot_id));
    } else if (descriptor == DESC_LONG) {
        frame.stack.push<jlong>(slots.get<jlong>(slot_id));
    } else if (descriptor == DESC_DOUBLE) {
        frame.stack.push<jdouble>(slots.get<jdouble>(slot_id));
    } else if (descriptor[0] == DESC_ARRAY_HEAD || descriptor[0] == DESC_CLASS_HEAD) {
        frame.stack.push<runtime::jobject>(slots.get<runtime::jobject>(slot_id));
    } else {
        ERROR("未知的 descriptor: %s", descriptor.c_str());
        exit(0);
    }
}

void Interpreter::putfield() {
    auto index = reader.readShort();
    auto cur_method = frame.method;
    auto cur_clazz = cur_method -> clazz;
    auto constant_pool = cur_clazz -> constant_pool -> constants;
    auto field_ref = static_pointer_cast<runtime::FieldRef>(constant_pool[index]);
    auto field = field_ref -> resolvedField();
    auto clazz = field -> clazz;
    if (field -> haveAccess(ACCESS_STATIC)) {
        ERROR("java.lang.IncompatibleClassChangeError: %s", clazz -> this_name.c_str());
        exit(0);
    }
    if (field -> haveAccess(ACCESS_FINAL)) {
        if (*cur_clazz != *clazz || cur_method -> name != "<init>") {
            ERROR("java.lang.IllegalAccessError: %s", clazz -> this_name.c_str());
            exit(0);
        }
    }
    auto descriptor = field -> descriptor;
    auto slot_id = field -> slot_id;
    auto slots = clazz -> static_vars;
    auto isnull = [](runtime::jobject ref) {
        if (ref == nullptr) {
            ERROR("java.lang.NullPointerException: %s", ref -> clazz.this_name.c_str());
        }
    };
    if (descriptor == DESC_BOOLEAN ||
        descriptor == DESC_BYTE ||
        descriptor == DESC_CHAR ||
        descriptor == DESC_SHORT ||
        descriptor == DESC_INT) {
        auto val = frame.stack.pop<jint>();
        auto ref = frame.stack.pop<runtime::jobject>();
        isnull(ref);
        ref -> field.set<jint>(slot_id, val);
    } else if (descriptor == DESC_FLOAT) {
        auto val = frame.stack.pop<jfloat>();
        auto ref = frame.stack.pop<runtime::jobject>();
        isnull(ref);
        ref -> field.set<jfloat>(slot_id, val);
    } else if (descriptor == DESC_LONG) {
        auto val = frame.stack.pop<jlong>();
        auto ref = frame.stack.pop<runtime::jobject>();
        isnull(ref);
        ref -> field.set<jlong>(slot_id, val);
    } else if (descriptor == DESC_DOUBLE) {
        auto val = frame.stack.pop<jdouble>();
        auto ref = frame.stack.pop<runtime::jobject>();
        isnull(ref);
        ref -> field.set<jdouble>(slot_id, val);
    } else if (descriptor[0] == DESC_ARRAY_HEAD || descriptor[0] == DESC_CLASS_HEAD) {
        auto val = frame.stack.pop<runtime::jobject>();
        auto ref = frame.stack.pop<runtime::jobject>();
        isnull(ref);
        ref -> field.set<runtime::jobject>(slot_id, val);
    } else {
        ERROR("未知的 descriptor: %s", descriptor.c_str());
        exit(0);
    }
}

void Interpreter::invokevirtual() {
    uint16 index = reader.readShort();
    auto constant_pool = frame.method -> clazz -> constant_pool -> constants;
    auto method_ref = static_pointer_cast<runtime::MethodRef>(constant_pool[index]);
    if (method_ref -> name == "println") {
        if (method_ref -> descriptor == "(Z)V" ||
            method_ref -> descriptor == "(C)V" ||
            method_ref -> descriptor == "(B)V" ||
            method_ref -> descriptor == "(S)V" ||
            method_ref -> descriptor == "(I)V") {
            printf("%d\n", frame.stack.pop<jint>());
        } else if (method_ref -> descriptor == "(J)V") {
            printf("%lld\n", frame.stack.pop<jlong>());
        } else if (method_ref -> descriptor == "(F)V") {
            printf("%f\n", frame.stack.pop<jfloat>());
        } else if (method_ref -> descriptor == "(D)V") {
            printf("%lf\n", frame.stack.pop<jdouble>());
        } else {
            ERROR("println: %s", method_ref -> descriptor.c_str());
            exit(0);
        }
        frame.stack.pop<runtime::jobject>();
    }
}

void Interpreter::invokespecial() {
    uint16 index = reader.readShort();
}

void Interpreter::_new() {
    auto index = reader.readShort();
    auto constant_pool = frame.method -> clazz -> constant_pool -> constants;
    auto class_ref = static_pointer_cast<runtime::ClassRef>(constant_pool[index]);
    auto clazz = class_ref -> resolvedClass();
    if (clazz -> haveAccess(ACCESS_INTERFACE) || clazz -> haveAccess(ACCESS_ABSTRACT)) {
        ERROR("java.lang.InstantiationError: %s", clazz -> this_name.c_str());
        exit(0);
    }
    runtime::Object ref(*clazz, runtime::VariableTable(clazz -> instance_slot_count));
    frame.stack.push<runtime::jobject>(runtime::make_jobject(ref));
}

void Interpreter::instanceof() {
    auto index = reader.readShort();
    auto ref = frame.stack.pop<runtime::jobject>();
    if (ref == nullptr) {
        frame.stack.push<jint>(0);
        return;
    }
    auto constant_pool = frame.method -> clazz -> constant_pool -> constants;
    auto class_ref = static_pointer_cast<runtime::ClassRef>(constant_pool[index]);
    auto clazz = class_ref -> resolvedClass();
    if (ref -> isInstanceOf(*clazz)) {
        frame.stack.push<jint>(1);
    } else {
        frame.stack.push<jint>(0);
    }

}

void Interpreter::checkcast() {
    auto index = reader.readShort();
    auto ref = frame.stack.pop<runtime::jobject>();
    if (ref == nullptr) {
        frame.stack.push<jint>(0);
        return;
    }
    auto constant_pool = frame.method -> clazz -> constant_pool -> constants;
    auto class_ref = static_pointer_cast<runtime::ClassRef>(constant_pool[index]);
    auto clazz = class_ref -> resolvedClass();
    if (!ref -> isInstanceOf(*clazz)) {
        ERROR("java.lang.ClassCastException: %s", clazz -> this_name.c_str());
        exit(0);
    }
}

// 暂时
void panic(uint code) {
    ERROR("unknown code: 0x%X", code);
    exit(0);
}

void Interpreter::execute(uint code) {
    if (code == 0x00) nop();
    if (code == 0x01) constn<runtime::jobject>(nullptr);
    if (code == 0x02) constn<jint>(-1);
    if (code == 0x03) constn<jint>(0);
    if (code == 0x04) constn<jint>(1);
    if (code == 0x05) constn<jint>(2);
    if (code == 0x06) constn<jint>(3);
    if (code == 0x07) constn<jint>(4);
    if (code == 0x08) constn<jint>(5);
    if (code == 0x09) constn<jlong>(0);
    if (code == 0x0A) constn<jlong>(1);
    if (code == 0x0B) constn<jfloat>(0);
    if (code == 0x0C) constn<jfloat>(1);
    if (code == 0x0D) constn<jfloat>(2);
    if (code == 0x0E) constn<jdouble>(0);
    if (code == 0x0F) constn<jdouble>(1);
    if (code == 0x10) push<jbyte>();
    if (code == 0x11) push<jshort>();
    if (code == 0x12) ldc();
    if (code == 0x13) ldc(true);
    if (code == 0x14) ldc(true, true);
    if (code == 0x15) load<jint>();
    if (code == 0x16) load<jlong>();
    if (code == 0x17) load<jfloat>();
    if (code == 0x18) load<jdouble>();
    if (code == 0x19) load<runtime::jobject>();
    if (code == 0x1A) loadn<jint>(0);
    if (code == 0x1B) loadn<jint>(1);
    if (code == 0x1C) loadn<jint>(2);
    if (code == 0x1D) loadn<jint>(3);
    if (code == 0x1E) loadn<jlong>(0);
    if (code == 0x1F) loadn<jlong>(1);
    if (code == 0x20) loadn<jlong>(2);
    if (code == 0x21) loadn<jlong>(3);
    if (code == 0x22) loadn<jfloat>(0);
    if (code == 0x23) loadn<jfloat>(1);
    if (code == 0x24) loadn<jfloat>(2);
    if (code == 0x25) loadn<jfloat>(3);
    if (code == 0x26) loadn<jdouble>(0);
    if (code == 0x27) loadn<jdouble>(1);
    if (code == 0x28) loadn<jdouble>(2);
    if (code == 0x29) loadn<jdouble>(3);
    if (code == 0x2A) loadn<runtime::jobject>(0);
    if (code == 0x2B) loadn<runtime::jobject>(1);
    if (code == 0x2C) loadn<runtime::jobject>(2);
    if (code == 0x2D) loadn<runtime::jobject>(3);
    if (code == 0x2E) panic(code); // {}
    if (code == 0x2F) panic(code); // {}
    if (code == 0x30) panic(code); // {}
    if (code == 0x31) panic(code); // {}
    if (code == 0x32) panic(code); // {}
    if (code == 0x33) panic(code); // {}
    if (code == 0x34) panic(code); // {}
    if (code == 0x35) panic(code); // {}
    if (code == 0x36) store<jint>();
    if (code == 0x37) store<jlong>();
    if (code == 0x38) store<jfloat>();
    if (code == 0x39) store<jdouble>();
    if (code == 0x3A) store<runtime::jobject>();
    if (code == 0x3B) storen<jint>(0);
    if (code == 0x3C) storen<jint>(1);
    if (code == 0x3D) storen<jint>(2);
    if (code == 0x3E) storen<jint>(3);
    if (code == 0x3F) storen<jlong>(0);
    if (code == 0x40) storen<jlong>(1);
    if (code == 0x41) storen<jlong>(2);
    if (code == 0x42) storen<jlong>(3);
    if (code == 0x43) storen<jfloat>(0);
    if (code == 0x44) storen<jfloat>(1);
    if (code == 0x45) storen<jfloat>(2);
    if (code == 0x46) storen<jfloat>(3);
    if (code == 0x47) storen<jdouble>(0);
    if (code == 0x48) storen<jdouble>(1);
    if (code == 0x49) storen<jdouble>(2);
    if (code == 0x4A) storen<jdouble>(3);
    if (code == 0x4B) storen<runtime::jobject>(0);
    if (code == 0x4C) storen<runtime::jobject>(1);
    if (code == 0x4D) storen<runtime::jobject>(2);
    if (code == 0x4E) storen<runtime::jobject>(3);
    if (code == 0x4F) panic(code); // {}
    if (code == 0x50) panic(code); // {}
    if (code == 0x51) panic(code); // {}
    if (code == 0x52) panic(code); // {}
    if (code == 0x53) panic(code); // {}
    if (code == 0x54) panic(code); // {}
    if (code == 0x55) panic(code); // {}
    if (code == 0x56) panic(code); // {}
    if (code == 0x57) pop(1);
    if (code == 0x58) pop(2);
    if (code == 0x59) dup(1);
    if (code == 0x5A) panic(code); // {}
    if (code == 0x5B) panic(code); // {}
    if (code == 0x5C) dup(2);
    if (code == 0x5D) panic(code); // {}
    if (code == 0x5E) panic(code); // {}
    if (code == 0x5F) swap();
    if (code == 0x60) add<jint>();
    if (code == 0x61) add<jlong>();
    if (code == 0x62) add<jfloat>();
    if (code == 0x63) add<jdouble>();
    if (code == 0x64) sub<jint>();
    if (code == 0x65) sub<jlong>();
    if (code == 0x66) sub<jfloat>();
    if (code == 0x67) sub<jdouble>();
    if (code == 0x68) mul<jint>();
    if (code == 0x69) mul<jlong>();
    if (code == 0x6A) mul<jfloat>();
    if (code == 0x6B) mul<jdouble>();
    if (code == 0x6C) div<jint>();
    if (code == 0x6D) div<jlong>();
    if (code == 0x6E) div<jfloat>();
    if (code == 0x6F) div<jdouble>();
    if (code == 0x70) rem<jint>();
    if (code == 0x71) rem<jlong>();
    if (code == 0x72) rem<jfloat>();
    if (code == 0x73) rem<jdouble>();
    if (code == 0x74) neg<jint>();
    if (code == 0x75) neg<jlong>();
    if (code == 0x76) neg<jfloat>();
    if (code == 0x77) neg<jdouble>();
    if (code == 0x78) shl<jint>();
    if (code == 0x79) shl<jlong>();
    if (code == 0x7A) shr<jint>();
    if (code == 0x7B) shr<jlong>();
    if (code == 0x7C) ushr<jint>();
    if (code == 0x7D) ushr<jlong>();
    if (code == 0x7E) _and<jint>();
    if (code == 0x7F) _and<jlong>();
    if (code == 0x80) _or<jint>();
    if (code == 0x81) _or<jlong>();
    if (code == 0x82) _xor<jint>();
    if (code == 0x83) _xor<jlong>();
    if (code == 0x84) inc<jint>();
    if (code == 0x85) n2m<jint, jlong>();
    if (code == 0x86) n2m<jint, jfloat>();
    if (code == 0x87) n2m<jint, jdouble>();
    if (code == 0x88) n2m<jlong, jint>();
    if (code == 0x89) n2m<jlong, jfloat>();
    if (code == 0x8A) n2m<jlong, jdouble>();
    if (code == 0x8B) n2m<jfloat, jint>();
    if (code == 0x8C) n2m<jfloat, jlong>();
    if (code == 0x8D) n2m<jfloat, jdouble>();
    if (code == 0x8E) n2m<jdouble, jint>();
    if (code == 0x8F) n2m<jdouble, jlong>();
    if (code == 0x90) n2m<jdouble, jfloat>();
    if (code == 0x91) n2m<jint, jbyte>();
    if (code == 0x92) n2m<jint, jchar>();
    if (code == 0x93) n2m<jint, jshort>();
    if (code == 0x94) cmp<jlong>();
    if (code == 0x95) cmp<jfloat>(-1);
    if (code == 0x96) cmp<jfloat>(1);
    if (code == 0x97) cmp<jdouble>(-1);
    if (code == 0x98) cmp<jdouble>(1);
    if (code == 0x99) _if<jint>([](jint v) -> bool {return v == 0;});
    if (code == 0x9A) _if<jint>([](jint v) -> bool {return v != 0;});
    if (code == 0x9B) _if<jint>([](jint v) -> bool {return v < 0;});
    if (code == 0x9C) _if<jint>([](jint v) -> bool {return v >= 0;});
    if (code == 0x9D) _if<jint>([](jint v) -> bool {return v > 0;});
    if (code == 0x9E) _if<jint>([](jint v) -> bool {return v <= 0;});
    if (code == 0x9F) if_cmp<jint>([](jint v1, jint v2) -> bool {return v1 == v2;});
    if (code == 0xA0) if_cmp<jint>([](jint v1, jint v2) -> bool {return v1 != v2;});
    if (code == 0xA1) if_cmp<jint>([](jint v1, jint v2) -> bool {return v1 < v2;});
    if (code == 0xA2) if_cmp<jint>([](jint v1, jint v2) -> bool {return v1 >= v2;});
    if (code == 0xA3) if_cmp<jint>([](jint v1, jint v2) -> bool {return v1 > v2;});
    if (code == 0xA4) if_cmp<jint>([](jint v1, jint v2) -> bool {return v1 <= v2;});
    if (code == 0xA5) if_cmp<runtime::jobject>([](runtime::jobject obj1, runtime::jobject obj2) -> bool {return obj1 == obj2;});
    if (code == 0xA6) if_cmp<runtime::jobject>([](runtime::jobject obj1, runtime::jobject obj2) -> bool {return obj1 != obj2;});
    if (code == 0xA7) _goto();
    if (code == 0xA8) panic(code); // {}
    if (code == 0xA9) panic(code); // {}
    if (code == 0xAA) panic(code); // {}
    if (code == 0xAB) panic(code); // {}
    if (code == 0xAC) panic(code); // {}
    if (code == 0xAD) panic(code); // {}
    if (code == 0xAE) panic(code); // {}
    if (code == 0xAF) panic(code); // {}
    if (code == 0xB0) panic(code); // {}
    if (code == 0xB1) panic(code); // {}
    if (code == 0xB2) getstatic();
    if (code == 0xB3) putstatic();
    if (code == 0xB4) getfield();
    if (code == 0xB5) putfield();
    if (code == 0xB6) invokevirtual();
    if (code == 0xB7) invokespecial();
    if (code == 0xB8) panic(code); // {}
    if (code == 0xB9) panic(code); // {}
    if (code == 0xBA) panic(code); // {}
    if (code == 0xBB) _new();
    if (code == 0xBC) panic(code); // {}
    if (code == 0xBD) panic(code); // {}
    if (code == 0xBE) panic(code); // {}
    if (code == 0xBF) panic(code); // {}
    if (code == 0xC0) checkcast();
    if (code == 0xC1) instanceof();
    if (code == 0xC2) panic(code); // {}
    if (code == 0xC3) panic(code); // {}
    if (code == 0xC4) panic(code); // {}
    if (code == 0xC5) panic(code); // {}
    if (code == 0xC6) panic(code); // {}
    if (code == 0xC7) panic(code); // {}
    if (code == 0xC8) panic(code); // {}
    if (code == 0xC9) panic(code); // {}
    if (code == 0xCA) panic(code); // {}
    if (code == 0xFE) panic(code); // {}
    if (code == 0xFF) panic(code); // {}
    if (code > 0xCA && code < 0xFE) {
        ERROR("unknown code: 0x%X", code);
        exit(0);
    }
}

