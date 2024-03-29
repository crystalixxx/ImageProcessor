#include "exceptions.h"

InvalidInput::InvalidInput(const std::string& wrong) : msg_{wrong} {
}

const char* InvalidInput::what() const noexcept {
    return msg_.c_str();
}

InvalidParams::InvalidParams(const std::string& wrong) : msg_{wrong} {
}

const char* InvalidParams::what() const noexcept {
    return msg_.c_str();
}
