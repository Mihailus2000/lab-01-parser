
#include <gtest/gtest.h>
#include "Json.h"


TEST(JsonObject, ExampleTest){
    std::string json = R"({
    "lastname" : "Ivanov",
    "firstname" : "Ivan",
    "age" : 25,
    "islegal" : false,
    "marks" : [
    	4,5,5,5,2,3
    ],
    "address" : {
    	"city" : "Moscow",
        "street" : "Vozdvijenka"
    }
}   )";

    Json object = Json::parse(json);
    EXPECT_EQ(std::any_cast<std::string>(object["lastname"]), "Ivanov");
    EXPECT_EQ(std::any_cast<bool>(object["islegal"]), false);
    EXPECT_EQ(std::any_cast<int>(object["age"]), 25);

    auto marks = std::any_cast<Json>(object["marks"]);
    EXPECT_EQ(std::any_cast<int>(marks[0]), 4);
    EXPECT_EQ(std::any_cast<int>(marks[1]), 5);

    auto address = std::any_cast<Json>(object["address"]);
    EXPECT_EQ(std::any_cast<std::string>(address["city"]), "Moscow");
    EXPECT_EQ(std::any_cast<std::string>(address["street"]), "Vozdvijenka");
}

TEST(JsonObject, EmptyObject)
{
    Json json{"{}"};
    EXPECT_EQ(json.is_object(), true);
    EXPECT_EQ(json.is_array(), false);
}

TEST(JsonObject, EmptyObjectWithSpaces)
{
    Json json{"{  \n  \t \t \n }"};
    EXPECT_EQ(json.is_object(), true);
    EXPECT_EQ(json.is_array(), false);
}

TEST(JsonObject, SimpleObject)
{
    Json json{R"({ "key" : "value" })"};
    EXPECT_EQ(json.is_object(), true);
    EXPECT_EQ(json.is_array(), false);

    EXPECT_EQ(std::any_cast<std::string>(json["key"]), "value");
}

TEST(JsonObject, NestedObject)
{
    Json json{R"({ "key" : { "number":   9.5 } })"};
    EXPECT_EQ(json.is_object(), true);
    EXPECT_EQ(json.is_array(), false);

    Json nested = std::any_cast<Json >(json["key"]);
    EXPECT_EQ(nested.is_object(), true);
    EXPECT_EQ(nested.is_array(), false);

    EXPECT_EQ(std::any_cast<float>(nested["number"]), 9.5);
}

TEST(JsonObject, NestedWithArray)
{
    Json json{R"({ "key" : [1,2,3] })"};
    EXPECT_EQ(json.is_object(), true);
    EXPECT_EQ(json.is_array(), false);

    Json nested = std::any_cast<Json>(json["key"]);
    EXPECT_EQ(nested.is_object(), false);
    EXPECT_EQ(nested.is_array(), true);

    EXPECT_EQ(std::any_cast<int>(nested[0]), 1);
}

TEST(JsonObject, NestedWithArrayWithValues)
{
    Json json{R"({ "key" : [1,2,3], "key2": true })"};
    EXPECT_EQ(json.is_object(), true);
    EXPECT_EQ(json.is_array(), false);

    Json nested = std::any_cast<Json >(json["key"]);
    EXPECT_EQ(nested.is_object(), false);
    EXPECT_EQ(nested.is_array(), true);

    EXPECT_EQ(std::any_cast<int>(nested[0]), 1);
    EXPECT_EQ(std::any_cast<int>(nested[1]), 2);
    EXPECT_EQ(std::any_cast<int>(nested[2]), 3);

    EXPECT_EQ(std::any_cast<bool>(json["key2"]), true);
}

TEST(JsonObject, WrongKeywordStart)
{
    EXPECT_THROW(
            Json{R"({ "some_key": "some_value", "new_key": undefined    })"},
            json_exception
    );
}

TEST(JsonObject, WrongEnd)
{
    EXPECT_THROW(
            Json{R"({ "key": {    })"},
            json_exception
    );
}

TEST(JsonObject, WrongObject){
EXPECT_THROW(
        Json{R"({ "some_key": "some_value", "value again"    })"},
        json_exception
);
                }



TEST(JsonObject, WrongKeywordEnd) {
    EXPECT_THROW(
Json{R"({ "some_key": "some_value", "new_key": fallen    })"},
        json_exception
    );
}

TEST(JsonObject, CharAfterEndException){
    EXPECT_THROW(
Json{R"({},)"},
    json_exception
    );
}


TEST(JsonObject, ExpectedKeyException){
    EXPECT_THROW(
       Json{R"({ 123: 123 })"},
       json_exception
    );
}

TEST(JsonObject, ExpectedSomething){
    EXPECT_THROW(
Json{R"({ "123": })"},
          json_exception
    );
}

TEST(JsonObject, KeyRepeat){
    EXPECT_THROW(
Json{R"({ "123":1, "123": 2 })"},
          json_exception
    );
}

TEST(JsonObject, BackslashInKey){
    Json json{R"({ "1\"2" : true })"};
    EXPECT_EQ(json.is_object(), true);
    EXPECT_EQ(json.is_array(), false);
    EXPECT_EQ(std::any_cast<bool>(json["1\"2"]), true);
}

TEST(JsonObject, BackslashInKeyBad){
    EXPECT_THROW(
Json{R"({ "\1"":1 })"},
          json_exception
    );
}