//
// Created by sun on 2020/2/27.
//


#include <iostream>
#include <chrono>
#include <thread>
#include <utility>

#include "raw_controller.h"

FANUC::RawController::RawController(const std::string &address, const unsigned short int port) :
        job_finished(false),
        correct_seq_num_(0),
        read_io_buffer_(nullptr),
        write_io_buffer_(nullptr),
        req_buffer_(),
        req_(ControlREQ::NOCONTROL),
        state_(),
        fsm_state_(FSMState::START),
        callback_(),
        socket_(Socket::Udp(Socket::Address::Ipv4(address, port))),
        start_cmd_cycle(true),
        running_(true) {

}

void showq(std::queue <FANUC::RawController::ControlREQ>& gq)
{
    std::queue <FANUC::RawController::ControlREQ> g = gq;
    while (!g.empty())
    {
        std::cout << '\t' << g.front();
        g.pop();
    }
    std::cout << '\n';
}

void FANUC::RawController::run() {
    while (running_) {
        std::cout << "running : " << running_ << std::endl;
        showq(req_buffer_);
        if(req_buffer_.empty()){
            req_ = ControlREQ::NOCONTROL;
        }else{
            req_ = req_buffer_.front();
            req_buffer_.pop();
            if((req_ == ControlREQ::CARTESIANREQ) or (req_ == ControlREQ::JOINTREQ)){
                callback_ = callback_buffer_.front();
                callback_buffer_.pop();
            }
        }

        switch (fsm_state_) {
            case FSMState::START:
                this->open_();
                this->start_();
                fsm_state_ = FSMState::IDLE;
                spdlog::info("State: start -> idle");
                if (req_ == ControlREQ::STOPREQ) {
                    req_ = ControlREQ::NOCONTROL;
                    fsm_state_ = FSMState::STOP;
                }
                break;
            case FSMState::IDLE:
                this->idle_();
                if (req_ == ControlREQ::READIOREQ) {
                    spdlog::info("State: idle -> read io");
                    req_ = ControlREQ::NOCONTROL;
                    fsm_state_ = FSMState::READIO;
                } else if (req_ == ControlREQ::SETIOREQ) {
                    spdlog::info("State: idle -> set io");
                    req_ = ControlREQ::NOCONTROL;
                    fsm_state_ = FSMState::SETIO;
                } else if (req_ == ControlREQ::CARTESIANREQ) {
                    spdlog::info("State: idle -> cartesian");
                    req_ = ControlREQ::NOCONTROL;
                    fsm_state_ = FSMState::CARTESIAN;
                } else if (req_ == ControlREQ::JOINTREQ) {
                    spdlog::info("State: idle -> joint");
                    req_ = ControlREQ::NOCONTROL;
                    fsm_state_ = FSMState::JOINT;
                } else if (req_ == ControlREQ::STOPREQ) {
                    spdlog::info("State: idle -> stop_req");
                    req_ = ControlREQ::NOCONTROL;
                    fsm_state_ = FSMState::STOP;
                }
                break;
            case FSMState::READIO:
                this->read_io_();

                change_fsm(req_, std::string("read io"));
                break;
            case FSMState::SETIO:
                this->set_io_();

                change_fsm(req_, std::string("set io"));
                break;
            case FSMState::CARTESIAN:
                this->cartesian_();

                change_fsm(req_, std::string("cartesian"));
                break;
            case FSMState::JOINT:
                this->joint_();

                change_fsm(req_, std::string("joint"));
                break;
            case FSMState::STOP:
                this->stop_();
                running_ = false;
                break;
            default:
                break;
        }
    }
}

void FANUC::RawController::stop_req() {
    req_buffer_.push(ControlREQ::STOPREQ);
}

void FANUC::RawController::open_() {
    socket_.open();
    socket_.connect();
}

