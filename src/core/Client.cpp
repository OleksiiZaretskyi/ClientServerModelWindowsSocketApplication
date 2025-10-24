#include "../include/Client.h"

namespace WindowsSocketApp {

Client::Client(std::string send_buff_val, size_t recv_capacity_val,
               std::string server_ip_val, std::string port_val)
    : send_buffer_{std::move(send_buff_val)},
      recv_buffer_capacity_{recv_capacity_val},
      server_ip_{std::move(server_ip_val)},
      port_{std::move(port_val)},
      client_initialization_status_{
          Client_Initialization_Status::NOT_CONNECTED},
      client_sockaddr_struct_state_{Sockaddr_Struct_State::EMPTY},
      result_{nullptr},
      addrinfo_ptr_{nullptr},
      hints_{},
      connect_socket_{} {
  hints_.ai_family = default_ai_family;
  hints_.ai_socktype = default_ai_socktype;
  hints_.ai_protocol = default_ai_protocol;

  recv_buffer_.reserve(recv_buffer_capacity_);
}

void Client::connect_to_server() {
  if (!resolve_address_and_port(server_ip_.c_str(), port_.c_str(), &hints_,
                                result_)) {
    std::cerr << "Failed to resolve client address and port." << std::endl;
    return;
  }
  client_sockaddr_struct_state_ = Sockaddr_Struct_State::CREATED;

  if (!result_) {
    std::cerr << "Error: result_ pointer to addrinfo is null after client "
                 "address resolution."
              << std::endl;
    return;
  }

  // Attempt to connect to an address until one succeeds
  for (addrinfo_ptr_ = result_.get(); addrinfo_ptr_ != nullptr;
       addrinfo_ptr_ = addrinfo_ptr_->ai_next) {
    connect_socket_ = SocketWrapper(create_socket(addrinfo_ptr_->ai_family,
                                                  addrinfo_ptr_->ai_socktype,
                                                  addrinfo_ptr_->ai_protocol));
    if (!connect_socket_.valid()) {
      std::cerr << "Failed to create client socket." << std::endl;
      continue;
    }

    if (SOCKET_ERROR == connect(connect_socket_.get(), addrinfo_ptr_->ai_addr,
                                static_cast<int>(addrinfo_ptr_->ai_addrlen))) {
      connect_socket_.close();
      continue;
    }
    // Connection successful!
    break;
  }
  if (!connect_socket_.valid()) {
    std::cerr << "Unable to connect to server on any available address."
              << std::endl;
    return;
  }
  result_.reset();
  client_sockaddr_struct_state_ = Sockaddr_Struct_State::EMPTY;
  client_initialization_status_ = Client_Initialization_Status::CONNECTED;
  std::cout << "Client successfully connected to server." << std::endl;
}

void Client::send_buffer_to_server() const {
  if (!send_buffer_content(connect_socket_.get(), send_buffer_)) {
    std::cerr << "Client failed to send message buffer to server." << std::endl;
  }
}

void Client::shutdown_message_sending() {
  if (!shutdown_sending_side(connect_socket_.get())) {
    std::cerr << "Failed to shutdown client sending side" << std::endl;
  } else {
    client_initialization_status_ =
        Client_Initialization_Status::SHUTDOWN_FOR_SENDING;
  }
}

void Client::receive_server_message() {
  recv_buffer_.clear();
  recv_buffer_.resize(recv_buffer_capacity_);
  if (!receive_until_empty_input(connect_socket_.get(), recv_buffer_)) {
    std::cerr << "Client failed to receive server message." << std::endl;
  }
}

Client_Initialization_Status Client::get_client_init_status() const {
  return client_initialization_status_;
}

const std::string &Client::get_send_message_buffer() const {
  return send_buffer_;
}

void Client::set_send_message_buffer(std::string send_message_buffer) {
  this->send_buffer_ = std::move(send_message_buffer);
}

void Client::display_recv_buffer() const {
  for (const auto c : recv_buffer_) {
    std::cout << c;
  }
  std::cout << std::endl;
}

size_t Client::get_recv_buffer_capacity() const {
  return recv_buffer_capacity_;
}

const std::string &Client::get_port() const { return port_; }

void Client::set_port(std::string port) { this->port_ = std::move(port); }

const std::string &Client::get_server_ip() const { return server_ip_; }

void Client::set_server_ip(std::string server_ip) {
  this->server_ip_ = std::move(server_ip);
}

}  // namespace WindowsSocketApp