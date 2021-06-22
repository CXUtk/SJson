#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "SJson.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

template<typename T>
void expect_parse_throw_error(const std::string& text, const char* file, int line, const char* name) {
    test_count++;
    try {
        SJson::SJsonParse(text);
    }
    catch (T e) {
        test_pass++;
        return;
    }
    catch (std::exception e) {
        fprintf(stderr, "%s:%d: expect throw: %s, actual: %s\n", file, line, name, e.what());
        return;
    }
    fprintf(stderr, "%s:%d: expect throw: %s\n", file, line, name);
}

void expect_parse_nothrow(const std::string& text, const char* file, int line) {
    test_count++;
    try {
        SJson::SJsonParse(text);
    }
    catch (std::exception e) {
        fprintf(stderr, "%s:%d: unexpected error thrown: %s\n", file, line, e.what());
        return;
    }
    test_pass++;
}

void expect_parse_type(const std::string& text, const char* file, int line, SJson::SJsonNodeType type) {
    test_count++;
    try {
        auto node = SJson::SJsonParse(text);
        if (node->GetType() == type) {
            test_pass++;
            return;
        }
        else {
            fprintf(stderr, "%s:%d: type mismatch, expected %s, actual %s\n", file, line, 
                SJson::SJsonGetNodeTypeName(type).c_str(),
                SJson::SJsonGetNodeTypeName(node->GetType()).c_str());
        }
    }
    catch (std::exception e) {
        fprintf(stderr, "%s:%d: unexpected error thrown: %s\n", file, line, e.what());
    }

}

#define EXPECT_PARSE_THROW(text, error) expect_parse_throw_error<error>(text, __FILE__, __LINE__, #error)
#define EXPECT_PARSE_NOTHROW(text) expect_parse_nothrow(text, __FILE__, __LINE__)
#define EXPECT_PARSE_TYPE(text, type) expect_parse_type(text, __FILE__, __LINE__, type)

//
//#define EXPECT_EQ_BASE(equality, expect, actual, format) \
//    do {\
//        test_count++;\
//        if (equality)\
//            test_pass++;\
//        else {\
//            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
//            main_ret = 1;\
//        }\
//    } while(0)
//
//
//#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")
//#define EXPECT_EQ_LINT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%lld")
//#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%lf")
//#define EXPECT_EQ_STRING(expect, actual) EXPECT_EQ_BASE(!strcmp((expect), (actual)), expect, actual, "%s")
//#define EXPECT_EQ_ENUM_ERR(expect, actual) EXPECT_EQ_BASE((expect) == (actual), SJsonGetErrEnumName(expect).c_str(), SJsonGetErrEnumName(actual).c_str(), "%s")
//#define EXPECT_EQ_ENUM_NODE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), SJsonGetNodeEnumName(expect).c_str(), SJsonGetNodeEnumName(actual).c_str(), "%s")
//#define EXPECT_EQ_NULL(actual) EXPECT_EQ_BASE((nullptr) == (actual), 0LL, (ll)actual, "%0llX")
//
static void test_parse_null() {
    EXPECT_PARSE_NOTHROW("null");
    EXPECT_PARSE_TYPE("null", SJson::SJsonNodeType::JSON_NULL);

    EXPECT_PARSE_THROW("nul", SJson::UnexpectedTokenError);
}
//
static void test_parse_expect_value() {
    EXPECT_PARSE_THROW("", SJson::ExpectValueError);
    EXPECT_PARSE_THROW(" ", SJson::ExpectValueError);
    EXPECT_PARSE_THROW("\t", SJson::ExpectValueError);
    EXPECT_PARSE_THROW("\n", SJson::ExpectValueError);
    EXPECT_PARSE_THROW("{", SJson::ExpectValueError);
    EXPECT_PARSE_THROW("[", SJson::ExpectValueError);
}

static void test_parse_invalid_value() {
    EXPECT_PARSE_THROW(",", SJson::InvalidValueError);
    EXPECT_PARSE_THROW("u", SJson::InvalidValueError);
    EXPECT_PARSE_THROW("}", SJson::InvalidValueError);
    EXPECT_PARSE_THROW("\b", SJson::InvalidValueError);
    EXPECT_PARSE_THROW("*", SJson::InvalidValueError);
    EXPECT_PARSE_THROW("?", SJson::InvalidValueError);

}

