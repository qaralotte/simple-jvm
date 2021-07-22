#include <locale>
#include <codecvt>

#include "include/runtime/metaspace/string.h"
#include "include/runtime/metaspace/clazz.h"
#include "include/runtime/metaspace/object.h"

using namespace runtime;

map<string, jobject> JString::pool;

jobject JString::toObject() {
    if (pool.find(raw_string) != pool.end()) return pool[raw_string];
    jstring utf16 = wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(raw_string);
    auto jchars = ClassLoader("[C").loadClass() -> initArrayWithData<jstring>(utf16);
    auto jstr = ClassLoader("java/lang/String").loadClass() -> toObject();
    jstr -> setRefVal("value", "[C", jchars);
    pool[raw_string] = jstr;
    return jstr;
}