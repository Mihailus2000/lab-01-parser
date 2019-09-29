#include <iostream>
#include "Json.h"
int main(){
    std::cout << "---------------- TASK 2 ----------------\n\n";
    std::string text = R"({
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
    Json task(text);
    task.readAllElements();
    return 0;
}
