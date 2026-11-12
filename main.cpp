#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "config.hpp"
#include "serial_client.hpp"
#include "udp.hpp"
#include "udp_client.hpp"

namespace
{

void print_usage(std::string_view basename)
{
    std::cout << "Usage: " << basename << " <CONFIG_FILE>\n";
}

void print_prompt()
{
    std::cout << "Enter message: ";
}

} // namespace

int main(int argc, char* argv[])
{
    if (argc == 2 && std::string_view(argv[1]) == "-h")
    {
        print_usage(argv[0]);
        return EXIT_SUCCESS;
    }

    if (argc < 2)
    {
        std::cerr << "Not enough arguments!\n";
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    try
    {
        using namespace multicast;
        Config config{argv[1]};
        udp::Socket socket{};

        std::vector<std::unique_ptr<Client>> clients;
        clients.reserve(
            config.GetUdpEntries().size() + config.GetSerialEntries().size());

        for (const auto& entry : config.GetUdpEntries())
        {
            clients.emplace_back(
                std::make_unique<UdpClient>(socket, entry.address));
        }

        for (const auto& entry : config.GetSerialEntries())
            clients.emplace_back(std::make_unique<SerialClient>(entry.device));


        std::string input{};
        while (print_prompt(), std::getline(std::cin, input))
        {
            input.push_back('\n');

            for (auto& client : clients)
                client->send(input);
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& err)
    {
        std::cerr << "Multicast failure: " << err.what() << '\n';
    }
    return EXIT_FAILURE;
}
