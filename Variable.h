#ifndef VARIABLE_H
#define VARIABLE_H

#include "kontr.h"
#include <string>

namespace kontr {

template <typename T>
class Variable {
public:
    enum class VariableType {
        Constant,
        Variable
    };
    static const unsigned int VARIABLE_TYPE_LENGTH = 2;

    enum class DataType {
        Int,
        Bool,
        Float,
        String
    };
    static const unsigned int DATA_TYPE_LENGTH = 4;

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
    /// Type of variable (Constant, Variable)
    VariableType variableType;

    /// Type of data (Int, Float, ...)
    DataType dataType;

    /// Stored data
    Storage data;

    /// Name of variable
    std::string variableName;

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

    Variable& operator=(const Variable&) = default;
    Variable& operator=(Variable&&) = default;
    Variable(const Variable&) = default;

public:
#define VARIABLE_CONST(TYPE, INT_TYPE, VARIABLE) \
    Variable(TYPE VARIABLE) : \
        variableType(VariableType::Constant), \
        dataType(INT_TYPE), \
        data((TYPE) VARIABLE) {} \
 \
    Variable(T& instance, TYPE VARIABLE): \
        instancePtr(&instance), \
        variableType(VariableType::Constant), \
        dataType(INT_TYPE), \
        data((TYPE) VARIABLE) {} \
 \
    Variable(const char* name, T& instance, TYPE VARIABLE): \
        instancePtr(&instance), \
        variableType(VariableType::Variable), \
        dataType(INT_TYPE), \
        data((TYPE) VARIABLE), \
        variableName(name) {}

    VARIABLE_CONST(int, DataType::Int, i)
    VARIABLE_CONST(bool, DataType::Bool, b)
    VARIABLE_CONST(double, DataType::Float, f)
    VARIABLE_CONST(const char*, DataType::String, s)

#undef VARIABLE_CONST

    virtual ~Variable() = default;
};

template<typename T>
class VariableDelegator : public Variable<T> {
    typename T::Variable delegate;
public:
    typename T::Variable& __getDelegate() {
        return delegate;
    }

#define DELEGATOR_CONST(TYPE, VARIABLE) \
    VariableDelegator(TYPE VARIABLE) : \
        Variable<T>(VARIABLE), delegate(VARIABLE) {} \
 \
    VariableDelegator(T& instance, TYPE VARIABLE) : \
        Variable<T>(instance, VARIABLE), \
        delegate(instance, VARIABLE) {} \
 \
    VariableDelegator(const char* name, T& instance, TYPE VARIABLE) : \
        Variable<T>(name, instance, VARIABLE), \
        delegate(name, instance, VARIABLE) {}

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

    VariableDelegator& operator=(const VariableDelegator& other) {
        if (this != &other) {
            delegate = other.delegate;
        }
        return *this;
    }

    VariableDelegator& operator=(VariableDelegator&& other) {
        delegate = other.delegate;
        return *this;
    }

    VariableDelegator(const VariableDelegator&) = default;

    VariableDelegator& toInt() {
        delegate = delegate.toInt();
        return *this;
    }

    VariableDelegator& toFloat() {
        delegate = delegate.toFloat();
        return *this;
    }

    VariableDelegator& toBool() {
        delegate = delegate.toBool();
        return *this;
    }

    VariableDelegator& toString() {
        delegate = delegate.toString();
        return *this;
    }
};

}
#endif // VARIABLE_H
