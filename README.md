# Windows Socket Client-Server Application

A C++17 client-server application demonstrating Windows Socket (Winsock) programming with TCP communication and real-time message analytics.

## 🚀 Features

- **TCP-based Communication**: Client-server message exchange using Windows Sockets
- **Real-time Message Analytics**: Server analyzes received messages and provides statistics in the response
- **Multi-threaded Architecture**: Concurrent server and client operations using `std::thread`
- **Error Handling**: Socket cleanup and error management
- **Character Analysis**: Text processing including vowel/consonant detection
- **Shutdown**: Connection termination and resource cleanup

## 📋 Requirements

- **Operating System**: Windows 10 or later
- **Compiler**: C++17 compatible MinGW compiler 
- **Build System**: CMake 3.31 or later
- **IDE**: CLion (recommended) or Visual Studio
- **Libraries**: Windows Socket API (Winsock2)

## 🏗️ Building the Project

### Using CLion:
1. Open the project in CLion
2. CLion will automatically configure CMake
3. Build the project using `Ctrl+F9` or **Build** → **Build Project**

### Using Command Line:
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```
### Using CMake directly:
```shell
cmake -B build -S .
cmake --build build
```
## 🎮 Running the Application

1. **Start the application**:
```shell
./ClientServerModelWindowsSocketApplication.exe
```
2. **Enter your message** when prompted:
```
Enter the message to be sent to the server for analysis: Hello World! 123
```
3. **View the results**: The application will display:
- Connection establishment messages
- Message transmission details
- Message analytics
- Clean shutdown confirmation

## 📁 Project Structure

```
ClientServerModelWindowsSocketApplication/
├── main.cpp                # Application entry point and orchestration
├── Server.h/cpp            # Server class implementation
├── Client.h/cpp            # Client class implementation
├── WinSockFunctions.h      # Windows Socket utility functions
├── HelperFunctions.h       # Text analysis utilities (vowel/consonant detection)
├── CMakeLists.txt          # Build configuration
└── README.md               # This file
```
## 🔧 Core Components
### Server (`Server.h/cpp`)
- **Port Binding**: Listens on configurable port (default: 27015)
- **Connection Handling**: Accepts and manages client connections
- **Message Processing**: Receives, analyzes, and responds to client messages
- **Analytics Engine**: Provides detailed message statistics

### Client (`Client.h/cpp`)
- **Server Connection**: Connects to server using TCP
- **Message Transmission**: Sends user input to server
- **Response Handling**: Receives and displays server analytics

### Utility Functions
- : Socket operations, error handling, cleanup **WinSockFunctions.h**
- : Character classification (vowels/consonants) **HelperFunctions.h**

## 📊 Message Analytics
The server provides analysis of the received messages:
- **Length**: Total character count
- **Punctuation Marks**: Count of special characters
- **Spaces**: Whitespace character count
- **Digits**: Numeric character count (0-9)
- **Uppercase Letters**: Capital letter count (A-Z)
- **Lowercase Letters**: Lowercase letter count (a-z)
- **Vowels**: Count of vowel characters (a, e, i, o, u)
- **Consonants**: Count of consonant characters

## 🧵 Architecture Highlights
### Multi-threading Design
``` cpp
std::thread server_thread {&WindowsSocketApp::Server::accept_connections, &new_server};
std::thread client_thread {&WindowsSocketApp::Client::connect_to_server, &new_client};
```
### Character Analysis (from HelperFunctions.h)
``` cpp
inline bool is_vowel(char c) {
    switch (c | 0x20) {  // Bitwise conversion to lowercase
        case 'a': case 'e': case 'i': case 'o': case 'u':
            return true;
        default:
            return false;
    }
}
```
### Resource Management
- Automatic socket cleanup using RAII principles
- `addrinfo` structure management
- WSACleanup() ensures proper Winsock termination

## 🛠️ Configuration
### Default Settings
- **Server Port**: 27015
- **Server IP**: localhost
- **Buffer Size**: 1024 bytes (dynamically adjustable)
- **Protocol**: TCP (IPPROTO_TCP)

### Customization
The application supports runtime customization:
- Message content (user input)
- Buffer sizes (automatic adjustment based on message length)
- Connection parameters (modifiable through class methods)

## 🔍 Example Output
``` 
Enter the message to be sent to the server for analysis: Hello World! 123
Server is started in the listening mode.
Client successfully connected to server.
Connection between Server and Client established. Starting message exchange...

=== Client sending message to server ===
Bytes sent: 15

=== Server receiving message from client ===
Bytes received: 15
Hello World! 123

=== Server calculating analytics ===
Received message analytics: Length: 15, punctuation marks count: 1, spaces count: 2, digits count: 3, uppercase letters count: 2, lowercase laters count: 8, vowels count: 4, consonants count: 6

=== Cleanup ===
Message exchange completed successfully!
Application cleanup completed.
```
## 🤝 Contributing
This project demonstrates Windows Socket programming concepts. Feel free to:
- Add new message analysis features
- Implement additional client-server protocols
- Enhance error handling mechanisms
- Add configuration file support

## 📝 Technical Notes
- **Windows-Specific**: Uses Winsock2 API (not portable to Unix systems)
- **Memory Management**: Handling of `addrinfo` structures and socket descriptors
- **Thread Safety**: Proper synchronization between server and client threads
- **Error Handling**: Error checking with detailed output to the terminal window

## 🎯 Learning Outcomes
This project demonstrates:
- Windows Socket API usage
- TCP client-server architecture
- Multi-threaded programming in C++
- Resource management and cleanup
- Text processing and analysis algorithms
- CMake build system configuration

## 📜 License
This project is created for educational purposes, demonstrating Windows Socket programming concepts and best practices.

**Author**: Created as a learning project for Windows Socket programming and C++ network development.