void FANUC::RawController::start_() {
    Header header;
    header.packet_type = 0;
    header.version_num = 1;

    std::array<uint8_t, START_MSG_SIZE> buffer{};
    uint8_t *ptr = &buffer[0];

    serialize(header.packet_type, ptr);
    serialize(header.version_num, ptr);
    socket_.send(buffer.data(), buffer.size());

    /*wait until the robot get ready */
    bool started = false;
    while (!started) {
        /* loop to clear the buffer */
        while (socket_.select(true, false, std::chrono::nanoseconds(100000)) > 0) {
            std::array<uint8_t, STATE_MSG_SIZE> buffer_recv{};
            uint8_t *ptr_recv = &buffer_recv[0];

            socket_.recv(buffer_recv.data(), STATE_MSG_SIZE);
            unpack_state_msg(ptr_recv);
            spdlog::debug("seq num {0:d}, time {1:d}, status {2:b}", state_.sequence_num, state_.time_stamp,
                          state_.status);
        }

        if ((state_.status & 0x0001) == 1 and ((state_.status >> 2) & 0x0001) == 1) {
            spdlog::info("Controller started, status: {0:b}", state_.status);
            correct_seq_num_ = state_.sequence_num + 1;
            started = true;
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            spdlog::info("Please make sure the robot ready for stream motion ...");
        }
    }
    job_finished = true;
}

void FANUC::RawController::unpack_state_msg(uint8_t *ptr_recv) {
    this->unserialize(ptr_recv, this->state_.header.packet_type);
    this->unserialize(ptr_recv, this->state_.header.version_num);

    this->unserialize(ptr_recv, this->state_.sequence_num);
    this->unserialize(ptr_recv, this->state_.status);
    this->unserialize(ptr_recv, this->state_.read_io_type);
    this->unserialize(ptr_recv, this->state_.read_io_index);
    this->unserialize(ptr_recv, this->state_.read_io_mask);
    this->unserialize(ptr_recv, this->state_.read_io_value);
    this->unserialize(ptr_recv, this->state_.time_stamp);
    this->unserialize(ptr_recv, this->state_.world.x);
    this->unserialize(ptr_recv, this->state_.world.y);
    this->unserialize(ptr_recv, this->state_.world.z);
    this->unserialize(ptr_recv, this->state_.world.w);
    this->unserialize(ptr_recv, this->state_.world.p);
    this->unserialize(ptr_recv, this->state_.world.r);
    this->unserialize(ptr_recv, this->state_.world.a1);
    this->unserialize(ptr_recv, this->state_.world.a2);
    this->unserialize(ptr_recv, this->state_.world.a3);
    this->unserialize(ptr_recv, this->state_.joint.j1);
    this->unserialize(ptr_recv, this->state_.joint.j2);
    this->unserialize(ptr_recv, this->state_.joint.j3);
    this->unserialize(ptr_recv, this->state_.joint.j4);
    this->unserialize(ptr_recv, this->state_.joint.j5);
    this->unserialize(ptr_recv, this->state_.joint.j6);
    this->unserialize(ptr_recv, this->state_.joint.j7);
    this->unserialize(ptr_recv, this->state_.joint.j8);
    this->unserialize(ptr_recv, this->state_.current.j1);
    this->unserialize(ptr_recv, this->state_.current.j2);
    this->unserialize(ptr_recv, this->state_.current.j3);
    this->unserialize(ptr_recv, this->state_.current.j4);
    this->unserialize(ptr_recv, this->state_.current.j5);
    this->unserialize(ptr_recv, this->state_.current.j6);
    this->unserialize(ptr_recv, this->state_.current.j7);
    this->unserialize(ptr_recv, this->state_.current.j8);
}

void FANUC::RawController::test() {
    open_();
    run();
//    start_();
//    while (running_){
//        idle_();
//    }
#if 0
    read_io_buffer_ = new ReadIOBuffer{};
    read_io_buffer_->type = IOType::F;
    read_io_buffer_->index = 0x0003;
    read_io_buffer_->mask = 0x0001;

    read_io_();
#endif
#if 0
    write_io_buffer_ = new WriteIOBuffer{};
    write_io_buffer_->type = IOType::F;
    write_io_buffer_->index = 0x0005;
    write_io_buffer_->mask = 0x0001;
    write_io_buffer_->value = 1;
    set_io_();
    stop_();
//    while (!read_io_buffer_->ok) {
//
//    }
//    spdlog::debug("type: {0:d}, index: {1:d}, mask: {2:d}, value: {3:b}", read_io_buffer_->type, read_io_buffer_->index,
//                  read_io_buffer_->mask, read_io_buffer_->value);
    CmdMsg cmd_msg{};
    StateMsg state_msg{};
    std::cout << "state size : " << sizeof(state_msg) << std::endl;
    std::cout << "cmd   size : " << sizeof(cmd_msg) << std::endl;
#endif
//    std::function<RawPt(double_t)> callback = [&](double_t time) {
//        RawPt pt{};
//        pt.joint = state_.joint;
//        return pt;
//    };
//    callback_ = callback;
//    joint_();
}

