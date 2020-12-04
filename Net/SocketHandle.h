#pragma once

#if defined(_WIN32)

    #pragma warning (disable: 4996)

    // Force to include needed API prototypes
    #if (_WIN32_WINNT < 0x0601)
        #undef  _WIN32_WINNT
        #define _WIN32_WINNT _WIN32_WINNT_WIN7
    #endif
    // Disable macros min() and max()
    #if !defined(NOMINMAX)
        #define NOMINMAX
    #endif
    // Excludes APIs such as Cryptography, DDE, RPC, Socket
    #if !defined(WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif

    #include <WinSock2.h>
    #include <WS2tcpip.h>

    #undef WIN32_LEAN_AND_MEAN
    #undef NOMINMAX

    #if defined(_MSC_VER)
        #pragma comment(lib, "ws2_32.lib")
    #endif

    #if defined(FD_SETSIZE)
        #undef FD_SETSIZE       // prevent redefinition compiler warning
        #define FD_SETSIZE 1024 // max number of fds in fd_set
    #endif

#else

    #include <errno.h>
    #include <sys/mman.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <sys/ioctl.h>
    #include <sys/types.h>
    #include <netdb.h>
    #include <asm/ioctls.h>
    #include <netinet/in.h>
    #include <netinet/tcp.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>


    using SOCKET = unsigned int;

    #define INVALID_SOCKET  (SOCKET)(~0)
    #define NO_ERROR         0
    #define SOCKET_ERROR    -1

    using BOOL = int;
    using ULONG = unsigned long;
    #define TRUE    1
    #define FALSE   0
#endif

// MSG_NOSIGNAL is not available on some platforms, if it doesn't exist define it as 0
#if !defined(MSG_NOSIGNAL)
    #define MSG_NOSIGNAL 0
#endif

// MSG_DONTWAIT is not available on some platforms, if it doesn't exist define it as 0
#if !defined(MSG_DONTWAIT)
    #define MSG_DONTWAIT 0
#endif

extern int socket_getError();

extern SOCKET socket_create(int addressFamily, int type = SOCK_STREAM, int protocol = IPPROTO_TCP) noexcept;
extern bool socket_close(SOCKET &hSocket) noexcept;

extern bool socket_isValid(SOCKET hSocket) noexcept;
extern bool socket_isSelectable(SOCKET hSocket) noexcept;

extern bool socket_setBlocking(SOCKET hSocket, bool isBlocking) noexcept;
extern bool socket_setOption(SOCKET hSocket, int protocol, int optionname, BOOL value) noexcept;

extern bool socket_bind(SOCKET hSocket, sockaddr const *addr, socklen_t addrLen) noexcept;
extern bool socket_listen(SOCKET hSocket, int backlog) noexcept;
extern SOCKET socket_accept(SOCKET hSocket, sockaddr *addr, socklen_t *addrLen) noexcept;

extern bool socket_connect(SOCKET hSocket, sockaddr const *addr, socklen_t addrLen) noexcept;

extern int socket_send(SOCKET hSocket, char const *buffer, int buffLen, int flags) noexcept;
extern int socket_recv(SOCKET hSocket, char       *buffer, int buffLen, int flags) noexcept;
extern bool socket_shutdown(SOCKET hSocket, int how) noexcept;

extern bool socket_getPeerName(SOCKET hSocket, sockaddr *addr, socklen_t *addrLen) noexcept;

// ------------

#include <iostream>

inline int socket_getError() {
#if defined(_WIN32)
    return WSAGetLastError();
#else
    return errno;
#endif
}

inline SOCKET socket_create(int addressFamily, int type, int protocol) noexcept {

    // Attempt to create socket (addressFamily = domain)
    SOCKET hSocket = ::socket(addressFamily, type, protocol);
    if (!socket_isValid(hSocket)) {
        std::cerr << "::socket_create() : " << socket_getError() << '\n';
    }
    return hSocket;
}
inline bool socket_close(SOCKET &hSocket) noexcept {

    if (!socket_isValid(hSocket)) {
        return false;
    }

    int ret;
#if defined(_WIN32)
    ret = ::closesocket(hSocket);
#else
    ret = ::close(hSocket);
#endif
    if (ret != NO_ERROR) {
        std::cerr << "::socket_close() : " << socket_getError() << '\n';
    }
    hSocket = INVALID_SOCKET;
    return ret == NO_ERROR;
}

