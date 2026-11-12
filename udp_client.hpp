#pragma once

#include <netinet/in.h>

#include "client.hpp"

namespace multicast
{
namespace udp
{

class Socket;

} // namespace udp

class UdpClient : public Client
{
public:
    UdpClient(udp::Socket& socket, const sockaddr_in& address);
    UdpClient(const UdpClient&) = delete;
    UdpClient(UdpClient&&) = delete;
    UdpClient& operator=(const UdpClient&) = delete;
    UdpClient& operator=(UdpClient&&) = delete;
    ~UdpClient() override = default;

    void send(std::string_view message) override;

private:
    udp::Socket& socket_;
    sockaddr_in address_;
};

} // namespace multicast
