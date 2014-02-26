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
        const char * chars = "\t\n";
        std::map<char, string> replace;
        replace['\t'] = "\\t"; replace['\n'] = "\\n";
        bool specialChars = false;
        char delimiter;
        if (find_first_of(str.begin(), str.end(), &chars[0], &chars[2]) != str.end()) {
            specialChars = true;
            delimiter = '"';
        }
        else {
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
            delimiter = (s <= d ? '\'' : '"');
        }
        out << delimiter;
        for (const char c : str) {
            if (specialChars) {
                auto it = replace.find(c);
                if (it != replace.end()) {
                    out << it->second;
                    continue;
                }
            }
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
        std::string& indent = T::instance().storage.indent;
        out << indent << "my " << (dataType == DataType::Array ? '@' : '$')
            << variableName << " = ";
        printScalar(out);
        out << ";" << std::endl;
    }

    void printScalar(std::ostream& out) const {
        switch (dataType) {
        case DataType::Int: out << data.Int; return;
        case DataType::Float: out << data.Float; return;
        case DataType::Bool: out << data.Bool; return;
        case DataType::String: printString(out, data.String); return;
        case DataType::Array:
            out << '(';
            bool first = true;
            for (const ::kontr::Variable::Delegator<T>& i : data.Array) {
                if (!first) out << ", ";
                i.__generate(out);
                first = false;
            }
            out << ')';
        }
    }
private:
    void checkArray(const std::initializer_list < ::kontr::Variable::Delegator<T> >& arr) const {
        unsigned counter = 1;
        DataType type;
        for (const ::kontr::Variable::Delegator<T>& i : arr) {
            if (i.__getDelegate().dataType == DataType::Array) {
                T::instance().report.create(Report::ERROR,
                                            "Array of arrays is not valid");
            }
            if (counter == 1) {
                type = i.__getDelegate().dataType;
            }
            else {
                if (i.__getDelegate().dataType != type) {
                    T::instance().report.create(Report::ERROR,
                                                "Element number " + std::to_string(counter) + " has different type then the first element");
                }
            }
            ++ counter;
        }
    }

public:
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
    //Array constructor has to check the type
    Variable(std::initializer_list< ::kontr::Variable::Delegator<T> > arr) :
        ::kontr::Variable::Data<T>(arr) {
        checkArray(arr);
    }

    Variable(const char* name, std::initializer_list< ::kontr::Variable::Delegator<T> > arr) :
        ::kontr::Variable::Data<T>(name, arr) {
        checkArray(arr);
        printVariable();
    }

    Variable(const char* name, const ::kontr::Variable::Delegator<T>& other) :
        ::kontr::Variable::Data<T>(name, other) {
        if (testOutPtr()) return;
        std::ostream& out = *(T::instance().storage.out_ptr);
        std::string& indent = T::instance().storage.indent;
        out << indent << "my " << (other.__getDelegate().dataType == DataType::Array ? '@' : '$')
            << variableName << " = " << other << ';' << std::endl;
    }

    virtual void __generate(std::ostream &out) const {
        switch(variableType) {
        case VariableType::Constant: printScalar(out); return;
        case VariableType::Variable: out << (dataType == DataType::Array ? '@' : '$')
                                         << variableName; return;
        case VariableType::Expression: out << data.Expression; return;
        }
    }

    Variable& operator=(const Variable& other) {
        if (this != &other) {
            if (testOutPtr()) return *this;
            if (T::instance().storage.inArrayGenerating) {
                std::ostream& out = *(T::instance().storage.out_ptr);
                std::string& indent = T::instance().storage.indent;
                if (variableType == VariableType::Variable) {
                    out << indent << (other.dataType == DataType::Array ? '@' : '$') << variableName;
                }
                else {
                    __generate(out);
                }
                out << " = " << other << ";" << std::endl;
            }

            bool inArray = T::instance().storage.inArrayGenerating;
            T::instance().storage.inArrayGenerating = false;
            //The operator= will print something if the array is not empty

            this->data = other.data;
            this->dataType = other.dataType;

            T::instance().storage.inArrayGenerating = inArray;
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

    virtual ::kontr::Variable::Delegator<T> operator[] (const ::kontr::Variable::Delegator<T>& o) const {
        const Variable& other = o.__getDelegate();
        if (other.dataType == DataType::Int &&
                dataType == DataType::Array &&
                data.Array.size()) {
            //The elements have all the same type, so it is the first one
            return ::kontr::Variable::Delegator<T>::__create(
                        Variable(data.Array[0].__getDelegate().dataType, std::string("$") + variableName + "[" + other.__toString(true) + "]")
                    );
        }
        else if (dataType == DataType::String) {
            return substr(o, 1);
        }
        //Error
        RET_EXP(Bool, this->__toString(true) + "[" + other.__toString(true) + "]");
    }

    virtual ::kontr::Variable::Delegator<T> size() const {
        if (dataType == DataType::Array) {
            RET_EXP(Int, "scalar " + this->__toString(true));
        }
        else if (dataType == DataType::String) {
            RET_EXP(Int, "length " + this->__toString(true));
        }
        //Error
        return toInt();
    }

    virtual ::kontr::Variable::Delegator<T> substr (
            const ::kontr::Variable::Delegator<T> &from,
            const ::kontr::Variable::Delegator<T> &to) const {
        const Variable& f = from.__getDelegate(), &t = to.__getDelegate();
        if (dataType == DataType::String) {
            RET_EXP(String, "substr " + this->__toString(true) + ", " + f.__toString(true) + ", " + t.__toString(true));
        }
        //Error
        return ::kontr::Variable::Delegator<T>::__create(*this);
    }
    virtual ::kontr::Variable::Delegator<T> split (
            const ::kontr::Variable::Delegator<T> &sep) const {
        const Variable& s = sep.__getDelegate();
        if (dataType == DataType::String && s.dataType == DataType::String) {
            ::kontr::Generator::Variable<T> res(DataType::Array,
                "split(" + s.__toString(true) + ", " + this->__toString(true) + ")");
            res.data.Array.push_back(sep);
            res.data.Array.push_back(
                ::kontr::Variable::Delegator<T>::__create(*this)
            );
            return ::kontr::Variable::Delegator<T>::__create(res);
        }
        //Error
        return ::kontr::Variable::Delegator<T>::__create(*this);
    }

#undef RET_EXP

};

} //Generator
} //kontr
#endif // VARIABLE_H
