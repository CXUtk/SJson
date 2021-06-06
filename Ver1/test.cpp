#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SJson.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do {\
        test_count++;\
        if (equality)\
            test_pass++;\
        else {\
            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
            main_ret = 1;\
        }\
    } while(0)


#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")
#define EXPECT_EQ_LINT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%lld")
#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%lf")
#define EXPECT_EQ_STRING(expect, actual) EXPECT_EQ_BASE(!strcmp((expect), (actual)), expect, actual, "%s")
#define EXPECT_EQ_ENUM_ERR(expect, actual) EXPECT_EQ_BASE((expect) == (actual), SJsonGetErrEnumName(expect).c_str(), SJsonGetErrEnumName(actual).c_str(), "%s")
#define EXPECT_EQ_ENUM_NODE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), SJsonGetNodeEnumName(expect).c_str(), SJsonGetNodeEnumName(actual).c_str(), "%s")
#define EXPECT_EQ_NULL(actual) EXPECT_EQ_BASE((nullptr) == (actual), 0LL, (ll)actual, "%0llX")

static void test_parse_null() {
    SJsonErrorCode code;
    auto node = SJsonParse("null", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_NULL, node->GetType());
}

static void test_parse_expect_value() {
    SJsonErrorCode code;
    auto node = SJsonParse("", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_EXPECT_VALUE, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_NULL, node->GetType());

    node = SJsonParse(" ", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_EXPECT_VALUE, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_NULL, node->GetType());
}

static void test_parse_invalid_value() {
    SJsonErrorCode code;
    auto node = SJsonParse("nul", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJOSN_INVALID_VALUE, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_NULL, node->GetType());

    node = SJsonParse("?", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJOSN_INVALID_VALUE, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_NULL, node->GetType());
}

static void test_parse_root_not_singular() {
    SJsonErrorCode code;
    auto node = SJsonParse("null x", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_ROOT_NOT_SINGULAR, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_NULL, node->GetType());

    node = SJsonParse("null, null", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_ROOT_NOT_SINGULAR, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_NULL, node->GetType());

    node = SJsonParse("true, false", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_ROOT_NOT_SINGULAR, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_NULL, node->GetType());
}

static void test_parse_true() {
    SJsonErrorCode code;
    auto node = SJsonParse("true", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_BOOL, node->GetType());
    EXPECT_EQ_INT(true, (bool)*node);

    node = SJsonParse(" true ", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_BOOL, node->GetType());
    EXPECT_EQ_INT(true, (bool)*node);
}

static void test_parse_false() {
    SJsonErrorCode code;
    auto node = SJsonParse("false ", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_BOOL, node->GetType());
    EXPECT_EQ_INT(false, (bool)*node);

    node = SJsonParse(" false ", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_BOOL, node->GetType());
    EXPECT_EQ_INT(false, (bool)*node);
}


static void validate_int_number(const std::string& text, ll number) {
    SJsonErrorCode code;
    auto node = SJsonParse(text, code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_INT, node->GetType());
    EXPECT_EQ_LINT(number, (ll)*node);
}

static void validate_float_number(const std::string& text, double number) {
    SJsonErrorCode code;
    auto node = SJsonParse(text, code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_FLOAT, node->GetType());
    EXPECT_EQ_DOUBLE(number, (double)*node);
}

static void test_parse_int() {
    validate_int_number("0", 0);
    validate_int_number("-0", 0);
    validate_int_number("1", 1);
    validate_int_number("-1", -1);
    validate_int_number("2147483647", 2147483647);
    validate_int_number("-2147483647", -2147483647);
    validate_int_number("-2147483648", -2147483648);

    SJsonErrorCode code;
    auto node = SJsonParse("007", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJOSN_INVALID_VALUE, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_NULL, node->GetType());

    node = SJsonParse("32242gg", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJOSN_INVALID_VALUE, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_NULL, node->GetType());

    node = SJsonParse("+4321", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJOSN_INVALID_VALUE, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_NULL, node->GetType());

    node = SJsonParse(".", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJOSN_INVALID_VALUE, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_NULL, node->GetType());
}

