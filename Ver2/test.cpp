#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "SJson.hpp"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

template<typename T>
void expect_equal_vector(const std::vector<T>& expected, const std::vector<T>& actual, const char* file, int line)
{
    test_count++;
    try
    {
        if (expected == actual)
        {
            test_pass++;
            return;
        }
        else
        {
            fprintf(stderr, "%s:%d: Value mismatch, expected \"%s\", actual \"%s\"\n",
                file, line, SJson::JsonConvert::Serialize(expected, SJson::DefaultOption).c_str(), 
                SJson::JsonConvert::Serialize(actual, SJson::DefaultOption).c_str());
        }
    }
    catch (std::exception& e)
    {
        fprintf(stderr, "%s:%d: unexpected error thrown: %s\n", file, line, e.what());
    }
}

template<typename T>
void expect_parse_throw_error(const std::string& text, const char* file, int line, const char* name)
{
    test_count++;
    try
    {
        SJson::JsonConvert::Parse(text);
    }
    catch (T& e)
    {
        fprintf(stdout, "Passed Throw: %s\n", e.what());
        test_pass++;
        return;
    }
    catch (std::exception& e)
    {
        fprintf(stderr, "%s:%d: expect throw: %s, actual: %s\n", file, line, name, typeid(e).name());
        return;
    }
    fprintf(stderr, "%s:%d: expect to throw %s, but none was thrown\n", file, line, name);
}


template<typename T, typename E>
void expect_deserialize_throw_error(const std::string& text, const char* file, int line, const char* name)
{
    test_count++;

    try
    {
        SJson::JsonConvert::Deserialize<T>(text);
    }
    catch (E& e)
    {
        fprintf(stdout, "Passed Throw: %s\n", e.what());
        test_pass++;
        return;
    }
    catch (std::exception& e)
    {
        fprintf(stderr, "%s:%d: expect throw: %s, actual: %s\n", file, line, name, typeid(e).name());
        return;
    }
    fprintf(stderr, "%s:%d: expect to throw %s, but none was thrown\n", file, line, name);
}


SJson::JsonNode expect_parse_nothrow(const std::string& text, const char* file, int line)
{
    test_count++;
    SJson::JsonNode node;
    try
    {
        node = SJson::JsonConvert::Parse(text);
    }
    catch (std::exception& e)
    {
        fprintf(stderr, "%s:%d: unexpected error was thrown: %s\n", file, line, e.what());
        return nullptr;
    }
    test_pass++;
    return node;
}

void expect_parse_type(const std::string& text, const char* file, int line, SJson::ValueType type)
{
    test_count++;
    try
    {
        auto node = SJson::JsonConvert::Parse(text);
        if (node.GetType() == type)
        {
            test_pass++;
            return;
        }
        else
        {
            fprintf(stderr, "%s:%d: type mismatch, expected %s, actual %s\n", file, line,
                SJson::GetValueTypeName(type).c_str(),
                SJson::GetValueTypeName(node.GetType()).c_str());
        }
    }
    catch (std::exception& e)
    {
        fprintf(stderr, "%s:%d: unexpected error was thrown: %s\n", file, line, e.what());
    }
}

template<typename T>
void expect_parse_value(const std::string& text, const char* file, int line, T expected)
{
    static_assert(false, "This data type is unsupported");
}

template<>
void expect_parse_value(const std::string& text, const char* file, int line, int64_t expected)
{
    test_count++;
    try
    {
        auto node = SJson::JsonConvert::Parse(text);
        int64_t value = node.Get<int64_t>();
        if (value == expected)
        {
            test_pass++;
            return;
        }
        else
        {
            fprintf(stderr, "%s:%d: Value mismatch, expected %lld, actual %lld\n", file, line, expected, value);
        }
    }
    catch (std::exception& e)
    {
        fprintf(stderr, "%s:%d: unexpected error was thrown: %s\n", file, line, e.what());
    }
}

template<>
void expect_parse_value(const std::string& text, const char* file, int line, std::string expect)
{
    test_count++;
    try
    {
        auto node = SJson::JsonConvert::Parse(text);
        auto value = node.Get<std::string>();
        if (value == expect)
        {
            test_pass++;
            return;
        }
        else
        {
            fprintf(stderr, "%s:%d: Value mismatch, expected \"%s\", actual \"%s\"\n",
                file, line, expect.c_str(), value.c_str());
        }
    }
    catch (std::exception& e)
    {
        fprintf(stderr, "%s:%d: unexpected error thrown: %s\n", file, line, e.what());
    }
}

