#ifndef TWITCHUSERREPLY_HPP
#define TWITCHUSERREPLY_HPP

#include "twitchreply.hpp"
#include "twitchuser.hpp"

namespace Twitch {
class UserReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;

    Twitch::User user();

protected:
    virtual void parseData(const JSON&) override;
};

class UsersReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;

    Twitch::Users users();

protected:
    virtual void parseData(const JSON&) override;
};

#include "twitchuserreply.inl"
}

#endif // TWITCHUSERREPLY_HPP
