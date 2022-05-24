#pragma once
#include "net_common.h"
#include "net_message.h"
#include "net_queue.h"
#include "net_connection.h"

namespace jl
{
    namespace net
    {
        template <typename T>
        class client_interface
        {
            client_interface() : m_socket(m_context)
            {

            }

            virtual ~client_interface()
            {
                Disconnect();
            }
            public:
                bool Connect(const std::string& host, const uint16_t port)
                {
                    try
                    {
                        m_connection = std::make_unique<connection<T>>();

                        asio::ip::tcp::resolver resolver(m_context);
                        m_endpoints = resolver.resolve(host, std::to_string(port));

                        m_connection->ConnectToServer(m_endpoints);

                        // Start context thread by using lambda
                        thrContext = std::thread([this]() {m_context.run();});
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << "Client Exception: " << e.what() << '\n';
                        return false;
                    }
                    
                    return true;
                }

                void Disconnect()
                {
                    if (IsConnected())
                    {
                        m_connection->Disconnect();
                    }

                    m_context.stop()
                    
                    if(thrContext.joinable())
                        thrContext.join();

                    m_connection.release();
                }
                
                void IsConnected()
                {
                    if (m_connection)
                        return m_connection->IsConnected();
                    else
                        return false;
                }
            protected:
                asio::io_context m_context;
                std::thread thrContext;
                asio::ip::tcp::socket m_socket;
                std::unique_ptr<connection<T>> m_connection;

            private:
                tsqueue<owned_message<T>> m_qMessagesIn;
        }
    }
}