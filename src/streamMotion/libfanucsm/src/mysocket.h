//
// Created by sun on 2020/2/27.
//

#pragma once

#ifdef WIN32
#include <winsock2.h>
#else

#include <sys/socket.h>
#include <sys/ioctl.h>


#endif // WIN32

#include <chrono>
#include <string>
#include <vector>

#include "exception.h"


class ComException : public Exception {
public:
    ComException(const ::std::string &what_arg) : Exception(what_arg) {

    }

    ComException(const int &errnum) : Exception(errnum) {

    }

    virtual ~ComException() throw() {

    }

protected:

private:

};


class TimeoutException : public ComException {
public:
    TimeoutException():ComException("Timeout"){

    };

    virtual ~TimeoutException() throw(){

    };

protected:

private:

};

class  Com {
public:
    Com():connected(false){

    };

    virtual ~Com()= default;

    virtual void close() = 0;

    bool isConnected() const{
        return connected;
    };

    virtual void open() = 0;

protected:
    void setConnected(const bool &connected){
        this->connected = connected;
    };

private:
    bool connected;
};


class Socket : public Com{
public:
    class Address {
    public:
        Address();

        Address(const Address &address);

        Address(const ::sockaddr_storage &addr);

        virtual ~Address();

        static Address
        Ipv4(const ::std::string &string, const unsigned short int &port, const bool &asNumeric = false);

        static Address
        Ipv4(const ::std::string &string, const ::std::string &port, const bool &asNumeric = false);

        static Address
        Ipv6(const ::std::string &string, const unsigned short int &port, const bool &asNumeric = false);

        static Address
        Ipv6(const ::std::string &string, const ::std::string &port, const bool &asNumeric = false);

        const ::sockaddr_storage &get() const;

        ::std::vector<unsigned char> getHexadecimal();

        ::std::size_t getLength() const;

        ::std::string getNameInfo(const bool &asNumeric = false) const;

        Socket::Address &operator=(const Socket::Address &other);

        void
        setInfo(const ::std::string &string, const unsigned short int &port, const bool &asNumeric = false);

        void setInfo(const ::std::string &string, const ::std::string &port, const bool &asNumeric = false);

    protected:
        Address(const int &family);

    private:
        ::sockaddr_storage addr;
    };

    enum Option {
        OPTION_KEEPALIVE,
        OPTION_MULTICAST_LOOP,
        OPTION_MULTICAST_TTL,
#if defined(__APPLE__) || defined(__QNX__) || defined(WIN32)
        OPTION_NODELAY
#else // __APPLE__ || __QNX__ || WIN32
        OPTION_NODELAY,
        OPTION_QUICKACK
#endif // __APPLE__ || __QNX__ || WIN32
    };

    Socket(const Socket &socket);

    virtual ~Socket();

    static Socket Tcp(const Address &address);

    static Socket Udp(const Address &address);

    Socket accept();

    void bind();

    void close();

    void connect();

    const Address &getAddress() const;

    int getOption(const Option &option) const;

    const int &getProtocol() const;

    const int &getType() const;

    void listen();

    void listen(const int &backlog);

    void open();

    Socket &operator=(const Socket &other);

    ::std::size_t recv(void *buf, const ::std::size_t &count);

    ::std::size_t recvfrom(void *buf, const ::std::size_t &count, Address &address);

    ::std::size_t select(const bool &read, const bool &write, const ::std::chrono::nanoseconds &timeout);

    ::std::size_t send(const void *buf, const ::std::size_t &count);

    ::std::size_t sendto(const void *buf, const ::std::size_t &count, const Address &address);

    void setAddress(const Address &address);

    void setOption(const Option &option, const int &value);

    void shutdown(const bool &read = true, const bool &write = true);

protected:
    Socket(const int &type, const int &protocol, const Address &address);

#ifdef WIN32
    Socket(const int& type, const int& protocol, const Address& address, const SOCKET& fd);
#else // WIN32

    Socket(const int &type, const int &protocol, const Address &address, const int &fd);

#endif // WIN32

#ifdef WIN32
    SOCKET fd;
#else // WIN32
    int fd;
#endif // WIN32

private:
#ifdef WIN32
    static void cleanup();

        static void startup();
#endif // WIN32

    Address address;

    int protocol;

    int type;
};






