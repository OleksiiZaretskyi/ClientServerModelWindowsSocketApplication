#include "../include/Server.h"

#include "../utils/HelperFunctions.h"

namespace WindowsSocketApp {

Server::Server(int buff_len_val, std::string port_val)
    : buffer_length_{buff_len_val}, port_{std::move(port_val)} {
  server_initialization_status_ = Server_Initialization_Status::NOT_STARTED;
  server_sockaddr_struct_state_ = Sockaddr_Struct_State::EMPTY;
  result_ = nullptr;
  hints_ = {};

  listen_socket_ = INVALID_SOCKET;
  client_socket_ = INVALID_SOCKET;

  ZeroMemory(&hints_, sizeof(hints_));
  hints_.ai_family = default_ai_family;
  hints_.ai_socktype = default_ai_socktype;
  hints_.ai_protocol = default_ai_protocol;
  hints_.ai_flags = default_ai_flags;

  recv_buffer_.reserve(buffer_length_);
  recv_message_analytics_.clear();
}

void Server::start_server() {
  if (!resolve_address_and_port(nullptr, port_.c_str(), &hints_, &result_)) {
    socket_cleanup();
    return;
  } else {
    server_sockaddr_struct_state_ = Sockaddr_Struct_State::CREATED;
  }

  listen_socket_ = create_socket(result_->ai_family, result_->ai_socktype,
                                 result_->ai_protocol);
  if (listen_socket_ == INVALID_SOCKET) {
    socket_cleanup();
    return;
  }

  if (!bind_socket(listen_socket_, result_->ai_addr,
                   static_cast<int>(result_->ai_addrlen))) {
    socket_cleanup();
    return;
  } else {
    freeaddrinfo(result_);
    server_sockaddr_struct_state_ = Sockaddr_Struct_State::EMPTY;
    result_ = nullptr;
  }

  if (!listen_on_socket(listen_socket_, maximum_pending_connections)) {
    socket_cleanup();
    return;
  } else {
    server_initialization_status_ =
        Server_Initialization_Status::LISTENING_FOR_CONNECTION;
    std::cout << "Server is started in the listening mode." << std::endl;
  }
}

void Server::accept_connections() {
  client_socket_ = accept_socket(listen_socket_);
  if (client_socket_ == INVALID_SOCKET) {
    socket_cleanup();
    return;
  } else {
    server_initialization_status_ =
        Server_Initialization_Status::CLIENT_CONNECTION_HANDLED;
    std::cout << "Client connection handled for client socket: "
              << client_socket_ << std::endl;
    // No longer need the server socket in this particular single-thread
    // example, should be re-factored when introducing the multi-threading later
    closesocket(listen_socket_);
    listen_socket_ = INVALID_SOCKET;
  }
}

void Server::receive_client_message() {
  recv_buffer_.clear();
  recv_buffer_.resize(buffer_length_);
  if (!receive_until_empty_input(client_socket_, recv_buffer_)) {
    socket_cleanup();
  }
}

void Server::calculate_recv_message_analytics() {
  size_t recv_message_length{0};
  int punctuation_marks_count{0};
  int spaces_count{0};
  int digits_count{0};
  int uppercase_count{0};
  int lowercase_count{0};
  int vowels_count{0};
  int consonants_count{0};

  if (!recv_buffer_.empty()) {
    recv_message_length = recv_buffer_.size();

    for (const auto c : recv_buffer_) {
      if (c == ' ') {
        ++spaces_count;
      } else if ((c >= '!' && c <= '/') || (c >= ':' && c <= '@') ||
                 (c >= '[' && c <= '`') || (c >= '{' && c <= '~')) {
        ++punctuation_marks_count;
      } else if (c >= '0' && c <= '9') {
        ++digits_count;
      } else if (c >= 'A' && c <= 'Z') {
        ++uppercase_count;
      } else if (c >= 'a' && c <= 'z') {
        ++lowercase_count;
      }
      if (is_vowel(c)) {
        ++vowels_count;
      } else if (is_consonant(c)) {
        ++consonants_count;
      }
    }
  }
  recv_message_analytics_ =
      "Received message analytics: Length: " +
      std::to_string(recv_message_length) +
      ", punctuation marks count: " + std::to_string(punctuation_marks_count) +
      ", spaces count: " + std::to_string(spaces_count) +
      ", digits count: " + std::to_string(digits_count) +
      ", uppercase letters count: " + std::to_string(uppercase_count) +
      ", lowercase laters count: " + std::to_string(lowercase_count) +
      ", vowels count: " + std::to_string(vowels_count) +
      ", consonants count: " + std::to_string(consonants_count);
}

void Server::echo_message_to_client() {
  if (!send_buffer_content(client_socket_, recv_buffer_)) {
    socket_cleanup();
  }
}

void Server::send_recv_message_analytics_to_client() {
  if (!send_buffer_content(client_socket_, recv_message_analytics_)) {
    socket_cleanup();
  }
}

void Server::shutdown_message_sending() {
  if (!shutdown_sending_side(client_socket_)) {
    socket_cleanup();
  } else {
    server_initialization_status_ =
        Server_Initialization_Status::SHUTDOWN_FOR_SENDING;
  }
}

void Server::stop_server() { socket_cleanup(); }

void Server::socket_cleanup() {
  std::cout << "Closing all server sockets, freeing address info, stopping the "
               "server... "
            << std::endl;
  if (server_sockaddr_struct_state_ == Sockaddr_Struct_State::CREATED) {
    freeaddrinfo(result_);
    result_ = nullptr;
    server_sockaddr_struct_state_ = Sockaddr_Struct_State::EMPTY;
  }
  if (listen_socket_ != INVALID_SOCKET) {
    closesocket(listen_socket_);
    listen_socket_ = INVALID_SOCKET;
  }
  if (client_socket_ != INVALID_SOCKET) {
    closesocket(client_socket_);
    client_socket_ = INVALID_SOCKET;
  }
  server_initialization_status_ = Server_Initialization_Status::NOT_STARTED;
}

Server_Initialization_Status Server::get_server_init_status() const {
  return server_initialization_status_;
}

void Server::display_recv_buffer() const {
  for (const auto c : recv_buffer_) {
    std::cout << c;
  }
  std::cout << std::endl;
}

const char* Server::get_port() const { return port_.c_str(); }

void Server::set_port(std::string port) { this->port_ = std::move(port); }

}  // namespace WindowsSocketApp