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
        REQUIRE(js.has("name"));
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
        REQUIRE(js.has("Key Name"));
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
        REQUIRE(js.has("Name"));
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
        REQUIRE(js.has("LongName-TheJsonKey"));
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
        REQUIRE(js.has("Name123"));
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
        REQUIRE(js.has(""));
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

TEST_CASE("Invalid value string", "[value]") {
    SECTION("Opening quote is expected") {
        const auto in = R"(
            {
                "name" : value"
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
    }

    SECTION("Closing quote is expected") {
        const auto in = R"(
            {
                "name" : "value
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
    }

    SECTION("Invalid quote symbols") {
        const auto in = R"(
            {
                "name" : 'value'
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
    }

    SECTION("Double colon symbols") {
        const auto in = R"(
            {
                "name" : :"value"
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
    }

    SECTION("Illigal LF in the string") {
        const auto in = R"(
            {
                "name" : "va
lue"
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
    }

    SECTION("Illigal vertical tab in the name") {
        const auto in = R"(
            {
                "name" : "value 	 string"
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
    }
}

TEST_CASE("Valid value string", "[value]") {
    SECTION("value1") {
        const auto in = R"(
            {
                "name" : "value"
            }
        )";

        json js(in);
        REQUIRE(js.is_valid());
        REQUIRE(js.size() == 1);
        REQUIRE(js.has("name"));
        REQUIRE(js["name"] == "value");
    }

    SECTION("value2") {
        const auto in = R"(
            {
                "Key Name" : "Value String"
            }
        )";

        json js(in);
        REQUIRE(js.is_valid());
        REQUIRE(js.size() == 1);
        REQUIRE(js.has("Key Name"));
        REQUIRE(js["Key Name"] == "Value String");
    }

    SECTION("value3") {
        const auto in = R"(
            {
                "Name" : ""
            }
        )";

        json js(in);
        REQUIRE(js.is_valid());
        REQUIRE(js.size() == 1);
        REQUIRE(js.has("Name"));
        REQUIRE(js["Name"] == "");
    }

    SECTION("value4") {
        const auto in = R"(
            {
                "LongName-TheJsonKey" : "Some long value string w/ spaces"
            }
        )";

        json js(in);
        REQUIRE(js.is_valid());
        REQUIRE(js.size() == 1);
        REQUIRE(js.has("LongName-TheJsonKey"));
        REQUIRE(js["LongName-TheJsonKey"] == "Some long value string w/ spaces");
    }

    SECTION("value5") {
        const auto in = R"(
            {
                "Name123" : "String w/ symbols: 1234 $%#($@)&$! []"
            }
        )";

        json js(in);
        REQUIRE(js.is_valid());
        REQUIRE(js.size() == 1);
        REQUIRE(js.has("Name123"));
        REQUIRE(js["Name123"] == "String w/ symbols: 1234 $%#($@)&$! []");
    }

    SECTION("Empty value string") {
        const auto in = R"(
            {
                "" : ""
            }
        )";

        json js(in);
        REQUIRE(js.is_valid());
        REQUIRE(js.size() == 1);
        REQUIRE(js.has(""));
        REQUIRE(js[""] == "");
    }
}