void FANUC::RawController::idle_() {
    /* check if there is packet in 4ms*/
    spdlog::debug("idel");
    recv_state_msg();


    spdlog::debug("Send cmd msg {0:d}", state_.sequence_num);

    send_idle_cmd_msg();
    spdlog::debug("idel out");


}

void FANUC::RawController::pack_cmd_msg(FANUC::RawController::CmdMsg &cmd_msg, uint8_t *ptr_send) {
    this->serialize(cmd_msg.header.packet_type, ptr_send);
    this->serialize(cmd_msg.header.version_num, ptr_send);
    this->serialize(cmd_msg.sequence_num, ptr_send);
    this->serialize(cmd_msg.last_data, ptr_send);
    this->serialize(cmd_msg.read_io_type, ptr_send);
    this->serialize(cmd_msg.read_io_index, ptr_send);
    this->serialize(cmd_msg.read_io_mask, ptr_send);
    this->serialize(cmd_msg.data_style, ptr_send);
    this->serialize(cmd_msg.write_io_type, ptr_send);
    this->serialize(cmd_msg.write_io_index, ptr_send);
    this->serialize(cmd_msg.write_io_mask, ptr_send);
    this->serialize(cmd_msg.write_io_value, ptr_send);
    this->serialize(cmd_msg.unused, ptr_send);
    this->serialize(cmd_msg.position.joint.j1, ptr_send);
    this->serialize(cmd_msg.position.joint.j2, ptr_send);
    this->serialize(cmd_msg.position.joint.j3, ptr_send);
    this->serialize(cmd_msg.position.joint.j4, ptr_send);
    this->serialize(cmd_msg.position.joint.j5, ptr_send);
    this->serialize(cmd_msg.position.joint.j6, ptr_send);
    this->serialize(cmd_msg.position.joint.j7, ptr_send);
    this->serialize(cmd_msg.position.joint.j8, ptr_send);
    this->serialize(cmd_msg.position.joint.j9, ptr_send);
}

void FANUC::RawController::read_io_() {
    assert(read_io_buffer_ != nullptr);
    /* check if there is packet in 4ms*/


    bool repeat = true;
    int cnt = 0; // only send the query msg 10 times
    while (running_ and cnt < 10) {

        recv_state_msg();

        if (state_.read_io_type == read_io_buffer_->type
            and state_.read_io_index == read_io_buffer_->index
            and state_.read_io_mask == read_io_buffer_->mask) {
            spdlog::info("[read io] Read IO success");
            read_io_buffer_->value = state_.read_io_value;
            read_io_buffer_->ok = true;

            send_idle_cmd_msg();

            break;
        } else {
            spdlog::debug("[read io] type: {0:d}, index: {1:d}, mask: {2:d}, value: {3:b}", state_.read_io_type,
                          state_.read_io_index, state_.read_io_mask, state_.read_io_value);
            spdlog::debug("[read io] Send io msg {0:d}", state_.sequence_num);

            CmdMsg cmd_msg{};
            cmd_msg.header.packet_type = 1;
            cmd_msg.header.version_num = 1;
            cmd_msg.sequence_num = state_.sequence_num;
            cmd_msg.last_data = 0;
            cmd_msg.read_io_type = read_io_buffer_->type;
            cmd_msg.read_io_index = read_io_buffer_->index;
            cmd_msg.read_io_mask = read_io_buffer_->mask;
            cmd_msg.data_style = DataType::JNT; // Default is joint
            cmd_msg.write_io_type = IOType::NORW;
            cmd_msg.write_io_index = 0;
            cmd_msg.write_io_mask = 0;
            cmd_msg.write_io_value = 0;
            cmd_msg.unused = 0;
            cmd_msg.position.joint = state_.joint;

            std::array<uint8_t, CMD_MSG_SIZE> buffer_send{};
            uint8_t *ptr_send = &buffer_send[0];

            pack_cmd_msg(cmd_msg, ptr_send);

            socket_.send(buffer_send.data(), CMD_MSG_SIZE);
        }
        cnt++;
    }
    if(cnt == 10){
        spdlog::info("[read io] time out ");
        read_io_buffer_->value = 0;
        read_io_buffer_->ok    = true;
    }

    job_finished = true;
}