static void test_parse_root_not_singular() {
    EXPECT_PARSE_THROW("null x", SJson::RootNotSingularError);
    EXPECT_PARSE_THROW("null,null", SJson::RootNotSingularError);
    EXPECT_PARSE_THROW("null,", SJson::RootNotSingularError);
    EXPECT_PARSE_THROW("1,2,3,4,5", SJson::RootNotSingularError);
    EXPECT_PARSE_THROW("{}[]", SJson::RootNotSingularError);
    EXPECT_PARSE_THROW("{},", SJson::RootNotSingularError);
    EXPECT_PARSE_THROW("[1, 2, 3[]]", SJson::RootNotSingularError);

    EXPECT_PARSE_NOTHROW(R"( {"A":[]} )");
    EXPECT_PARSE_NOTHROW(R"( [[[]]] )");
    EXPECT_PARSE_NOTHROW(R"( [1, 2, 3, [], []] )");
}

//static void test_parse_true() {
//    SJsonErrorCode code;
//    auto node = SJsonParse("true", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_BOOL, node->GetType());
//    EXPECT_EQ_INT(true, node->GetBool());
//
//    node = SJsonParse(" true ", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_BOOL, node->GetType());
//    EXPECT_EQ_INT(true, node->GetBool());
//}
//
//static void test_parse_false() {
//    SJsonErrorCode code;
//    auto node = SJsonParse("false ", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_BOOL, node->GetType());
//    EXPECT_EQ_INT(false, node->GetBool());
//
//    node = SJsonParse(" false ", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_BOOL, node->GetType());
//    EXPECT_EQ_INT(false, node->GetBool());
//}
//
//
//static void validate_int_number(const std::string& text, ll number) {
//    SJsonErrorCode code;
//    auto node = SJsonParse(text, code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_INT, node->GetType());
//    EXPECT_EQ_LINT(number, node->GetInt());
//}
//
//static void validate_float_number(const std::string& text, double number) {
//    SJsonErrorCode code;
//    auto node = SJsonParse(text, code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_FLOAT, node->GetType());
//    EXPECT_EQ_DOUBLE(number, node->GetFloat());
//}
//
//static void test_parse_int() {
//    validate_int_number("0", 0);
//    validate_int_number("-0", 0);
//    validate_int_number("1", 1);
//    validate_int_number("-1", -1);
//    validate_int_number("2147483647", 2147483647);
//    validate_int_number("-2147483647", -2147483647);
//    validate_int_number("-2147483648", -2147483648);
//
//    SJsonErrorCode code;
//    auto node = SJsonParse("007", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJOSN_INVALID_VALUE, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_NULL, node->GetType());
//
//    node = SJsonParse("32242gg", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJOSN_INVALID_VALUE, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_NULL, node->GetType());
//
//    node = SJsonParse("+4321", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJOSN_INVALID_VALUE, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_NULL, node->GetType());
//
//    node = SJsonParse(".", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJOSN_INVALID_VALUE, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_NULL, node->GetType());
//}
//
//static void test_parse_float() {
//    validate_float_number("0.0", 0.0);
//    validate_float_number("-0.0", -0.0);
//    validate_float_number("1.0", 1.0);
//    validate_float_number("-1.0", -1.0);
//    validate_float_number("2147483647.0", 2147483647.0);
//    validate_float_number("-2147483647.0", -2147483647.0);
//    validate_float_number(".e14", 0.e14);
//    validate_float_number("3.14e19", 3.14e19);
//    validate_float_number("-3.14e19", -3.14e19);
//    validate_float_number("3.14e-19", 3.14e-19);
//    validate_float_number("-3.14e-19", -3.14e-19);
//    validate_float_number("3.14e+19", 3.14e19);
//    validate_float_number("1e-10000", 0.0);
//}
//
//static void test_parse_string() {
//    SJsonErrorCode code;
//    auto node = SJsonParse("\"Hello\"", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_STRING, node->GetType());
//    EXPECT_EQ_STRING("Hello", node->GetString().c_str());
//
//    node = SJsonParse("\"Hello  \"", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_STRING, node->GetType());
//    EXPECT_EQ_STRING("Hello  ", node->GetString().c_str());
//
//    node = SJsonParse("\"\"", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_STRING, node->GetType());
//    EXPECT_EQ_STRING("", node->GetString().c_str());
//
//    node = SJsonParse("\" Hello\"", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_STRING, node->GetType());
//    EXPECT_EQ_STRING(" Hello", node->GetString().c_str());
//
//    node = SJsonParse("\"Hello\n\"", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_STRING, node->GetType());
//    EXPECT_EQ_STRING("Hello\n", node->GetString().c_str());
//
//    node = SJsonParse("\"Hello\\\\\"", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_STRING, node->GetType());
//    EXPECT_EQ_STRING("Hello\\", node->GetString().c_str());
//
//    node = SJsonParse("\"Hello\\q\"", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJOSN_INVALID_VALUE, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_NULL, node->GetType());
//
//    node = SJsonParse("\"Hello\\ \"", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJOSN_INVALID_VALUE, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_NULL, node->GetType());
//}
//
//
//static void test_parse_array() {
//    SJsonErrorCode code;
//    auto node = SJsonParse(" [ 1, 2, 3 ]", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_ARRAY, node->GetType());
//
//    std::vector<ll> test1 = { 1, 2, 3 };
//    int id = 0;
//    for (auto it = node->begin(); it != node->end(); it++) {
//        EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_INT, (*it)->GetType());
//        EXPECT_EQ_LINT(test1[id], (*it)->GetInt());
//        id++;
//    }
//
//    node = SJsonParse("[1, 2, [1, 2]]", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_ARRAY, node->GetType());
//
//    node = SJsonParse("[null, true, false, 123, \"abc\"]", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_ARRAY, node->GetType());
//
//    node = SJsonParse("[[[]]]", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_ARRAY, node->GetType());
//
//    node = SJsonParse("[ [], [1, 2, 3] ]", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_ARRAY, node->GetType());
//}
//
//static void test_parse_object() {
//    SJsonErrorCode code;
//    auto node = SJsonParse(R"({"Name": "Test", "Age": 114514})", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_OBJECT, node->GetType());
//
//    auto age = (*node)["Age"];
//    EXPECT_EQ_LINT(114514L, age->GetInt());
//
//    auto name = (*node)["Name"];
//    EXPECT_EQ_STRING("Test", name->GetString().c_str());
//
//    node = SJsonParse(R"({ "Obj": {}, "Arr": [1, 2, 3], "": 12})", code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_OBJECT, node->GetType());
//
//    auto obj = (*node)[std::string("Obj")];
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_OBJECT, obj->GetType());
//
//    auto arr = (*node)[std::string("Arr")];
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_ARRAY, arr->GetType());
//
//}

static char* JSON = R"(
{
    "Width": 800,
    "Height": 600,
    "SamplesPerPixel": 64,
    "Integrator": "PathTracer",
    "Camera": {
        "Position": [ 0, 0, -5 ],
        "FOV": 90,
        "LookAt": [ 0, 0, 0 ],
        "Up": [0, 1, 0]
    },
    "Objects": [
        {
            "Type": "TriangleMesh",
            "FilePath": "cube2.obj",
            "Position": [ 0, 0, 0 ],
            "Scale": [ 1, 1, 1 ]
        },
        {
            "Type": "TriangleMesh",
            "FilePath": "bunny.obj",
            "Position": [ 0, 1, 0 ],
            "Scale": [ 1, 1, 1 ]
        }
    ]
}
)";

//
//static void test_parse_json() {
//    SJsonErrorCode code;
//    auto node = SJsonParse(JSON, code);
//    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
//    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_OBJECT, node->GetType());
//}


static void test_parse() {

    test_parse_null();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
    //test_parse_true();
    //test_parse_false();
    //test_parse_int();
    //test_parse_float();
    //test_parse_string();
    //test_parse_array();
    //test_parse_object();
    //test_parse_json();
}

int main() {
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}