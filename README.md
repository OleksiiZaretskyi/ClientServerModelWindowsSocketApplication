# Windows Socket Client-Server Application

A C++17 client-server application demonstrating Windows Socket (Winsock) programming with TCP communication and real-time message analytics. The project consists of two separate applications: a **Server** and a **Client**.

## 🚀 Features

- **Separate Server & Client Applications**: Independent executables for server and client functionality
- **TCP-based Communication**: Client-server message exchange using Windows Sockets
- **Interactive Configuration**: Runtime configuration for ports, server IP, and messages
- **Real-time Message Analytics**: Server analyzes received messages and provides detailed statistics
- **Error Handling**: Comprehensive socket cleanup and error management
- **Character Analysis**: Advanced text processing including vowel/consonant detection
- **Graceful Shutdown**: Proper connection termination and resource cleanup

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
4. Both Server and Client executables will be built

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

The build process creates two executables in the `build/bin` directory:
- `Server.exe` - Server application
- `Client.exe` - Client application

## 🎮 Running the Application

### Starting the Server:
1. **Run the server application**:
```shell
./build/bin/Server.exe
```
2. **Configure server settings** when prompted:
```
Enter sever port number (default port is 27015 if skipped): 
```
3. **Server will start listening** for client connections

### Running the Client:
1. **Run the client application** (in a separate terminal/window):
```shell
./build/bin/Client.exe
```
2. **Configure client settings** when prompted:
```
Enter server IP address (default: localhost): 
Enter server port (default: 27015): 
Enter message to send to server: Hello World! 123
```
3. **Client will connect** to the server and exchange messages

## 📁 Project Structure

```
ClientServerModelWindowsSocketApplication/
├── src/
│   ├── apps/
│   │   ├── server_main.cpp     # Server application entry point
│   │   └── client_main.cpp     # Client application entry point
│   ├── core/
│   │   ├── Server.cpp          # Server implementation
│   │   └── Client.cpp          # Client implementation
│   ├── include/
│   │   ├── Server.h            # Server class header
│   │   ├── Client.h            # Client class header
│   │   └── WinSockFunctions.h  # Windows Socket utility functions
│   └── utils/
│       └── HelperFunctions.h   # Text analysis utilities (vowel/consonant detection)
├── CMakeLists.txt              # Build configuration
└── README.md                   # This file
```

## 🔧 Core Components

### Server Application (`server_main.cpp` + `Server.h/cpp`)
- **Interactive Setup**: User configurable port (default: 27015)
- **Connection Handling**: Accepts and manages client connections
- **Message Processing**: Receives, analyzes, and responds to client messages
- **Analytics Engine**: Provides detailed message statistics
- **Graceful Shutdown**: Proper cleanup and connection termination

### Client Application (`client_main.cpp` + `Client.h/cpp`)
- **Interactive Setup**: User configurable server IP, port, and message
- **Server Connection**: Connects to server using TCP
- **Message Transmission**: Sends user-defined messages to server
- **Response Handling**: Receives and displays server analytics

### Utility Functions
- **WinSockFunctions.h**: Socket operations, error handling, cleanup
- **HelperFunctions.h**: Character classification (vowels/consonants)

## 📊 Message Analytics

The server provides comprehensive analysis of received messages:
- **Length**: Total character count
- **Punctuation Marks**: Count of special characters
- **Spaces**: Whitespace character count
- **Digits**: Numeric character count (0-9)
- **Uppercase Letters**: Capital letter count (A-Z)
- **Lowercase Letters**: Lowercase letter count (a-z)
- **Vowels**: Count of vowel characters (a, e, i, o, u)
- **Consonants**: Count of consonant characters

## 🛠️ Configuration

### Default Settings
- **Server Port**: 27015
- **Server IP**: localhost
- **Buffer Size**: 1024 bytes
- **Protocol**: TCP (IPPROTO_TCP)

### Runtime Customization
Both applications support interactive configuration:

#### Server Configuration:
- Port number (user input or default)

#### Client Configuration:
- Server IP address (user input or default to localhost)
- Server port (user input or default to 27015)
- Message content (user input or default message)

## 🔍 Example Usage

### Server Terminal:
```
Enter sever port number (default port is 27015 if skipped): 

Starting server on port 27015...
Server is ready. Waiting for client connections...
Client connected successfully!
Waiting for client message...
Received message from client:
Hello World! 123
Calculating message analytics...
Sending analytics to client...
Shutting down sending...
Message processing completed successfully!
Server shutdown completed.

Enter any character and press Enter to stop this application and close the window:
```

### Client Terminal:
```
Enter server IP address (default: localhost): 
Enter server port (default: 27015): 
Enter message to send to server: Hello World! 123
Connecting to server at localhost:27015...
Connected to server successfully!
Sending message: "Hello World! 123"
Shutting down sending...
Waiting for server response...
Received analytics from server:
Received message analytics: Length: 15, punctuation marks count: 1, spaces count: 2, digits count: 3, uppercase letters count: 2, lowercase laters count: 8, vowels count: 4, consonants count: 6
Communication completed successfully!
Client shutdown completed.

Enter any character and press Enter to stop this application and close the window:
```

## 🧵 Architecture Highlights

### Separate Application Design
- **Independent Executables**: Server and Client run as separate processes
- **Flexible Deployment**: Can run on same machine or across network
- **Easy Testing**: Simple to test different scenarios with multiple clients

### Character Analysis (from HelperFunctions.h)
```cpp
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
- Interactive shutdown with user confirmation

## 🚦 Usage Workflow

1. **Build** the project to create both executables
2. **Start the Server** first and configure the port
3. **Start the Client** in a separate terminal/window
4. **Configure Client** with server details and message
5. **Watch the Communication** happen automatically
6. **Review Analytics** displayed by the client
7. **Shutdown** both applications gracefully

## 🤝 Contributing

This project demonstrates Windows Socket programming concepts. Feel free to:
- Add new message analysis features
- Implement multiple client support
- Enhance error handling mechanisms
- Add configuration file support
- Create GUI versions of the applications

## 📝 Technical Notes

- **Windows-Specific**: Uses Winsock2 API (not portable to Unix systems)
- **Memory Management**: Proper handling of `addrinfo` structures and socket descriptors
- **Error Handling**: Comprehensive error checking with detailed console output
- **Interactive Design**: User-friendly console interfaces for both applications
- **Separate Build Targets**: CMake creates independent executables

## 🎯 Learning Outcomes

This project demonstrates:
- Windows Socket API usage
- TCP client-server architecture
- Separate application design patterns
- Interactive console application development
- Resource management and cleanup
- Text processing and analysis algorithms
- CMake build system with multiple targets

## 📜 License

This project is created for educational purposes, demonstrating Windows Socket programming concepts and best practices.

**Author**: Created as a learning project for Windows Socket programming and C++ network development.
