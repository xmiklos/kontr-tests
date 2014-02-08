#ifndef VARIABLE_H
#define VARIABLE_H

#include "kontr-internal.h"

namespace kontr {
namespace Variable {

template <typename T>
class Delegator;

enum class VariableType {
    Constant,
    Variable,
    Expression
};
static const unsigned int VARIABLE_TYPE_LENGTH = 3;

enum class DataType {
    Int,
    Bool,
    Float,
    String,
    Array
};
static const unsigned int DATA_TYPE_LENGTH = 4;

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
protected:
    explicit Interface(const DataType& type, const std::string& expression)
    { ::kontr::unused(type, expression); }
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
    VARIABLE_CONST(std::initializer_list<Delegator<T>>, array)
#undef VARIABLE_CONST
    Interface(const char* name, const Delegator<T>& other)
    { ::kontr::unused(name, other); }

    virtual ~Interface() {}

    /**
      Convert to Int
     * @brief toInt
     * @return
     */
    virtual Delegator<T> toInt() const = 0;

    /**
      Convert to Float
     * @brief toFloat
     * @return
     */
    virtual Delegator<T> toFloat() const = 0;

    /**
      Convert to Bool
     * @brief toBool
     * @return
     */
    virtual Delegator<T> toBool() const = 0;

    /**
      Convert to String
     * @brief toString
     * @return
     */
    virtual Delegator<T> toString() const = 0;

    /**
      Comparison operator
     * @brief operator ==
     * @return
     */
    virtual Delegator<T> operator== (const Delegator<T>& other) const = 0;

    /**
      Comparison operator
     * @brief operator !=
     * @return
     */
    virtual Delegator<T> operator!= (const Delegator<T>& other) const = 0;

    virtual Delegator<T> operator! () const = 0;
    virtual Delegator<T> operator&& (const Delegator<T>& other) const = 0;
    virtual Delegator<T> operator|| (const Delegator<T>& other) const = 0;
    virtual Delegator<T> operator+ (const Delegator<T>& other) const = 0;

    /**
      Array subscript and string character
     * @brief operator []
     * @param other
     * @return
     */
    virtual Delegator<T> operator[] (const Delegator<T>& other) const = 0;

    /**
      Get the size of an array or string
     * @brief size
     * @return
     */
    virtual Delegator<T> size() const = 0;

    /**
      Substring of a string
     * @brief substr
     * @param from
     * @param to
     * @return
     */
    virtual Delegator<T> substr(const Delegator<T>& from, const Delegator<T>& to) const = 0;


};

/// Data storing version of Interface
template <typename T>
class Data : public Interface<T> {
public:

    //TODO - change to union (not easy because of String)
    //http://www.informit.com/guides/content.aspx?g=cplusplus&seqNum=556
    struct Storage {
        int Int;
        bool Bool;
        double Float;
        std::string String;
        std::string Expression;
        std::vector<Delegator<T>> Array;

        Storage(int i) : Int(i) {}
        Storage(bool b) : Bool(b) {}
        Storage(double f) : Float(f) {}
        Storage(const char* c) : String(c) {}
        Storage(std::initializer_list<Delegator<T>> a) : Array(a) {}
        explicit Storage(const std::string& e) :
            Expression(e) {}
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

protected:
    explicit Data (const DataType& type, const std::string& expression) :
        Interface<T>(type, expression),
        variableType(VariableType::Expression),
        dataType(type),
        data(expression)
    {}


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
    VARIABLE_CONST(std::initializer_list<Delegator<T>>, DataType::Array, a)

#undef VARIABLE_CONST
    Data(const char* name, const Delegator<T>& other) :
        Data(other.__getDelegate()) {
        //The other can only be Variable or Expression -> convert to Variable
        this->variableName = name;
        this->variableType = VariableType::Variable;
    }

    virtual ~Data() = default;
};

/// Empty Variable
template<typename T>
class Empty : public Data<T> {
public:
    using Data<T>::Data;

