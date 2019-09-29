
#include <gtest/gtest.h>
#include "Json.h"

TEST(Json, TestParseFromFile){
    Json object;
    object.parseFile("../tests/TestData.json");

    EXPECT_EQ(object.is_array(), false);
    EXPECT_EQ(object.is_object(), true);
}

TEST(Json, ParseFileException){
    EXPECT_THROW(
            Json::parseFile("Not existing file path"),
            json_exception
            );
}


