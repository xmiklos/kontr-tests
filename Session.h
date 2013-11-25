#ifndef SESSION_H
#define SESSION_H

#include "kontr.h"
#include <vector>
#include <functional>

namespace kontr {

class ISession;

typedef const std::vector<std::function<::kontr::MasterTestDelegator(::kontr::IMasterTest&)> > TMasterTests;
typedef const std::function<void(const ISession&)> TPost;



class ISession {
    const char* script_dir;
    const char* files_dir;
    TMasterTests nanecisto;
    TMasterTests naostro;
    TPost post;
public:
    ISession(const char* script_dir, const char* files_dir,
             TMasterTests nanecisto, TMasterTests naostro,
             TPost post = nullptr);

};

}

#endif // SESSION_H
