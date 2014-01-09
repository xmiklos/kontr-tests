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

    virtual void perl(const char *code) {
        std::ostream& out = *(T::instance().storage.out_ptr);
        out << code << std::endl;
    }
    
    virtual void _return() {
		std::ostream& out = *(T::instance().storage.out_ptr);
        out << "return;" << std::endl;
	}
};

} //Generator
} //kontr
#endif // LANGUAGE_H