template<>
void expect_parse_value(const std::string& text, const char* file, int line, double expected)
{
    test_count++;
    try
    {
        auto node = SJson::JsonConvert::Parse(text);
        auto value = node.Get<double>();
        if (std::abs(value - expected) < 1e-6)
        {
            test_pass++;
            return;
        }
        else
        {
            fprintf(stderr, "%s:%d: Value mismatch, expected %lf, actual %lf\n", file, line, expected, value);
        }
    }
    catch (std::exception& e)
    {
        fprintf(stderr, "%s:%d: unexpected error thrown: %s\n", file, line, e.what());
    }
}

template<>
void expect_parse_value(const std::string& text, const char* file, int line, bool expected)
{
    test_count++;
    try
    {
        auto node = SJson::JsonConvert::Parse(text);
        auto value = node.Get<bool>();
        if (value == expected)
        {
            test_pass++;
            return;
        }
        else
        {
            fprintf(stderr, "%s:%d: Value mismatch, expected %s, actual %s\n", file, line,
                expected ? "true" : "false", value ? "true" : "false");
        }
    }
    catch (std::exception& e)
    {
        fprintf(stderr, "%s:%d: unexpected error thrown: %s\n", file, line, e.what());
    }
}

template<typename T>
void expect_equal(const SJson::JsonNode& node, T value, const char* file, int line)
{
    static_assert(false, "This data type is unsupported");
}

template<>
void expect_equal(const SJson::JsonNode& node, bool expected, const char* file, int line)
{
    test_count++;
    try
    {
        auto value = node.Get<bool>();
        if (expected == value)
        {
            test_pass++;
            return;
        }
        else
        {
            fprintf(stderr, "%s:%d: Value mismatch, expected %s, actual %s\n", file, line,
                expected ? "true" : "false", value ? "true" : "false");
        }
    }
    catch (std::exception& e)
    {
        fprintf(stderr, "%s:%d: unexpected error thrown: %s\n", file, line, e.what());
    }
}


template<>
void expect_equal(const SJson::JsonNode& node, int64_t expected, const char* file, int line)
{
    test_count++;
    try
    {
        auto value = node.Get<int64_t>();
        if (value == expected)
        {
            test_pass++;
            return;
        }
        else
        {
            fprintf(stderr, "%s:%d: Value mismatch, expected %lld, actual %lld\n", file, line, expected, value);
        }
    }
    catch (std::exception& e)
    {
        fprintf(stderr, "%s:%d: unexpected error thrown: %s\n", file, line, e.what());
    }
}

template<>
void expect_equal(const SJson::JsonNode& node, double expected, const char* file, int line)
{
    test_count++;
    try
    {
        auto value = node.Get<double>();
        if (std::abs(value - expected) < 1e-6)
        {
            test_pass++;
            return;
        }
        else
        {
            fprintf(stderr, "%s:%d: Value mismatch, expected %.9lf, actual %.9lf\n", file, line, expected, value);
        }
    }
    catch (std::exception& e)
    {
        fprintf(stderr, "%s:%d: unexpected error thrown: %s\n", file, line, e.what());
    }
}

template<>
void expect_equal(const SJson::JsonNode& node, std::string expected, const char* file, int line)
{
    test_count++;
    try
    {
        auto value = node.Get<std::string>();
        if (value == expected)
        {
            test_pass++;
            return;
        }
        else
        {
            fprintf(stderr, "%s:%d: Value mismatch, expected \"%s\", actual \"%s\"\n",
                file, line, expected.c_str(), value.c_str());
        }
    }
    catch (std::exception& e)
    {
        fprintf(stderr, "%s:%d: unexpected error thrown: %s\n", file, line, e.what());
    }
}