void FANUC::RawController::recv_state_msg() {
    std::array<uint8_t, STATE_MSG_SIZE> buffer_recv{};
    uint8_t *ptr_recv = &buffer_recv[0];

    socket_.recv(buffer_recv.data(), STATE_MSG_SIZE);
    std::unique_lock<std::mutex> unique_locker(mut_);
    unpack_state_msg(ptr_recv);
    mut_.unlock();

    if (start_cmd_cycle) {
        start_cmd_cycle = false;
    } else {
        if (state_.sequence_num != correct_seq_num_) {
            spdlog::info("Wrong seq number, exiting ");
            running_ = false;
            goto end;
        }
        if (((state_.status >> 1) & (0x0001)) == 0) {
            spdlog::info("Robot: No cmd msg received");
            running_ = false;
            goto end;
        }
        if (((state_.status & 0x0001) == 0) or (((state_.status >> 2) & 0x0001) == 0)) {
            spdlog::info("Controller stopped, status: {0:b}", state_.status);
            spdlog::info("exiting ...");
            running_ = false;
            goto end;
        }
    }
    end:
    correct_seq_num_++;
    spdlog::debug("Receive state msg {0:d}, with status {1:b}", state_.sequence_num, state_.status);
}

void FANUC::RawController::update_(CmdMsg &cmd_msg) {
    /* check if there is packet in 4ms*/
    if (socket_.select(true, false, std::chrono::nanoseconds(4000000)) > 0) {

        recv_state_msg();

        if (socket_.select(false, true, std::chrono::nanoseconds(1000000)) > 0) {

            cmd_msg.sequence_num = state_.sequence_num;
            spdlog::debug("Send cmd msg {0:d}", cmd_msg.sequence_num);

            std::array<uint8_t, CMD_MSG_SIZE> buffer_send{};
            uint8_t *ptr_send = &buffer_send[0];

            pack_cmd_msg(cmd_msg, ptr_send);

            socket_.send(buffer_send.data(), CMD_MSG_SIZE);
        }
    }
}

void FANUC::RawController::stop_() {
    {
        recv_state_msg();
        spdlog::debug("Send stop_req control msg {0:d}", state_.sequence_num);
        send_idle_cmd_msg(true);

    }

    {
//        recv_state_msg();
        Header header{};
        header.packet_type = 2;
        header.version_num = 1;

        std::array<uint8_t, STOP_MSG_SIZE> buffer{};
        uint8_t *ptr = &buffer[0];
        serialize(header.packet_type, ptr);
        serialize(header.version_num, ptr);
        socket_.send(buffer.data(), buffer.size());
    }
    job_finished = true;
}

void FANUC::RawController::send_idle_cmd_msg(bool last) {
    CmdMsg cmd_msg{};
    cmd_msg.header.packet_type = 1;
    cmd_msg.header.version_num = 1;
    cmd_msg.sequence_num = state_.sequence_num;
    cmd_msg.last_data = last ? 1 : 0;
    cmd_msg.read_io_type = IOType::NORW;
    cmd_msg.read_io_index = 0;
    cmd_msg.read_io_mask = 0;
    cmd_msg.data_style = DataType::JNT; // Default is joint
    cmd_msg.write_io_type = IOType::NORW;
    cmd_msg.write_io_index = 0;
    cmd_msg.write_io_mask = 0;
    cmd_msg.write_io_value = 0;
    cmd_msg.unused = 0;
    cmd_msg.position.joint = state_.joint;


    std::array<uint8_t, CMD_MSG_SIZE> buffer_send{};
    uint8_t *ptr_send = &buffer_send[0];

    pack_cmd_msg(cmd_msg, ptr_send);

    socket_.send(buffer_send.data(), CMD_MSG_SIZE);
}

