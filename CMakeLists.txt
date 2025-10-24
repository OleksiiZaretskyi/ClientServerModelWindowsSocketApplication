cmake_minimum_required(VERSION 3.31)
project(ClientServerModelWindowsSocketApplication)

set(CMAKE_CXX_STANDARD 17)

set(SERVER_SOURCES
        src/core/Server.cpp
)

set(CLIENT_SOURCES
        src/core/Client.cpp
)

set(SERVER_HEADERS
        src/include/Server.h
        src/include/WinSockFunctions.h
        src/utils/HelperFunctions.h
        src/include/NetworkTypes.h
        src/include/SocketWrapper.h
)

set(CLIENT_HEADERS
        src/include/Client.h
        src/include/WinSockFunctions.h
        src/include/NetworkTypes.h
        src/include/SocketWrapper.h
)

add_executable(Server
        src/apps/server_main.cpp
        ${SERVER_SOURCES}
        ${SERVER_HEADERS}
)

add_executable(Client
        src/apps/client_main.cpp
        ${CLIENT_SOURCES}
        ${CLIENT_HEADERS}
)

# Link Windows socket libraries
if(WIN32)
    target_link_libraries(Server
            ws2_32      # Winsock 2.0
            wsock32     # Winsock 1.1 (for compatibility)
            mswsock     # Microsoft Winsock extensions
            advapi32    # Advapi32.lib
    )

    target_link_libraries(Client
            ws2_32      # Winsock 2.0
            wsock32     # Winsock 1.1 (for compatibility)
            mswsock     # Microsoft Winsock extensions
            advapi32    # Advapi32.lib
    )
endif()

set_target_properties(Server Client PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