#define EXPECT_PARSE_THROW(text, error) expect_parse_throw_error<error>(text, __FILE__, __LINE__, #error)
#define EXPECT_PARSE_NOTHROW(text) expect_parse_nothrow(text, __FILE__, __LINE__)
#define EXPECT_PARSE_TYPE(text, type) expect_parse_type(text, __FILE__, __LINE__, type)
#define EXPECT_PARSE_BOOL_VALUE(text, value) expect_parse_value<bool>(text, __FILE__, __LINE__, value)
#define EXPECT_PARSE_INT_VALUE(text, value) expect_parse_value<int64_t>(text, __FILE__, __LINE__, value)
#define EXPECT_PARSE_FLOAT_VALUE(text, value) expect_parse_value<double>(text, __FILE__, __LINE__, value)
#define EXPECT_PARSE_STRING_VALUE(text, value) expect_parse_value<std::string>(text, __FILE__, __LINE__, value)
#define EXPECT_EQ_INT(node, value) expect_equal<int64_t>(node, value, __FILE__, __LINE__)
#define EXPECT_EQ_FLOAT(node, value) expect_equal<double>(node, value, __FILE__, __LINE__)
#define EXPECT_EQ_BOOL(node, value) expect_equal<bool>(node, value, __FILE__, __LINE__)
#define EXPECT_EQ_STRING(node, value) expect_equal<std::string>(node, value, __FILE__, __LINE__)
#define EXPECT_EQ_VECTOR(actual, expected) expect_equal_vector((expected), actual, __FILE__, __LINE__)
#define EXPECT_NODE_TYPE(node, type) \
    test_count++; \
    if (node.GetType() == type) { \
        test_pass++;\
    }\
    else{\
        fprintf(stderr, "%s:%d: Type mismatch, expected %s, actual %s\n", __FILE__, __LINE__, \
            SJson::GetValueTypeName(type).c_str(), SJson::GetValueTypeName(node.GetType()).c_str());\
    }
#define EXPECT_DESERIALIZE_THROW(type, text, error) expect_deserialize_throw_error<type, error>(text, __FILE__, __LINE__, #error)
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
static void test_parse_null()
{
    EXPECT_PARSE_NOTHROW("null");
    EXPECT_PARSE_TYPE("null", SJson::ValueType::Null);
    EXPECT_PARSE_THROW("nul", SJson::lexical_error);
}

static void test_parse_expect_value()
{
    EXPECT_PARSE_THROW("", SJson::invalid_eof);
    EXPECT_PARSE_THROW(" ", SJson::invalid_eof);
    EXPECT_PARSE_THROW("\t", SJson::invalid_eof);
    EXPECT_PARSE_THROW("\n", SJson::invalid_eof);
    EXPECT_PARSE_THROW("{", SJson::invalid_eof);
    EXPECT_PARSE_THROW("[", SJson::invalid_eof);
}

static void test_parse_invalid_value()
{
    EXPECT_PARSE_THROW(",", SJson::parse_match_failed);
    EXPECT_PARSE_THROW("u", SJson::lexical_error);
    EXPECT_PARSE_THROW("}", SJson::parse_match_failed);
    EXPECT_PARSE_THROW("\b", SJson::lexical_error);
    EXPECT_PARSE_THROW("*", SJson::lexical_error);
    EXPECT_PARSE_THROW("?", SJson::lexical_error);
}

static void test_parse_root_not_singular()
{
    EXPECT_PARSE_THROW("null true", SJson::root_not_singular_error);
    EXPECT_PARSE_THROW("null,null", SJson::root_not_singular_error);
    EXPECT_PARSE_THROW("null,", SJson::root_not_singular_error);
    EXPECT_PARSE_THROW("1,2,3,4,5", SJson::root_not_singular_error);
    EXPECT_PARSE_THROW("{}[]", SJson::root_not_singular_error);
    EXPECT_PARSE_THROW("{},", SJson::root_not_singular_error);
    EXPECT_PARSE_THROW("[1, 2, 3[]]", SJson::expect_token_error);
    EXPECT_PARSE_THROW("[1, 2, 3", SJson::expect_token_error);

    EXPECT_PARSE_NOTHROW(R"( {"A":[]} )");
    EXPECT_PARSE_NOTHROW(R"( [[[]]] )");
    EXPECT_PARSE_NOTHROW(R"( [1, 2, 3, [], []] )");
}

static void test_parse_true()
{
    EXPECT_PARSE_BOOL_VALUE("true", true);
    EXPECT_PARSE_BOOL_VALUE(" true", true);
    EXPECT_PARSE_BOOL_VALUE(" true ", true);
}

