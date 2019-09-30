//
// Created by mihailus on 23.09.2019.
//

#ifndef JSON_CONSTANTS_H
#define JSON_CONSTANTS_H

#pragma once

#include <iostream>
#include <set>

namespace JsonConstants {
    const char JSON_COMMA = ',';
    const char JSON_COLON = ':';
    const char JSON_LEFTBRACKET = '[';
    const char JSON_RIGHTBRACKET = ']';
    const char JSON_LEFTBRACE = '{';
    const char JSON_RIGHTBRACE = '}';
    const char JSON_QUOTE = '"';

    const int TRUE_LEN = std::string("true").length();
    const int FALSE_LEN = std::string("false").length();
    const int NULL_LEN = std::string("null").length();
    inline std::set JSON_WHITESPACE = { ' ', '\t', '\b', '\n', '\r' };

    inline std::set JSON_SYNTAX = { JSON_COMMA, JSON_COLON, JSON_LEFTBRACKET, JSON_RIGHTBRACKET,
                                    JSON_LEFTBRACE, JSON_RIGHTBRACE };

}

#endif //JSON_CONSTANTS_H
