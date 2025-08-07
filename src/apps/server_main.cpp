#include "../include/Server.h"

int main() {
  // Initialize Winsock
  WSADATA wsaData;
  if (!WindowsSocketApp::initialize_winsock_2_0(wsaData)) {
    std::cerr << "Failed to initialize Winsock 2.0. Exiting from main()..."
              << std::endl;
    return -1;
  }

  // Collect server configuration from user
  std::string port{};
  std::cout << "Enter sever port number (default port is 27015 if skipped): ";
  std::getline(std::cin, port);
  if (port.empty()) {
    port = "27015";
  }

  // Create and start server
  WindowsSocketApp::Server new_server{1024, port};
  std::cout << "\nStarting server on port " << port << "..." << std::endl;
  new_server.start_server();

  if (new_server.get_server_init_status() ==
      WindowsSocketApp::Server_Initialization_Status::
          LISTENING_FOR_CONNECTION) {
    std::cout << "Server is ready. Waiting for client connections..."
              << std::endl;

    // Accept client connection
    new_server.accept_connections();

    if (new_server.get_server_init_status() ==
        WindowsSocketApp::Server_Initialization_Status::
            CLIENT_CONNECTION_HANDLED) {
      std::cout << "Client connected successfully!" << std::endl;

      // Receive message from client
      std::cout << "Waiting for client message..." << std::endl;
      new_server.receive_client_message();

      std::cout << "Received message from client:" << std::endl;
      new_server.display_recv_buffer();

      // Calculate analytics
      std::cout << "Calculating message analytics..." << std::endl;
      new_server.calculate_recv_message_analytics();

      // Send analytics back to client
      std::cout << "Sending analytics to client..." << std::endl;
      new_server.send_recv_message_analytics_to_client();

      // Shutdown sending
      std::cout << "Shutting down sending..." << std::endl;
      new_server.shutdown_message_sending();

      std::cout << "Message processing completed successfully!" << std::endl;
    }
  }
  // Cleanup
  new_server.stop_server();
  WSACleanup();

  std::cout << "Server shutdown completed." << std::endl;

  // Keep window open
  char exit_key;
  std::cout << "\nEnter any character and press Enter to stop this application "
               "and close the window:";
  std::cin >> exit_key;

  return 0;
}