static void test_parse_false()
{
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

static void test_parse_int()
{
    EXPECT_PARSE_INT_VALUE("0", 0LL);
    EXPECT_PARSE_INT_VALUE("-0", 0LL);
    EXPECT_PARSE_INT_VALUE("1", 1LL);
    EXPECT_PARSE_INT_VALUE("-1", -1LL);
    EXPECT_PARSE_INT_VALUE("007", 7LL);
    EXPECT_PARSE_INT_VALUE("+110", 110LL);
    EXPECT_PARSE_INT_VALUE("2147483647", 2147483647LL);
    EXPECT_PARSE_INT_VALUE("-2147483647", -2147483647LL);
    EXPECT_PARSE_INT_VALUE("-2147483648", -2147483648LL);
    EXPECT_PARSE_INT_VALUE("1145141919810132", 1145141919810132LL);
    EXPECT_PARSE_INT_VALUE("1145141919810132", 1145141919810132LL);

    EXPECT_PARSE_THROW("32242gg", SJson::lexical_error);
    EXPECT_PARSE_THROW(".", SJson::lexical_error);
}

static void test_parse_float()
{
    EXPECT_PARSE_FLOAT_VALUE("0.0", 0.0);
    EXPECT_PARSE_FLOAT_VALUE("-0.0", -0.0);
    EXPECT_PARSE_FLOAT_VALUE("1.0", 1.0);
    EXPECT_PARSE_FLOAT_VALUE("-1.0", -1.0);
    EXPECT_PARSE_FLOAT_VALUE("2147483647.0", 2147483647.0);
    EXPECT_PARSE_FLOAT_VALUE("-2147483647.0", -2147483647.0);
    EXPECT_PARSE_FLOAT_VALUE("0.e14", 0.e14);
    EXPECT_PARSE_FLOAT_VALUE("3.14e19", 3.14e19);
    EXPECT_PARSE_FLOAT_VALUE("-3.14e19", -3.14e19);
    EXPECT_PARSE_FLOAT_VALUE("3.14e-19", 3.14e-19);
    EXPECT_PARSE_FLOAT_VALUE("-3.14e-19", -3.14e-19);
    EXPECT_PARSE_FLOAT_VALUE("3.14e+19", 3.14e19);

    EXPECT_PARSE_THROW("1e-10000", std::out_of_range);
    EXPECT_PARSE_THROW("7e1232", std::out_of_range);
    EXPECT_PARSE_THROW("7f123", SJson::lexical_error);
    EXPECT_PARSE_THROW("2.0e1e8", SJson::lexical_error);
    EXPECT_PARSE_THROW("..", SJson::lexical_error);
    EXPECT_PARSE_THROW("7.0f", SJson::lexical_error);
}

static void test_parse_string()
{
    EXPECT_PARSE_STRING_VALUE(R"("Hello")", "Hello");
    EXPECT_PARSE_STRING_VALUE(R"(" Hello ")", " Hello ");
    EXPECT_PARSE_STRING_VALUE(R"("")", "");
    EXPECT_PARSE_STRING_VALUE(R"(" ")", " ");
    EXPECT_PARSE_STRING_VALUE(R"("He ll o")", "He ll o");
    EXPECT_PARSE_STRING_VALUE(R"("Hello\n")", "Hello\n");
    EXPECT_PARSE_STRING_VALUE(R"("\n")", "\n");

    EXPECT_PARSE_THROW(R"(")", SJson::lexical_error);
    EXPECT_PARSE_THROW(R"("123142)", SJson::lexical_error);
    EXPECT_PARSE_THROW(R"('23142')", SJson::lexical_error);
    EXPECT_PARSE_THROW(R"("\q")", SJson::invalid_escape_char);
    EXPECT_PARSE_THROW(R"("\ ")", SJson::invalid_escape_char);
}


static void test_parse_array()
{
    auto node = SJson::JsonConvert::Parse(" [1, 2, 3] ");
    EXPECT_EQ_INT(node[0], 1LL);
    EXPECT_EQ_INT(node[1], 2LL);
    EXPECT_EQ_INT(node[2], 3LL);

    node = SJson::JsonConvert::Parse(" [1, 2, 3] # array");
    EXPECT_EQ_INT(node[0], 1LL);
    EXPECT_EQ_INT(node[1], 2LL);
    EXPECT_EQ_INT(node[2], 3LL);

    node = SJson::JsonConvert::Parse(" [ 1, 2, [1, 2] ] ");
    EXPECT_EQ_INT(node[0], 1LL);
    EXPECT_EQ_INT(node[1], 2LL);
    EXPECT_NODE_TYPE(node[2], SJson::ValueType::Array);


    node = EXPECT_PARSE_NOTHROW("[null, true, false, 123, \"abc\"]");
    EXPECT_NODE_TYPE(node[0], SJson::ValueType::Null);
    EXPECT_NODE_TYPE(node[1], SJson::ValueType::Boolean);
    EXPECT_NODE_TYPE(node[2], SJson::ValueType::Boolean);
    EXPECT_NODE_TYPE(node[3], SJson::ValueType::Integer);
    EXPECT_NODE_TYPE(node[4], SJson::ValueType::String);

    EXPECT_PARSE_NOTHROW("[[[]]]");
    EXPECT_PARSE_NOTHROW("[ [], [1, 2, 3] ]");
    EXPECT_PARSE_NOTHROW("[ {}, [1, 2, 3] ]");

}

static void test_parse_object()
{
    auto node = EXPECT_PARSE_NOTHROW(R"({"Name": "Test", "Age": 114514})");
    EXPECT_NODE_TYPE(node, SJson::ValueType::Object);
    EXPECT_EQ_STRING(node["Name"], "Test");
    EXPECT_EQ_INT(node["Age"], 114514LL);

    node = EXPECT_PARSE_NOTHROW(R"({ "Obj": {}, "Arr": [1, 2, 3], "": 12})");
    EXPECT_NODE_TYPE(node, SJson::ValueType::Object);
    EXPECT_NODE_TYPE(node["Obj"], SJson::ValueType::Object);
    EXPECT_NODE_TYPE(node["Arr"], SJson::ValueType::Array);
    EXPECT_NODE_TYPE(node[""], SJson::ValueType::Integer);

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

static const char* JSON = R"(
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


static void test_parse_json()
{
    EXPECT_PARSE_NOTHROW(JSON);
}

static void test_to_string()
{
    SJson::JsonNode node = SJson::object({
        {"Name", "DXTsT"},
        {"Age", 21},
        {"Company", "Microsoft"},
        {"Object", {1, 2, 3}},
        {"Test", SJson::array({1, 2, "123", false})},
        });
    EXPECT_EQ_STRING(node.ToString(SJson::DefaultOption), 
        R"({Age: 21, Company: "Microsoft", Name: "DXTsT", Object: [1, 2, 3], Test: [1, 2, "123", false]})");
    EXPECT_EQ_STRING(node.ToString(SJson::InlineWithQuoteOption), 
        R"({"Age": 21, "Company": "Microsoft", "Name": "DXTsT", "Object": [1, 2, 3], "Test": [1, 2, "123", false]})");

    node = 10;
    EXPECT_EQ_STRING(node.ToString(SJson::DefaultOption), "10");

    node = true;
    EXPECT_EQ_STRING(node.ToString(SJson::DefaultOption), "true");

    node = SJson::JsonNode();
    EXPECT_EQ_STRING(node.ToString(SJson::DefaultOption), "null");

    node = 3.14159265357;
    EXPECT_EQ_STRING(node.ToString(SJson::DefaultOption), "3.1415926535700001");

    node = "hello world";
    EXPECT_EQ_STRING(node.ToString(SJson::DefaultOption), R"("hello world")");

    node = {1, 3, 5, 7, 9};
    EXPECT_EQ_STRING(node.ToString(SJson::DefaultOption), R"([1, 3, 5, 7, 9])");

    node = SJson::object({ {"first", 1}, {"second", 2} });
    EXPECT_EQ_STRING(node.ToString(SJson::DefaultOption), R"({first: 1, second: 2})");
}


enum class SType
{
    A,
    B,
    C
};

template<>
struct SRefl::EnumInfo<SType>
{
    SREFL_TYPEINFO_HEADER(SType);
    constexpr static auto _ENUMLIST()
    {
        return std::make_tuple(
            SREFL_ENUM_TERM(A),
            SREFL_ENUM_TERM(B),
            SREFL_ENUM_TERM(C)
        );
    }
#define LISTFUNC(F) F(A) F(B) F(C)
    GENERATE_ENUM_MAPPING(SType, LISTFUNC)
#undef LISTFUNC
};

//#define FIELDS(F) F(A) F(B) F(C)
//GENERATE_ENUM(SType, FIELDS)
//#undef KEYS

//template<>
//struct SJson::enum_mapper<SType>
//{
//    using ENUM_TYPE = SType;
//    static const char* const enum_to_string(ENUM_TYPE v)
//    {
//        switch (v)
//        {
//        case ENUM_TYPE::A: return "A";
//        case ENUM_TYPE::B: return "B";
//        case ENUM_TYPE::C: return "C";
//        default:
//            break;
//        }
//        throw std::invalid_argument("Invalid enum type");
//    }
//
//    static ENUM_TYPE string_to_enum(const std::string& str)
//    {
//        static std::map<std::string, ENUM_TYPE> stringmap = {
//            {"A", ENUM_TYPE::A},
//            {"B", ENUM_TYPE::B},
//            {"C", ENUM_TYPE::C},
//        };
//        std::map<std::string, ENUM_TYPE>::iterator it;
//        if ((it = stringmap.find(str)) != stringmap.end())
//        {
//            return it->second;
//        }
//        throw std::invalid_argument("Invalid enum type");
//    }
//};

class Internal
{
public:
    int A;
    float B;

    //PROPERTIES(Internal, PROPERTY(A), PROPERTY(B));

    bool operator==(const Internal& other) const
    {
        return A == other.A && B == other.B;
    }
};


template<>
struct SRefl::TypeInfo<Internal>
{
    SREFL_TYPEINFO_HEADER(Internal);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(A),
            SREFL_FIELD(B)
        );
    }
};

