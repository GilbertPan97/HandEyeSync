//
// Created by sun on 2020/2/27.
//

#pragma once


#include <stdexcept>
#include <string>

#ifdef WIN32
#include <windows.h>
#endif // WIN32

#include <cstring>
#include <sstream>


class Exception : public ::std::runtime_error {
public:
    Exception(const ::std::string &what_arg) : std::runtime_error(what_arg) {

    }
    Exception(const int& errnum): std::runtime_error(strerror(errnum)) {

    }

    virtual ~Exception() throw() {

    }

protected:

private:
    static ::std::string strerror(const int& errnum) {
#ifdef WIN32
        ::LPTSTR buffer = nullptr;
			::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, errnum, 0, reinterpret_cast<LPTSTR>(&buffer), 0, nullptr);
			::std::string message = nullptr != buffer ? buffer : std::to_string(errnum);
			::LocalFree(buffer);
			return message;
#else // WIN32
        return ::strerror(errnum);
#endif // WIN32
    }
};

