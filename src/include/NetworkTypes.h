#ifndef NETWORKTYPES_H
#define NETWORKTYPES_H

#include <winsock2.h>
#include <ws2tcpip.h>

#include <memory>

namespace WindowsSocketApp {
enum class Sockaddr_Struct_State { EMPTY, CREATED };

struct AddrInfoDeleter {
  void operator()(addrinfo *addrinfo_ptr) const {
    if (addrinfo_ptr != nullptr) {
      freeaddrinfo(addrinfo_ptr);
    }
  }
};

using AddrInfoPtr = std::unique_ptr<addrinfo, AddrInfoDeleter>;

}  // namespace WindowsSocketApp

#endif  // NETWORKTYPES_H