void FANUC::RawController::set_io_() {
    assert(write_io_buffer_ != nullptr);

    recv_state_msg();

    CmdMsg cmd_msg{};
    cmd_msg.header.packet_type = 1;
    cmd_msg.header.version_num = 1;
    cmd_msg.sequence_num = state_.sequence_num;
    cmd_msg.last_data = 0;
    cmd_msg.read_io_type = IOType::NORW;
    cmd_msg.read_io_index = 0;
    cmd_msg.read_io_mask = 0;
    cmd_msg.data_style = DataType::JNT; // Default is joint
    cmd_msg.write_io_type = write_io_buffer_->type;
    cmd_msg.write_io_index = write_io_buffer_->index;
    cmd_msg.write_io_mask = write_io_buffer_->mask;
    cmd_msg.write_io_value = write_io_buffer_->value;
    cmd_msg.unused = 0;
    cmd_msg.position.joint = state_.joint;

    std::array<uint8_t, CMD_MSG_SIZE> buffer_send{};
    uint8_t *ptr_send = &buffer_send[0];

    pack_cmd_msg(cmd_msg, ptr_send);

    socket_.send(buffer_send.data(), CMD_MSG_SIZE);
    job_finished = true;
    delete write_io_buffer_;
    write_io_buffer_ = nullptr;
}

void FANUC::RawController::cartesian_() {
    assert(callback_ != nullptr);
    double_t time_start = state_.time_stamp;
    double_t time = 0.0;
    bool finished = false;
    World last_value = state_.world;
    auto send_cmd_msg = [&](){
        recv_state_msg();

        auto data = callback_(time);
        spdlog::debug("interpolator time: {0:f}", time);
        time = (state_.time_stamp - time_start) / 1000.0;
        World cartensian_pt = data.world;
        WriteIOBuffer &write_io_buf = data.write_io_buf;
        finished = data.finished;
        cartensian_pt.x = lowpass_filter(SAMPLE_TIME,cartensian_pt.x,last_value.x,CUTOFF);
        cartensian_pt.y = lowpass_filter(SAMPLE_TIME,cartensian_pt.y,last_value.y,CUTOFF);
        cartensian_pt.z = lowpass_filter(SAMPLE_TIME,cartensian_pt.z,last_value.z,CUTOFF);
        cartensian_pt.w = lowpass_filter(SAMPLE_TIME,cartensian_pt.w,last_value.w,CUTOFF);
        cartensian_pt.p = lowpass_filter(SAMPLE_TIME,cartensian_pt.p,last_value.p,CUTOFF);
        cartensian_pt.r = lowpass_filter(SAMPLE_TIME,cartensian_pt.r,last_value.r,CUTOFF);
        cartensian_pt.a1 = lowpass_filter(SAMPLE_TIME,cartensian_pt.a1,last_value.a1,CUTOFF);
        cartensian_pt.a2 = lowpass_filter(SAMPLE_TIME,cartensian_pt.a2,last_value.a2,CUTOFF);
        cartensian_pt.a3 = lowpass_filter(SAMPLE_TIME,cartensian_pt.a3,last_value.a3,CUTOFF);

        CmdMsg cmd_msg{};
        cmd_msg.header.packet_type = 1;
        cmd_msg.header.version_num = 1;
        cmd_msg.sequence_num = state_.sequence_num;
        cmd_msg.last_data = 0;
        cmd_msg.read_io_type = IOType::NORW;
        cmd_msg.read_io_index = 0;
        cmd_msg.read_io_mask = 0;
        cmd_msg.data_style = DataType::CART;
        cmd_msg.write_io_type = write_io_buf.type;
        cmd_msg.write_io_index = write_io_buf.index;
        cmd_msg.write_io_mask = write_io_buf.mask;
        cmd_msg.write_io_value = write_io_buf.value;
        cmd_msg.unused = 0;
        cmd_msg.position.world.x = cartensian_pt.x == 0 ? state_.world.x : cartensian_pt.x;
        cmd_msg.position.world.y = cartensian_pt.y == 0 ? state_.world.y : cartensian_pt.y;
        cmd_msg.position.world.z = cartensian_pt.z == 0 ? state_.world.z : cartensian_pt.z;
        cmd_msg.position.world.w = cartensian_pt.w == 0 ? state_.world.w : cartensian_pt.w;
        cmd_msg.position.world.p = cartensian_pt.p == 0 ? state_.world.p : cartensian_pt.p;
        cmd_msg.position.world.r = cartensian_pt.r == 0 ? state_.world.r : cartensian_pt.r;
        cmd_msg.position.world.a1 = cartensian_pt.a1 == 0 ? state_.world.a1 : cartensian_pt.a1;
        cmd_msg.position.world.a2 = cartensian_pt.a2 == 0 ? state_.world.a2 : cartensian_pt.a2;
        cmd_msg.position.world.a3 = cartensian_pt.a3 == 0 ? state_.world.a3 : cartensian_pt.a3;


        std::array<uint8_t, CMD_MSG_SIZE> buffer_send{};
        uint8_t *ptr_send = &buffer_send[0];

        pack_cmd_msg(cmd_msg, ptr_send);

        socket_.send(buffer_send.data(), CMD_MSG_SIZE);
    };
    do {

        send_cmd_msg();
    } while (not finished and running_);

    while(is_motion()){

        send_cmd_msg();
    }
    callback_ = nullptr;
    job_finished = true;
}

