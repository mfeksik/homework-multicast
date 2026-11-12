#include <iostream>
#include <string_view>

#include "serial.hpp"
#include "serial_client.hpp"

namespace multicast
{

SerialClient::SerialClient(std::string_view device_path) : device_(device_path)
{
}

void SerialClient::send(std::string_view message)
{
    if (!serial::send(device_, message))
    {
        std::cerr << "failed to send serial message\n";
    }
}

} // namespace multicast
