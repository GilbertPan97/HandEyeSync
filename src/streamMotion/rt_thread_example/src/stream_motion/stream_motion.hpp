#ifndef STREAM_MOTION_HPP
#define STREAM_MOTION_HPP

#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <spdlog/spdlog.h>
#include <sstream>          // for std::ostringstream
#include <iomanip>          // for std::setw, std::setfill
#include <cstdint>          // For fixed-width integer types

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#endif

// Check if it's a Linux system and define #pragma pack(1) if true
#ifdef __linux__
#pragma pack(1)
#endif

typedef int16_t               SHORT;
typedef int32_t               LONG;
typedef uint8_t               UBYTE;
typedef uint16_t              USHORT;
typedef uint32_t              ULONG;

typedef struct INIT_PKT_T
{
    ULONG   pkt_type;
    ULONG   version;
} __attribute__((packed)) INIT_PKT_T;

typedef struct STAT_PKT_T
{
    ULONG   pkt_type;
    ULONG   version;
    ULONG   sequence_no;
    UBYTE   status;
    UBYTE   io_r_type_o;
    USHORT  io_r_idx_o;
    USHORT  io_r_mask_o;
    USHORT  r_io_val;
    ULONG   time_stamp;
    float   cart[9];
    float   ang[9];
    float   q_current[9];
} __attribute__((packed)) STAT_PKT_T;

typedef struct CMD_PKT_T
{
    ULONG   pkt_type;
    ULONG   version;
    ULONG   sequence_no;
    UBYTE   last;
    UBYTE   io_r_type_i;
    USHORT  io_r_idx_i;
    USHORT  io_r_mask_i;
    UBYTE   data_type;
    UBYTE   io_w_type;
    USHORT  io_w_idx;
    USHORT  io_w_mask;
    USHORT  io_w_val;
    USHORT  unused;
    float   cmd[9];
} __attribute__((packed)) CMD_PKT_T;

typedef struct END_PKT_T
{
    ULONG   pkt_type;
    ULONG   version;
} __attribute__((packed)) END_PKT_T;



class StreamMotion {
public:
    StreamMotion();
    ~StreamMotion();

    bool connect(const std::string& ip, int port);

    bool init();

    bool run(CMD_PKT_T cmd_pkt);

    bool end();

    std::tm current_time(bool print = true);

private:
    int rc_sock_;
    struct sockaddr_in rc_addr_;

    void cleanup(); 

    void print_status_info(const STAT_PKT_T& stat_pkt);     // Function to print status information
};

// Restore default structure alignment
#ifdef __linux__
#pragma pack()
#endif

#endif // STREAM_MOTION_HPP