class TestParent2
{
public:
    int		ParentAge2;

    //PROPERTIES(TestParent2, PROPERTY(ParentAge2));

    bool operator==(const TestParent2& other) const
    {
        return ParentAge2 == other.ParentAge2;
    }
};

template<>
struct SRefl::TypeInfo<TestParent2>
{
    SREFL_TYPEINFO_HEADER(TestParent2);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(ParentAge2)
        );
    }
};

class TestParent
{
public:
    int		ParentAge;

    // PROPERTIES(TestParent, PROPERTY(ParentAge));

    bool operator==(const TestParent& other) const
    {
        return ParentAge == other.ParentAge;
    }
};

template<>
struct SRefl::TypeInfo<TestParent>
{
    SREFL_TYPEINFO_HEADER(TestParent);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(ParentAge)
        );
    }
};

class TestObject : public TestParent, public TestParent2
{
public:
    int										Age;
    float									Weight;
    bool									Male;
    std::string								Name;
    std::vector<int>						List;
    std::map<int, std::vector<std::string>> Mapp;
    Internal								InternalData;
    SType									EnumValue;

    bool operator==(const TestObject& other) const
    {
        return TestParent::operator==(other) && TestParent2::operator==(other) && Age == other.Age 
            && Weight == other.Weight && Male == other.Male
            && Name == other.Name && List == other.List && Mapp == other.Mapp
            && InternalData == other.InternalData && EnumValue == other.EnumValue;
    }

