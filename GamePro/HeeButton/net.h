// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// net.h v1.2

#ifndef _NET_H                  // Prevent multiple definitions if this 
#define _NET_H                  // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

class Net;

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#pragma comment(lib,"Ws2_32.lib")

// Network I/O

namespace netNS
{
    const USHORT DEFAULT_PORT = 48161;
    const int MIN_PORT = 1024;
    const int BUFFER_LENGTH = 4096;
    const int IP_SIZE = 16;     // size of "nnn.nnn.nnn.nnn"

    // Mode
    const int UNINITIALIZED = 0;
    const int SERVER = 1;
    const int CLIENT = 2;

    // Connection type
    const int UNCONNECTED = -1;
    const int UDP = 0;
    const int TCP = 1;
    const int UNCONNECTED_TCP = 2;
    const int CONNECTED_TCP = 3;

    // Status codes
    const int STATUS_MASK = 0X0FFFF;    // AND with return to reveal Status code
    const int NET_OK = 0;
    const int NET_ERROR = 1;
    const int NET_INIT_FAILED = 2;
    const int NET_INVALID_SOCKET = 3;
    const int NET_GET_HOST_BY_NAME_FAILED = 4;
    const int NET_BIND_FAILED = 5;
    const int NET_CONNECT_FAILED = 6;
    const int NET_ADDR_IN_USE = 7;
    const int NET_DOMAIN_NOT_FOUND = 8;

    const int PACKETS_PER_SEC = 30;     // Number of packets to send per second
    const float NET_TIME = 1.0f/PACKETS_PER_SEC;    // time between net transmissions
    const int MAX_ERRORS = PACKETS_PER_SEC*30;  // Packets/Sec * 30 Sec
    const int MAX_COMM_WARNINGS = 10;       // max packets out of sync before time reset

    // Connection response messages, ===== MUST BE SAME SIZE =====
    const int RESPONSE_SIZE = 12;
    const char CLIENT_ID[RESPONSE_SIZE]   = "Client v1.0";  // client ID
    const char SERVER_ID[RESPONSE_SIZE]   = "Server v1.1";  // server ID
    const char SERVER_FULL[RESPONSE_SIZE] = "Server Full";  // server full

    const int ERROR_CODES = 10;
    // Network error codes
    static const char *codes[ERROR_CODES] = {
        "No errors reported",
        "General network error: ",
        "Network init failed: ",
        "Invalid socket: ",
        "Get host by name failed: ",
        "Bind failed: ",
        "Connect failed: ",
        "Port already in use: ",
        "Domain not found: ",
        "Unknown network error: "
    };

    struct ErrorCode
    {
        int sockErr;        // Windows Socket Error Code
        char *message;
    };

    const int REMOTE_DISCONNECT = 0x2775;

    const int SOCK_CODES = 29;
    // Windows Socket Error Codes
    static const ErrorCode errorCodes[SOCK_CODES] = {
        {0x2714, "A blocking operation was interrupted"},
        {0x271D, "Socket access permission violation"},
        {0x2726, "Invalid argument"},
        {0x2728, "Too many open sockets"},
        {0x2735, "Operation in progress"},
        {0x2736, "Operation on non-socket"},
        {0x2737, "Address missing"},
        {0x2738, "Message bigger than buffer"},
        {0x273F, "Address incompatible with protocol"},
        {0x2740, "Address is already in use"},
        {0x2741, "Address not valid in current context"},
        {0x2742, "Network is down"},
        {0x2743, "Network unreachable"},
        {0x2744, "Connection broken during operation"},
        {0x2745, "Connection aborted by host software"},
        {0x2746, "Connection reset by remote host"},
        {0x2747, "Insufficient buffer space"},
        {0x2748, "Connect request on already connected socket"},
        {0x2749, "Socket not connected or address not specified"},
        {0x274A, "Socket already shut down"},
        {0x274C, "Operation timed out"},
        {0x274D, "Connection refused by target"},
        {0x274E, "Cannot translate name"},
        {0x274F, "Name too long"},
        {0x2750, "Destination host down"},
        {0x2751, "Host unreachable"},
        {0x276B, "Network cannot initialize, system unavailable"},
        {0x276D, "Network has not been initialized"},
        {0x2775, "Remote has disconnected"},
    };

}

class Net 
{
private:
    // Network Variables
    UINT         bufLength;     // Length of send and receive buffers
    WSADATA      wsd;
    SOCKET       sock;
    int          ret;
    int          remoteAddrSize;
    SOCKADDR_IN  remoteAddr, localAddr;
    bool         netInitialized;
    bool         bound;
    char         mode;
    int          type;

