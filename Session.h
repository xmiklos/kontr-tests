#ifndef SESSION_H
#define SESSION_H

#include "kontr.h"
#include <vector>
#include <functional>

namespace kontr {

class ISession;

typedef const std::vector<std::function<::kontr::MasterTestDelegator(::kontr::IMasterTest&)> > TMasterTests;
typedef const std::function<void(const ISession&)> TPost;

struct SessionConfiguration {
    const char* script_dir;
    const char* files_dir;
    TMasterTests nanecisto;
    TMasterTests naostro;
    TPost post;
};


class ISession {
    SessionConfiguration conf;
public:
    ISession(SessionConfiguration conf);
};

}

#endif // SESSION_H
