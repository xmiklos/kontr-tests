#ifndef GENERATOR_VARIABLE_H
#define GENERATOR_VARIABLE_H

#include "../kontr.h"
#include <iostream>
#include <algorithm>

namespace kontr {
namespace Generator {

template<typename T>
class Variable : public ::kontr::Variable<T> {
    using DataType = typename ::kontr::Variable<T>::DataType;
    using VariableType = typename ::kontr::Variable<T>::VariableType;
    using ::kontr::Variable<T>::instancePtr;

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
        if (instancePtr->out_ptr == nullptr) {
            instancePtr->report.create(Report::ERROR,
                                        "No out_ptr to print to variable declaration!");
            return true;
        }
        return false;
    }

    void printVariable() const {
        using namespace std;
        if (testOutPtr()) return;
        std::ostream& out = *(instancePtr->out_ptr);
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

public:
    using ::kontr::Variable<T>::variableType;
    using ::kontr::Variable<T>::variableName;
    using ::kontr::Variable<T>::dataType;
    using ::kontr::Variable<T>::data;

#define CONST(TYPE, VARIABLE) \
    Variable(TYPE VARIABLE) : \
        ::kontr::Variable<T>(VARIABLE) {} \
 \
    Variable(T& instance, TYPE VARIABLE) : \
        ::kontr::Variable<T>(instance, VARIABLE) {} \
 \
    Variable(const char* name, T& instance, TYPE VARIABLE) : \
        ::kontr::Variable<T>(name, instance, VARIABLE) { printVariable(); }

    CONST(int, i)
    CONST(bool, b)
    CONST(double, f)
    CONST(const char*, s)
#undef CONST

    using ::kontr::Variable<T>::__setInstance;

    virtual void __generate(std::ostream &out) const {
        switch(variableType) {
        case VariableType::Constant: printScalar(out); return;
        case VariableType::Variable: out << '$' << variableName; return;
        }
    }

    Variable&  operator=(const Variable& other) {
        if (this != &other) {
            if (testOutPtr()) return *this;
            std::ostream& out = *(instancePtr->out_ptr);
            out << '$' << variableName << " = " << other << ";" << std::endl;
            this->data = other.data;
            this->dataType = other.dataType;
        }
        return *this;
    }

    Variable& toInt() {
        return *this;
    }

    Variable& toFloat() {
        return *this;
    }

    Variable& toBool() {
        return *this;
    }

    Variable& toString() {
        return *this;
    }

};

}
}

#endif // VARIABLE_H
