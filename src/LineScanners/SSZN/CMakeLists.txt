set(TARGET_NAME Sszn_Driver)

find_package(SR7Link REQUIRED)

# Define source files and header files
set(SRC_FILES
    Initialize.c
    Discover.c
    Open.c
    ReceiveProfile.c
    GetDevSetting.c
    ../CLogger/clog.c
    ../CLogger/logger.c
)

set_source_files_properties(Initialize.c PROPERTIES LANGUAGE CXX)
set_source_files_properties(Discover.c PROPERTIES LANGUAGE CXX)
set_source_files_properties(Open.c PROPERTIES LANGUAGE CXX)
set_source_files_properties(ReceiveProfile.c PROPERTIES LANGUAGE CXX)
set_source_files_properties(GetDevSetting.c PROPERTIES LANGUAGE CXX)

set(HEADER_FILES
    SRCommon.h
)

# Build driver lib
add_library(${TARGET_NAME} ${SRC_FILES})

target_link_libraries(${TARGET_NAME} PUBLIC ${SR7Link_LIBRARIES})
message(STATUS "SR7Link libs is: ${SR7Link_LIBRARIES}")

target_include_directories(${TARGET_NAME} PUBLIC
    ${SR7Link_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../CLogger
)

set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# Copy SR7Link DLLs to the target output directory
file(GLOB SR7Link_DLLS "${SR7Link_EXECUTABLE_DIRS}/*.dll")
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${SR7Link_DLLS}
    $<TARGET_FILE_DIR:${TARGET_NAME}>
)

if(OFF)
    add_executable(Sszn_test test.cpp)
    target_link_libraries(Sszn_test PRIVATE ${TARGET_NAME})
    target_include_directories(Sszn_test PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()