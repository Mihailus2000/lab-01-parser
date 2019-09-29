#include "Json.h"
#include <fstream>
#include <set>
#include <iomanip>
#include "json_exception.h"
#include "Constants.h"



using TokensType = std::vector<std::any>;
using namespace JsonConstants;
Json::Json(const std::string& s) {
	from_string(s);
}

Json::~Json()
{

}

bool Json::is_array() const
{
	if (mode == ARRAY)
		return true;
	else
		return false;
}

bool Json::is_object() const
{
	if (mode == OBJECT)
		return true;
	else
		return false;
}

std::any& Json::operator[](const std::string& key)
{
	if (is_object()) {
		if (jsonObject.find(key) != jsonObject.end()) {
			return jsonObject.at(key);
		}
		else {
			std::string exc = "Error : Not found key \"" + key + "\" in object\n";
			throw json_exception(exc.c_str());
		}
	}
	else {
		throw json_exception("Error : This element not an Object\n");
	}
}

std::any& Json::operator[](unsigned int _index)
{
	if (is_array()) {
		if (_index < jsonArray.size()) {
			return jsonArray.at(_index);
		}
		else {
			throw json_exception("Error : Array index out of bounds\n");
		}
	}
	else {
		throw json_exception("Error : This element not an Array\n");
	}
}



void Json::readAllElements()
{
    std::cout << "\n---------------------------\n";
	if (mode == OBJECT) {
		std::cout <<  jsonObject;
	}
	else {
		std::cout  << jsonArray;
	}
	std::cout << "\n";
}



Json Json::parseFile(const std::string& path_to_file)
{
	std::ifstream fin(path_to_file, std::ios::binary);
	if (!fin.fail()) {
		std::string File(
			std::istreambuf_iterator<char>{fin},
			std::istreambuf_iterator<char>()
		);
		return Json(File);
	}
	else {
        throw json_exception("Unknown file");
	}
}





std::any Json::parseTokens(TokensType &tokens,  bool is_root = false) {
    auto token = tokens[0];
    if (token.type() == typeid(char)) {
        char t_str = std::any_cast<char>(token);
        if (is_root && (t_str != JSON_LEFTBRACE)) {
//            throw json_exception("Root must be an object! ");
        }

        tokens.erase(tokens.begin(),tokens.begin() + 1);

        if (t_str == JSON_LEFTBRACKET) {
            Json Array;
            Array.setMode(Json::ARRAY);
            return Array.parse_array(tokens);
        } else if (t_str == JSON_LEFTBRACE) {
            Json Object;
            Object.setMode(Json::OBJECT);
            return Object.parse_object(tokens);
//          return  NULL;
        } else {
            tokens.erase(tokens.begin(), tokens.begin() + 1);
            return token;
        }
    } else {
        tokens.erase(tokens.begin(), tokens.begin() + 1);
        return token;
    }
}





Json Json::parse_array(TokensType &tokens) {
    auto token = tokens[0];

    if(token.type() == typeid(char)) {
        if (std::any_cast<char>(token) == JSON_RIGHTBRACKET) {
            tokens.erase(tokens.begin(), tokens.begin() + 1);
            return *this;
        }
    }

    while(!tokens.empty()){
        auto array_element = parseTokens(tokens);
        jsonArray.push_back(array_element);
        token = tokens[0];
        if(token.type() == typeid(char)) {
            if(std::any_cast<char>(token) == JSON_RIGHTBRACKET) {
                tokens.erase(tokens.begin(),tokens.begin()+1);
                return *this;
            }
            else if(std::any_cast<char>(token) !=  JSON_COMMA)
                throw json_exception("Expected comma after pair in object, got :" + token);

            tokens.erase(tokens.begin(),tokens.begin()+1); // Skip COMMON
        }
        else
            throw json_exception("Expected comma after pair in object, got :" + token);
    }
    return *this;
}


Json Json::parse_object(TokensType &tokens) {
    auto token = tokens[0];

    if (token.type() == typeid(char) ){
        if(std::any_cast<char>(token) == JSON_RIGHTBRACE){
            tokens.erase(tokens.begin(),tokens.begin()+1);
            return *this;
        }
    }

    while(!tokens.empty()){
        std::string json_key;
        if(tokens[0].type() == typeid(std::string)) {
            json_key = std::any_cast<std::string>(tokens[0]);
            if(jsonObject.find(json_key) != jsonObject.end())
                throw json_exception("Error : Key repeating in object\n");
            tokens.erase(tokens.begin(), tokens.begin()+1);
        }
        else
            throw json_exception("Expected string key, got :" + tokens[0]);

        if (tokens[0].type() == typeid(char) ) {
            if (std::any_cast<char>(tokens[0]) != JSON_COLON) {
                throw json_exception("Expected colon after key in object, got :" + tokens[0]);

            }

        }
        else
            throw json_exception("Expected colon after key in object, got :" + tokens[0]);


        TokensType tmp_tokens(tokens.begin()+1,tokens.end());
        auto json_value = parseTokens(tmp_tokens);
        tokens = std::move(tmp_tokens);
        jsonObject.insert({json_key, json_value});


        token = tokens[0];
        if(tokens[0].type() == typeid(char)) {
            if(std::any_cast<char>(token) == JSON_RIGHTBRACE) {
                tokens.erase(tokens.begin(),tokens.begin()+1);
                return *this;

            }
            else if(std::any_cast<char>(token) !=  JSON_COMMA)
                throw json_exception("Expected comma after pair in object, got :" + token);

            tokens.erase(tokens.begin(),tokens.begin()+1); // skip COMMON

        }
    }

    throw json_exception("Expected end-of-object bracket");

}

void Json::from_string(std::string Instring) {
    std::vector<std::any> tokens = lex(Instring);
    std::cout << "\nTOKENS: ";
    for(auto element : tokens){
        std::cout <<  element;
    }
    if(tokens[0].type() == typeid(char)) {
        if (std::any_cast<char>(tokens[0]) == JSON_LEFTBRACKET) {
            mode = ARRAY;
            jsonArray = std::any_cast<Json>(parseTokens(tokens, true)).getArray();
        } else if (std::any_cast<char>(tokens[0]) == JSON_LEFTBRACE) {
            mode = OBJECT;
            jsonObject = std::any_cast<Json>(parseTokens(tokens, true)).getObject();

        }
        return;
    }
    else
        throw json_exception("From start of JSON expected '{' or '[', but got : " + tokens[0]);

}


