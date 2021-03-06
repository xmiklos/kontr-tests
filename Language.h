#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "kontr-internal.h"

namespace kontr {
namespace Language {

/// Pure interface
template<typename T>
class Interface {
public:
    /// Parameter type
    using Variable = typename ::kontr::Variable::Delegator<T>;
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
      FOREACH statement
     * @brief _foreach
     * @param iter
     * @param array
     * @param _cycle
     */
    virtual void _foreach(Variable iter, Variable array, Callback _cycle) = 0;

    /**
      Raw perl code
     * @brief perl
     * @param code
     */
    virtual void perl(const char* code) = 0;
    
    virtual void _return() = 0;

    virtual void _break() = 0;

    virtual void _continue() = 0;

    /**
      Does file exists? (boolean)
     * @brief file_exists
     * @param file
     * @return
     */
    virtual Variable file_exists(Variable file) = 0;

    /**
      Is file empty? (boolean)
     * @brief file_empty
     * @param file
     * @return
     */
    virtual Variable file_empty(Variable file) = 0;

    /**
      Return size of file (int)
     * @brief file_size
     * @param file
     * @return
     */
    virtual Variable file_size(Variable file) = 0;
    
    virtual Variable range(Variable from, Variable to) = 0;
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
    virtual void _foreach(Variable iter, Variable array, Callback _cycle) {
        kontr::unused(iter, array, _cycle);
    }
    virtual void perl(const char *code) {
        kontr::unused(code);
    }
    virtual void _return() {}
    virtual void _break() {}
    virtual void _continue() {}
    virtual Variable file_exists(Variable) { return false; }
    virtual Variable file_empty(Variable) { return false; }
    virtual Variable file_size(Variable) { return 0; }
    virtual Variable range(Variable from, Variable to) { return {from, to}; }
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

    virtual void _foreach(Variable iter, Variable array, Callback _cycle) {
        delegate._foreach(iter, array, _cycle);
    }

    virtual void perl(const char *code) {
        delegate.perl(code);
    }
    
    virtual void _return() { delegate._return(); }
    virtual void _break() { delegate._break(); }
    virtual void _continue() { delegate._continue(); }
    virtual Variable file_exists(Variable file) { return delegate.file_exists(file); }
    virtual Variable file_empty(Variable file) { return delegate.file_empty(file); }
    virtual Variable file_size(Variable file) { return delegate.file_size(file); }
    virtual Variable range(Variable from, Variable to) { return delegate.range(from, to); }
};

} //Language
} //kontr
#endif // LANGUAGE_H
