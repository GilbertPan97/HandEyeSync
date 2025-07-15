//
// Created by sun on 2020/2/27.
//

#pragma once

#include <cmath>
#include <functional>
#include <Eigen/Core>
#include <spdlog/spdlog.h>
#include <queue>

#include "mysocket.h"
#include "myendian.h"

#define STATE_MSG_SIZE 132
#define CMD_MSG_SIZE   64
#define START_MSG_SIZE 8
#define STOP_MSG_SIZE 8



namespace FANUC{


    class RawController {
    public:
        const double_t SAMPLE_TIME = 0.008;
        const double_t CUTOFF = 80;
        bool job_finished;
        enum FSMState{
            START,
            STOP,
            IDLE,
            READIO,
            SETIO,
            JOINT,
            CARTESIAN
        };

        enum DataType{
            CART,
            JNT
        };

        enum ControlREQ{
            NOCONTROL,
            JOINTREQ,
            CARTESIANREQ,
            READIOREQ,
            SETIOREQ,
            STOPREQ
        };

#pragma pack(push,1)
        struct World {
            /*X axis coordinate value [mm]*/
            float_t x;
            /*Y axis coordinate value [mm]*/
            float_t y;
            /*Z axis coordinate value [mm]*/
            float_t z;
            /*W axis coordinate value [deg]*/
            float_t w;
            /*P axis coordinate value [deg]*/
            float_t p;
            /*R axis coordinate value [deg]*/
            float_t r;
            /*extend axis 1 coordinate value [mm] if linear, [deg] if rotary*/
            float_t a1;
            /*extend axis 2 coordinate value [mm] if linear, [deg] if rotary*/
            float_t a2;
            /*extend axis 3 coordinate value [mm] if linear, [deg] if rotary*/
            float_t a3;
        };

        struct Joint{
            /*J1 axis angle [deg]*/
            float_t j1;
            /*J2 axis angle [deg]*/
            float_t j2;
            /*J3 axis angle [deg]*/
            float_t j3;
            /*J4 axis angle [deg]*/
            float_t j4;
            /*J5 axis angle [deg]*/
            float_t j5;
            /*J6 axis angle [deg]*/
            float_t j6;
            /*J7 axis angle [deg]*/
            float_t j7;
            /*J8 axis angle [deg]*/
            float_t j8;
            /*J9 axis angle [deg]*/
            float_t j9;
        };
#pragma pack(pop)

        struct ReadIOBuffer{
            uint8_t type;
            uint16_t index;
            uint16_t mask;
            uint16_t value;
            bool     ok;
        };

        struct WriteIOBuffer{
            uint8_t type;
            uint16_t index;
            uint16_t mask;
            uint16_t value;
            bool    ok;
        };

        /* now we suppose there is no need to read during the path */
        struct RawPt{
            union {
                World world;
                Joint joint;
            };
            WriteIOBuffer write_io_buf;
            bool finished;
        };

        enum IOType{
            NORW = 0,
            DI = 1,
            DO = 2,
            RI = 8,
            RO = 9,
            SI = 11,
            SO = 12,
            WI = 16,
            WO = 17,
            UI = 20,
            UO = 21,
            WSI = 26,
            WSO = 27,
            F   = 35,
            M   = 36
        };

        RawController(const std::string& address,
                     const unsigned short int port);

        ~RawController();

        /* run the state machine*/
        void run();

        FSMState fsm_state();

        void joint_req(std::function<RawPt(double_t)> callback);

        void cart_req(std::function<RawPt(double_t)> callback);

        bool read_io_req(IOType type, uint16_t port_num);

        void write_io_req(IOType type, uint16_t port_num, bool value);

        Joint cur_joint();

        World cur_world();

        bool is_ready();

        bool is_motion();

        /* stop the loop*/
        void stop_req();

        void test();
    private:

        void start_();
        /* state machine of idle state */
        void idle_();
        /* state machine of read io state*/
        void read_io_();
        /* state machine of set io state */
        void set_io_();
        /* state machine of cartesian state */
        void cartesian_();
        /* state machine of joint motion state*/
        void joint_();
        /* send the msg to stop the control of the robot*/
        void stop_();

