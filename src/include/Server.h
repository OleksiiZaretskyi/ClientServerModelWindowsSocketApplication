#ifndef SERVER_H
#define SERVER_H

#include <memory>
#include <string>
#include <vector>

#include "NetworkTypes.h"
#include "SocketWrapper.h"
#include "WinSockFunctions.h"

namespace WindowsSocketApp {

enum class Server_Initialization_Status {
  NOT_STARTED,
  LISTENING_FOR_CONNECTION,
  CLIENT_CONNECTION_HANDLED,
  SHUTDOWN_FOR_SENDING
};

class Server {
 private:
  static constexpr int default_recv_buffer_capacity{1024};
  inline static const char *default_port{"27015"};

  static constexpr auto default_ai_family{AF_INET};
  static constexpr auto default_ai_socktype{SOCK_STREAM};
  static constexpr auto default_ai_protocol{IPPROTO_TCP};
  static constexpr auto default_ai_flags{AI_PASSIVE};
  static constexpr auto maximum_pending_connections{SOMAXCONN};

  size_t recv_buffer_capacity_;
  std::string port_;

  Server_Initialization_Status server_initialization_status_;
  Sockaddr_Struct_State server_sockaddr_struct_state_;

  AddrInfoPtr result_;
  addrinfo hints_;

  SocketWrapper listen_socket_;
  SocketWrapper client_socket_;

  std::vector<char> recv_buffer_;
  std::string recv_message_analytics_;

 public:
  explicit Server(size_t recv_capacity_val = default_recv_buffer_capacity,
                  std::string port_val = default_port);

  ~Server() = default;

  Server(const Server &source) = delete;
  Server &operator=(const Server &other) = delete;

  Server(Server &&source) noexcept = default;
  Server &operator=(Server &&other) noexcept = default;

  void start_server();
  void accept_connections();
  void receive_client_message();
  void calculate_recv_message_analytics();
  void echo_message_to_client() const;
  void send_recv_message_analytics_to_client() const;
  void shutdown_message_sending();

  [[nodiscard]] Server_Initialization_Status get_server_init_status() const;
  void display_recv_buffer() const;
  [[nodiscard]] size_t get_recv_buffer_capacity() const;
  [[nodiscard]] const std::string &get_port() const;
  void set_port(std::string port);
};

}  // namespace WindowsSocketApp

#endif  // SERVER_H
