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
        CLASS(NAME)() : ::kontr::MasterTest::Delegator<T>(#NAME) {} \
        virtual void execute () override; \
        using kontr::MasterTest::Delegator<T>::name; \
        using kontr::MasterTest::Delegator<T>::register_unit; \
        using kontr::MasterTest::Delegator<T>::stage_compiled_file; \
        using kontr::MasterTest::Delegator<T>::stage_compiled_student_file; \
        using kontr::MasterTest::Delegator<T>::stage_file; \
        using kontr::MasterTest::Delegator<T>::stage_student_file; \
    }; \
    template<typename T> \
    void NAME (std::unique_ptr<::kontr::MasterTest::Delegator<T>>& ret) { \
        ret = std::unique_ptr<::kontr::MasterTest::Delegator<T>> (new CLASS(NAME)<T>()); \
    } \
    template<typename T> \
    void CLASS(NAME)<T>::execute ()

/// Unit test
/// Usage: UNIT_TEST(name) { code; }
#define UNIT_TEST(NAME) \
    template<typename T> \
    class CLASS(NAME) : public ::kontr::UnitTest::Delegator<T> { \
    public: \
        CLASS(NAME)() : kontr::UnitTest::Delegator<T>(#NAME) {} \
        virtual void execute () override; \
        using kontr::UnitTest::Delegator<T>::name; \
        using kontr::UnitTest::Delegator<T>::stage_compiled_file; \
        using kontr::UnitTest::Delegator<T>::stage_compiled_student_file; \
        using kontr::UnitTest::Delegator<T>::stage_file; \
        using kontr::UnitTest::Delegator<T>::stage_student_file; \
        using kontr::UnitTest::Delegator<T>::execution; \
        using kontr::UnitTest::Delegator<T>::compilation; \
        using kontr::UnitTest::Delegator<T>::add_tag; \
        using kontr::UnitTest::Delegator<T>::add_points; \
        using kontr::UnitTest::Delegator<T>::work_path; \
        using kontr::UnitTest::Delegator<T>::file_path; \
        using kontr::UnitTest::Delegator<T>::extra_compiler_flags; \
        using kontr::UnitTest::Delegator<T>::compilation_log_errors; \
        using kontr::UnitTest::Delegator<T>::analysis; \
        using kontr::UnitTest::Delegator<T>::difference; \
        using kontr::UnitTest::Delegator<T>::valgrind; \
        using kontr::UnitTest::Delegator<T>::compile; \
        using kontr::UnitTest::Delegator<T>::run; \
        using kontr::UnitTest::Delegator<T>::run_grind; \
        using kontr::UnitTest::Delegator<T>::diff_stdout; \
        using kontr::UnitTest::Delegator<T>::diff_stderr; \
        using kontr::UnitTest::Delegator<T>::diff_generic; \
        using kontr::UnitTest::Delegator<T>::analyze_stdout; \
        using kontr::UnitTest::Delegator<T>::analyze_stderr; \
        using kontr::UnitTest::Delegator<T>::analyze; \
        using kontr::UnitTest::Delegator<T>::log; \
        using kontr::UnitTest::Delegator<T>::log_file; \
        using kontr::UnitTest::Delegator<T>::log_run_fail; \
        using kontr::UnitTest::Delegator<T>::log_tag; \
        using kontr::UnitTest::Delegator<T>::log_valgrind; \
        using kontr::UnitTest::Delegator<T>::subtest; \
        using kontr::UnitTest::Delegator<T>::session; \
    }; \
    template<typename T> \
    void NAME (std::unique_ptr<::kontr::UnitTest::Delegator<T>>& ret) { \
        ret = std::unique_ptr<::kontr::UnitTest::Delegator<T>> (new CLASS(NAME)<T>()); \
    } \
    template<typename T> \
    void CLASS(NAME)<T>::execute ()

#define SESSION_COMPLETE(NAME, ...) \
    template <typename T> \
    class CLASS(NAME) : public ::kontr::Session::Delegator<T> { \
    public: \
        CLASS(NAME)();\
        using ::kontr::Session::Delegator<T>::pre_test; \
        using ::kontr::Session::Delegator<T>::post_test; \
        using ::kontr::Session::Delegator<T>::run_type; \
        using ::kontr::Session::Delegator<T>::has_tag; \
        using ::kontr::Session::Delegator<T>::add_summary; \
        using ::kontr::Session::Delegator<T>::get_points; \
    }; \
    template <typename T> \
    void NAME (std::unique_ptr<::kontr::Session::Delegator<T>>& ret) { \
        ret = std::unique_ptr<::kontr::Session::Delegator<T>> (new CLASS(NAME)<T>()); \
    } \
    template <typename T> \
    CLASS(NAME)<T>::CLASS(NAME)() : \
        ::kontr::Session::Delegator<T>(#NAME, \
        __VA_ARGS__ \
                                     ) \
    {}
#define SESSION_CALLBACK(NAME, SCRIPTS, FILES, NANECISTO, NAOSTRO, CALLBACK) SESSION_COMPLETE(NAME, SCRIPTS, FILES, NANECISTO, NAOSTRO, [&] { CALLBACK })
#define GET_8TH_ARG(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, ...) arg8
#define SESSION_CHOOSER(...) GET_8TH_ARG(__VA_ARGS__, SESSION_COMPLETE, SESSION_CALLBACK)

/// Session with name
/// Usage: SESSION_NAME(name, "script", "files", {nanecisto}, {naostro}, [&]{ post;} )
/// or SESSION_NAME(name, "script", "files", {nanecisto}, {naostro}, valgrind, bonus )
#define SESSION_NAME(...) SESSION_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

/// Global session (named Session)
/// Usage: SESSION("script", "files", {nanecisto}, {naostro}, [&]{ post;} )
#define SESSION(...) SESSION_NAME(Session, __VA_ARGS__)

#define VAR(name, data) typename T::VariableDelegator name(#name, data)

#define IF_NOELSE(cond, IF) T::instance().language._if( (cond) , [&] { IF } )
#define IF_ELSE(cond, IF, ELSE) T::instance().language._if( (cond) , [&] { IF }, [&] { ELSE })

//http://stackoverflow.com/questions/3046889/optional-parameters-with-c-macros
#define GET_4RD_ARG(arg1, arg2, arg3, arg4, ...) arg4
#define IF_CHOOSER(...) GET_4RD_ARG(__VA_ARGS__, IF_ELSE, IF_NOELSE)
#define IF(...) IF_CHOOSER(__VA_ARGS__)(__VA_ARGS__)
#define WHILE(cond, while) T::instance().language._while(cond, [&] { while })
#define PERL(code) T::instance().language.perl(code)
#define RETURN T::instance().language._return
#define BREAK T::instance().language._break
#define CONTINUE T::instance().language._continue

#endif // MACROSTYPES_H
