// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// net.cpp v1.0

#include "net.h"
using namespace netNS;

//=============================================================================
// Constructor
//=============================================================================
Net::Net()
{
    bufLength = BUFFER_LENGTH;     // Length of send and receive buffers
    sock = NULL;
    ret = 0;
    remoteAddrSize = 0;
    netInitialized = false;
    bound = false;
    mode = UNINITIALIZED;
    type = UNCONNECTED;
}

//=============================================================================
// Destructor
//=============================================================================
Net::~Net()
{
    closeSocket();                  // close connection, release memory
}

//=============================================================================
// Initialize network
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
int Net::initialize(int port, int protocol)
{
    unsigned long ul = 1;
    int           nRet;
    int status;

    if(netInitialized)              // if network currently initialized
        closeSocket();              // close current network and start over

    mode = UNINITIALIZED;

    status = WSAStartup(0x0202, &wsd);  // initiate the use of winsock 2.2
    if (status != 0)
        return ( (status << 16) + NET_INIT_FAILED);

    switch (protocol)
    {
    case UDP:     // UDP
        // Create UDP socket and bind it to a local interface and port
        sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sock == INVALID_SOCKET) {
            WSACleanup();
            status = WSAGetLastError();         // get detailed error
            return ( (status << 16) + NET_INVALID_SOCKET);
        }
        type = UDP;
        break;
    case TCP:     // TCP
        // Create TCP socket and bind it to a local interface and port
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock == INVALID_SOCKET) {
            WSACleanup();
            status = WSAGetLastError();         // get detailed error
            return ( (status << 16) + NET_INVALID_SOCKET);
        }
        type = UNCONNECTED_TCP;
        break;
    default:    // Invalid type
        return (NET_INIT_FAILED);
    }

    // put socket in non-blocking mode
    nRet = ioctlsocket(sock, FIONBIO, (unsigned long *) &ul);
    if (nRet == SOCKET_ERROR) {
        WSACleanup();
        status = WSAGetLastError();             // get detailed error
        return ( (status << 16) + NET_INVALID_SOCKET);
    }

    // set local family and port
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons((u_short)port);    // port number

    // set remote family and port
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons((u_short)port);   // port number

    netInitialized = true;
    return NET_OK;
}

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
//=============================================================================
int Net::createServer(int port, int protocol) 
{
    int status;

    // ----- Initialize network stuff -----
    status = initialize(port, protocol);
    if (status != NET_OK)
        return status;

    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);    // listen on all addresses

    // bind socket
    if (bind(sock, (SOCKADDR *)&localAddr, sizeof(localAddr)) == SOCKET_ERROR)
    {
        status = WSAGetLastError();          // get detailed error
        return ((status << 16) + NET_BIND_FAILED);
    }
    bound = true;
    mode = SERVER;

    return NET_OK;
}

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
int Net::createClient(char *server, int port, int protocol) 
{
    int status;
    char localIP[IP_SIZE];  // IP as string (e.g. "192.168.1.100");
    ADDRINFOA host;
    ADDRINFOA *result = NULL;

    // ----- Initialize network stuff -----
    status = initialize(port, protocol);
    if (status != NET_OK)
        return status;

    // if server does not contain a dotted quad IP address nnn.nnn.nnn.nnn
    if ((remoteAddr.sin_addr.s_addr = inet_addr(server)) == INADDR_NONE)
    {
        // setup host structure for use in getaddrinfo() function
        ZeroMemory(&host, sizeof(host));
        host.ai_family = AF_INET;
        host.ai_socktype = SOCK_STREAM;
        host.ai_protocol = IPPROTO_TCP;

        // get address information
        status = getaddrinfo(server,NULL,&host,&result);
        if(status != 0)                 // if getaddrinfo failed
        {
            status = WSAGetLastError();
            return ((status << 16) + NET_DOMAIN_NOT_FOUND);
        }
        // get IP address of server
        remoteAddr.sin_addr = ((SOCKADDR_IN *) result->ai_addr)->sin_addr;
        strncpy_s(server, IP_SIZE, inet_ntoa(remoteAddr.sin_addr), IP_SIZE);
    }

    // set local IP address
    getLocalIP(localIP);          // get local IP
    localAddr.sin_addr.s_addr = inet_addr(localIP);   // local IP

    mode = CLIENT;
    return NET_OK;
}

