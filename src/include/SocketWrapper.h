#ifndef SOCKETWRAPPER_H
#define SOCKETWRAPPER_H

#include <winsock2.h>

namespace WindowsSocketApp {

class SocketWrapper {
 private:
  SOCKET socket_;

 public:
  SocketWrapper() : socket_{INVALID_SOCKET} {}
  explicit SocketWrapper(SOCKET socket_val) : socket_{socket_val} {}

  ~SocketWrapper() { close(); }

  SocketWrapper(SocketWrapper &&source) noexcept : socket_{source.socket_} {
    source.socket_ = INVALID_SOCKET;
  }

  SocketWrapper &operator=(SocketWrapper &&rhs) noexcept {
    if (this != &rhs) {
      this->close();
      this->socket_ = rhs.socket_;
      rhs.socket_ = INVALID_SOCKET;
    }
    return *this;
  }

  SocketWrapper(const SocketWrapper &source) = delete;
  SocketWrapper &operator=(const SocketWrapper &rhs) = delete;

  void close() {
    if (INVALID_SOCKET != socket_) {
      closesocket(socket_);
      socket_ = INVALID_SOCKET;
    }
  }

  [[nodiscard]] SOCKET get() const {return socket_;};

  [[nodiscard]] bool valid() const {return socket_ != INVALID_SOCKET;}

  explicit operator SOCKET() const {return socket_;}

  SOCKET release() {
    SOCKET temp = socket_;
    socket_ = INVALID_SOCKET;
    return temp;
  }
};

}  // namespace WindowsSocketApp
#endif  // SOCKETWRAPPER_H
