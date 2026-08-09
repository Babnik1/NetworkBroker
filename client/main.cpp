/// @file main.cpp
///
/// Клиент.
///

#include <boost/asio.hpp>

#include <iostream>
#include <string>
#include <thread>

using boost::asio::ip::tcp;

class Client
{
public:
    Client(const std::string& host, const std::string& port)
        : socket_{ ioContext_ }
        , host_{ host }
        , port_{ port }
    {
    }

    void Run()
    {
        try
        {
            Connect();

            std::thread reader(
                [this]()
                {
                    Read();
                });

            Write();

            if (reader.joinable())
            {
                reader.join();
            }
        }
        catch (const std::exception& error)
        {
            std::cerr << "Client error: "
                      << error.what()
                      << '\n';
        }
    }

private:
    void Connect()
    {
        tcp::resolver resolver{ ioContext_ };

        auto endpoints =
            resolver.resolve(host_, port_);

        boost::asio::connect(socket_, endpoints);

        std::cout << "Connected to "
                  << host_
                  << ':'
                  << port_
                  << '\n';
    }

    void Write()
    {
        std::string command;

        while (std::getline(std::cin, command))
        {
            if (command == "exit")
            {
                boost::system::error_code error;
                socket_.shutdown(
                    tcp::socket::shutdown_both,
                    error);
                socket_.close(error);

                return;
            }

            command += '\n';

            boost::system::error_code error;

            boost::asio::write(
                socket_,
                boost::asio::buffer(command),
                error);

            if (error)
            {
                std::cerr << "Write error: "
                          << error.message()
                          << '\n';

                return;
            }
        }
    }

    void Read()
    {
        boost::asio::streambuf buffer;

        while (true)
        {
            boost::system::error_code error;

            boost::asio::read_until(
                socket_,
                buffer,
                '\n',
                error);

            if (error)
            {
                if (error != boost::asio::error::eof &&
                    error != boost::asio::error::operation_aborted)
                {
                    std::cerr << "Read error: "
                              << error.message()
                              << '\n';
                }

                return;
            }

            std::istream input{ &buffer };

            std::string message;
            std::getline(input, message);

            std::cout << "\n[SERVER] "
                      << message
                      << '\n';
        }
    }

private:
    boost::asio::io_context ioContext_;
    tcp::socket socket_;

    std::string host_;
    std::string port_;
};

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr
            << "Usage: "
            << argv[0]
            << " <host> <port>\n";

        return 1;
    }

    Client client{ argv[1], argv[2] };
    client.Run();

    return 0;
}