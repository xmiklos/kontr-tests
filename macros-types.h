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
        CLASS(NAME)() : kontr::MasterTestDelegator<T>(#NAME) {} \
        virtual void execute () override; \
        using kontr::MasterTestDelegator<T>::name; \
        using kontr::MasterTestDelegator<T>::register_unit; \
        using kontr::MasterTestDelegator<T>::stage_compiled_file; \
        using kontr::MasterTestDelegator<T>::stage_compiled_student_file; \
        using kontr::MasterTestDelegator<T>::stage_file; \
        using kontr::MasterTestDelegator<T>::stage_student_file; \
    }; \
    template<typename T> \
    std::unique_ptr<::kontr::MasterTestDelegator<T>> NAME() { \
        return std::unique_ptr<::kontr::MasterTestDelegator<T>> (new CLASS(NAME)<T>()); \
    } \
    template<typename T> \
    void CLASS(NAME)<T>::execute ()

/// Session with name
/// Usage: SESSION_NAME(name, "script", "files", {nanecisto}, {naostro}, [&]{ post;} )
#define SESSION_NAME(NAME, ...) \
    template <typename T> \
    class CLASS(NAME) : public ::kontr::SessionDelegator<T> { \
    public: \
        CLASS(NAME)();\
        using ::kontr::SessionDelegator<T>::pre_test; \
        using ::kontr::SessionDelegator<T>::post_test; \
    }; \
    template <typename T> \
    std::unique_ptr<::kontr::SessionDelegator<T>> NAME() { \
        return std::unique_ptr<::kontr::SessionDelegator<T>> (new CLASS(NAME)<T>()); \
    } \
    template <typename T> \
    CLASS(NAME)<T>::CLASS(NAME)() : \
        ::kontr::SessionDelegator<T>( \
        __VA_ARGS__ \
                                     ) \
    {}

/// Global session (named Session)
/// Usage: SESSION("script", "files", {nanecisto}, {naostro}, [&]{ post;} )
#define SESSION(...) SESSION_NAME(Session, __VA_ARGS__)

#define VAR(name, data) typename T::VariableDelegator name(#name, data)

#endif // MACROSTYPES_H
