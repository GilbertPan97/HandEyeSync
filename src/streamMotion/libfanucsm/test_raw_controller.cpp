//
// Created by sun on 2020/2/27.
//

#include <thread>
#include <chrono>
#include <iostream>

#include "raw_controller.h"


int main(){
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S] [thread %t] %v");
    const char *string = "192.168.1.171";
//    const char *string = "";
    FANUC::RawController controller(string, 60015);
    controller.test();
}