    //=============================================================================
    // Initialize network (for class use only)
    // protocol = UDP or TCP
    // Called by netCreateServer and netCreatClient
    // Pre:
    //   port = Port number.
    //   protocol = UDP or TCP.
    // Post:
    //   Returns two part int code on error.
    //     The low 16 bits contains Status code as defined in net.h.
    //     The high 16 bits contains "Windows Socket Error Code".
    //=============================================================================
    int initialize(int port, int protocol);    

public:
    // Constructor
    Net();
    // Destructor
    virtual ~Net();

    //=============================================================================
    // Setup network for use as server
    // May not be configured as Server and Client at the same time.
    // Pre: 
    //   port = Port number to listen on.
    //     Port numbers 0-1023 are used for well-known services.
    //     Port numbers 1024-65535 may be freely used.
    //   protocol = UDP or TCP
    // Post:
    //   Returns NET_OK on success
    //   Returns two part int code on error.
    //     The low 16 bits contains Status code as defined in net.h.
    //     The high 16 bits contains "Windows Socket Error Code".
    //   *server = IP address connected to as null terminated string.
    //=============================================================================
    int createServer(int port, int protocol);

    //=============================================================================
    // Setup network for use as a Client
    // Pre: 
    //   *server = IP address of server to connect to as null terminated
    //     string (e.g. "192.168.1.100") or null terminated hostname
    //     (e.g. "www.programming2dgames.com").
    //   port = Port number. Port numbers 0-1023 are used for well-known services.
    //     Port numbers 1024-65535 may be freely used.
    //   protocol = UDP or TCP
    // Post:
    //   Returns NET_OK on success
    //   Returns two part int code on error.
    //     The low 16 bits contains Status code as defined in net.h.
    //     The high 16 bits contains "Windows Socket Error Code".
    //   *server = IP address connected to as null terminated string.
    //=============================================================================
    int createClient(char *server, int port, int protocol);

    //=============================================================================
    // Send data
    // Pre:
    //   *data = Data to send
    //   size = Number of bytes to send
    //   *remoteIP = Destination IP address as null terminated char array
    //   port = Destination port number.
    // Post: 
    //   Returns NET_OK on success. Success does not indicate data was sent.
    //   Returns two part int code on error.
    //     The low 16 bits contains Status code as defined in net.h.
    //     The high 16 bits contains "Windows Socket Error Code".
    //   size = Number of bytes sent, 0 if no data sent, unchanged on error.
    //=============================================================================
    int sendData(const char *data, int &size, const char *remoteIP);
    int sendData(const char *data, int &size, const char *remoteIP, const USHORT port);

    //=============================================================================
    // Read data
    // Pre:
    //   *data = Buffer for received data.
    //   size = Number of bytes to receive.
    //   *senderIP = NULL
    //   port = undefined
    // Post: 
    //   Returns NET_OK on success.
    //   Returns two part int code on error.
    //     The low 16 bits contains Status code as defined in net.h.
    //     The high 16 bits contains "Windows Socket Error Code".
    //   size = Number of bytes received, may be 0. Unchanged on error.
    //   *senderIP = IP address of sender as null terminated string.
    //   port = port number of sender.
    //=============================================================================
    int readData(char *data, int &size, char *senderIP);
    int readData(char *data, int &size, char *senderIP, USHORT &port);

    //=============================================================================
    // Close socket and free resources
    // Post:
    //   Socket is closed and buffer memory is released.
    //   Returns two part int code on error.
    //     The low 16 bits contains Status code as defined in net.h.
    //     The high 16 bits contains "Windows Socket Error Code".
    //=============================================================================
    int closeSocket();

    //=============================================================================
    // Get the IP address of this computer as a string
    // Post:
    //   *localIP = IP address of local computer as null terminated string on success.
    //   Returns two part int code on error.
    //     The low 16 bits contains Status code as defined in net.h.
    //     The high 16 bits contains "Windows Socket Error Code".
    //=============================================================================
    int getLocalIP(char *localIP);

    //=============================================================================
    // Return mode
    // Valid modes are: UNINITIALIZED, SERVER, CLIENT
    //=============================================================================
    char getMode()      {return mode;}

    //=============================================================================
    // Returns detailed error message from two part error code
    //=============================================================================
    std::string getError(int error);
};

#endif