    //PROPERTIES(TestObject, 
    //    PROPERTY(Age), 
    //    PROPERTY(Weight), 
    //    PROPERTY(Male),
    //    PROPERTY(Name),
    //    PROPERTY(List),
    //    PROPERTY(Mapp),
    //    PROPERTY(InternalData),
    //    PROPERTY(EnumValue)
    //    );
    //constexpr static auto parents()
    //{
    //    return std::make_tuple(
    //        BASECLASS(TestParent),
    //        BASECLASS(TestParent2)
    //    );
    //}
};

template<>
struct SRefl::TypeInfo<TestObject>
{
    SREFL_TYPEINFO_HEADER(TestObject);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(Age),
            SREFL_FIELD(Weight),
            SREFL_FIELD(Male),
            SREFL_FIELD(Name),
            SREFL_FIELD(List),
            SREFL_FIELD(Mapp),
            SREFL_FIELD(InternalData),
            SREFL_FIELD(EnumValue)
        );
    }

    constexpr static auto _BASELIST()
    {
        return std::make_tuple(
            SREFL_BASECLASS(TestParent),
            SREFL_BASECLASS(TestParent2)
        );
    }
};



static void test_serialization()
{
    EXPECT_EQ_STRING(SJson::JsonConvert::Serialize(true, SJson::DefaultOption), "true");

    EXPECT_EQ_STRING(SJson::JsonConvert::Serialize(false, SJson::DefaultOption), "false");

    EXPECT_EQ_STRING(SJson::JsonConvert::Serialize(155541213, SJson::DefaultOption), "155541213");

    EXPECT_EQ_STRING(SJson::JsonConvert::Serialize(3.14159265358, SJson::DefaultOption), "3.1415926535800001");

    EXPECT_EQ_STRING(SJson::JsonConvert::Serialize("hello world", SJson::DefaultOption), R"("hello world")");

    std::vector<int> intArr = { 1, 4, 2, 8, 5, 7 };
    EXPECT_EQ_STRING(SJson::JsonConvert::Serialize(intArr, SJson::DefaultOption), R"([1, 4, 2, 8, 5, 7])");

    std::vector<std::string> strArr = { "a", "b", "c"};
    EXPECT_EQ_STRING(SJson::JsonConvert::Serialize(strArr, SJson::DefaultOption), R"(["a", "b", "c"])");

    std::tuple<int, float, std::string> tuple1 = { 114514, 3.14159, "test"};
    EXPECT_EQ_STRING(SJson::JsonConvert::Serialize(tuple1, SJson::DefaultOption), R"([114514, 3.1415901184082031, "test"])");

    std::map<int, double> map1 = { {1, 3.14}, {2, 6.28} };
    EXPECT_EQ_STRING(SJson::JsonConvert::Serialize(map1, SJson::DefaultOption), R"([{key: 1, value: 3.1400000000000001}, {key: 2, value: 6.2800000000000002}])");

    std::map<std::string, int> map2 = { {"A", 1}, {"B", 2} };
    EXPECT_EQ_STRING(SJson::JsonConvert::Serialize(map2, SJson::DefaultOption), R"([{key: "A", value: 1}, {key: "B", value: 2}])");

    SType enumVal = SType::A;
    EXPECT_EQ_STRING(SJson::JsonConvert::Serialize(enumVal, SJson::DefaultOption),R"("A")");

    TestObject test = {};
    test.Age = 21;
    test.List = { 1, 2, 3 };
    test.Mapp = { {1, {"A", "B"}}, {2, {"C", "D"}} };
    test.Weight = 199.45;
    test.Male = true;
    test.Name = "DXTsT";
    test.ParentAge = 55;
    test.ParentAge2 = 54;
    test.EnumValue = SType::C;
    EXPECT_EQ_STRING(SJson::JsonConvert::Serialize(test, SJson::DefaultOption),
        R"({$TestParent: {ParentAge: 55}, $TestParent2: {ParentAge2: 54}, Age: 21, EnumValue: "C", InternalData: {A: 0, B: 0.00000000000000000}, List: [1, 2, 3], Male: true, Mapp: [{key: 1, value: ["A", "B"]}, {key: 2, value: ["C", "D"]}], Name: "DXTsT", Weight: 199.44999694824218750})");
}


