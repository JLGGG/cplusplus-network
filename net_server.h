#pragma once

#include "net_common.h"
#include "net_queue.h"
#include "net_message.h"
#include "net_connection.h"

namespace jl
{
    namespace net
    {
        template<typename T>
        class server_interface
        {
            public:
                server_interface(uint16_t port)
                    :m_asioAcceptor(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
                {

                }
                virtual ~server_interface()
                {
                    Stop()
                }
                bool Start()
                {
                    try
                    {
                        WaitForClientConnection();
                        m_threadContext = std::thread([this]() {m_asioContext.run();});
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << "[SERVER] Exception: " << e.what() << '\n';
                        return false;
                    }

                    std::cout << "[SERVER] Started!\n"
                    return true;
                    
                }
                void Stop()
                {
                    m_asioContext.stop();

                    if (m_threadContext.joinable()) m_threadContext.join();

                    std::cout << "[SERVER] Stopped!\n"
                }

                void WaitForClientConnection()
                {
                    m_asioAcceptor.async_accept(
                        [this](std::error_code ec, asio::ip::tcp::socket socket)
                        {
                            if (!ec)
                            {
                                std::cout << "[SERVER] New Connection: " <<socket.remote_endpoint() << "\n";

                                std::shared_ptr<connection<T>> newConn =
                                    std::make_shared<connection<T>>(connection<T>::owner::server,
                                        m_asioContext, std::move(socket), m_qMessagesIn);

                                // In progress..
                            }
                        }
                    )
                }
                void MessageClient(std::shared_prt<connection<T>> client, const message<T>& msg)
                {

                }
        };
    }
}