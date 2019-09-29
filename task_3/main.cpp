#include <nlohmann/json.hpp>
#include "gtest/gtest.h"
#include <iostream>
#include <iomanip>

int main(){
    auto text = (R"(
    [
        ["Si-9.15", "RTS-9.15", "GAZP-9.15"],
        [100024, 100027, 100050],
        ["Futures contract for USD/RUB", "Futures contract for index RTS", "Futures contract for Gazprom shares"]
    ]
    )");

    nlohmann::json json = nlohmann::json::parse(text);
    std::cout << "Coming json : \n" << std::setw(4) << json << std::endl;

    nlohmann::json readyJson;
    for(size_t i = 0; i < json[0].size(); i++){
        readyJson.push_back(
                nlohmann::json{
                        {"ticker", json[0][i]},
                        {"id", json[1][i]},
                        {"description", json[2][i]}
                }
        );
    }

    std::cout << "Refactor json : \n";
    std::cout << std::setw(4) << readyJson << std::endl;

    std::string tewdlxt = R"({
        "KEY" : 123,
        "KEY2" : true,
        "ARRAY" : [
            1, "str", -12.22, true, {
                                        "home" : [{"lamp" : true}, {"water" : false} ]
                                    }
        ],
        "TROLOLO" : {
            "LOL" : [ 666,666,666]
        }
    })";
    nlohmann::json sh = nlohmann::json::parse(tewdlxt);
    std::cout << std::setw(4) << sh << std::endl;

    return 0;
}
