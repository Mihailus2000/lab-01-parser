

#include <optional>
#include <iostream>
#include <any>
#include <tuple>
#include <string>
#include <vector>
#include "Constants.h"
#include "json_exception.h"
#include "Json.h"

using namespace JsonConstants;




std::optional<std::any>
lex_string(std::string &string) {
	std::string json_string = "";

	if (string[0] == JSON_QUOTE)
		string.erase(1, string.length() - 1);
	else
		return std::nullopt;

	for (auto element : string) {
		if (element == JSON_QUOTE) {
            string = string.substr(json_string.length() + 1);
            return json_string;
        }
		else
			json_string += element;
	}
    throw json_exception("Expected end-of-string quote");
}

std::optional<std::any>
lex_number(std::string string) {
	std::string json_number = "";

	std::set<std::any> number_characters = { 0,1,2,3,4,5,6,7,8,9,'-','e','.' };
	for (auto element : string) {
		if (number_characters.find(element) != number_characters.end())
			json_number += element;
		else
			break;
	}

	auto rest = string.substr(json_number.length());

	if (json_number.empty())
		return std::nullopt;

	if (json_number.find('.'))
		return std::stof(json_number);

	return std::stoi(json_number);
}


std::optional<std::any>
lex_bool(std::string &string) {
	const int string_len = string.length();

    if (string_len >= TRUE_LEN && string.substr(0, TRUE_LEN) == "true") {
        string = string.substr(TRUE_LEN);
        return true;
    } else if (string_len >= FALSE_LEN && string.substr(0, FALSE_LEN) == "false") {
        string = string.substr(FALSE_LEN);
        return false;
    }
	return std::nullopt;
}

std::optional<std::any>
lex_null(std::string &string) {
	const int string_len = string.length();

    if (string_len >= NULL_LEN && string.substr(0, NULL_LEN) == "null") {
        string = string.substr(NULL_LEN);
        return nullptr;
    }

	return std::nullopt;
}

std::vector<std::any> lex(std::string & string) {
	std::vector<std::any> tokens;

	while (string.length()) {
		std::optional<std::string> json_string = "";
		json_string = std::any_cast<std::optional<std::string>>(string);
		if (json_string.has_value()) {
			tokens.emplace_back(json_string);
			continue;
		}

//		int json_number_int;
//		float json_number_float;
		std::optional<std::any> json_number;
		json_number = lex_number(string);
		if (json_number.has_value()) {
			tokens.emplace_back(json_number);
			continue;
		}

		std::optional<bool> json_bool;
		json_bool = std::any_cast<bool>(lex_bool(string));
		if (json_bool.has_value()) {
			tokens.emplace_back(json_bool);
			continue;
		}

		std::optional<nullptr_t > json_null;
        json_null = std::any_cast<std::nullptr_t>(lex_null(string));
		if (json_null.has_value()) {
			tokens.emplace_back(json_null);
			continue;
		}

		char character = string[0];

		if (JSON_WHITESPACE.find(character) != JSON_WHITESPACE.end())
			string.erase(0, 1);
		else if (JSON_SYNTAX.find(character) != JSON_SYNTAX.end()) {
			tokens.emplace_back(character);
		}
		else {
		    std::string message = std::string("Unexpected character : ") + character;
		    throw json_exception(message);
		}

		return tokens;
	}
}



void from_string(std::string inputString) {
	std::vector<std::any> tokens = lex(inputString);

	for (auto i : tokens) {
		std::cout << i << " ";
	}
}