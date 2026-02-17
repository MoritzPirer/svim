///
/// @file: FileException.hpp
/// @description: description
///
/// @date: 2026-02-17
/// @author: Moritz Pirer
///

#ifndef FILE_EXCEPTION_HPP
#define FILE_EXCEPTION_HPP

#include <exception>
#include <string>

class FileException: public std::exception {
private:
  std::string message_;
public:
    FileException(): message_("FileException") {}
    FileException(const std::string& msg): message_(msg) {}

    virtual const char* what() const noexcept override {
        return message_.c_str();
    }
};

#endif //FILE_EXCEPTION_HPP
