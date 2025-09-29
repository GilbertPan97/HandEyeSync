project(third_party)

cmake_minimum_required(VERSION 3.10)

add_custom_target(third_party ALL)

subdirs(boost-1.78)
