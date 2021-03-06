#ifndef REPORTER_H
#define REPORTER_H

#include "kontr-internal.h"

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

void print_report_text(std::ostream& where, const Severity& severity, const std::string& type) {
    where << std::endl << "==================" << std::endl
         << Severity_strings[severity] << ": " << type << std::endl;
}

template<typename T>
class Reporter
{
    std::set<std::pair<Severity, std::string>> suppressed;
    bool macroWarnings = true;
public:
    /**
     * @brief Create a report
     * @param severity
     * @param type
     */
    void create(const Severity severity, const std::string& type, bool suppressable = true) {
        _create(severity, type, T::instance().ReportConfiguration.report[severity], suppressable);
    }

    void _create(const Severity severity, const std::string& type, const Reporting& reporting, bool suppressable = true) {
        using namespace std;
        auto it = suppressed.find(make_pair(severity, type));
        if (suppressable && it != suppressed.end()) {
            suppressed.erase(it);
        }
        else {
            print_report_text(cerr, severity, type);
#ifdef BACKTRACEXX
            cerr << backtracexx::scan() << endl;
#endif
            if (reporting == Reporting::ERR_ABORT) {
                abort();
            }
            if (reporting == Reporting::ERR_EXCEPTION) {
                switch(severity) {
                    case NOTICE: throw ReportNotice(type.c_str());
                    case WARNING: throw ReportWarning(type.c_str());
                    case ERROR: throw ReportError(type.c_str());
                }
            }
        }
    }

    void createMacroWarning(const char* type) {
        if (!macroWarnings) return;
        create(WARNING, type);
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

    /// Suppress warning by macros (warnings.h)
    void suppressMacroWarnings(bool on) {
        macroWarnings = !on;
    }

};

/// Default error reporting configuration - print notices and warnings, abort on error
typedef ReportConfiguration<Reporting::ERR, Reporting::ERR, Reporting::ERR_ABORT>
    Default;

/// Error reporting configuration used for testing - always throw exceptions
typedef ReportConfiguration<Reporting::ERR_EXCEPTION, Reporting::ERR_EXCEPTION, Reporting::ERR_EXCEPTION>
    Exception;

} //Report
} //kontr
#endif // REPORTER_H