void FANUC::RawController::joint_() {
    assert(callback_ != nullptr);
    double_t time_start = state_.time_stamp;
    double_t time = 0.0;
    bool finished = false;
    Joint last_value = state_.joint;

    auto send_cmd_msg = [&](){
        recv_state_msg();

        auto data = callback_(time);
        spdlog::debug("interpolator time: {0:f}", time);
        time = (state_.time_stamp - time_start) / 1000.0;
        Joint &joint_pt = data.joint;
        WriteIOBuffer &write_io_buf = data.write_io_buf;
        finished = data.finished;

        joint_pt.j1 = lowpass_filter(SAMPLE_TIME,joint_pt.j1,last_value.j1,CUTOFF);
        joint_pt.j2 = lowpass_filter(SAMPLE_TIME,joint_pt.j2,last_value.j2,CUTOFF);
        joint_pt.j3 = lowpass_filter(SAMPLE_TIME,joint_pt.j3,last_value.j3,CUTOFF);
        joint_pt.j4 = lowpass_filter(SAMPLE_TIME,joint_pt.j4,last_value.j4,CUTOFF);
        joint_pt.j5 = lowpass_filter(SAMPLE_TIME,joint_pt.j5,last_value.j5,CUTOFF);
        joint_pt.j6 = lowpass_filter(SAMPLE_TIME,joint_pt.j6,last_value.j6,CUTOFF);
        joint_pt.j7 = lowpass_filter(SAMPLE_TIME,joint_pt.j7,last_value.j7,CUTOFF);
        joint_pt.j8 = lowpass_filter(SAMPLE_TIME,joint_pt.j8,last_value.j8,CUTOFF);
        joint_pt.j9 = lowpass_filter(SAMPLE_TIME,joint_pt.j9,last_value.j9,CUTOFF);
        /* update last value */
        last_value = state_.joint;

        CmdMsg cmd_msg{};
        cmd_msg.header.packet_type = 1;
        cmd_msg.header.version_num = 1;
        cmd_msg.sequence_num = state_.sequence_num;
        cmd_msg.last_data = 0;
        cmd_msg.read_io_type = IOType::NORW;
        cmd_msg.read_io_index = 0;
        cmd_msg.read_io_mask = 0;
        cmd_msg.data_style = DataType::JNT;
        cmd_msg.write_io_type = write_io_buf.type;
        cmd_msg.write_io_index = write_io_buf.index;
        cmd_msg.write_io_mask = write_io_buf.mask;
        cmd_msg.write_io_value = write_io_buf.value;
        cmd_msg.unused = 0;
        cmd_msg.position.joint.j1 = joint_pt.j1 == 0 ? state_.joint.j1: joint_pt.j1;
        cmd_msg.position.joint.j2 = joint_pt.j2 == 0 ? state_.joint.j2: joint_pt.j2;
        cmd_msg.position.joint.j3 = joint_pt.j3 == 0 ? state_.joint.j3: joint_pt.j3;
        cmd_msg.position.joint.j4 = joint_pt.j4 == 0 ? state_.joint.j4: joint_pt.j4;
        cmd_msg.position.joint.j5 = joint_pt.j5 == 0 ? state_.joint.j5: joint_pt.j5;
        cmd_msg.position.joint.j6 = joint_pt.j6 == 0 ? state_.joint.j6: joint_pt.j6;
        cmd_msg.position.joint.j7 = joint_pt.j7 == 0 ? state_.joint.j7: joint_pt.j7;
        cmd_msg.position.joint.j8 = joint_pt.j8 == 0 ? state_.joint.j8: joint_pt.j8;
        cmd_msg.position.joint.j9 = joint_pt.j9 == 0 ? state_.joint.j9: joint_pt.j9;


        std::array<uint8_t, CMD_MSG_SIZE> buffer_send{};
        uint8_t *ptr_send = &buffer_send[0];

        pack_cmd_msg(cmd_msg, ptr_send);

        socket_.send(buffer_send.data(), CMD_MSG_SIZE);
    };

    do {

        send_cmd_msg();
    } while (not finished and running_);

    while (is_motion()){
        send_cmd_msg();
    }

    callback_ = nullptr;
    job_finished = true;
}