inline bool socket_isValid(SOCKET hSocket) noexcept {
    return hSocket != INVALID_SOCKET;
}

inline bool socket_isSelectable(SOCKET hSocket) noexcept {
#if defined(USE_POLL) || defined(WIN32)
    return true;
#else
    return (hSocket < FD_SETSIZE);
#endif
}

inline bool socket_setBlocking(SOCKET hSocket, bool isBlocking) noexcept {

    int ret;
#if defined(_WIN32)

    u_long blocking = isBlocking ? 0 : 1;
    ret = ::ioctlsocket(hSocket, FIONBIO, &blocking);

#else

    ret = ::fcntl(hSocket, F_SETFL,
            isBlocking ?
                (::fcntl(hSocket, F_GETFL) & ~O_NONBLOCK) :
                (::fcntl(hSocket, F_GETFL) |  O_NONBLOCK));

#endif

    if (ret != NO_ERROR) {
        std::cerr << "::socket_setBlocking() : " << socket_getError() << '\n';
    }
    return ret == NO_ERROR;
}

// Example:
// setOption(IPPROTO_TCP, TCP_NODELAY, TRUE);
// setOption(IPPROTO_IPV6, IPV6_V6ONLY, FALSE);
inline bool socket_setOption(SOCKET hSocket, int protocol, int optionName, BOOL optionValue) noexcept {

    int ret = ::setsockopt(hSocket, protocol, optionName, (const char*)&optionValue, sizeof(optionValue));
    if (ret != NO_ERROR) {
        std::cerr << "::socket_setOption() : " << socket_getError() << '\n';
    }
    return ret == NO_ERROR;
}

inline bool socket_bind(SOCKET hSocket, sockaddr const *addr, socklen_t addrLen) noexcept {

    int ret = ::bind(hSocket, addr, addrLen);
    if (ret != NO_ERROR) {
        std::cerr << "::socket_bind() : " << socket_getError() << '\n';
    }
    return ret == NO_ERROR;
}

inline bool socket_listen(SOCKET hSocket, int backlog) noexcept {

    int ret = ::listen(hSocket, backlog);
    if (ret != NO_ERROR) {
        std::cerr << "::socket_listen() : " << socket_getError() << '\n';
    }
    return ret == NO_ERROR;
}

inline SOCKET socket_accept(SOCKET hSocket, sockaddr *addr, socklen_t *addrLen) noexcept {

    SOCKET acceptedSocket = ::accept(hSocket, addr, addrLen);
    if (!socket_isValid(acceptedSocket)) {
        std::cerr << "::socket_accept() : " << socket_getError() << '\n';
    }
    return acceptedSocket;
}

inline bool socket_connect(SOCKET hSocket, sockaddr const *addr, socklen_t addrLen) noexcept {

    int ret = ::connect(hSocket, addr, addrLen);
    if (ret != NO_ERROR) {
        std::cerr << "::socket_connect() : " << socket_getError() << '\n';
    }
    return ret == NO_ERROR;
}

inline int socket_send(SOCKET hSocket, char const *buffer, int buffLen, int flags) noexcept {

    int len = ::send(hSocket, buffer, buffLen, flags);
    if (len == SOCKET_ERROR) {
        std::cerr << "::socket_send() : " << socket_getError() << '\n';
    }
    return len;
}

inline int socket_recv(SOCKET hSocket, char       *buffer, int buffLen, int flags) noexcept {

    int len = ::recv(hSocket, buffer, buffLen, flags);
    if (len == SOCKET_ERROR) {
        std::cerr << "::socket_recv() : " << socket_getError() << '\n';
    }
    return len;
}

inline bool socket_shutdown(SOCKET hSocket, int how) noexcept {

    int ret = ::shutdown(hSocket, how);
    if (ret != NO_ERROR) {
        std::cerr << "::socket_shutdown() : " << socket_getError() << '\n';
    }
    return ret == NO_ERROR;
}

inline bool socket_getPeerName(SOCKET hSocket, sockaddr *addr, socklen_t *addrLen) noexcept {

    int ret = ::getpeername(hSocket, addr, addrLen);
    if (ret != NO_ERROR) {
        std::cerr << "::socket_getPeerName() : " << socket_getError() << '\n';
    }
    return ret == NO_ERROR;
}

