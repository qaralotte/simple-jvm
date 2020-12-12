#include "include/jvm/engine/engine.h"
#include "include/log.h"

int main() {

    Engine::init();
    classfile::ClassFile clazz = Engine::load("../test/Main.class");

    ClassLoader::release(clazz);
    return 0;
}