static void test_deserialization()
{
    int i1 = SJson::JsonConvert::Deserialize<int>("114514");
    EXPECT_EQ_INT(i1, 114514);

    float f1 = SJson::JsonConvert::Deserialize<float>("114514.0");
    EXPECT_EQ_FLOAT(f1, 114514.f);

    float f2 = SJson::JsonConvert::Deserialize<double>("3.14");
    EXPECT_EQ_FLOAT(f2, 3.14);

    bool b1 = SJson::JsonConvert::Deserialize<bool>("true");
    EXPECT_EQ_BOOL(b1, true);

    bool b2 = SJson::JsonConvert::Deserialize<bool>("false");
    EXPECT_EQ_BOOL(b2, false);

    auto s1 = SJson::JsonConvert::Deserialize<std::string>(R"("hello")");
    EXPECT_EQ_STRING(s1, "hello");

    auto v1 = SJson::JsonConvert::Deserialize<std::vector<int>>(R"([1, 2, 3, 4])");
    auto exv1 = std::vector<int>{ 1, 2, 3, 4 };
    EXPECT_EQ_VECTOR(v1, exv1);

    auto v2 = SJson::JsonConvert::Deserialize<std::vector<int>>(R"([])");
    auto exv2 = std::vector<int>{};
    EXPECT_EQ_VECTOR(v2, exv2);

    auto m1 = SJson::JsonConvert::Deserialize<std::map<int, std::string>>(R"([{"key": 1, "value": "hello"}])");
    EXPECT_EQ_STRING(m1[1], "hello");

    auto m2 = SJson::JsonConvert::Deserialize<std::map<std::string, int>>(R"([{"key": "test", "value": 114514}])");
    EXPECT_EQ_INT(m2["test"], 114514);

    auto enum1 = SJson::JsonConvert::Deserialize<SType>(R"("C")");
    EXPECT_EQ_INT((int)enum1, (int)SType::C);

    EXPECT_DESERIALIZE_THROW(SType, R"("AAAAAAAAAAAA")", std::invalid_argument);

    TestObject test = {};
    test.Age = 21;
    test.List = { 1, 2, 3 };
    test.Mapp = { {1, {"A", "B"}}, {2, {"C", "D"}} };
    test.Weight = 199.45;
    test.Male = true;
    test.Name = "DXTsT";
    test.ParentAge = 55;
    test.ParentAge2 = 54;
    test.EnumValue = SType::C;
    auto object1 = SJson::JsonConvert::Deserialize<TestObject>(R"({"$TestParent": {"ParentAge": 55}, "$TestParent2": {"ParentAge2": 54}, "Age": 21, "EnumValue": "C", "InternalData": {"A": 0, "B": 0.0}, "List": [1, 2, 3], "Male": true, "Mapp": [{"key": 1, "value": ["A", "B"]}, {"key": 2, "value": ["C", "D"]}], "Name": "DXTsT", "Weight": 199.44999694824219})");
    assert(test == object1);
}

