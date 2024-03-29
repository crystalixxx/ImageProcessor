#include <exception>
#include <string>

#pragma once
class InvalidInput : public std::exception {
public:
    explicit InvalidInput(const std::string& wrong);

    const char* what() const noexcept override;

private:
    std::string msg_;
};

class InvalidParams : public std::exception {
public:
    explicit InvalidParams(const std::string& wrong);

    const char* what() const noexcept override;

private:
    std::string msg_;
};