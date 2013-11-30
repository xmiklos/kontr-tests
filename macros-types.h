#ifndef MACROSTYPES_H
#define MACROSTYPES_H

/// Class name of created auxiliary class
#define CLASS(NAME) cl_##NAME

/// Master test
/// Usage: MASTER_TEST(name) { code; }
#define MASTER_TEST(NAME) \
    template<typename T> \
    class CLASS(NAME) : public ::kontr::MasterTestDelegator<T> { \
    public: \
        CLASS(NAME)(T& instance) : kontr::MasterTestDelegator<T>(instance, #NAME) {} \
        virtual void execute () override; \
        using kontr::MasterTestDelegator<T>::name; \
        using kontr::MasterTestDelegator<T>::register_unit; \
        using kontr::MasterTestDelegator<T>::stage_compiled_file; \
        using kontr::MasterTestDelegator<T>::stage_compiled_student_file; \
        using kontr::MasterTestDelegator<T>::stage_file; \
        using kontr::MasterTestDelegator<T>::stage_student_file; \
    }; \
    template<typename T> \
    kontr::MasterTestDelegator<T>* NAME(T& instance) { \
        return new CLASS(NAME)<T>(instance); \
    } \
    template<typename T> \
    void CLASS(NAME)<T>::execute ()

/// Session with name
/// Usage: SESSION_NAME(name, "script", "files", {nanecisto}, {naostro}, [&]{ post;} )
#define SESSION_NAME(NAME, ...) \
    template <typename T> \
    class CLASS(NAME) : public ::kontr::SessionDelegator<T> { \
    public: \
        using ::kontr::SessionDelegator<T>::pre_test; \
        using ::kontr::SessionDelegator<T>::post_test; \
        CLASS(NAME)(T& instance); \
    }; \
    template <typename T> \
    ::kontr::SessionDelegator<T>* NAME(T& instance) { \
        return new CLASS(NAME)<T>(instance); \
    } \
    template <typename T> \
    CLASS(NAME)<T>::CLASS(NAME)(T &instance) : \
        ::kontr::SessionDelegator<T>(instance, \
        __VA_ARGS__ \
                                     ) \
    {}

/// Global session (named Session)
/// Usage: SESSION("script", "files", {nanecisto}, {naostro}, [&]{ post;} )
#define SESSION(...) SESSION_NAME(Session, __VA_ARGS__)

#endif // MACROSTYPES_H
