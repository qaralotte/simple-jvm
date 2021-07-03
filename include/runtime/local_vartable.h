#ifndef JVM_RUNTIME_LOCAL_H
#define JVM_RUNTIME_LOCAL_H

#include <memory>
#include <vector>

#include "include/std.h"

#include "slot.h"
#include "heap/object.h"

namespace runtime {

    class LocalVariableTable {
    private:
        uint capacity;
        vector<Slot> slots;
    public:
        LocalVariableTable() = default;
        explicit LocalVariableTable(uint _capacity) : capacity(_capacity) {slots.resize(capacity);};
        uint getCapacity();
    public:
        template<typename T> T get(uint);
        template<typename T> void set(uint, T);
    };
}

#endif //JVM_RUNTIME_LOCAL_H