//=============================================================================
// Send data to remote IP on current port
// Pre:
//   *data = Data to send
//   size = Number of bytes to send
//   if SERVER
//     *remoteIP = Destination IP address as null terminated char array
// Post: 
//   Returns NET_OK on success. Success does not indicate data was sent.
//   Returns two part int code on error.
//     The low 16 bits contains Status code as defined in net.h.
//     The high 16 bits contains "Windows Socket Error Code".
//   size = Number of bytes sent, 0 if no data sent.
//=============================================================================
int Net::sendData(const char *data, int &size, const char *remoteIP) 
{
    int status;
    int sendSize = size;

    size = 0;       // assume 0 bytes sent, changed if send successful

    if (mode == SERVER)
        remoteAddr.sin_addr.s_addr = inet_addr(remoteIP);

    if(mode == CLIENT && type == UNCONNECTED_TCP) 
    {
        ret = connect(sock,(SOCKADDR*)(&remoteAddr),sizeof(remoteAddr));
        if (ret == SOCKET_ERROR) {
            status = WSAGetLastError();
            if (status == WSAEISCONN)   // if connected
            {
                ret = 0;          // clear SOCKET_ERROR
                type = CONNECTED_TCP;
            } 
            else 
            {
                if ( status == WSAEWOULDBLOCK || status == WSAEALREADY) 
                    return NET_OK;  // no connection yet
                else 
                    return ((status << 16) + NET_ERROR);
            }
        }
    }

    ret = sendto(sock, data, sendSize, 0, (SOCKADDR *)&remoteAddr, sizeof(remoteAddr));
    if (ret == SOCKET_ERROR) 
    {
        status = WSAGetLastError();
        return ((status << 16) + NET_ERROR);
    }
    bound = true;         // automatic binding by sendto if unbound
    size = ret;           // number of bytes sent, may be 0
    return NET_OK;
}

//=============================================================================
// Send data to remoteIP and port
// Pre:
//   *data = Data to send
//   size = Number of bytes to send
//   if SERVER
//     *remoteIP = Destination IP address as null terminated char array
//     port = Destination port number.
// Post: 
//   Returns NET_OK on success. Success does not indicate data was sent.
//   Returns two part int code on error.
//     The low 16 bits contains Status code as defined in net.h.
//     The high 16 bits contains "Windows Socket Error Code".
//   size = Number of bytes sent, 0 if no data sent.
//=============================================================================
int Net::sendData(const char *data, int &size, const char *remoteIP, const USHORT port) 
{
    int status;
    int sendSize = size;

    size = 0;       // assume 0 bytes sent, changed if send successful

    if (mode == SERVER)
    {
        remoteAddr.sin_addr.s_addr = inet_addr(remoteIP);
        remoteAddr.sin_port = port;
    }

    if(mode == CLIENT && type == UNCONNECTED_TCP) 
    {
        ret = connect(sock,(SOCKADDR*)(&remoteAddr),sizeof(remoteAddr));
        if (ret == SOCKET_ERROR) {
            status = WSAGetLastError();
            if (status == WSAEISCONN)   // if connected
            {
                ret = 0;          // clear SOCKET_ERROR
                type = CONNECTED_TCP;
            } 
            else 
            {
                if ( status == WSAEWOULDBLOCK || status == WSAEALREADY) 
                    return NET_OK;  // no connection yet
                else 
                    return ((status << 16) + NET_ERROR);
            }
        }
    }

    ret = sendto(sock, data, sendSize, 0, (SOCKADDR *)&remoteAddr, sizeof(remoteAddr));
    if (ret == SOCKET_ERROR) 
    {
        status = WSAGetLastError();
        return ((status << 16) + NET_ERROR);
    }
    bound = true;         // automatic binding by sendto if unbound
    size = ret;           // number of bytes sent, may be 0
    return NET_OK;
}

