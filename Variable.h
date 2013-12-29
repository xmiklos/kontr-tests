#ifndef VARIABLE_H
#define VARIABLE_H

#include "kontr.h"
#include <string>

namespace kontr {
namespace Variable {

/// Pure interface without any data storage
/// To be used by Delegator, rest should use Data
template <typename T>
class Interface {
public:
    /// Print representation to ostream (for Generation)
    virtual void __generate(std::ostream& out) const = 0;

    /// Operator for printing
    friend std::ostream& operator<< (std::ostream& stream, const Interface& var) {
        var.__generate(stream);
        return stream;
    }

public:
#define VARIABLE_CONST(TYPE, VARIABLE) \
    Interface(TYPE VARIABLE) \
        { ::kontr::unused(VARIABLE); } \
 \
    Interface(const char* name, TYPE VARIABLE) \
        { ::kontr::unused(name, VARIABLE); }

    VARIABLE_CONST(int, i)
    VARIABLE_CONST(bool, b)
    VARIABLE_CONST(double, f)
    VARIABLE_CONST(const char*, s)
#undef VARIABLE_CONST

    virtual ~Interface() {}
};

/// Data storing version of Interface
template <typename T>
class Data : public Interface<T> {
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

public:
    /// Type of variable (Constant, Variable, ...)
    VariableType variableType;

    /// Type of data (Int, Float, ...)
    DataType dataType;

    /// Stored data
    Storage data;

    /// Name of variable
    std::string variableName;

public:
#define VARIABLE_CONST(TYPE, INT_TYPE, VARIABLE) \
    Data(TYPE VARIABLE) : \
        Interface<T>(VARIABLE), \
        variableType(VariableType::Constant), \
        dataType(INT_TYPE), \
        data((TYPE) VARIABLE) {} \
 \
    Data(const char* name, TYPE VARIABLE): \
        Interface<T>(name, VARIABLE), \
        variableType(VariableType::Variable), \
        dataType(INT_TYPE), \
        data((TYPE) VARIABLE), \
        variableName(name) {}

    VARIABLE_CONST(int, DataType::Int, i)
    VARIABLE_CONST(bool, DataType::Bool, b)
    VARIABLE_CONST(double, DataType::Float, f)
    VARIABLE_CONST(const char*, DataType::String, s)

#undef VARIABLE_CONST

    virtual ~Data() = default;
};

/// Empty Variable
template<typename T>
class Empty : public Data<T> {
public:
    using Data<T>::Data;

    virtual void __generate(std::ostream &out) const { kontr::unused(out); }
};

/// Delegator
template<typename T>
class Delegator : public Interface<T> {
    typename T::Variable delegate;
public:
    /// For testing purposes only!
    const typename T::Variable& __getDelegate() const {
        return delegate;
    }

#define DELEGATOR_CONST(TYPE, VARIABLE) \
    Delegator(TYPE VARIABLE) : \
        Interface<T>(VARIABLE), delegate(VARIABLE) {} \
 \
    Delegator(const char* name, TYPE VARIABLE) : \
        Interface<T>(name, VARIABLE), \
        delegate(name, VARIABLE) {}

    DELEGATOR_CONST(int, i)
    DELEGATOR_CONST(bool, b)
    DELEGATOR_CONST(double, f)
    DELEGATOR_CONST(const char*, s)
#undef DELEGATOR_CONST

    virtual void __generate(std::ostream& out) const {
        delegate.__generate(out);
    }

    Delegator& toInt() {
        delegate = delegate.toInt();
        return *this;
    }

    Delegator& toFloat() {
        delegate = delegate.toFloat();
        return *this;
    }

    Delegator& toBool() {
        delegate = delegate.toBool();
        return *this;
    }

    Delegator& toString() {
        delegate = delegate.toString();
        return *this;
    }
};

} //Variable
} //kontr
#endif // VARIABLE_H
