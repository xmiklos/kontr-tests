#ifndef GENERATOR_LANGUAGE_H
#define GENERATOR_LANGUAGE_H

#include "../kontr-internal.h"

namespace kontr {
namespace Generator {

template<typename T>
class Language : public ::kontr::Language::Interface<T> {
public:
    using typename ::kontr::Language::Interface<T>::Variable;
    using typename ::kontr::Language::Interface<T>::Callback;

    virtual void _if(Variable cond, Callback _if, Callback _else = nullptr) {
        std::ostream& out = *(T::instance().storage.out_ptr);
        out << "if (" << cond << ") {" << std::endl;
        _if();
        if (_else == nullptr) {
            out << "}" << std::endl;
        }
        else {
            out << "} else {" << std::endl;
            _else();
            out << "}" << std::endl;
        }
    }

    virtual void _while(Variable cond, Callback _cycle) {
        std::ostream& out = *(T::instance().storage.out_ptr);
        out << "while (" << cond << ") {" << std::endl;
        _cycle();
        out << "}" << std::endl;
    }

    virtual void _foreach(Variable iter, Variable array, Callback _cycle) {
        std::ostream& out = *(T::instance().storage.out_ptr);
        out << "foreach " << iter << " (" << array << ") {" << std::endl;
        _cycle();
        out << "}" << std::endl;
    }

    virtual void perl(const char *code) {
        std::ostream& out = *(T::instance().storage.out_ptr);
        out << code << std::endl;
    }
    
    virtual void _return() {
		std::ostream& out = *(T::instance().storage.out_ptr);
        out << "return;" << std::endl;
	}

    virtual void _break() {
        std::ostream& out = *(T::instance().storage.out_ptr);
        out << "break;" << std::endl;
    }

    virtual void _continue() {
        std::ostream& out = *(T::instance().storage.out_ptr);
        out << "continue;" << std::endl;
    }

//Return expression
#define RET_EXP(TYPE, EXP) return ::kontr::Variable::Delegator<T>::__create( ::kontr::Generator::Variable<T>(DataType::TYPE, EXP));
    virtual Variable file_exists(Variable file) {
        RET_EXP(Bool, "-e " + file.__getDelegate().__toString(true));
    }
    virtual Variable file_empty(Variable file) {
        RET_EXP(Bool, "-z " + file.__getDelegate().__toString(true));
    }
    virtual Variable file_size(Variable file) {
        RET_EXP(Int, "-s " + file.__getDelegate().__toString(true));
    }
#undef RET_EXP
};

} //Generator
} //kontr
#endif // LANGUAGE_H