//=============================================================================
// Read data, return sender's IP
// Pre:
//   *data = Buffer for received data.
//   size = Number of bytes to receive.
//   *senderIP = NULL
// Post: 
//   Returns NET_OK on success.
//   Returns two part int code on error.
//     The low 16 bits contains Status code as defined in net.h.
//     The high 16 bits contains "Windows Socket Error Code".
//   size = Number of bytes received, may be 0.
//   *senderIP = IP address of sender as null terminated string.
//=============================================================================
int Net::readData(char *data, int &size, char *senderIP) 
{
    int status;
    int readSize = size;

    size = 0;           // assume 0 bytes read, changed if read successful
    if(bound == false)  // no receive from unbound socket
        return NET_OK;

    if(mode == SERVER && type == UNCONNECTED_TCP) 
    {
        ret = listen(sock,1);
        if (ret == SOCKET_ERROR) 
        {
            status = WSAGetLastError();
            return ((status << 16) + NET_ERROR);
        }
        SOCKET tempSock;
        tempSock = accept(sock,NULL,NULL);
        if (tempSock == INVALID_SOCKET) 
        {
            status = WSAGetLastError();
            if ( status != WSAEWOULDBLOCK)  // don't report WOULDBLOCK error
                return ((status << 16) + NET_ERROR);
            return NET_OK;      // no connection yet
        }
        closesocket(sock);      // don't need old socket
        sock = tempSock;        // TCP client connected
        type = CONNECTED_TCP;
    }

    if(mode == CLIENT && type == UNCONNECTED_TCP) 
        return NET_OK;  // no connection yet

    if(sock != NULL)
    {
        remoteAddrSize = sizeof(remoteAddr);
        ret = recvfrom(sock, data, readSize, 0, (SOCKADDR *)&remoteAddr,
                       &remoteAddrSize);
        if (ret == SOCKET_ERROR) {
            status = WSAGetLastError();
            if ( status != WSAEWOULDBLOCK)  // don't report WOULDBLOCK error
                return ((status << 16) + NET_ERROR);
            ret = 0;            // clear SOCKET_ERROR
        // if TCP connection did graceful close
        } else if(ret == 0 && type == CONNECTED_TCP)
            // return Remote Disconnect error
            return ((REMOTE_DISCONNECT << 16) + NET_ERROR);
        if (ret)
            //IP of sender
            strncpy_s(senderIP, IP_SIZE, inet_ntoa(remoteAddr.sin_addr), IP_SIZE);
        size = ret;           // number of bytes read, may be 0
    }
    return NET_OK;
}

