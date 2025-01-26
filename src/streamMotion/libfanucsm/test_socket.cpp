//
// Created by sun on 2020/2/27.
//

#include <iostream>
#include "mysocket.h"

int main(){
    Socket::Address address = Socket::Address::Ipv4("",10000);
    Socket socket = Socket::Udp(address);
    socket.open();
    socket.connect();
    std::string send_msg("hello world");
    std::size_t size = socket.send(send_msg.c_str(),send_msg.size());
    std::cout << "send msg: " << send_msg << " size: " << size << std::endl;
    std::cout << "str  size: " << send_msg.size() << std::endl;
}