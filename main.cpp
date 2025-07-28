#include <thread>

#include "Client.h"
#include "Server.h"

int main() {
  WSADATA wsaData;
  if (!WindowsSocketApp::initialize_winsock_2_0(wsaData)) {
    std::cout << "Failed to initialize Winsock 2.0. Exiting from main()..."
              << std::endl;
    return -1;
  }

  std::string client_send_buffer{};
  std::cout << "Enter the message to be sent to the server for analysis:";
  std::getline(std::cin, client_send_buffer);
  std::size_t client_send_buffer_length = client_send_buffer.size();

  WindowsSocketApp::Server new_server{
      static_cast<int>(client_send_buffer_length)};
  WindowsSocketApp::Client new_client{client_send_buffer};

  new_server.start_server();

  std::thread server_thread{&WindowsSocketApp::Server::accept_connections,
                            &new_server};
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  std::thread client_thread{&WindowsSocketApp::Client::connect_to_server,
                            &new_client};

  server_thread.join();
  client_thread.join();

  if (new_server.get_server_init_status() ==
          WindowsSocketApp::Server_Initialization_Status::
              CLIENT_CONNECTION_HANDLED &&
      new_client.get_client_init_status() ==
          WindowsSocketApp::Client_Initialization_Status::CONNECTED) {
    std::cout << "Connection between Server and Client established. Starting "
                 "message exchange..."
              << std::endl;

    std::cout << "\n=== Client sending message to server ===" << std::endl;
    new_client.send_buffer_to_server();

    std::cout << "\n=== Client shutting down sending ===" << std::endl;
    new_client.shutdown_message_sending();

    std::cout << "\n=== Server receiving message from client ===" << std::endl;
    new_server.receive_client_message();
    new_server.display_recv_buffer();

    std::cout << "\n=== Server calculating analytics on the received message "
                 "and sending it back to client ==="
              << std::endl;
    new_server.calculate_recv_message_analytics();
    new_server.send_recv_message_analytics_to_client();

    std::cout << "\n=== Server shutting down sending ===" << std::endl;
    new_server.shutdown_message_sending();

    std::cout << "\n=== Client receiving message analytics from server ==="
              << std::endl;
    new_client.receive_server_message();
    new_client.display_recv_buffer();

    std::cout << "\n=== Cleanup ===" << std::endl;
    new_client.close_connection_to_server();
    new_server.stop_server();

    std::cout << "\nMessage exchange completed successfully!" << std::endl;
  } else {
    std::cout << "\nFailed to establish connection between server and client."
              << std::endl;
  }

  WSACleanup();
  std::cout << "\nApplication cleanup completed." << std::endl;

  char exit_key;
  std::cout << "\nEnter any character and press Enter to stop this application "
               "and close the window:";
  std::cin >> exit_key;

  return 0;
}
