#ifndef VARIABLE_H
#define VARIABLE_H

#include "kontr.h"
#include <string>

namespace kontr {

template <typename T>
class Variable {
public:
    enum class Type {
        Int,
        Bool,
        Float,
        String
    };
    const unsigned int TYPE_LENGTH = 4;

    //TODO - change to union (not easy because of String)
    //http://www.informit.com/guides/content.aspx?g=cplusplus&seqNum=556
    struct Storage {
        int Int;
        bool Bool;
        double Float;
        std::string String;

        Storage(int i) : Int(i) {}
        Storage(bool b) : Bool(b) {}
        Storage(double f) : Float(f) {}
        Storage(const char* c) : String(c) {}
    };
protected:
    mutable T* instancePtr = nullptr;

    Variable() = delete;

public:
    Type type;
    Storage data;

    /// Required for methods taking Variable
    virtual void __setInstance(T& instance) const {
        instancePtr = &instance;
    }

    /// Print representation to ostream (for Generation)
    virtual void __generate(std::ostream& out) const = 0;

    /// Operator for printing
    friend std::ostream& operator<< (std::ostream& stream, const Variable& var) {
        var.__generate(stream);
        return stream;
    }

public:
#define VARIABLE_CONST(TYPE, INT_TYPE, VARIABLE) \
    Variable(TYPE VARIABLE) : \
        type(INT_TYPE), \
        data((TYPE) VARIABLE) {} \
 \
    Variable(T& instance, TYPE VARIABLE): \
        instancePtr(&instance), \
        type(INT_TYPE), \
        data((TYPE) VARIABLE) {}

    VARIABLE_CONST(int, Type::Int, i)
    VARIABLE_CONST(bool, Type::Bool, b)
    VARIABLE_CONST(double, Type::Float, f)
    VARIABLE_CONST(const char*, Type::String, s)

#undef VARIABLE_CONST

    virtual ~Variable() = default;
};

template<typename T>
class VariableDelegator : public Variable<T> {
    typename T::Variable delegate;
public:

#define DELEGATOR_CONST(TYPE, VARIABLE) \
    VariableDelegator(TYPE VARIABLE) : \
        Variable<T>(VARIABLE), delegate(VARIABLE) {} \
 \
    VariableDelegator(T& instance, TYPE VARIABLE) : \
        Variable<T>(instance, VARIABLE), \
        delegate(instance, VARIABLE) {}

    DELEGATOR_CONST(int, i)
    DELEGATOR_CONST(bool, b)
    DELEGATOR_CONST(double, f)
    DELEGATOR_CONST(const char*, s)
#undef DELEGATOR_CONST

    VariableDelegator() = delete;

    virtual void __setInstance(T &instance) const {
        this->Variable<T>::__setInstance(instance);
        delegate.__setInstance(instance);
    }

    virtual void __generate(std::ostream& out) const {
        delegate.__generate(out);
    }
};

}
#endif // VARIABLE_H
