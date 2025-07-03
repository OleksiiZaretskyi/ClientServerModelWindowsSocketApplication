#ifndef CLIENT_H
#define CLIENT_H

#include <string>

#include "WinSockFunctions.h"

namespace WindowsSocketApp {

    enum class Client_Initialization_Status {NOT_CONNECTED, CONNECTED, SHUTDOWN_FOR_SENDING};

class Client {
private:
    static constexpr int default_buffer_length {1024};
    inline static const char * default_server_ip {"localhost"};
    inline static const char * default_port {"27015"};
    inline static const char * default_send_buffer {"Test Message!"};

    static constexpr auto default_ai_family {AF_UNSPEC};
    static constexpr auto default_ai_socktype {SOCK_STREAM};
    static constexpr auto default_ai_protocol {IPPROTO_TCP};

    int buffer_length_;
    std::string server_ip_;
    std::string port_;
    std::string send_buffer_;

    Client_Initialization_Status client_initialization_status_;
    Sockaddr_Struct_State client_sockaddr_struct_state_;

    addrinfo *result_;
    addrinfo *addrinfo_ptr_;
    addrinfo hints_;

    SOCKET connect_socket_;

    std::vector<char>recv_buffer_;

    void socket_cleanup();

public:
    explicit Client(int buff_len_val = default_buffer_length, std::string server_ip_val = default_server_ip, std::string port_val = default_port, std::string send_buff_val = default_send_buffer);

    void connect_to_server();
    void send_buffer_to_server();
    void shutdown_message_sending();
    void receive_server_message();
    void close_connection_to_server();

    [[nodiscard]] Client_Initialization_Status get_client_init_status() const;
    [[nodiscard]] const char * get_send_message_buffer() const;
    bool set_send_message_buffer(std::string send_message_buffer);
    void display_recv_buffer() const;
    [[nodiscard]] const char * get_port() const;
    void set_port(std::string port);
    [[nodiscard]] const char * get_server_ip() const;
    void set_server_ip(std::string port);
};

} // WindowsSocketApp

#endif //CLIENT_H
