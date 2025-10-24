#include "../include/Server.h"

#include "../utils/HelperFunctions.h"

namespace WindowsSocketApp {

Server::Server(size_t recv_capacity_val, std::string port_val)
    : recv_buffer_capacity_{recv_capacity_val},
      port_{std::move(port_val)},
      server_initialization_status_{Server_Initialization_Status::NOT_STARTED},
      server_sockaddr_struct_state_{Sockaddr_Struct_State::EMPTY},
      result_{nullptr},
      hints_{},
      listen_socket_{},   // Default constructs to INVALID_SOCKET
      client_socket_{} {  // Default constructs to INVALID_SOCKET
  hints_.ai_family = default_ai_family;
  hints_.ai_socktype = default_ai_socktype;
  hints_.ai_protocol = default_ai_protocol;
  hints_.ai_flags = default_ai_flags;

  recv_buffer_.reserve(recv_buffer_capacity_);
  recv_message_analytics_.clear();
}

void Server::start_server() {
  if (!resolve_address_and_port(nullptr, port_.c_str(), &hints_, result_)) {
    std::cerr << "Failed to resolve server address and port." << std::endl;
    return;
  }
  server_sockaddr_struct_state_ = Sockaddr_Struct_State::CREATED;

  if (!result_) {
    std::cerr << "Error: result_ pointer to addrinfo is null after server "
                 "address resolution"
              << std::endl;
    return;
  }

  listen_socket_ = SocketWrapper{create_socket(
      result_->ai_family, result_->ai_socktype, result_->ai_protocol)};
  if (!listen_socket_.valid()) {
    std::cerr << "Failed to create server listen socket" << std::endl;
    return;
  }

  if (!bind_socket(listen_socket_.get(), result_->ai_addr,
                   static_cast<int>(result_->ai_addrlen))) {
    std::cerr << "Failed to bind server socket" << std::endl;
    return;
  }

  result_.reset();
  server_sockaddr_struct_state_ = Sockaddr_Struct_State::EMPTY;

  if (!listen_on_socket(listen_socket_.get(), maximum_pending_connections)) {
    std::cerr << "Failed to listen on socket" << std::endl;
    return;
  }
  server_initialization_status_ =
      Server_Initialization_Status::LISTENING_FOR_CONNECTION;
  std::cout << "Server is started in the listening mode." << std::endl;
}

void Server::accept_connections() {
  client_socket_ = SocketWrapper{accept_socket(listen_socket_.get())};

  if (!client_socket_.valid()) {
    std::cerr << "Failed to accept client connection" << std::endl;
    return;
  }

  server_initialization_status_ =
      Server_Initialization_Status::CLIENT_CONNECTION_HANDLED;
  std::cout << "Client connection handled for client socket: "
            << client_socket_.get() << std::endl;
  // No longer need the server socket in this particular single-thread
  // example, should be re-factored when introducing the multi-threading later
  listen_socket_.close();
}

void Server::receive_client_message() {
  recv_buffer_.clear();
  recv_buffer_.resize(recv_buffer_capacity_);

  if (!receive_until_empty_input(client_socket_.get(), recv_buffer_)) {
    std::cerr << "Failed to receive client message" << std::endl;
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
      ", lowercase letters count: " + std::to_string(lowercase_count) +
      ", vowels count: " + std::to_string(vowels_count) +
      ", consonants count: " + std::to_string(consonants_count);
}

void Server::echo_message_to_client() const {
  if (!send_buffer_content(client_socket_.get(), recv_buffer_)) {
    std::cerr << "Failed to echo message to client" << std::endl;
  };
}

void Server::send_recv_message_analytics_to_client() const {
  if (!send_buffer_content(client_socket_.get(), recv_message_analytics_)) {
    std::cerr << "Failed to send analytics to client" << std::endl;
  }
}

void Server::shutdown_message_sending() {
  if (!shutdown_sending_side(client_socket_.get())) {
    std::cerr << "Failed to shutdown server sending side" << std::endl;
    return;
  }

  server_initialization_status_ =
      Server_Initialization_Status::SHUTDOWN_FOR_SENDING;
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

size_t Server::get_recv_buffer_capacity() const {
  return recv_buffer_capacity_;
}

const std::string &Server::get_port() const { return port_; }

void Server::set_port(std::string port) { this->port_ = std::move(port); }

}  // namespace WindowsSocketApp