//=============================================================================
// Read data, return sender's IP and port
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
//   size = Number of bytes received, may be 0.
//   *senderIP = IP address of sender as null terminated string.
//   port = port number of sender.
//=============================================================================
int Net::readData(char *data, int &size, char *senderIP, USHORT &port) 
{
    int status;
    int readSize = size;

    size = 0;           // assume 0 bytes read, changed if read successful
    if(bound == false)  // no receive from unbound socket
        return NET_OK;

    if(mode == SERVER && type == UNCONNECTED_TCP) 
    {
        ret = listen(sock,1);
        if (ret == SOCKET_ERROR) 
        {
            status = WSAGetLastError();
            return ((status << 16) + NET_ERROR);
        }
        SOCKET tempSock;
        tempSock = accept(sock,NULL,NULL);
        if (tempSock == INVALID_SOCKET) 
        {
            status = WSAGetLastError();
            if ( status != WSAEWOULDBLOCK)  // don't report WOULDBLOCK error
                return ((status << 16) + NET_ERROR);
            return NET_OK;      // no connection yet
        }
        closesocket(sock);      // don't need old socket
        sock = tempSock;        // TCP client connected
        type = CONNECTED_TCP;
    }

    if(mode == CLIENT && type == UNCONNECTED_TCP) 
        return NET_OK;  // no connection yet

    if(sock != NULL)
    {
        remoteAddrSize = sizeof(remoteAddr);
        ret = recvfrom(sock, data, readSize, 0, (SOCKADDR *)&remoteAddr,
                       &remoteAddrSize);
        if (ret == SOCKET_ERROR) {
            status = WSAGetLastError();
            if ( status != WSAEWOULDBLOCK)  // don't report WOULDBLOCK error
                return ((status << 16) + NET_ERROR);
            ret = 0;            // clear SOCKET_ERROR
        // if TCP connection did graceful close
        } else if(ret == 0 && type == CONNECTED_TCP)
            // return Remote Disconnect error
            return ((REMOTE_DISCONNECT << 16) + NET_ERROR);
        if (ret)
        {
            //IP of sender
            strncpy_s(senderIP, IP_SIZE, inet_ntoa(remoteAddr.sin_addr), IP_SIZE);
            port = remoteAddr.sin_port;     //port number of sender
        }
        size = ret;           // number of bytes read, may be 0
    }
    return NET_OK;
}

//=============================================================================
// Close socket and free resources.
// Post:
//   Socket is closed
//   Returns two part int code on error.
//     The low 16 bits contains Status code as defined in net.h.
//     The high 16 bits contains "Windows Socket Error Code".
//=============================================================================
int Net::closeSocket() 
{
    int status;

    type = UNCONNECTED;
    bound = false;
    netInitialized = false;

    // closesocket() implicitly causes a shutdown sequence to occur
    if (closesocket(sock) == SOCKET_ERROR) 
    {
        status = WSAGetLastError();
        if ( status != WSAEWOULDBLOCK)  // don't report WOULDBLOCK error
            return ((status << 16) + NET_ERROR);
    }

    if (WSACleanup())
        return NET_ERROR;
    return NET_OK;
}

//=============================================================================
// Get the IP address of this computer as a string
// Post:
//   *localIP = IP address of local computer as null terminated string on success.
//   Returns two part int code on error.
//     The low 16 bits contains Status code as defined in net.h.
//     The high 16 bits contains "Windows Socket Error Code".
//=============================================================================
int Net::getLocalIP(char *localIP) 
{
    char hostName[40];
    ADDRINFOA host;
    ADDRINFOA *result = NULL;
    int status;

    gethostname (hostName,40);

    // setup host structure for use in getaddrinfo() function
    ZeroMemory(&host, sizeof(host));
    host.ai_family = AF_INET;
    host.ai_socktype = SOCK_STREAM;
    host.ai_protocol = IPPROTO_TCP;

    // get address information
    status = getaddrinfo(hostName,NULL,&host,&result);
    if(status != 0)                 // if getaddrinfo failed
    {
        status = WSAGetLastError();         // get detailed error
        return ( (status << 16) + NET_ERROR);
    }

    // get IP address of server
    IN_ADDR in_addr = ((SOCKADDR_IN *) result->ai_addr)->sin_addr;
    strncpy_s(localIP, IP_SIZE, inet_ntoa(in_addr), IP_SIZE);

    return NET_OK;
}

//=============================================================================
// Returns detailed error message from two part error code
//=============================================================================
std::string Net::getError(int error)
{
    int sockErr = error >> 16;  // upper 16 bits is sockErr
    std::string errorStr;

    error &= STATUS_MASK;       // remove extended error code
    if(error > ERROR_CODES-2)   // if unknown error code
        error = ERROR_CODES-1;
    errorStr = codes[error];
    for (int i=0; i< SOCK_CODES; i++)
    {
        if(errorCodes[i].sockErr == sockErr)
        {
            errorStr += errorCodes[i].message;
            break;
        }
    }
    return errorStr;
}
