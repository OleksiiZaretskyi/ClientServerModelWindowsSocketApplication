#include "../include/Client.h"

int main() {
  WSADATA wsaData;
  if (!WindowsSocketApp::initialize_winsock_2_0(wsaData)) {
    std::cerr << "Failed to initialize Winsock 2.0. Exiting from main()..."
              << std::endl;
    return 1;
  }

  // Collect client configuration from user
  std::string server_ip;
  std::string port;
  std::string message;

  std::cout << "Enter server IP address (default: localhost): ";
  std::getline(std::cin, server_ip);
  if (server_ip.empty()) {
    server_ip = "localhost";
  }

  std::cout << "Enter server port (default: 27015): ";
  std::getline(std::cin, port);
  if (port.empty()) {
    port = "27015";
  }

  std::cout << "Enter message to send to server: ";
  std::getline(std::cin, message);
  if (message.empty()) {
    message = "Hello from client!";
  }

  // Create client
  {  // Open scope for the Client object
    WindowsSocketApp::Client client{message, 1024, server_ip, port};

    std::cout << "Connecting to server at " << server_ip << ":" << port << "..."
              << std::endl;

    // Connect to server
    client.connect_to_server();

    if (client.get_client_init_status() ==
        WindowsSocketApp::Client_Initialization_Status::CONNECTED) {
      std::cout << "Connected to server successfully!" << std::endl;

      // Send the message
      std::cout << "Sending message: \"" << message << "\"" << std::endl;
      client.send_buffer_to_server();

      // Shutdown sending
      std::cout << "Shutting down sending..." << std::endl;
      client.shutdown_message_sending();

      // Receive response
      std::cout << "Waiting for server response..." << std::endl;
      client.receive_server_message();

      std::cout << "Received analytics from server:" << std::endl;
      client.display_recv_buffer();

      std::cout << "Communication completed successfully!" << std::endl;
    } else {
      std::cerr << "Failed to connect to server." << std::endl;
    }

    std::cout << "Client shutting down..." << std::endl;

  }  // Client destructor automatically called to clean up the resources.

  // Cleanup Winsock
  WSACleanup();
  std::cout << "Client shutdown completed." << std::endl;

  // Keep the window open
  std::cout << "\nPress Enter to exit...";
  std::cin.get();

  return 0;
}
