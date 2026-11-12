#pragma once

#include <string_view>

#include "client.hpp"
#include "serial.hpp"

namespace multicast
{

class SerialClient : public Client
{
public:
    explicit SerialClient(std::string_view device_path);
    SerialClient(const SerialClient&) = delete;
    SerialClient(SerialClient&&) = delete;
    SerialClient& operator=(const SerialClient&) = delete;
    SerialClient& operator=(SerialClient&&) = delete;
    ~SerialClient() override = default;

    void send(std::string_view message) override;

private:
    serial::Device device_;
};

} // namespace multicast
