#pragma once

#include <iostream>
#include <string>
#include <any>
#include <unordered_map>
#include <stack>
#include <experimental/filesystem>
#include <set>
#include "json_exception.h"
#include "Constants.h"
namespace fs = std::experimental::filesystem;
using TokensType = std::vector<std::any>;
using namespace JsonConstants;


class Json {
public:
	using ObjectType = std::unordered_map<std::string, std::any>;
	using ArrayType = std::vector<std::any>;



	// Конструктор из строки, содержащей Json-данные.
	explicit Json(const std::string& s);
	// Конструктор без входных данных
	explicit Json() = default;
	
	// Деструктор
	~Json();

	int numOfMapElem() {
		return jsonObject.size();
	}

	int numOfArrElem() {
		return jsonArray.size();
	}

	// Метод возвращает true, если данный экземпляр содержит в себе JSON-массив. Иначе false.
	bool is_array() const;
	// Метод возвращает true, если данный экземпляр содержит в себе JSON-объект. Иначе false.
	bool is_object() const;

	// Метод возвращает значение по ключу key, если экземпляр является JSON-объектом.
	// Значение может иметь один из следующих типов: Json, std::string, double, bool или быть пустым.
	// Если экземпляр не является JSON-массивом, генерируется исключение.
	std::any& operator[](const std::string& key);

	// Метод возвращает значение по индексу index, если экземпляр является JSON-массивом.
	// Значение может иметь один из следующих типов: Json, std::string, double, bool или быть пустым.
	// Если экземпляр не является JSON-объектом, генерируется исключение.
	std::any& operator[](unsigned index);

	friend std::ostream& operator<<(std::ostream& out, const std::any elem);


	// Метод возвращает объект класса Json из строки, содержащей Json-данные.
	static Json parse(const std::string& s) {
		return Json(s);
	}
		
	// Метод возвращает объекта класса Json из файла, содержащего Json-данные в текстовом формате.
	static Json parseFile(	const std::string& path_to_file);

	// Метод выводит json ползователю в соответствующим этому типа данных виде
	void readAllElements();



private:
	
	enum JsonType
	{
		OBJECT, ARRAY
	};

	JsonType mode = OBJECT;
	ObjectType jsonObject;
	ArrayType jsonArray;

	void from_string(std::string inputString);

    std::any parseTokens(TokensType& ,bool);


public:
    Json parse_object(TokensType &);
    Json parse_array(TokensType &);

	ObjectType getObject() { return jsonObject; }
	ArrayType getArray() { return jsonArray; }

    void setMode(JsonType jsonMode){
        mode = jsonMode;
    }

private:


    char generateEscapeSequences(char symbol){
	    std::set signs = {'b', 'f', 'n', 'r', 't', '\\'};
	    if(symbol == 'b')
	        return '\b';
        if(symbol == 'f')
            return '\f';
        if(symbol == 'n')
            return '\n';
        if(symbol == 'r')
            return '\r';
        if(symbol == 't')
            return '\t';
        if(symbol == '\\')
            return '\\';
        else
            throw json_exception("Lex_error : invalid character of escape-sequece\n");
    }

    std::optional<std::string>
    lex_string(std::string &string) {
        std::string json_string = "";

        if (string[0] == JSON_QUOTE)
            string.erase(0, 1);
        else
            return std::nullopt;

        bool haveBackSlash = false;
        int skipedSigns = 0;
        for (auto element : string) {
            if (element == JSON_QUOTE) {
                if(!haveBackSlash) {
                    string = string.substr(json_string.length() + skipedSigns +1,std::string::npos);
                    return json_string;
                }
                else{
                    json_string += element;
                    haveBackSlash = false;
                }
            }
            else {
                if (element == '\\') {
                    haveBackSlash = true;
                    skipedSigns++;
                    continue;
                }
                else{
                    if(!haveBackSlash)
                        json_string += element;
                    else{
                        std::set signs = {'b', 'f', 'n', 'r', 't', '/', '\\'};
                        if(signs.find(element) != signs.end())
                            json_string += generateEscapeSequences(element);
                        else
                            throw json_exception("Lex_error : invalid character of escape-sequece\n");
                    }
                }
            }
        }
        throw json_exception("Expected end-of-string quote");
    }

    std::optional<std::any>
    lex_number(std::string &string) {
        std::string json_number = "";

        std::set<char> number_characters = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', 'e', '.'};
        bool hasDot = false;
        for (auto element : string) {
            if (number_characters.find(element) != number_characters.end()){
                if (element == '.') {
                    if (!hasDot) {
                        json_number += element;
                        hasDot = true;
                    }
                    else {
                        throw json_exception("Expected ONE dot in number");
                    }
                } else {
                    json_number += element;
                }
            }
            else
                break;
        }

        auto rest = string.substr(json_number.length());

        if (json_number.empty())
            return std::nullopt;

        if (json_number.find(".") != std::string::npos) {
            string = rest;
            return std::stof(json_number);
        } else {
            string = rest;
            return std::stoi(json_number);
        }
    }


    std::optional<bool>
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

    std::optional<std::any >
    lex_null(std::string &string) {
        const int string_len = string.length();

        if (string_len >= NULL_LEN && string.substr(0, NULL_LEN) == "null") {
            string = string.substr(NULL_LEN);
            return std::any{};
        }

        return std::nullopt;
    }

