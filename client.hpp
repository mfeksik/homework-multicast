#pragma once

#include <string_view>

namespace multicast
{

class Client
{
public:
    virtual ~Client() = default;
    virtual void send(std::string_view message) = 0;
};

} // namespace multicast
