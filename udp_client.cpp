#include <iostream>
#include <string_view>

#include "udp.hpp"
#include "udp_client.hpp"

namespace multicast
{

UdpClient::UdpClient(udp::Socket& socket, const sockaddr_in& address)
    : socket_(socket), address_(address)
{
}

void UdpClient::send(std::string_view message)
{
    if (!udp::send(socket_, address_, message))
    {
        std::cerr << "failed to send UDP message\n";
    }
}

} // namespace multicast