    std::vector<std::any>   lex(std::string & string) {
        std::vector<std::any> tokens;

        while (string.length() != 0) {
            std::optional<std::string> json_string = "";
            json_string = lex_string(string);
            if (json_string.has_value()) {
                tokens.emplace_back(json_string.value());
                continue;
            }

            std::optional<std::any> return_number;
            return_number = lex_number(string);
            if (return_number.has_value()) {
                if(return_number.value().type() == typeid(int)){
                    tokens.emplace_back(std::any_cast<int>(return_number.value()));
                    continue;
                }
                else if(return_number.value().type() == typeid(float)){
                    tokens.emplace_back(std::any_cast<float>(return_number.value()));
                    continue;
                }
                else
                    throw json_exception("Error : expected number, but founded : "/* + return_number.value()*/);
            }

            std::optional<bool> json_bool;
            json_bool = lex_bool(string);
            if (json_bool.has_value()) {
                tokens.emplace_back(json_bool.value());
                continue;
            }

            std::optional<std::any > json_null;
            json_null = lex_null(string);
            if (json_null.has_value()) {
                tokens.emplace_back(json_null.value());
                continue;
            }

            char character = string[0];

            if (JSON_WHITESPACE.find(character) != JSON_WHITESPACE.end())
                string.erase(0, 1);
            else if (JSON_SYNTAX.find(character) != JSON_SYNTAX.end()) {
                string.erase(0, 1);
                tokens.emplace_back(character);
            }
            else {
                std::string message = std::string("Unexpected character : ") + character;
                throw json_exception(message);
            }
        }

        return tokens;
    }


};

inline std::string operator+(const std::string buf, std::any newInfo) {
	std::string result = "";
	if (!newInfo.has_value()) {
		result = buf + "null";
	}
	else
	if (newInfo.type() == typeid(std::string)) {
		result = buf + "\"" + std::any_cast<std::string>(newInfo) + "\"";
	}
	else if (newInfo.type() == typeid(int)) {
		result = buf + std::to_string(std::any_cast<int>(newInfo));
	}
	else if (newInfo.type() == typeid(float)) {
		result = buf + std::to_string(std::any_cast<float>(newInfo));
	}
	else if (newInfo.type() == typeid(std::unordered_map<std::string, std::any>)) {
		result += buf + "{\n";
		std::unordered_map<std::string, std::any> tmp = std::any_cast<std::unordered_map<std::string, std::any>>(newInfo);
		std::string tmpStr;
		for (auto i : tmp) {
			tmpStr += "\"" + i.first + "\":" + i.second;
			tmpStr += ",";
		}
		tmpStr.erase(tmpStr.size() - 1, 1); // delete last ','
		result +=  tmpStr + "\n}";
	}
	else if (newInfo.type() == typeid(std::vector<std::any>)) {
		std::vector<std::any> tmp = std::any_cast<std::vector<std::any>>(newInfo);
		result += buf + "[\n" ;
		for (unsigned i = 0; i < tmp.size(); i++) {
			result = result + tmp[i];
			if (i != tmp.size() - 1) {
				result += ",";
			}
		}
		result += "\n]";
	}
	else if (newInfo.type() == typeid(bool)) {
		//result = buf +  std::any_cast<bool>(newInfo);
		if (std::any_cast<bool>(newInfo) == 1) {
			result = buf + "true";
		}
		else if (std::any_cast<bool>(newInfo) == 0) {
			result = buf + "false";
		}
		else {
			throw json_exception("Operator '+' error : unknown type of element");
		}
	}
    else if (newInfo.type() == typeid(char)){
        result = std::any_cast<char>(newInfo);
    }
	else if (newInfo.type() == typeid(Json)) {
		Json tmp = std::any_cast<Json>(newInfo);
		if (tmp.is_array()) {
			result = buf + tmp.getArray();
		}
		else {
			result = buf + tmp.getObject();
		}
	}
	else {
		throw json_exception("Operator '+' error : unknown type");
	}
	return result;
}

inline std::ostream& operator<<(std::ostream& out, const std::any &elem) {

	if (!elem.has_value()) {
		out << "null";
	}
	else if (elem.type() == typeid(char)){
	    out << std::any_cast<char>(elem);
	}
	else if (elem.type() == typeid(std::unordered_map<std::string, std::any>)) {
		std::unordered_map<std::string, std::any> tmp = std::any_cast<std::unordered_map<std::string, std::any>>(elem) ;
		out << "{\n";
		std::string tmpStr;
		for (auto i : tmp) {
			tmpStr += "\"" + i.first + "\":" + i.second;
			tmpStr += ",";
		}
		tmpStr.erase(tmpStr.size() - 1, 1); // delete last ','
		out << tmpStr + "\n}";

	}
	else if (elem.type() == typeid(std::vector<std::any>)) {
		std::vector<std::any> tmp = std::any_cast<std::vector<std::any>>(elem);
		out << "[\n";
		for (unsigned i = 0; i < tmp.size(); i++) {
			out << tmp[i];
			if (i != tmp. size() - 1) {
				out << ",";
			}
		}
		out << "\n]";
	}
	else if (elem.type() == typeid(int)) {
		out << std::to_string(std::any_cast<int>(elem));
	}
	else if (elem.type() == typeid(float)){
		out << std::to_string(std::any_cast<float>(elem));
	}
	else if (elem.type() == typeid(std::string)) {
		out << "\"" << std::any_cast<std::string>(elem) << "\"";
	}
	else if (elem.type() == typeid(bool)) {
		if (std::any_cast<bool>(elem) == 1) {
			out << "true";
		}
		else if (std::any_cast<bool>(elem) == 0) {
			out << "false";
		}
		else {
			throw json_exception("Operator '+' error : unknown type of element");
		}
	}
	else {
		throw json_exception("Operator '<<' error : unknown type of element");
	}
	return out;
}