#include <boost/asio.hpp>
#include <iostream>
#include <csignal>
#include <thread>

#include "listener.hpp"  
#include "session.hpp" 

namespace net = boost::asio;


void SignalHandler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        std::cout << "Press Enter to exit..." << std::endl;
    }
}


int main(int argc, char** argv)
{
    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    if (argc != 3) {
        std::cout << "Incorrect number of input arguments\n";
        return 1;
    }

    const std::string proxy_ip(argv[1]);
    const uint16_t proxy_port = static_cast<uint16_t>(std::stoi(argv[2]));

    std::cout << "Proxy IP: " << proxy_ip << " , port: " << proxy_port << "\n";

    int num_threads = std::thread::hardware_concurrency() * 2;
    net::thread_pool pool(num_threads);
    net::io_context ioc(num_threads);

    // Создание work guard для предотвращения преждевременного завершения работы io_context
    net::executor_work_guard guard = boost::asio::make_work_guard(ioc);

    tcp::endpoint endpoint(net::ip::make_address(proxy_ip), proxy_port);
    std::make_shared<Listener>(ioc, endpoint)->Run();

    // Запуск пула потоков
    for (std::size_t i = 0; i < num_threads; ++i) {
        net::post(pool, [&ioc]() { ioc.run(); });
    }

    std::cin.get(); // Ожидание ввода Enter
    std::cout << "Exiting...\n";

    ioc.stop();
    pool.join();

    return 0;
}