static void test_parse_float() {
    validate_float_number("0.0", 0.0);
    validate_float_number("-0.0", -0.0);
    validate_float_number("1.0", 1.0);
    validate_float_number("-1.0", -1.0);
    validate_float_number("2147483647.0", 2147483647.0);
    validate_float_number("-2147483647.0", -2147483647.0);
    validate_float_number(".e14", 0.e14);
    validate_float_number("3.14e19", 3.14e19);
    validate_float_number("-3.14e19", -3.14e19);
    validate_float_number("3.14e-19", 3.14e-19);
    validate_float_number("-3.14e-19", -3.14e-19);
    validate_float_number("3.14e+19", 3.14e19);
    validate_float_number("1e-10000", 0.0);
}

static void test_parse_string() {
    SJsonErrorCode code;
    auto node = SJsonParse("\"Hello\"", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_STRING, node->GetType());
    EXPECT_EQ_STRING("Hello", ((std::string)*node).c_str());

    node = SJsonParse("\"Hello  \"", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_STRING, node->GetType());
    EXPECT_EQ_STRING("Hello  ", ((std::string)*node).c_str());

    node = SJsonParse("\"\"", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_STRING, node->GetType());
    EXPECT_EQ_STRING("", ((std::string)*node).c_str());

    node = SJsonParse("\" Hello\"", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_STRING, node->GetType());
    EXPECT_EQ_STRING(" Hello", ((std::string)*node).c_str());

    node = SJsonParse("\"Hello\n\"", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_STRING, node->GetType());
    EXPECT_EQ_STRING("Hello\n", ((std::string)*node).c_str());

    node = SJsonParse("\"Hello\\\\\"", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_STRING, node->GetType());
    EXPECT_EQ_STRING("Hello\\", ((std::string)*node).c_str());

    node = SJsonParse("\"Hello\\q\"", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJOSN_INVALID_VALUE, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_NULL, node->GetType());

    node = SJsonParse("\"Hello\\ \"", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJOSN_INVALID_VALUE, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_NULL, node->GetType());
}


static void test_parse_array() {
    SJsonErrorCode code;
    auto node = SJsonParse(" [ 1, 2, 3 ]", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_ARRAY, node->GetType());

    std::vector<ll> test1 = { 1, 2, 3 };
    int id = 0;
    for (auto it = node->begin(); it != node->end(); it++) {
        EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_INT, (*it)->GetType());
        EXPECT_EQ_LINT(test1[id], (ll)*(*it));
        id++;
    }

    node = SJsonParse("[1, 2, [1, 2]]", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_ARRAY, node->GetType());

    node = SJsonParse("[null, true, false, 123, \"abc\"]", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_ARRAY, node->GetType());

    node = SJsonParse("[[[]]]", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_ARRAY, node->GetType());

    node = SJsonParse("[ [], [1, 2, 3] ]", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_ARRAY, node->GetType());
}

static void test_parse_object() {
    SJsonErrorCode code;
    auto node = SJsonParse(R"({"Name": "Test", "Age": 114514})", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_OBJECT, node->GetType());

    auto age = (*node)["Age"];
    EXPECT_EQ_LINT(114514L, (ll)*age);

    auto name = (*node)["Name"];
    EXPECT_EQ_STRING("Test", ((std::string)*name).c_str());

    node = SJsonParse(R"({ "Obj": {}, "Arr": [1, 2, 3], "": 12})", code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_OBJECT, node->GetType());

    auto obj = (*node)[std::string("Obj")];
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_OBJECT, obj->GetType());

    auto arr = (*node)[std::string("Arr")];
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_ARRAY, arr->GetType());

}

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


static void test_parse_json() {
    SJsonErrorCode code;
    auto node = SJsonParse(JSON, code);
    EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_OBJECT, node->GetType());
}


static void test_parse() {
    test_parse_null();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
    test_parse_true();
    test_parse_false();
    test_parse_int();
    test_parse_float();
    test_parse_string();
    test_parse_array();
    test_parse_object();
    test_parse_json();
}

int main() {
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}