#ifndef GENERATOR_VARIABLE_H
#define GENERATOR_VARIABLE_H

#include "../kontr.h"
#include <iostream>
#include <algorithm>
#include <sstream>

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
        using namespace std;
        if (testOutPtr()) return;
        std::ostream& out = *(T::instance().storage.out_ptr);
        out << '$' << variableName << " = ";
        printScalar(out);
        out << ";" << endl;
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

    explicit Variable(const DataType& type, const std::string& expression) :
        ::kontr::Variable::Data<T>(type, expression) {}

public:
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
            out << '$' << variableName << " = " << other << ";" << std::endl;
            this->data = other.data;
            this->dataType = other.dataType;
        }
        return *this;
    }

    virtual ::kontr::Variable::Delegator<T> toInt() const {
        if (dataType == DataType::Int) {
            return ::kontr::Variable::Delegator<T>::__create(*this);
        }
        return ::kontr::Variable::Delegator<T>::__create( Variable(DataType::Int, "0 + " + this->__toString(true)) );
    }

    virtual ::kontr::Variable::Delegator<T> toFloat() const {
        if (dataType == DataType::Float) {
            return ::kontr::Variable::Delegator<T>::__create(*this);
        }
        return ::kontr::Variable::Delegator<T>::__create( Variable(DataType::Int, "0.0 + " + this->__toString(true)) );
    }

    virtual ::kontr::Variable::Delegator<T> toBool() const {
        if (dataType == DataType::Bool) {
            return ::kontr::Variable::Delegator<T>::__create(*this);
        }
        return ::kontr::Variable::Delegator<T>::__create( Variable(DataType::Int, "!!" + this->__toString(true)) );
    }

    virtual ::kontr::Variable::Delegator<T> toString() const {
        if (dataType == DataType::String) {
            return ::kontr::Variable::Delegator<T>::__create(*this);
        }
        return ::kontr::Variable::Delegator<T>::__create( Variable(DataType::Int, "\"\" . " + this->__toString(true)) );
    }

    virtual ::kontr::Variable::Delegator<T> operator== (const ::kontr::Variable::Delegator<T>& o) const {
        const Variable& other = o.__getDelegate();
        if (this->dataType == DataType::String && other.dataType == DataType::String) {
            return ::kontr::Variable::Delegator<T>::__create(
                            Variable(DataType::Bool, this->__toString(true) + " eq " + other.__toString(true))
                        );
        }
        else {
            return ::kontr::Variable::Delegator<T>::__create(
                            Variable(DataType::Bool, this->__toString(true) + " == " + other.__toString(true))
                        );
        }
    }

    virtual ::kontr::Variable::Delegator<T> operator!= (const ::kontr::Variable::Delegator<T>& o) const {
        const Variable& other = o.__getDelegate();
        if (this->dataType == DataType::String && other.dataType == DataType::String) {
            return ::kontr::Variable::Delegator<T>::__create(
                            Variable(DataType::Bool, this->__toString(true) + " ne " + other.__toString(true))
                        );
        }
        else {
            return ::kontr::Variable::Delegator<T>::__create(
                            Variable(DataType::Bool, this->__toString(true) + " != " + other.__toString(true))
                        );
        }
    }

};

} //Generator
} //kontr
#endif // VARIABLE_H
