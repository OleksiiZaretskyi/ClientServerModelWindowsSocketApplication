#ifndef SERVER_H
#define SERVER_H

#include <string>

#include "WinSockFunctions.h"

namespace WindowsSocketApp {

    enum class Server_Initialization_Status {NOT_STARTED, LISTENING_FOR_CONNECTION, CLIENT_CONNECTION_HANDLED, SHUTDOWN_FOR_SENDING};

class Server {
private:
    static constexpr int default_buffer_length {1024};
    inline static const char * default_port {"27015"};

    static constexpr auto default_ai_family {AF_INET};
    static constexpr auto default_ai_socktype {SOCK_STREAM};
    static constexpr auto default_ai_protocol {IPPROTO_TCP};
    static constexpr auto default_ai_flags {AI_PASSIVE};
    static constexpr auto maximum_pending_connections {SOMAXCONN};

    int buffer_length_;
    std::string port_;

    Server_Initialization_Status server_initialization_status_;
    Sockaddr_Struct_State server_sockaddr_struct_state_;

    addrinfo *result_;
    addrinfo hints_;

    SOCKET listen_socket_;
    SOCKET client_socket_;

    std::vector<char>recv_buffer_;
    std::string recv_message_analytics_;

    void socket_cleanup();

public:
    explicit Server(int buff_len_val=default_buffer_length, std::string port_val=default_port);

    void start_server();
    void accept_connections();
    void receive_client_message();
    void calculate_recv_message_analytics();
    void echo_message_to_client();
    void send_recv_message_analytics_to_client();
    void shutdown_message_sending();
    void stop_server();

    [[nodiscard]] Server_Initialization_Status get_server_init_status() const;
    void display_recv_buffer() const;
    [[nodiscard]] const char * get_port() const;
    void set_port(std::string port);
};

} // WindowsSocketApp

#endif //SERVER_H
