//
// Created by mihailus on 16.09.2019.
//

#ifndef JSON_JSON_EXCEPTION_H
#define JSON_JSON_EXCEPTION_H

#include <exception>
#include <iostream>

class json_exception : public std::exception {
public:
    json_exception(std::string const &message) : msg(message) {}
    virtual char const *what() const noexcept {
        std::cerr << msg << std::endl;
        return msg.c_str();
    }

private:

    std::string msg;
};


#endif //JSON_JSON_EXCEPTION_H
