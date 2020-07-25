#define CATCH_CONFIG_MAIN
#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING

#include "catch.hpp"

#include <mjson/mjson.hpp>
using namespace mjson;

TEST_CASE("Empty string", "[header]") {
    json js("");
    REQUIRE_FALSE(js.is_valid());
}

TEST_CASE("Not a valid json string", "[header]") {
    const auto in = R"(*)";

    json js(in);
    REQUIRE_FALSE(js.is_valid());
}

TEST_CASE("Not a valid json body", "[header]") {
    SECTION("Partial") {
        const auto in = R"(
            {
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
    }

    SECTION("Illegal symbol") {
        const auto in = R"(
            ,{}
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
    }

    SECTION("No opening curly brace") {
        const auto in = R"(
            "name" : value
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
    }

    SECTION("Not valid") {
        const auto in = R"(
            }{
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
    }
}

TEST_CASE("A valid json body", "[header]") {
    SECTION("Valid") {
        const auto in = R"(
            {
            }
        )";

        json js(in);
        REQUIRE(js.is_valid());
    }

    SECTION("Valid /w escapes") {
        const auto in = "\n{ \t \n \r }\n";

        json js(in);
        REQUIRE(js.is_valid());
    }
}

TEST_CASE("Invalid key name string", "[key]") {
    SECTION("Opening quote is expected") {
        const auto in = R"(
            {
                name" : ""
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
    }

    SECTION("Closing quote is expected") {
        const auto in = R"(
            {
                "name : ""
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
    }

    SECTION("Invalid quote symbols") {
        const auto in = R"(
            {
                'name' : ""
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
    }

    SECTION("Illigal LF in the name") {
        const auto in = R"(
            {
                "Na
me" : ""
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
    }

    SECTION("Illigal vertical tab in the name") {
        const auto in = R"(
            {
                "na	me" : ""
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
    }
}

TEST_CASE("Valid key name", "[key]") {
    SECTION("key1") {
        const auto in = R"(
            {
                "name" : ""
            }
        )";

        json js(in);
        REQUIRE(js.is_valid());
        REQUIRE(js.size() == 1);
        REQUIRE(js["name"] == "");
    }

    SECTION("key2") {
        const auto in = R"(
            {
                "Key Name" : ""
            }
        )";

        json js(in);
        REQUIRE(js.is_valid());
        REQUIRE(js.size() == 1);
        REQUIRE(js["Key Name"] == "");
    }

    SECTION("key3") {
        const auto in = R"(
            {
                "Name" : ""
            }
        )";

        json js(in);
        REQUIRE(js.is_valid());
        REQUIRE(js.size() == 1);
        REQUIRE(js["Name"] == "");
    }

    SECTION("key4") {
        const auto in = R"(
            {
                "LongName-TheJsonKey" : ""
            }
        )";

        json js(in);
        REQUIRE(js.is_valid());
        REQUIRE(js.size() == 1);
        REQUIRE(js["LongName-TheJsonKey"] == "");
    }

    SECTION("key5") {
        const auto in = R"(
            {
                "Name123" : ""
            }
        )";

        json js(in);
        REQUIRE(js.is_valid());
        REQUIRE(js.size() == 1);
        REQUIRE(js["Name123"] == "");
    }

    SECTION("Empty string") {
        const auto in = R"(
            {
                "" : ""
            }
        )";

        json js(in);
        REQUIRE(js.is_valid());
        REQUIRE(js.size() == 1);
        REQUIRE(js[""] == "");
    }
}

TEST_CASE("Not a valid value separator", "[value]") {
    SECTION("No colon") {
        const auto in = R"(
            {
                "name" value
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
        REQUIRE(js.size() == 0);
    }

    SECTION("Invalid separator") {
        const auto in = R"(
            {
                "name" ; value
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
        REQUIRE(js.size() == 0);
    }
}