    virtual void __generate(std::ostream &out) const { kontr::unused(out); }

#define DELEGATE return Delegator<T>::__create(dynamic_cast<const typename T::Variable&>(*this));

    virtual Delegator<T> toInt() const { DELEGATE }
    virtual Delegator<T> toFloat() const { DELEGATE }
    virtual Delegator<T> toBool() const { DELEGATE }
    virtual Delegator<T> toString() const { DELEGATE }

    virtual Delegator<T> operator== (const Delegator<T>&) const { DELEGATE }
    virtual Delegator<T> operator!= (const Delegator<T>&) const { DELEGATE }
    virtual Delegator<T> operator! () const { DELEGATE }
    virtual Delegator<T> operator&& (const Delegator<T>&) const { DELEGATE }
    virtual Delegator<T> operator|| (const Delegator<T>&) const { DELEGATE }
    virtual Delegator<T> operator+ (const Delegator<T>&) const { DELEGATE }
    virtual Delegator<T> operator[] (const Delegator<T>&) const { DELEGATE }

    virtual Delegator<T> size() const { DELEGATE }
    virtual Delegator<T> substr(const Delegator<T> &, const Delegator<T> &) const { DELEGATE }

#undef DELEGATE

};

/// Delegator
template<typename T>
class Delegator : public Interface<T> {
    typename T::Variable delegate;

    Delegator(const typename T::Variable& create) :
        Interface<T>(create), delegate(create) {}
    Delegator(typename T::Variable&& create) :
        Interface<T>(create), delegate(create) {}

public:
    static Delegator __create(const typename T::Variable& create) {
        return Delegator(create);
    }

    static Delegator __create(typename T::Variable&& create) {
        return Delegator(create);
    }

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
    DELEGATOR_CONST(std::initializer_list<Delegator<T>>, a)
#undef DELEGATOR_CONST

    Delegator(const char* name, const Delegator<T>& other) :
        Interface<T>(name, other),
        delegate(name, other) {}

    virtual void __generate(std::ostream& out) const {
        delegate.__generate(out);
    }

    virtual Delegator toInt() const {
        return delegate.toInt();
    }

    virtual Delegator toFloat() const {
        return delegate.toFloat();
    }

    virtual Delegator toBool() const {
        return delegate.toBool();
    }

    virtual Delegator toString() const {
        return delegate.toString();
    }

    virtual Delegator<T> operator== (const Delegator<T>& other) const {
        return delegate == other;
    }

    virtual Delegator<T> operator!= (const Delegator<T>& other) const {
        return delegate != other;
    }

    virtual Delegator<T> operator! () const { return !delegate; }

    virtual Delegator<T> operator&& (const Delegator<T>& other) const {
        return delegate && other;
    }

    virtual Delegator<T> operator|| (const Delegator<T>& other) const {
        return delegate || other;
    }

    virtual Delegator<T> operator+ (const Delegator<T>& other) const {
        return delegate + other;
    }

    virtual Delegator<T> operator[] (const Delegator<T>& other) const {
        return delegate[other];
    }

    virtual Delegator<T> size() const {
        return delegate.size();
    }

    virtual Delegator<T> substr(const Delegator<T> &from, const Delegator<T> &to) const {
        return delegate.substr(from, to);
    }

#define OP_DEL_SINGLE(OP, TYPE) friend Delegator<T> operator OP (TYPE first, const Delegator<T>& other) \
        { return Delegator<T>(first) OP other; }
#define OP_DEL(OP) OP_DEL_SINGLE(OP, bool) OP_DEL_SINGLE(OP, int) OP_DEL_SINGLE(OP, double) OP_DEL_SINGLE(OP, const char *)

    OP_DEL(+)
    OP_DEL(||)
    OP_DEL(&&)
    OP_DEL(==)
    OP_DEL(!=)

#undef OP_DEL_SINGLE
#undef OP_DEL


};

} //Variable
} //kontr
#endif // VARIABLE_H
