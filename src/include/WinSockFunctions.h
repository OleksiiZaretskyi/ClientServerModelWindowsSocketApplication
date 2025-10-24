#ifndef WINSOCKFUNCTIONS_H
#define WINSOCKFUNCTIONS_H

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

#include "NetworkTypes.h"

// Need to link with Ws2_32.lib for Server
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib for Client
// #pragma comment (lib, "Ws2_32.lib"). Commented out for MinGW Compiler,
// Windows socket libraries are linked in CMakeLists.txt #pragma comment (lib,
// "Mswsock.lib"). Commented out for MinGW Compiler, Windows socket libraries
// are linked in CMakeLists.txt #pragma comment (lib, "AdvApi32.lib"). Commented
// out for MinGW Compiler, Windows socket libraries are linked in CMakeLists.txt

namespace WindowsSocketApp {

inline bool initialize_winsock_2_0(WSADATA &wsaData) {
  int i_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (i_result != 0) {
    std::cout << "WSAStartup failed with error: " << i_result << std::endl;
    return false;
  }
  return true;
}

inline bool resolve_address_and_port(
    const char *nodename, const char *servname, const struct addrinfo *hints,
    std::unique_ptr<addrinfo, AddrInfoDeleter> &res) {
  addrinfo *raw_result{nullptr};
  int i_result = getaddrinfo(nodename, servname, hints, &raw_result);
  if (i_result != 0) {
    std::cout << "getaddrinfo failed with error: " << i_result << std::endl;
    return false;
  }
  res.reset(raw_result);
  return true;
}

inline SOCKET create_socket(int af, int type, int protocol) {
  SOCKET new_socket = socket(af, type, protocol);
  if (new_socket == INVALID_SOCKET) {
    std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
    return INVALID_SOCKET;
  }
  return new_socket;
}

inline bool bind_socket(SOCKET s, const sockaddr *name, int namelen) {
  auto i_result = bind(s, name, namelen);
  if (i_result == SOCKET_ERROR) {
    std::cout << "bind failed with error: " << WSAGetLastError() << std::endl;
    return false;
  }
  return true;
}

inline bool listen_on_socket(SOCKET s, int backlog) {
  auto i_result = listen(s, backlog);
  if (i_result == SOCKET_ERROR) {
    std::cout << "Listen failed with error: " << WSAGetLastError() << std::endl;
    return false;
  }
  return true;
}

inline SOCKET accept_socket(SOCKET listen_socket) {
  SOCKET new_socket = accept(listen_socket, nullptr, nullptr);
  if (new_socket == INVALID_SOCKET) {
    std::cout << "accept failed: " << WSAGetLastError() << std::endl;
    return INVALID_SOCKET;
  }
  return new_socket;
}

inline bool receive_until_empty_input(SOCKET sender_socket,
                                      std::vector<char> &recv_buffer) {
  int i_receive_result{-1};
  do {
    i_receive_result = recv(sender_socket, recv_buffer.data(),
                            static_cast<int>(recv_buffer.size()), 0);
    if (i_receive_result > 0) {
      std::cout << "Bytes received: " << i_receive_result << '\n';
      recv_buffer.resize(i_receive_result);
    } else if (i_receive_result == 0)
      std::cout << "Connection closing...\n";
    else {
      std::cout << "recv failed with error: " << WSAGetLastError() << std::endl;
      return false;
    }

  } while (i_receive_result > 0);
  return true;
}
// Overloaded version of send_buffer_content for vector buffers
inline bool send_buffer_content(SOCKET receiver_socket,
                                const std::vector<char> &buffer) {
  auto i_send_result =
      send(receiver_socket, buffer.data(), static_cast<int>(buffer.size()), 0);
  if (i_send_result == SOCKET_ERROR) {
    std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
    return false;
  }
  std::cout << "Bytes sent: " << i_send_result << std::endl;
  return true;
}
// Overloaded version of send_buffer_content for std::string buffers
inline bool send_buffer_content(SOCKET receiver_socket,
                                const std::string &buffer) {
  auto i_send_result =
      send(receiver_socket, buffer.c_str(), static_cast<int>(buffer.size()), 0);
  if (i_send_result == SOCKET_ERROR) {
    std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
    return false;
  }
  std::cout << "Bytes sent: " << i_send_result << std::endl;
  return true;
}

inline bool shutdown_sending_side(SOCKET receiver_socket) {
  auto i_send_result = shutdown(receiver_socket, SD_SEND);
  if (i_send_result == SOCKET_ERROR) {
    std::cout << "shutdown failed: " << WSAGetLastError() << std::endl;
    return false;
  }
  return true;
}

}  // namespace WindowsSocketApp

#endif  // WINSOCKFUNCTIONS_H
