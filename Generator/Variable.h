#ifndef GENERATOR_VARIABLE_H
#define GENERATOR_VARIABLE_H

#include "../kontr-internal.h"
#include <iostream>


namespace kontr {
namespace Generator {

using DataType = ::kontr::Variable::DataType;
using VariableType = ::kontr::Variable::VariableType;

template<typename T>
class Variable : public ::kontr::Variable::Data<T> {

    static void printString(std::ostream& out, const std::string& str) {
        using namespace std;
        size_t s = count(str.begin(), str.end(), '\''); //Number of single quotes
        if (s == 0) {
            out << '\'' << str << '\'';
            return;
        }
        size_t d = count(str.begin(), str.end(), '\"'); //Number of double quotes
        if (d == 0) {
            out << '"' << str << '"';
            return;
        }
        //There are both, choose the one with less occurences
        const char delimiter = (s <= d ? '\'' : '"');
        out << delimiter;
        for (const char c : str) {
            if (c == delimiter || c == '\\') {
                out << '\\';
            }
            out << c;
        }
        out << delimiter;
    }

    bool testOutPtr () const {
        if (T::instance().storage.out_ptr == nullptr) {
            T::instance().report.create(Report::ERROR,
                                        "No out_ptr to print to variable declaration!");
            return true;
        }
        return false;
    }

    void printVariable() const {
        if (testOutPtr()) return;
        std::ostream& out = *(T::instance().storage.out_ptr);
        out << '$' << variableName << " = ";
        printScalar(out);
        out << ";" << std::endl;
    }

    void printScalar(std::ostream& out) const {
        switch (dataType) {
        case DataType::Int: out << data.Int; return;
        case DataType::Float: out << data.Float; return;
        case DataType::Bool: out << data.Bool; return;
        case DataType::String: printString(out, data.String); return;
        }
    }

    /// Convert to string. Add parentheses if it is an expression
    std::string __toString(bool parentheses) const {
        std::stringstream ss;
        if (parentheses && this->variableType == VariableType::Expression) {
            ss << "(";
        }

        this->__generate(ss);

        if (parentheses && this->variableType == VariableType::Expression) {
            ss << ")";
        }
        return ss.str();
    }

public:
    explicit Variable(const DataType& type, const std::string& expression) :
        ::kontr::Variable::Data<T>(type, expression) {}

    using ::kontr::Variable::Data<T>::variableType;
    using ::kontr::Variable::Data<T>::variableName;
    using ::kontr::Variable::Data<T>::dataType;
    using ::kontr::Variable::Data<T>::data;

#define CONST(TYPE, VARIABLE) \
    Variable(TYPE VARIABLE) : \
        ::kontr::Variable::Data<T>(VARIABLE) {} \
 \
    Variable(const char* name, TYPE VARIABLE) : \
        ::kontr::Variable::Data<T>(name, VARIABLE) { printVariable(); }

    CONST(int, i)
    CONST(bool, b)
    CONST(double, f)
    CONST(const char*, s)
#undef CONST
    Variable(const char* name, const ::kontr::Variable::Delegator<T>& other) :
        ::kontr::Variable::Data<T>(name, other) {
        if (testOutPtr()) return;
        std::ostream& out = *(T::instance().storage.out_ptr);
        out << '$' << variableName << " = " << other << ';' << std::endl;
    }

    virtual void __generate(std::ostream &out) const {
        switch(variableType) {
        case VariableType::Constant: printScalar(out); return;
        case VariableType::Variable: out << '$' << variableName; return;
        case VariableType::Expression: out << data.Expression; return;
        }
    }

    Variable& operator=(const Variable& other) {
        if (this != &other) {
            if (testOutPtr()) return *this;
            std::ostream& out = *(T::instance().storage.out_ptr);
            __generate(out);
            out << " = " << other << ";" << std::endl;
            this->data = other.data;
            this->dataType = other.dataType;
        }
        return *this;
    }

//Return expression
#define RET_EXP(TYPE, EXP) return ::kontr::Variable::Delegator<T>::__create( Variable(DataType::TYPE, EXP));
    virtual ::kontr::Variable::Delegator<T> toInt() const {
        if (dataType == DataType::Int) {
            return ::kontr::Variable::Delegator<T>::__create(*this);
        }
        RET_EXP(Int, "0 + " + this->__toString(true));
    }

    virtual ::kontr::Variable::Delegator<T> toFloat() const {
        if (dataType == DataType::Float) {
            return ::kontr::Variable::Delegator<T>::__create(*this);
        }
        RET_EXP(Float, "0.0 + " + this->__toString(true));
    }

    virtual ::kontr::Variable::Delegator<T> toBool() const {
        if (dataType == DataType::Bool) {
            return ::kontr::Variable::Delegator<T>::__create(*this);
        }
        RET_EXP(Bool, "!!" + this->__toString(true));
    }

    virtual ::kontr::Variable::Delegator<T> toString() const {
        if (dataType == DataType::String) {
            return ::kontr::Variable::Delegator<T>::__create(*this);
        }
        RET_EXP(String, "\"\" . " + this->__toString(true));
    }

    virtual ::kontr::Variable::Delegator<T> operator== (const ::kontr::Variable::Delegator<T>& o) const {
        const Variable& other = o.__getDelegate();
        if (this->dataType == DataType::String || other.dataType == DataType::String) {
            RET_EXP(Bool, this->__toString(true) + " eq " + other.__toString(true));
        }
        else {
            RET_EXP(Bool, this->__toString(true) + " == " + other.__toString(true));
        }
    }

    virtual ::kontr::Variable::Delegator<T> operator!= (const ::kontr::Variable::Delegator<T>& o) const {
        const Variable& other = o.__getDelegate();
        if (this->dataType == DataType::String || other.dataType == DataType::String) {
            RET_EXP(Bool, this->__toString(true) + " ne " + other.__toString(true));
        }
        else {
            RET_EXP(Bool, this->__toString(true) + " != " + other.__toString(true));
        }
    }

    virtual ::kontr::Variable::Delegator<T> operator!() const {
        RET_EXP(Bool, "!" + this->__toString(true));
    }

    virtual ::kontr::Variable::Delegator<T> operator&& (const ::kontr::Variable::Delegator<T>& o) const {
        const Variable& other = o.__getDelegate();
        RET_EXP(Bool, this->__toString(true) + " && " + other.__toString(true));
    }

    virtual ::kontr::Variable::Delegator<T> operator|| (const ::kontr::Variable::Delegator<T>& o) const {
        const Variable& other = o.__getDelegate();
        RET_EXP(Bool, this->__toString(true) + " || " + other.__toString(true));
    }

    virtual ::kontr::Variable::Delegator<T> operator+ (const ::kontr::Variable::Delegator<T>& o) const {
        const Variable& other = o.__getDelegate();
        if (this->dataType == DataType::String || other.dataType == DataType::String) {
            RET_EXP(String, this->__toString(true) + " . " + other.__toString(true));
        }
        else if (this->dataType == DataType::Float || other.dataType == DataType::Float) {
            RET_EXP(Float, this->__toString(true) + " + " + other.__toString(true));
        }
        else if (this->dataType == DataType::Int || other.dataType == DataType::Int) {
            RET_EXP(Int, this->__toString(true) + " + " + other.__toString(true));
        }
        else { //Possibly error?
            RET_EXP(Bool, this->__toString(true) + " + " + other.__toString(true));
        }
    }

#undef RET_EXP

};

} //Generator
} //kontr
#endif // VARIABLE_H
