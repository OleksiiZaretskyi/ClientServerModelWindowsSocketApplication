cmake_minimum_required(VERSION 3.31)
project(ClientServerModelWindowsSocketApplication)

set(CMAKE_CXX_STANDARD 17)

add_executable(ClientServerModelWindowsSocketApplication main.cpp
        Server.cpp
        Server.h
        WinSockFunctions.h
        Client.cpp
        Client.h
        HelperFunctions.h)

# Link Windows socket libraries
if(WIN32)
    target_link_libraries(ClientServerModelWindowsSocketApplication
            ws2_32      # Winsock 2.0
            wsock32     # Winsock 1.1 (for compatibility)
            mswsock     # Microsoft Winsock extensions
            advapi32    # Advapi32.lib
    )
endif()
