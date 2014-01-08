#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "kontr.h"
#include <functional>

namespace kontr {
namespace Language {

/// Pure interface
template<typename T>
class Interface {
public:
    /// Parameter type
    using Variable = const typename T::VariableDelegator&;
    using Callback = std::function<void()>;

    /**
      IF statement
     * @brief _if
     * @param cond
     * @param _if
     * @param _else
     */
    virtual void _if (Variable cond, Callback _if, Callback _else = nullptr) = 0;

    /**
      WHILE statement
     * @brief _while
     * @param cond
     * @param _cycle
     */
    virtual void _while(Variable cond, Callback _cycle) = 0;

    /**
      Raw perl code
     * @brief perl
     * @param code
     */
    virtual void perl(const char* code) = 0;
    
    virtual void _return() = 0;
};

/// Empty class
template<typename T>
class Empty : public Interface<T> {
public:
    using typename Interface<T>::Variable;
    using typename Interface<T>::Callback;

    virtual void _if(Variable cond, Callback _if, Callback _else = nullptr) {
        kontr::unused(cond, _if, _else);
    }
    virtual void _while(Variable cond, Callback _cycle) {
        kontr::unused(cond, _cycle);
    }
    virtual void perl(const char *code) {
        kontr::unused(code);
    }
    virtual void _return() {}
};

/// Delegator class
template<typename T>
class Delegator : public Interface<T> {
    typename T::Language delegate;
public:
    using typename Interface<T>::Variable;
    using typename Interface<T>::Callback;

    virtual void _if(Variable cond, Callback _if, Callback _else = nullptr) {
        delegate._if(cond, _if, _else);
    }

    virtual void _while(Variable cond, Callback _cycle) {
        delegate._while(cond, _cycle);
    }

    virtual void perl(const char *code) {
        delegate.perl(code);
    }
    
    virtual void _return() { delegate._return(); }
};

} //Language
} //kontr
#endif // LANGUAGE_H