void FANUC::RawController::close_() {
    socket_.close();
}

void FANUC::RawController::change_fsm( ControlREQ& req, const std::string& state_str) {

    if (req == ControlREQ::READIOREQ) {
        spdlog::info("State: {0} -> read io", state_str);
        req = ControlREQ::NOCONTROL;
        fsm_state_ = FSMState::READIO;
    } else if (req == ControlREQ::SETIOREQ) {
        spdlog::info("State: {0} -> set io", state_str);
        req = ControlREQ::NOCONTROL;
        fsm_state_ = FSMState::SETIO;
    } else if (req == ControlREQ::CARTESIANREQ) {
        spdlog::info("State: {0} -> cartesian", state_str);
        req = ControlREQ::NOCONTROL;
        fsm_state_ = FSMState::CARTESIAN;
    } else if (req == ControlREQ::JOINTREQ) {
        spdlog::info("State: {0} -> joint", state_str);
        req = ControlREQ::NOCONTROL;
        fsm_state_ = FSMState::JOINT;
    } else if (req == ControlREQ::STOPREQ) {
        spdlog::info("State: {0} -> stop_req", state_str);
        req = ControlREQ::NOCONTROL;
        fsm_state_ = FSMState::STOP;
    } else {
        fsm_state_ = FSMState::IDLE;
        spdlog::info("State: {0} -> idle", state_str);
    }

}

FANUC::RawController::~RawController() {
    close_();
}

void FANUC::RawController::joint_req(std::function<RawPt(double_t)> callback) {
    callback_buffer_.push(std::move(callback));
    /* tell the state machine to change the state */
    req_buffer_.push( ControlREQ::JOINTREQ);
}

void FANUC::RawController::cart_req(std::function<RawPt(double_t)> callback) {
    callback_buffer_.push(std::move(callback));
    req_buffer_.push(ControlREQ::CARTESIANREQ);
}

bool FANUC::RawController::read_io_req(FANUC::RawController::IOType type, uint16_t port_num) {
    bool ret = false;
    read_io_buffer_ = new ReadIOBuffer{};
    read_io_buffer_->type = type;
    read_io_buffer_->index = port_num;
    read_io_buffer_->mask = 0x0001;
    req_buffer_.push(ControlREQ::READIOREQ);

    while(!read_io_buffer_->ok){
        // wait for return
    }

    if(read_io_buffer_->ok){
        ret =  read_io_buffer_->value == 1;
    }
    delete read_io_buffer_;
    read_io_buffer_ = nullptr;
    return ret;
}

void FANUC::RawController::write_io_req(FANUC::RawController::IOType type, uint16_t port_num, bool value) {
    write_io_buffer_ = new WriteIOBuffer{};
    write_io_buffer_->type = type;
    write_io_buffer_->index = port_num;
    write_io_buffer_->mask  = 0x0001;
    write_io_buffer_->value = value ? 0x0001 : 0x0000;

    req_buffer_.push(ControlREQ::SETIOREQ);
}

FANUC::RawController::Joint FANUC::RawController::cur_joint() {
    std::unique_lock<std::mutex> unique_locker(mut_);
    return state_.joint;
}

FANUC::RawController::World FANUC::RawController::cur_world() {
    std::unique_lock<std::mutex> unique_locker(mut_);
    return state_.world;
}

FANUC::RawController::FSMState FANUC::RawController::fsm_state() {
    return fsm_state_;
}

bool FANUC::RawController::is_ready() {
    std::unique_lock<std::mutex> unique_locker(mut_);
    return ((state_.status & 0x0001) == 1) and (((state_.status >> 2) & 0x0001) == 1);
}

bool FANUC::RawController::is_motion() {
    std::unique_lock<std::mutex> unique_locker(mut_);
    return (state_.status >> 3) & 0x0001 == 1;
}

double FANUC::RawController::lowpass_filter(double sample_time, double y, double y_last, double cutoff_frequency) {
    double gain = sample_time / (sample_time + (1.0 / (2.0 * M_PI * cutoff_frequency)));
    return gain * y + (1 - gain) * y_last;
}




