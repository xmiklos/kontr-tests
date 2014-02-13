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
        std::string& indent = T::instance().storage.indent;
        out << indent << "if (" << cond << ") {" << std::endl;
        T::instance().storage.addIndent();
        _if();
        T::instance().storage.removeIndent();
        if (_else == nullptr) {
            out << indent << "}" << std::endl;
        }
        else {
            out << indent << "} else {" << std::endl;
            T::instance().storage.addIndent();
            _else();
            T::instance().storage.removeIndent();
            out << indent << "}" << std::endl;
        }
    }

    virtual void _while(Variable cond, Callback _cycle) {
        std::ostream& out = *(T::instance().storage.out_ptr);
        std::string& indent = T::instance().storage.indent;
        out << indent << "while (" << cond << ") {" << std::endl;
        T::instance().storage.addIndent();
        _cycle();
        T::instance().storage.removeIndent();
        out << indent << "}" << std::endl;
    }

    virtual void _foreach(Variable iter, Variable array, Callback _cycle) {
        std::ostream& out = *(T::instance().storage.out_ptr);
        std::string& indent = T::instance().storage.indent;
        out << indent << "foreach " << iter << " (" << array << ") {" << std::endl;
        T::instance().storage.addIndent();
        _cycle();
        T::instance().storage.removeIndent();
        out << indent << "}" << std::endl;
    }

    virtual void perl(const char *code) {
        std::ostream& out = *(T::instance().storage.out_ptr);
        std::string& indent = T::instance().storage.indent;
        out << indent << code << std::endl;
    }
    
    virtual void _return() {
		std::ostream& out = *(T::instance().storage.out_ptr);
        std::string& indent = T::instance().storage.indent;
        out << indent << "return;" << std::endl;
	}

    virtual void _break() {
        std::ostream& out = *(T::instance().storage.out_ptr);
        std::string& indent = T::instance().storage.indent;
        out << indent << "last;" << std::endl;
    }

    virtual void _continue() {
        std::ostream& out = *(T::instance().storage.out_ptr);
        std::string& indent = T::instance().storage.indent;
        out << indent << "next;" << std::endl;
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
    virtual Variable range(Variable from, Variable to) {
	::kontr::Generator::Variable<T> res(DataType::Array, 
	    from.__getDelegate().__toString(true) + " .. " + to.__getDelegate().__toString(true));
	res.data.Array.push_back(from);
	res.data.Array.push_back(to);
	return ::kontr::Variable::Delegator<T>::__create(res);
    }
#undef RET_EXP
};

} //Generator
} //kontr
#endif // LANGUAGE_H
