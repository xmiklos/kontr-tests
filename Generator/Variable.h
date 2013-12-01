#ifndef GENERATOR_VARIABLE_H
#define GENERATOR_VARIABLE_H

#include "../kontr.h"
#include <iostream>
#include <algorithm>

namespace kontr {
namespace Generator {

template<typename T>
class Variable : public ::kontr::Variable<T> {
    using Type = typename ::kontr::Variable<T>::Type;
    using ::kontr::Variable<T>::type;
    using ::kontr::Variable<T>::data;

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
            if (c == delimiter) {
                out << '\\';
            }
            out << c;
        }
        out << delimiter;
    }

public:
    using ::kontr::Variable<T>::Variable;
    using ::kontr::Variable<T>::__setInstance;

    virtual void __generate(std::ostream &out) const {
        switch (type) {
        case Type::Int: out << data.Int;
        case Type::Float: out << data.Float;
        case Type::Bool: out << data.Bool;
        case Type::String: printString(out, data.String);
        }
    }
};

}
}

#endif // VARIABLE_H