        void open_();
        void close_();

        void recv_state_msg();

        void change_fsm(ControlREQ& req, const std::string& state_str);

        double lowpass_filter(double sample_time, double y, double y_last, double cutoff_frequency);



        template<typename T>
        void serialize(T& t, ::std::uint8_t*& ptr)
        {
            Endian::hostToBig(t);
            ::std::memcpy(ptr, &t, sizeof(t));
            ptr += sizeof(t);
        }

        template<typename T, ::std::size_t N>
        void serialize(T (&t)[N], ::std::uint8_t*& ptr)
        {
            for (::std::size_t i = 0; i < N; ++i)
            {
                this->serialize(t[i], ptr);
            }
        }

        template<typename T>
        void unserialize(::std::uint8_t*& ptr, T& t)
        {
            ::std::memcpy(&t, ptr, sizeof(t));
            Endian::bigToHost(t);
            ptr += sizeof(t);
        }

        template<typename T, ::std::size_t N>
        void unserialize(::std::uint8_t*& ptr, T (&t)[N])
        {
            for (::std::size_t i = 0; i < N; ++i)
            {
                this->unserialize(ptr, t[i]);
            }
        }

    private:
#pragma pack(push, 1)

        struct Current{
            /*J1 current [A]*/
            float_t j1;
            /*J2 current [A]*/
            float_t j2;
            /*J3 current [A]*/
            float_t j3;
            /*J4 current [A]*/
            float_t j4;
            /*J5 current [A]*/
            float_t j5;
            /*J6 current [A]*/
            float_t j6;
            /*J7 current [A]*/
            float_t j7;
            /*J8 current [A]*/
            float_t j8;
            /*J9 current [A]*/
            float_t j9;
        };

        struct Header{
            /* packet type */
            uint32_t packet_type;
            /* version num of stream motion*/
            uint32_t version_num;
        };

        struct StateMsg{
            Header header;
            /* sequence num of the packet */
            uint32_t sequence_num;
            /* status of stream motion and robot */
            uint8_t status;
            /* read io type*/
            uint8_t read_io_type;
            /* read io index*/
            uint16_t read_io_index;
            /* read io mask*/
            uint16_t read_io_mask;
            /* read io value*/
            uint16_t read_io_value;
            /* time stamp*/
            uint32_t time_stamp;
            /* robot current world pos*/
            World world;
            /* robot current joint pos*/
            Joint joint;
            /* robot current current*/
            Current current;
        };

        struct CmdMsg{
            union Position{
                World world;
                Joint joint;
            };
            Header header;
            /* sequence num of the packet */
            uint32_t sequence_num;
            /* last data*/
            uint8_t last_data;
            /* read io type*/
            uint8_t read_io_type;
            /* read io index*/
            uint16_t read_io_index;
            /* read io mask*/
            uint16_t read_io_mask;
            /* data style*/
            uint8_t data_style;
            /* write io type*/
            uint8_t write_io_type;
            /* write io index*/
            uint16_t write_io_index;
            /* write io mask*/
            uint16_t write_io_mask;
            /* write io value*/
            uint16_t write_io_value;
            /* unused*/
            uint16_t unused;
            /* position data*/
            Position position;
        };
#pragma pack(pop)



        void send_idle_cmd_msg(bool last = false);
        void update_(CmdMsg& cmd_msg);
        void unpack_state_msg(uint8_t *ptr_recv);
        void pack_cmd_msg(CmdMsg &cmd_msg, uint8_t *ptr_send);
        uint32_t correct_seq_num_;


        ReadIOBuffer* read_io_buffer_;
        WriteIOBuffer* write_io_buffer_;

        std::queue<ControlREQ> req_buffer_;
        std::queue<std::function<RawPt(double_t)>> callback_buffer_;
        ControlREQ req_;
        StateMsg state_;
        FSMState fsm_state_;
        Socket socket_;
        std::function<RawPt(double_t)> callback_;
        std::mutex mut_;
        bool start_cmd_cycle;
        bool running_;
    };
}