static void test_parse()
{
    test_parse_null();
    test_parse_expect_value();
    test_parse_true();
    test_parse_false();
    test_parse_invalid_value();
    test_parse_root_not_singular();
    test_parse_int();
    test_parse_float();
    test_parse_string();
    test_parse_array();
    test_parse_object();
    test_parse_json();
    test_to_string();
    test_serialization();
    test_deserialization();
}


int main()
{

    SJson::JsonNode json;
    json["name"]["s"] = "CXUtk";

    SJson::JsonNode node = SJson::object({
        {"Name", "CXUtk"},
        {"Age", 21},
        {"Company", "Microsoft"},
        {"Object", {1, 2, 3}},
        {"Test", SJson::array({1, 2, "123", false})},
        });
    printf("%s\n", node.ToString(SJson::DocumentOption).c_str());

    //SJson::JsonFormatOption option = {};
    //option.Inline = false;
    //option.KeysWithQuotes = true;
    //option.UseTab = false;
    //printf("%s\n", node.ToString(option).c_str());

    TestObject obj = {};
    //obj.ss = { {1, 3.14}, {2, 6.28} };
    //int TestObject::* a = nullptr;
    //a.
    //SJson::JsonNode json = toJson<TestObject>(obj);
    //printf("%s\n", json.ToString(SJson::DocumentOption).c_str());
    //property(&TestObject::Weight, "Weight");
    std::map<const std::string, int> mapp = { {"a", 1}, {"b", 2}, {"c", 3} };
    std::vector<int> arr = { 1342, 593, 114514, 1919810 };
    std::string str = "Hello";
    std::tuple<int, float, std::string> tup = { 1, 3.14, "hello" };

    auto value = SJson::de_serialize<int8_t>(SJson::JsonNode(244));

    auto value1 = SJson::serialize(obj);
    printf("%s\n", value1.ToString(SJson::DocumentOption).c_str());

    
    printf("%s\n", SJson::serialize(SType::B).ToString(SJson::DocumentOption).c_str());

    //printf("%s\n", SJson::serialize(obj).ToString(SJson::DocumentOption).c_str());

    try
    {
        auto node = SJson::JsonConvert::Parse(R"([1, 2, 3, [], []])");
    }
    catch (const std::exception& ex)
    {
        printf("%s\n", ex.what());
    }
    printf("Size of JsonNode: %d\n", sizeof(SJson::JsonNode));
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}