TEST_CASE("Illigal sequence", "[sequence]") {
    SECTION("Comma and the end") {
        const auto in = R"(
            {
                "name" : "value",
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
        REQUIRE(js.size() == 0);
    }

    SECTION("Quote is expected") {
        const auto in = R"(
            {
                "name" : "value",
                [
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
        REQUIRE(js.size() == 0);
    }

    SECTION("Double comma") {
        const auto in = R"(
            {
                "name" : "value",,
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
        REQUIRE(js.size() == 0);
    }

    SECTION("Illigal quote symbol") {
        const auto in = R"(
            {
                "name" : "value",
                'key1' : 'value2'
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
        REQUIRE(js.size() == 0);
    }
}

TEST_CASE("Duplicate key", "[sequence]") {
    SECTION("Illigal key") {
        const auto in = R"(
            {
                "name" : "string1",
                "name" : "Some string2"
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
        REQUIRE(js.size() == 0);
    }
}

TEST_CASE("Valid sequence of key/value pairs", "[sequence]") {
    SECTION("Two valid items") {
        const auto in = R"(
            {
                "name1" : "value",
                "name2" : "value"
            }
        )";

        json js(in);
        REQUIRE(js.is_valid());
        REQUIRE(js.size() == 2);
        REQUIRE(js.has("name1"));
        REQUIRE(js.has("name2"));
        REQUIRE(js["name1"] == "value");
        REQUIRE(js["name2"] == "value");
    }

    SECTION("Sequence of  valid items") {
        const auto in = R"(
            {
                "file name" : "image",
                "separator" : ".",
                "extension" : "png",
                "format" : "zip-3.14.betta"
            }
        )";

        json js(in);
        REQUIRE(js.is_valid());
        REQUIRE(js.size() == 4);
        REQUIRE(js.has("file name"));
        REQUIRE(js.has("separator"));
        REQUIRE(js.has("extension"));
        REQUIRE(js.has("format"));
        REQUIRE(js["file name"] == "image");
        REQUIRE(js["separator"] == ".");
        REQUIRE(js["extension"] == "png");
        REQUIRE(js["format"] == "zip-3.14.betta");
    }
}

TEST_CASE("Invalid array body", "[array]") {
    SECTION("Close bracket is expected") {
        const auto in = R"(
            {
                "array" : [
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
        REQUIRE(js.size() == 0);
    }
}

TEST_CASE("Invalid array items", "[array]") {
    SECTION("A valid value is expected") {
        const auto in = R"(
            {
                "array" : [ : ]
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
        REQUIRE(js.size() == 0);
    }

    SECTION("A valid value is expected") {
        const auto in = R"(
            {
                "array" : [,]
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
        REQUIRE(js.size() == 0);
    }

    SECTION("Array illigal sequence of items") {
        const auto in = R"(
            {
                "array" : [ "item1", ]
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
        REQUIRE(js.size() == 0);
    }
}

TEST_CASE("Valid array", "[array]") {
    SECTION("Empty array") {
        const auto in = R"(
            {
                "array" : []
            }
        )";

        json js(in);
        REQUIRE(js.is_valid());
        REQUIRE(js.size() == 1);
        REQUIRE(js.has_array("array"));
        REQUIRE(js.get_array("array").size() == 0);
    }

    SECTION("Array w/ 1 item of empty string") {
        const auto in = R"(
            {
                "array" : [ "" ]
            }
        )";

        json js(in);
        REQUIRE(js.is_valid());
        REQUIRE(js.size() == 1);
        REQUIRE(js.has_array("array"));
        REQUIRE(js.get_array("array").size() == 1);
        REQUIRE(js.get_array("array").at(0) == "");
    }

    SECTION("Array w/ 6 items of empty strings") {
        const auto in = R"(
            {
                "array" : [ "", "", "", "", "", "" ]
            }
        )";

        json js(in);
        REQUIRE(js.is_valid());
        REQUIRE(js.size() == 1);
        REQUIRE(js.has_array("array"));
        REQUIRE(js.get_array("array").size() == 6);
        for (size_t i = 0; i < js.get_array("array").size(); i++)
            REQUIRE(js.get_array("array").at(i) == "");
    }

    SECTION("Array w/ 4 items of strings; free formated") {
        const auto in = R"(
            {
                "array" : [
                    "i1",
                    "i2",
                    "i3",
                    "i4"
                ]
            }
        )";

        json js(in);
        REQUIRE(js.is_valid());
        REQUIRE(js.size() == 1);
        REQUIRE(js.has_array("array"));
        REQUIRE(js.get_array("array").size() == 4);
        REQUIRE(js.get_array("array").at(0) == "i1");
        REQUIRE(js.get_array("array").at(1) == "i2");
        REQUIRE(js.get_array("array").at(2) == "i3");
        REQUIRE(js.get_array("array").at(3) == "i4");
    }
}

TEST_CASE("Invalid object body", "[object]") {
    SECTION("Close bracket is expected") {
        const auto in = R"(
            {
                "object" : {
            }
        )";

        json js(in);
        REQUIRE_FALSE(js.is_valid());
        REQUIRE(js.size() == 0);
    }
}

TEST_CASE("Valid object", "[object]") {
    SECTION("Empty object") {
        const auto in = R"(
            {
                "object" : {}
            }
        )";

        json js(in);
        REQUIRE(js.is_valid());
        REQUIRE(js.size() == 1);
        REQUIRE(js.has_object("object"));
        REQUIRE(js.get_object("object").size() == 0);
    }

    SECTION("Object w/ some fields") {
        const auto in = R"(
            {
                "key1": "value1",
                "object" : {
                    "objkey1": "objvalue1",
                    "objkey2": "objvalue2"
                },
                "key2": "value2"
            }
        )";

        json js(in);
        REQUIRE(js.is_valid());
        REQUIRE(js.size() == 3);
        REQUIRE(js.has("key1"));
        REQUIRE(js.has("key2"));
        REQUIRE(js["key1"] == "value1");
        REQUIRE(js["key2"] == "value2");
        REQUIRE(js.has_object("object"));
        json& obj = js.get_object("object");
        REQUIRE(obj.size() == 2);
        REQUIRE(obj.has("objkey1"));
        REQUIRE(obj.has("objkey2"));
        REQUIRE(obj["objkey1"] == "objvalue1");
        REQUIRE(obj["objkey2"] == "objvalue2");
    }
}

/*
{
    "a": ["a1", ["sub_a1", "sub_a2"], "a3"]
}

    get_array("a").at(0) == "a1"
    get_array("a").at(1) == "__ref.73F60704-F98B-4F1F-BC47-983CC39FEE87"
    get_array("a").at(2) == "a3"

    get_array("__ref.73F60704-F98B-4F1F-BC47-983CC39FEE87")

    map[a]:
        vector[0] = "a1"
        vector[1] = "__ref.73F60704-F98B-4F1F-BC47-983CC39FEE87"
        vector[2] = "a3"

    map[__ref.73F60704-F98B-4F1F-BC47-983CC39FEE87]:
        vector[0] = "sub_a1"
        vector[1] = "sub_a2"
*/
