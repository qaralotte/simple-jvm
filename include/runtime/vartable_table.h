#ifndef JVM_RUNTIME_LOCAL_H
#define JVM_RUNTIME_LOCAL_H

#include <memory>
#include <vector>

#include "include/std.h"

#include "slot.h"

namespace runtime {

    class VariableTable {
    private:
        uint capacity = 0;
        bool mut = true;
        vector<Slot> slots;
    public:
        VariableTable() = default;
        explicit VariableTable(uint _capacity) : capacity(_capacity), mut(false) {slots.resize(capacity);};
        vector<Slot> getSlots() const;
    public:
        template<typename T> T get(uint);
        template<typename T> void set(uint, T);
    };
}

#endif //JVM_RUNTIME_LOCAL_H
