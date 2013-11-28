#ifndef REPORTER_H
#define REPORTER_H

#include "kontr.h"
#include <string>
#include <functional>
#include <set>
#include <utility>
#include <iostream>
#include "backtracexx/backtracexx.hpp"
#include <cstdlib>

namespace kontr {

namespace Report {

/**
 * @brief The Report enum - how to report an event
 */
enum class Reporting {
    ERR,
    ERR_ABORT,
    ERR_EXCEPTION
};

/**
 * @brief The Severity enum - severity of an event
 */
enum Severity {
    NOTICE,
    WARNING,
    ERROR
};
const unsigned SEVERITY_LENGTH = 4;

static const char* const Severity_strings[]  = {
    "NOTICE",
    "WARNING",
    "ERROR"
};

class ReportNotice : public std::runtime_error { using std::runtime_error::runtime_error; };
class ReportWarning : public std::runtime_error { using std::runtime_error::runtime_error; };
class ReportError : public std::runtime_error { using std::runtime_error::runtime_error; };

template<Reporting Notice, Reporting Warning, Reporting Error>
struct ReportConfiguration {
    const Reporting report[SEVERITY_LENGTH] = {
        Notice,
        Warning,
        Error
    };
};

template<typename T>
class Reporter
{
    T& instance;
    std::set<std::pair<Severity, std::string>> suppressed;
public:
    Reporter(T& instance) : instance(instance) {}

    /**
     * @brief Create a report
     * @param severity
     * @param type
     */
    void create(const Severity severity, const std::string& type, bool suppressable = true) {
        using namespace std;
        auto it = suppressed.find(make_pair(severity, type));
        if (suppressable && it != suppressed.end()) {
            suppressed.erase(it);
        }
        else {
            cerr << "=================" << endl
                 << Severity_strings[severity]
                 << ": " << type << endl;
            cerr << backtracexx::scan() << endl;
            if (instance.ReportConfiguration.report[severity] == Reporting::ERR_ABORT) {
                abort();
            }
            if (instance.ReportConfiguration.report[severity] == Reporting::ERR_EXCEPTION) {
                switch(severity) {
                    case NOTICE: throw new ReportNotice(type.c_str());
                    case WARNING: throw new ReportWarning(type.c_str());
                    case ERROR: throw new ReportError(type.c_str());
                }
            }
        }
    }

    void suppress(const Severity& severity, const std::string& type, std::function<void(void)> F) {
        using namespace std;
        auto pair = make_pair(severity, type);
        //Insert into map
        if (suppressed.find(pair) != suppressed.end())
        {
            create(NOTICE, string("Cannot suppress report with severity \"")
                   + Severity_strings[severity] + "\" with type \""
                   + type + "\" for the second time.", false);
        }
        else
            suppressed.insert(pair);

        F();

        auto it = suppressed.find(pair);
        if (it != suppressed.end()) {
            create(NOTICE, string("Did not suppress Report with severity \"")
                   + Severity_strings[severity] + "\" with type \""
                   + type + "\"", false);
            suppressed.erase(it);
        }
    }
};

}

}

#endif // REPORTER_H
