#include "Client.h"

namespace WindowsSocketApp {

Client::Client(std::string send_buff_val, int buff_len_val,
               std::string server_ip_val, std::string port_val)
    : send_buffer_{std::move(send_buff_val)},
      buffer_length_{buff_len_val},
      server_ip_{std::move(server_ip_val)},
      port_{std::move(port_val)} {
  client_initialization_status_ = Client_Initialization_Status::NOT_CONNECTED;
  client_sockaddr_struct_state_ = Sockaddr_Struct_State::EMPTY;
  addrinfo_ptr_ = nullptr;
  result_ = nullptr;
  hints_ = {};

  connect_socket_ = INVALID_SOCKET;

  ZeroMemory(&hints_, sizeof(hints_));
  hints_.ai_family = default_ai_family;
  hints_.ai_socktype = default_ai_socktype;
  hints_.ai_protocol = default_ai_protocol;

  recv_buffer_.reserve(buffer_length_);
}

void Client::connect_to_server() {
  if (!resolve_address_and_port(server_ip_.c_str(), port_.c_str(), &hints_,
                                &result_)) {
    socket_cleanup();
    return;
  } else {
    client_sockaddr_struct_state_ = Sockaddr_Struct_State::CREATED;
  }

  // Attempt to connect to an address until one succeeds
  for (addrinfo_ptr_ = result_; addrinfo_ptr_ != nullptr;
       addrinfo_ptr_ = addrinfo_ptr_->ai_next) {
    connect_socket_ =
        create_socket(addrinfo_ptr_->ai_family, addrinfo_ptr_->ai_socktype,
                      addrinfo_ptr_->ai_protocol);
    if (connect_socket_ == INVALID_SOCKET) {
      socket_cleanup();
      return;
    }

    if (connect(connect_socket_, addrinfo_ptr_->ai_addr,
                static_cast<int>(addrinfo_ptr_->ai_addrlen)) == SOCKET_ERROR) {
      closesocket(connect_socket_);
      connect_socket_ = INVALID_SOCKET;
      continue;
    }
    break;
  }
  if (connect_socket_ == INVALID_SOCKET) {
    std::cout << "Unable to connect to server!" << std::endl;
    socket_cleanup();
    return;
  } else {
    freeaddrinfo(result_);
    client_sockaddr_struct_state_ = Sockaddr_Struct_State::EMPTY;
    result_ = nullptr;
    client_initialization_status_ = Client_Initialization_Status::CONNECTED;
    std::cout << "Client successfully connected to server." << std::endl;
  }
}

void Client::send_buffer_to_server() {
  if (!send_buffer_content(connect_socket_, send_buffer_)) {
    socket_cleanup();
  }
}

void Client::shutdown_message_sending() {
  if (!shutdown_sending_side(connect_socket_)) {
    socket_cleanup();
  } else {
    client_initialization_status_ =
        Client_Initialization_Status::SHUTDOWN_FOR_SENDING;
  }
}

void Client::receive_server_message() {
  recv_buffer_.clear();
  recv_buffer_.resize(buffer_length_);
  if (!receive_until_empty_input(connect_socket_, recv_buffer_)) {
    socket_cleanup();
  }
}

void Client::close_connection_to_server() { socket_cleanup(); }

void Client::socket_cleanup() {
  std::cout << "Closing all client sockets, freeing address info, closing "
               "connection to server... "
            << std::endl;
  if (client_sockaddr_struct_state_ == Sockaddr_Struct_State::CREATED) {
    freeaddrinfo(result_);
    result_ = nullptr;
    client_sockaddr_struct_state_ = Sockaddr_Struct_State::EMPTY;
  }
  if (connect_socket_ != INVALID_SOCKET) {
    closesocket(connect_socket_);
    connect_socket_ = INVALID_SOCKET;
  }
  client_initialization_status_ = Client_Initialization_Status::NOT_CONNECTED;
}

Client_Initialization_Status Client::get_client_init_status() const {
  return client_initialization_status_;
}

const char *Client::get_send_message_buffer() const {
  return send_buffer_.c_str();
}

bool Client::set_send_message_buffer(std::string send_message_buffer) {
  if (send_message_buffer.length() <= buffer_length_) {
    this->send_buffer_ = std::move(send_message_buffer);
    return true;
  } else {
    return false;
  }
}

void Client::display_recv_buffer() const {
  for (const auto c : recv_buffer_) {
    std::cout << c;
  }
  std::cout << std::endl;
}

const char *Client::get_port() const { return port_.c_str(); }

void Client::set_port(std::string port) { this->port_ = std::move(port); }

const char *Client::get_server_ip() const { return server_ip_.c_str(); }

void Client::set_server_ip(std::string port) {
  this->server_ip_ = std::move(port);
}

}  // namespace WindowsSocketApp