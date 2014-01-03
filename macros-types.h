#ifndef MACROSTYPES_H
#define MACROSTYPES_H

/// Class name of created auxiliary class
#define CLASS(NAME) cl_##NAME

/// Master test
/// Usage: MASTER_TEST(name) { code; }
#define MASTER_TEST(NAME) \
    template<typename T> \
    class CLASS(NAME) : public ::kontr::MasterTest::Delegator<T> { \
    public: \
        CLASS(NAME)() : kontr::MasterTest::Delegator<T>(#NAME) {} \
        virtual void execute () override; \
        using kontr::MasterTest::Delegator<T>::name; \
        using kontr::MasterTest::Delegator<T>::register_unit; \
        using kontr::MasterTest::Delegator<T>::stage_compiled_file; \
        using kontr::MasterTest::Delegator<T>::stage_compiled_student_file; \
        using kontr::MasterTest::Delegator<T>::stage_file; \
        using kontr::MasterTest::Delegator<T>::stage_student_file; \
    }; \
    template<typename T> \
    std::unique_ptr<::kontr::MasterTest::Delegator<T>> NAME() { \
        return std::unique_ptr<::kontr::MasterTest::Delegator<T>> (new CLASS(NAME)<T>()); \
    } \
    template<typename T> \
    void CLASS(NAME)<T>::execute ()

/// Session with name
/// Usage: SESSION_NAME(name, "script", "files", {nanecisto}, {naostro}, [&]{ post;} )
#define SESSION_NAME(NAME, ...) \
    template <typename T> \
    class CLASS(NAME) : public ::kontr::Session::Delegator<T> { \
    public: \
        CLASS(NAME)();\
        using ::kontr::Session::Delegator<T>::pre_test; \
        using ::kontr::Session::Delegator<T>::post_test; \
    }; \
    template <typename T> \
    std::unique_ptr<::kontr::Session::Delegator<T>> NAME() { \
        return std::unique_ptr<::kontr::Session::Delegator<T>> (new CLASS(NAME)<T>()); \
    } \
    template <typename T> \
    CLASS(NAME)<T>::CLASS(NAME)() : \
        ::kontr::Session::Delegator<T>(#NAME, \
        __VA_ARGS__ \
                                     ) \
    {}

/// Global session (named Session)
/// Usage: SESSION("script", "files", {nanecisto}, {naostro}, [&]{ post;} )
#define SESSION(...) SESSION_NAME(Session, __VA_ARGS__)

#define VAR(name, data) typename T::VariableDelegator name(#name, data)

#define IF_NOELSE(cond, if) T::instance().language._if( (cond) , [&] { if } )
#define IF_ELSE(cond, if, else) T::instance().language._if( (cond) , [&] { if }, [&] { else } )

//http://stackoverflow.com/questions/3046889/optional-parameters-with-c-macros
#define GET_4RD_ARG(arg1, arg2, arg3, arg4, ...) arg4
#define IF_CHOOSER(...) GET_4RD_ARG(__VA_ARGS__, IF_ELSE, IF_NOELSE)
#define IF(...) IF_CHOOSER(__VA_ARGS__)(__VA_ARGS__)
#define WHILE(cond, while) T::instance().language._while(cond, [&] { while })
#define PERL(code) T::instance().language.perl(code)

#endif // MACROSTYPES_H
