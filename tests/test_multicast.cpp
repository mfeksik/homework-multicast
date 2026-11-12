#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "client.hpp"
#include "serial_client.hpp"
#include "udp.hpp"
#include "udp_client.hpp"

namespace
{

class MockClient : public multicast::Client
{
public:
    int call_count = 0;
    std::string received;

    void send(std::string_view message) override
    {
        ++call_count;
        received = message;
    }
};

void test_polymorphic_send()
{
    std::vector<std::unique_ptr<multicast::Client>> clients;

    auto mock1 = std::make_unique<MockClient>();
    auto mock2 = std::make_unique<MockClient>();
    auto* raw1 = mock1.get();
    auto* raw2 = mock2.get();

    clients.push_back(std::move(mock1));
    clients.push_back(std::move(mock2));

    const std::string message = "hello, multicast\n";
    for (auto& client : clients)
        client->send(message);


    assert(clients.size() == 2);
    assert(raw1->call_count == 1);
    assert(raw2->call_count == 1);
    assert(raw1->received == message);
    assert(raw2->received == message);

    std::cout << __func__ << ": PASSED\n";
}

void test_udp_client_delivery()
{
    int recv_fd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(recv_fd >= 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = 0;

    assert(
        bind(recv_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == 0);

    socklen_t len = sizeof(addr);
    assert(getsockname(recv_fd, reinterpret_cast<sockaddr*>(&addr), &len) == 0);

    multicast::udp::Socket sender_socket{};
    std::unique_ptr<multicast::Client> client =
        std::make_unique<multicast::UdpClient>(sender_socket, addr);

    const std::string payload = "udp test payload\n";
    client->send(payload);

    char buffer[256]{};
    const ssize_t n = recv(recv_fd, buffer, sizeof(buffer) - 1, 0);
    close(recv_fd);

    assert(n == static_cast<ssize_t>(payload.size()));
    assert(std::string_view(buffer, static_cast<std::size_t>(n)) == payload);

    std::cout << __func__ << ": PASSED\n";
}

void test_serial_client_delivery()
{
    int master_fd = posix_openpt(O_RDWR | O_NOCTTY);
    assert(master_fd >= 0);
    assert(grantpt(master_fd) == 0);
    assert(unlockpt(master_fd) == 0);

    const char* slave_name = ptsname(master_fd);
    assert(slave_name != nullptr);

    std::unique_ptr<multicast::Client> client =
        std::make_unique<multicast::SerialClient>(slave_name);

    const std::string payload = "serial test payload\n";
    client->send(payload);

    char buffer[256]{};
    const ssize_t n = read(master_fd, buffer, sizeof(buffer) - 1);
    close(master_fd);

    assert(n == static_cast<ssize_t>(payload.size()));
    assert(std::string_view(buffer, static_cast<std::size_t>(n)) == payload);

    std::cout << __func__ << ": PASSED\n";
}

} // namespace

int main()
{
    test_polymorphic_send();
    test_udp_client_delivery();
    test_serial_client_delivery();

    std::cout << "All tests passed!\n";
    return 0;
}
