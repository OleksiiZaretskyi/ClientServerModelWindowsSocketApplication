# Client-Server Model Windows Socket Application

C++17 client-server application demonstrating Windows Socket (Winsock) programming with TCP communication and real-time message analytics. Project consists of two separate applications: a **Server** and a **Client**.

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Requirements](#requirements)
- [Building](#building)
- [Usage](#usage)
- [Design Patterns](#design-patterns)
- [Code Quality](#code-quality)

## Overview
🎯
Project implements a simple client-server communication system where:
- **Server**: Listens for client connections, receives messages, analyzes them, and sends back analytics
- **Client**: Connects to server, sends messages, and receives analytics

The server performs text analysis on received messages, counting:
- Characters (uppercase, lowercase, digits, spaces, punctuation)
- Vowels and consonants

## Features

### ✨ Core Functionality
- ✅ TCP socket communication using WinSock2
- ✅ Text message transmission
- ✅ Real-time message analytics
- ✅ Connection handling
- ✅ Resource cleanup

### C++ Practices
- ✅ **RAII** (Resource Acquisition Is Initialization)
- ✅ **Smart pointers** (`std::unique_ptr` with custom deleters)
- ✅ **Move semantics** (non-copyable socket wrappers)
- ✅ **Exception safety**
- ✅ **Automatic resource management**
- ✅ **No memory leaks**

##  Project Structure
### 🏗️ Key Components

#### **SocketWrapper.h**
RAII wrapper for Windows SOCKET handles:
- Automatic socket cleanup on destruction
- Move-only semantics (non-copyable)
- Prevents resource leaks and double-close bugs

#### **NetworkTypes.h**
Common network type definitions:
- `AddrInfoDeleter`: Custom deleter for `addrinfo` structures
- `AddrInfoPtr`: Type alias for `std::unique_ptr<addrinfo, AddrInfoDeleter>`
- `Sockaddr_Struct_State`: Enum for address info state tracking

#### **WinSockFunctions.h**
Socket utility functions:
- Winsock initialization
- Address resolution
- Socket operations (bind, listen, accept, send, receive)
- Message sending shutdown

#### **Server.h/cpp**
Server implementation:
- Listens on specified port
- Accepts client connections
- Receives and analyzes messages
- Sends analytics back to the client

#### **Client.h/cpp**
Client implementation:
- Connects to server
- Sends messages
- Receives analytics
## Requirements
🔧
### System Requirements
- **OS**: Windows 10/11 (or Windows Server)
- **Compiler**: MSVC, MinGW-w64, or Clang with C++17 support
- **CMake**: Version 3.31 or higher
- **Winsock**: WinSock 2.0 (included with Windows)

### Libraries (Linked Automatically)
- `ws2_32.lib` - Winsock 2.0
- `wsock32.lib` - Winsock 1.1 (compatibility)
- `mswsock.lib` - Microsoft Winsock extensions
- `advapi32.lib` - Advanced Windows API

## Building

### 🔨 Using CMake (Recommended)

```bash
# Create build directory
mkdir build
cd build

# Configure
cmake ..

# Build
cmake --build .

# Executables will be in: build/bin/Server.exe and build/bin/Client.exe
```
### Using CMake GUI
1. Open CMake GUI
2. Set the source directory to the project root
3. Set build directory to `build/`
4. Click "Configure" → "Generate"
5. Open the generated solution/project
6. Build in your IDE

## Usage
🚀
### Running the Server
``` bash
# Navigate to build output
cd cmake-build-debug/bin

# Run server
./Server.exe

# Follow prompts:
Enter server port number (default: 27015): [Press Enter or type port]
```

Server will:
* Start listening on the specified port
* Wait for client connections
* Receive a message from the client
* Analyze the message
* Send analytics back to the client
* Shutdown the sending
### Running the Client
``` bash
# In a separate terminal
cd cmake-build-debug/bin

# Run client
./Client.exe

# Follow prompts:
Enter server IP address (default: localhost): [Press Enter or type IP]
Enter server port (default: 27015): [Press Enter or type port]
Enter message to send to server: Hello World!
```

Client will:
* Connect to the specified server
* Send a message to the server
* Receive analytics from the server
* Display results
* Disconnect
* Example Session

### Server Output:
```
Enter server port number (default: 27015):
Starting server on port 27015...
Server is started in the listening mode.
Server is ready. Waiting for client connections...
Client connected successfully!
Waiting for client message...
Bytes received: 12
Connection closing...
Received message from client:
Hello World!
Calculating message analytics...
Sending analytics to client...
Bytes sent: 173
Shutting down sending...
Message processing completed successfully!
Server is shutting down...
```

### Client Output:
``` 
Enter server IP address (default: localhost): 
Enter server port (default: 27015): 
Enter message to send to server: Hello World!
Connecting to server at localhost:27015...
Client successfully connected to server.
Connected to server successfully!
Sending message: "Hello World!"
Bytes sent: 12
Shutting down sending...
Waiting for server response...
Bytes received: 173
Connection closing...
Received analytics from server:
Received message analytics: Length: 12, punctuation marks count: 1, spaces count: 1, digits count: 0, uppercase letters count: 2, lowercase letters count: 8, vowels count: 3, consonants count: 7
Communication completed successfully!
```

## Design Patterns
🎨
* RAII (Resource Acquisition Is Initialization)
* All resources are managed automatically:
``` cpp
  {
  SocketWrapper socket{create_socket(...)};
  // Use socket...
  } // Socket automatically closed here - no manual cleanup needed!
```

* Smart Pointers with Custom Deleters
``` cpp
struct AddrInfoDeleter {
    void operator()(addrinfo* ptr) const noexcept {
        if (ptr != nullptr) {
            freeaddrinfo(ptr);
        }
    }
};

using AddrInfoPtr = std::unique_ptr<addrinfo, AddrInfoDeleter>;
```

* Move Semantics
* Sockets are move-only to prevent double-close bugs:
``` cpp
class SocketWrapper {
SocketWrapper(SocketWrapper&&) noexcept = default;           // ✅ Move allowed
SocketWrapper(const SocketWrapper&) = delete;                // ❌ Copy forbidden
};
```

## Code Quality
📊
### Safety Features
* ✅ No raw pointers - All managed by smart pointers or RAII wrappers
* ✅ No manual cleanup - Destructors handle everything
* ✅ Exception safe - Resources cleaned up even on exceptions
* ✅ No memory leaks - Verified with proper RAII usage
* ✅ Type safety - Strong typing with enums and type aliases

### Error Handling
* Error messages with std::cerr
* Validation at appropriate layers
* Failure modes
* Status enums for state tracking
* Code Style
* C++17 features
* Consistent naming conventions
* Clarifying comments
* Separation of concerns

### 🛠️ Customization
Changing Default Port in Server.h and Client.h:
``` cpp
inline static const char *default_port{"27015"};  // Change this
```

Adjusting Buffer Size in Server.h and Client.h:
``` cpp
static constexpr size_t default_buffer_length{1024};  // Change this
```

Adding Custom Analytics:
* Modify Server::calculate_recv_message_analytics() in Server.cpp:
``` cpp
void Server::calculate_recv_message_analytics() {
    // Add your custom analysis here
}
```

### 📝 Future Enhancements
Potential improvements:
* Multithreaded server (handle multiple clients)
* Asynchronous I/O
* SSL/TLS encryption
* Message protocol (length-prefixed messages)
* Connection pooling
* Logging framework
* Unit tests
* Cross-platform support (Linux/macOS with Berkeley sockets)

## 📄 License
This project is provided as-is for educational purposes.

## 👤 Author
Developed as a learning project for C++ socket programming with RAII principles.