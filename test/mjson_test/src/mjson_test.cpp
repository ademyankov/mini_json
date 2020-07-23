#define CATCH_CONFIG_MAIN
#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING

#include "catch.hpp"

#include <mjson/mjson.hpp>
using namespace mjson;

TEST_CASE("Empty string", "[json]") {
    json js("");
    REQUIRE_FALSE(js.is_valid());
}

TEST_CASE("Not a valid json string", "[json]") {
    const auto in = R"(*)";

    json js(in);
    REQUIRE_FALSE(js.is_valid());
}

TEST_CASE("Not a valid json body", "[json]") {
    const auto in = R"(
        {
    )";

    json js(in);
    REQUIRE_FALSE(js.is_valid());
}

TEST_CASE("A valid json body", "[json]") {
    const auto in = R"(
        {
        }
    )";

    json js(in);
    REQUIRE(js.is_valid());
}

