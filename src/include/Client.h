#ifndef CLIENT_H
#define CLIENT_H

#include <memory>
#include <string>
#include <vector>

#include "NetworkTypes.h"
#include "SocketWrapper.h"
#include "WinSockFunctions.h"

namespace WindowsSocketApp {

enum class Client_Initialization_Status {
  NOT_CONNECTED,
  CONNECTED,
  SHUTDOWN_FOR_SENDING
};

class Client {
 private:
  inline static const char *default_send_buffer{"Test Message!"};
  static constexpr int default_recv_buffer_capacity{1024};
  inline static const char *default_server_ip{"localhost"};
  inline static const char *default_port{"27015"};

  static constexpr auto default_ai_family{AF_UNSPEC};
  static constexpr auto default_ai_socktype{SOCK_STREAM};
  static constexpr auto default_ai_protocol{IPPROTO_TCP};

  std::string send_buffer_;
  size_t recv_buffer_capacity_;
  std::string server_ip_;
  std::string port_;

  Client_Initialization_Status client_initialization_status_;
  Sockaddr_Struct_State client_sockaddr_struct_state_;

  AddrInfoPtr result_;
  addrinfo *addrinfo_ptr_;
  addrinfo hints_;

  SocketWrapper connect_socket_;

  std::vector<char> recv_buffer_;

 public:
  explicit Client(std::string send_buff_val = default_send_buffer,
                  size_t recv_capacity_val = default_recv_buffer_capacity,
                  std::string server_ip_val = default_server_ip,
                  std::string port_val = default_port);

  ~Client() = default;

  Client(const Client &source) = delete;
  Client &operator=(const Client &other) = delete;

  Client(Client &&source) noexcept = default;
  Client &operator=(Client &&other) noexcept = default;

  void connect_to_server();
  void send_buffer_to_server() const;
  void shutdown_message_sending();
  void receive_server_message();

  [[nodiscard]] Client_Initialization_Status get_client_init_status() const;
  [[nodiscard]] const std::string &get_send_message_buffer() const;
  void set_send_message_buffer(std::string send_message_buffer);
  void display_recv_buffer() const;
  [[nodiscard]] size_t get_recv_buffer_capacity() const;
  [[nodiscard]] const std::string &get_port() const;
  void set_port(std::string port);
  [[nodiscard]] const std::string &get_server_ip() const;
  void set_server_ip(std::string server_ip);
};

}  // namespace WindowsSocketApp

#endif  // CLIENT_H
