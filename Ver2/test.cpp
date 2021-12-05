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

std::shared_ptr<SJson::SJsonNode> expect_parse_nothrow(const std::string& text, const char* file, int line) {
    test_count++;
    std::shared_ptr<SJson::SJsonNode> ptr;
    try {
        ptr = SJson::SJsonParse(text);
    }
    catch (std::exception e) {
        fprintf(stderr, "%s:%d: unexpected error thrown: %s\n", file, line, e.what());
        return nullptr;
    }
    test_pass++;
    return ptr;
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

template<typename T>
void expect_parse_value(const std::string& text, const char* file, int line, T value) {
    static_assert(false, "This data type is unsupported");
}

template<>
void expect_parse_value(const std::string& text, const char* file, int line, ll value) {
    test_count++;
    try {
        auto node = SJson::SJsonParse(text);
        if (node->GetInt() == value) {
            test_pass++;
            return;
        }
        else {
            fprintf(stderr, "%s:%d: Value mismatch, expected %lld, actual %lld\n", file, line, value, node->GetInt());
        }
    }
    catch (std::exception e) {
        fprintf(stderr, "%s:%d: unexpected error thrown: %s\n", file, line, e.what());
    }
}

template<>
void expect_parse_value(const std::string& text, const char* file, int line, std::string value) {
    test_count++;
    try {
        auto node = SJson::SJsonParse(text);
        if (node->GetString() == value) {
            test_pass++;
            return;
        }
        else {
            fprintf(stderr, "%s:%d: Value mismatch, expected \"%s\", actual \"%s\"\n",
                file, line, value.c_str(), node->GetString().c_str());
        }
    }
    catch (std::exception e) {
        fprintf(stderr, "%s:%d: unexpected error thrown: %s\n", file, line, e.what());
    }
}

template<>
void expect_parse_value(const std::string& text, const char* file, int line, double value) {
    test_count++;
    try {
        auto node = SJson::SJsonParse(text);
        if (node->GetFloat() == value) {
            test_pass++;
            return;
        }
        else {
            fprintf(stderr, "%s:%d: Value mismatch, expected %lf, actual %lf\n", file, line, value, node->GetFloat());
        }
    }
    catch (std::exception e) {
        fprintf(stderr, "%s:%d: unexpected error thrown: %s\n", file, line, e.what());
    }
}

template<>
void expect_parse_value(const std::string& text, const char* file, int line, bool value) {
    test_count++;
    try {
        auto node = SJson::SJsonParse(text);
        if (node->GetBool() == value) {
            test_pass++;
            return;
        }
        else {
            fprintf(stderr, "%s:%d: Value mismatch, expected %s, actual %s\n", file, line,
                value ? "true" : "false", node->GetBool() ? "true" : "false");
        }
    }
    catch (std::exception e) {
        fprintf(stderr, "%s:%d: unexpected error thrown: %s\n", file, line, e.what());
    }
}

template<typename T>
void expect_equal(const SJson::SJsonNode* node, T value, const char* file, int line) {
    static_assert(false, "This data type is unsupported");
    test_count++;
    try {
        if (node->GetBool() == value) {
            test_pass++;
            return;
        }
        else {
            fprintf(stderr, "%s:%d: Value mismatch, expected %s, actual %s\n", file, line,
                value ? "true" : "false", node->GetBool() ? "true" : "false");
        }
    }
    catch (std::exception e) {
        fprintf(stderr, "%s:%d: unexpected error thrown: %s\n", file, line, e.what());
    }
}


template<>
void expect_equal(const SJson::SJsonNode* node, bool value, const char* file, int line) {
    test_count++;
    try {
        if (node->GetBool() == value) {
            test_pass++;
            return;
        }
        else {
            fprintf(stderr, "%s:%d: Value mismatch, expected %s, actual %s\n", file, line,
                value ? "true" : "false", node->GetBool() ? "true" : "false");
        }
    }
    catch (std::exception e) {
        fprintf(stderr, "%s:%d: unexpected error thrown: %s\n", file, line, e.what());
    }
}


template<>
void expect_equal(const SJson::SJsonNode* node, ll value, const char* file, int line) {
    test_count++;
    try {
        if (node->GetInt() == value) {
            test_pass++;
            return;
        }
        else {
            fprintf(stderr, "%s:%d: Value mismatch, expected %lld, actual %lld\n", file, line, value, node->GetInt());
        }
    }
    catch (std::exception e) {
        fprintf(stderr, "%s:%d: unexpected error thrown: %s\n", file, line, e.what());
    }
}

template<>
void expect_equal(const SJson::SJsonNode* node, double value, const char* file, int line) {
    test_count++;
    try {
        if (node->GetFloat() == value) {
            test_pass++;
            return;
        }
        else {
            fprintf(stderr, "%s:%d: Value mismatch, expected %lf, actual %lf\n", file, line, value, node->GetFloat());
        }
    }
    catch (std::exception e) {
        fprintf(stderr, "%s:%d: unexpected error thrown: %s\n", file, line, e.what());
    }
}

template<>
void expect_equal(const SJson::SJsonNode* node, std::string value, const char* file, int line) {
    test_count++;
    try {
        if (node->GetString() == value) {
            test_pass++;
            return;
        }
        else {
            fprintf(stderr, "%s:%d: Value mismatch, expected \"%s\", actual \"%s\"\n",
                file, line, value.c_str(), node->GetString().c_str());
        }
    }
    catch (std::exception e) {
        fprintf(stderr, "%s:%d: unexpected error thrown: %s\n", file, line, e.what());
    }
}

#define EXPECT_PARSE_THROW(text, error) expect_parse_throw_error<error>(text, __FILE__, __LINE__, #error)
#define EXPECT_PARSE_NOTHROW(text) expect_parse_nothrow(text, __FILE__, __LINE__)
#define EXPECT_PARSE_TYPE(text, type) expect_parse_type(text, __FILE__, __LINE__, type)
#define EXPECT_PARSE_BOOL_VALUE(text, value) expect_parse_value<bool>(text, __FILE__, __LINE__, value)
#define EXPECT_PARSE_INT_VALUE(text, value) expect_parse_value<ll>(text, __FILE__, __LINE__, value)
#define EXPECT_PARSE_FLOAT_VALUE(text, value) expect_parse_value<double>(text, __FILE__, __LINE__, value)
#define EXPECT_PARSE_STRING_VALUE(text, value) expect_parse_value<std::string>(text, __FILE__, __LINE__, value)
#define EXPECT_EQ_INT(node, value) expect_equal<ll>(node, value, __FILE__, __LINE__)
#define EXPECT_EQ_FLOAT(node, value) expect_equal<double>(node, value, __FILE__, __LINE__)
#define EXPECT_EQ_BOOL(node, value) expect_equal<bool>(node, value, __FILE__, __LINE__)
#define EXPECT_EQ_STRING(node, value) expect_equal<std::string>(node, value, __FILE__, __LINE__)
#define EXPECT_NODE_TYPE(node, type) \
    test_count++; \
    if (node->GetType() == type) { \
        test_pass++;\
    }\
    else{\
        fprintf(stderr, "%s:%d: Type mismatch, expected %s, actual %s\n", __FILE__, __LINE__, \
            SJson::SJsonGetNodeTypeName(type).c_str(), SJson::SJsonGetNodeTypeName(node->GetType()).c_str());\
    }

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

static void test_parse_true() {
    EXPECT_PARSE_BOOL_VALUE("true", true);
    EXPECT_PARSE_BOOL_VALUE(" true", true);
    EXPECT_PARSE_BOOL_VALUE(" true ", true);
}

static void test_parse_false() {
    EXPECT_PARSE_BOOL_VALUE("false", false);
    EXPECT_PARSE_BOOL_VALUE(" false", false);
    EXPECT_PARSE_BOOL_VALUE(" false  ", false);
}
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
static void test_parse_int() {
    EXPECT_PARSE_INT_VALUE("0", 0LL);
    EXPECT_PARSE_INT_VALUE("-0", 0LL);
    EXPECT_PARSE_INT_VALUE("1", 1LL);
    EXPECT_PARSE_INT_VALUE("-1", -1LL);
    EXPECT_PARSE_INT_VALUE("2147483647", 2147483647LL);
    EXPECT_PARSE_INT_VALUE("-2147483647", -2147483647LL);
    EXPECT_PARSE_INT_VALUE("-2147483648", -2147483648LL);

    EXPECT_PARSE_THROW("007", SJson::InvalidValueError);
    EXPECT_PARSE_THROW("32242gg", SJson::InvalidValueError);
    EXPECT_PARSE_THROW("+4321", SJson::InvalidValueError);
    EXPECT_PARSE_THROW(".", SJson::InvalidValueError);
}

static void test_parse_float() {
    EXPECT_PARSE_FLOAT_VALUE("0.0", 0.0);
    EXPECT_PARSE_FLOAT_VALUE("-0.0", -0.0);
    EXPECT_PARSE_FLOAT_VALUE("1.0", 1.0);
    EXPECT_PARSE_FLOAT_VALUE("-1.0", -1.0);
    EXPECT_PARSE_FLOAT_VALUE("2147483647.0", 2147483647.0);
    EXPECT_PARSE_FLOAT_VALUE("-2147483647.0", -2147483647.0);
    EXPECT_PARSE_FLOAT_VALUE(".e14", 0.e14);
    EXPECT_PARSE_FLOAT_VALUE("3.14e19", 3.14e19);
    EXPECT_PARSE_FLOAT_VALUE("-3.14e19", -3.14e19);
    EXPECT_PARSE_FLOAT_VALUE("3.14e-19", 3.14e-19);
    EXPECT_PARSE_FLOAT_VALUE("-3.14e-19", -3.14e-19);
    EXPECT_PARSE_FLOAT_VALUE("3.14e+19", 3.14e19);
    EXPECT_PARSE_FLOAT_VALUE("1e-10000", 0.0);

    EXPECT_PARSE_THROW("7e1232.0", SJson::InvalidValueError);
    EXPECT_PARSE_THROW("7f123", SJson::InvalidValueError);
    EXPECT_PARSE_THROW("2.0e1e8", SJson::InvalidValueError);
    EXPECT_PARSE_THROW("..", SJson::InvalidValueError);
    EXPECT_PARSE_THROW("7.0f", SJson::InvalidValueError);
}

static void test_parse_string() {
    EXPECT_PARSE_STRING_VALUE(R"("Hello")", "Hello");
    EXPECT_PARSE_STRING_VALUE(R"(" Hello ")", " Hello ");
    EXPECT_PARSE_STRING_VALUE(R"("")", "");
    EXPECT_PARSE_STRING_VALUE(R"(" ")", " ");
    EXPECT_PARSE_STRING_VALUE(R"("He ll o")", "He ll o");
    EXPECT_PARSE_STRING_VALUE(R"("Hello\n")", "Hello\n");
    EXPECT_PARSE_STRING_VALUE(R"("\n")", "\n");

    EXPECT_PARSE_THROW(R"(")", SJson::ExpectTokenError);
    EXPECT_PARSE_THROW(R"("123142)", SJson::ExpectTokenError);
    EXPECT_PARSE_THROW(R"('23142')", SJson::InvalidValueError);
    EXPECT_PARSE_THROW(R"("\q")", SJson::UnexpectedTokenError);
    EXPECT_PARSE_THROW(R"("\ ")", SJson::UnexpectedTokenError);
}


static void test_parse_array() {
    auto node = SJson::SJsonParse(" [1, 2, 3] ");
    EXPECT_EQ_INT(node->ElementAt(0), 1LL);
    EXPECT_EQ_INT(node->ElementAt(1), 2LL);
    EXPECT_EQ_INT(node->ElementAt(2), 3LL);

    node = SJson::SJsonParse(" [ 1, 2, [1, 2] ] ");
    EXPECT_EQ_INT(node->ElementAt(0), 1LL);
    EXPECT_EQ_INT(node->ElementAt(1), 2LL);
    EXPECT_NODE_TYPE(node->ElementAt(2), SJson::SJsonNodeType::JSON_ARRAY);


    node = EXPECT_PARSE_NOTHROW("[null, true, false, 123, \"abc\"]");
    EXPECT_NODE_TYPE(node->ElementAt(0), SJson::SJsonNodeType::JSON_NULL);
    EXPECT_NODE_TYPE(node->ElementAt(1), SJson::SJsonNodeType::JSON_BOOL);
    EXPECT_NODE_TYPE(node->ElementAt(2), SJson::SJsonNodeType::JSON_BOOL);
    EXPECT_NODE_TYPE(node->ElementAt(3), SJson::SJsonNodeType::JSON_INT);
    EXPECT_NODE_TYPE(node->ElementAt(4), SJson::SJsonNodeType::JSON_STRING);

    EXPECT_PARSE_NOTHROW("[[[]]]");
    EXPECT_PARSE_NOTHROW("[ [], [1, 2, 3] ]");
    EXPECT_PARSE_NOTHROW("[ {}, [1, 2, 3] ]");

}

static void test_parse_object() {
    auto node = EXPECT_PARSE_NOTHROW(R"({"Name": "Test", "Age": 114514})");
    EXPECT_NODE_TYPE(node, SJson::SJsonNodeType::JSON_OBJECT);
    EXPECT_EQ_STRING(node->GetMember("Name"), "Test");
    EXPECT_EQ_INT(node->GetMember("Age"), 114514LL);

    node = EXPECT_PARSE_NOTHROW(R"({ "Obj": {}, "Arr": [1, 2, 3], "": 12})");
    EXPECT_NODE_TYPE(node, SJson::SJsonNodeType::JSON_OBJECT);
    EXPECT_NODE_TYPE(node->GetMember("Obj"), SJson::SJsonNodeType::JSON_OBJECT);
    EXPECT_NODE_TYPE(node->GetMember("Arr"), SJson::SJsonNodeType::JSON_ARRAY);
    EXPECT_NODE_TYPE(node->GetMember(""), SJson::SJsonNodeType::JSON_INT);

    //SJsonErrorCode code;
    //auto node = SJsonParse(R"({"Name": "Test", "Age": 114514})", code);
    //EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    //EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_OBJECT, node->GetType());

    //auto age = (*node)["Age"];
    //EXPECT_EQ_LINT(114514L, age->GetInt());

    //auto name = (*node)["Name"];
    //EXPECT_EQ_STRING("Test", name->GetString().c_str());

    //node = SJsonParse(R"({ "Obj": {}, "Arr": [1, 2, 3], "": 12})", code);
    //EXPECT_EQ_ENUM_ERR(SJsonErrorCode::SJSON_OK, code);
    //EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_OBJECT, node->GetType());

    //auto obj = (*node)[std::string("Obj")];
    //EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_OBJECT, obj->GetType());

    //auto arr = (*node)[std::string("Arr")];
    //EXPECT_EQ_ENUM_NODE(SJsonNodeType::JSON_ARRAY, arr->GetType());

}

static char* JSON = R"(
{
    "Camera": {
        "Type": "Normal",
        "Position": [
            0,
            0,
            -5
        ],
        "FOV": 90,
        "LookAt": [
            0,
            0,
            0
        ],
        "Up": [
            0,
            1,
            0
        ]
    },
    "Objects": [
        {
            "Type": "Geometry",
            "Shape": {
                "Type": "Sphere",
                "Position": [
                    -1.5,
                    0,
                    0
                ],
                "Radius": 1.0
            }
        },
        {
            "Type": "Geometry",
            "Shape": {
                "Type": "Sphere",
                "Position": [
                    1.5,
                    0,
                    0
                ],
                "Radius": 1.0
            }
        },
        {
            "Type": "Geometry",
            "Shape": {
                "Type": "TriangleMesh",
                "ObjFile": "../../../Models/Floor.obj",
                "Translation": [
                    0,
                    -1,
                    0
                ],
                "Scale": [
                    3,
                    1,
                    3
                ],
                "Rotation": [
                    0,
                    0,
                    0
                ]
            }
        }
    ]
}
)";


static void test_parse_json() {
    EXPECT_PARSE_NOTHROW(JSON);
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