#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "SJson.hpp"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

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
		if (value == expected)
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
		if (value == expected)
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
#define EXPECT_NODE_TYPE(node, type) \
    test_count++; \
    if (node.GetType() == type) { \
        test_pass++;\
    }\
    else{\
        fprintf(stderr, "%s:%d: Type mismatch, expected %s, actual %s\n", __FILE__, __LINE__, \
            SJson::GetValueTypeName(type).c_str(), SJson::GetValueTypeName(node.GetType()).c_str());\
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
static void test_parse_null()
{
	EXPECT_PARSE_NOTHROW("null");
	EXPECT_PARSE_TYPE("null", SJson::ValueType::Null);
	EXPECT_PARSE_THROW("nul", SJson::parse_error);
}

static void test_parse_expect_value()
{
	EXPECT_PARSE_THROW("", SJson::parse_error);
	EXPECT_PARSE_THROW(" ", SJson::parse_error);
	EXPECT_PARSE_THROW("\t", SJson::parse_error);
	EXPECT_PARSE_THROW("\n", SJson::parse_error);
	EXPECT_PARSE_THROW("{", SJson::parse_error);
	EXPECT_PARSE_THROW("[", SJson::parse_error);
}

static void test_parse_invalid_value()
{
	EXPECT_PARSE_THROW(",", SJson::parse_error);
	EXPECT_PARSE_THROW("u", SJson::parse_error);
	EXPECT_PARSE_THROW("}", SJson::parse_error);
	EXPECT_PARSE_THROW("\b", SJson::parse_error);
	EXPECT_PARSE_THROW("*", SJson::parse_error);
	EXPECT_PARSE_THROW("?", SJson::parse_error);
}

static void test_parse_root_not_singular()
{
	EXPECT_PARSE_THROW("null x", SJson::parse_error);
	EXPECT_PARSE_THROW("null,null", SJson::parse_error);
	EXPECT_PARSE_THROW("null,", SJson::parse_error);
	EXPECT_PARSE_THROW("1,2,3,4,5", SJson::parse_error);
	EXPECT_PARSE_THROW("{}[]", SJson::parse_error);
	EXPECT_PARSE_THROW("{},", SJson::parse_error);
	EXPECT_PARSE_THROW("[1, 2, 3[]]", SJson::parse_error);

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

	EXPECT_PARSE_THROW("32242gg", SJson::parse_error);
	EXPECT_PARSE_THROW(".", SJson::parse_error);
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
	EXPECT_PARSE_THROW("7f123", SJson::parse_error);
	EXPECT_PARSE_THROW("2.0e1e8", SJson::parse_error);
	EXPECT_PARSE_THROW("..", SJson::parse_error);
	EXPECT_PARSE_THROW("7.0f", SJson::parse_error);
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

	EXPECT_PARSE_THROW(R"(")", SJson::parse_error);
	EXPECT_PARSE_THROW(R"("123142)", SJson::parse_error);
	EXPECT_PARSE_THROW(R"('23142')", SJson::parse_error);
	EXPECT_PARSE_THROW(R"("\q")", SJson::parse_error);
	EXPECT_PARSE_THROW(R"("\ ")", SJson::parse_error);
}


static void test_parse_array()
{
	auto node = SJson::JsonConvert::Parse(" [1, 2, 3] ");
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


static void test_parse_json()
{
	EXPECT_PARSE_NOTHROW(JSON);
}

static void test_serialization()
{

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
	test_serialization();
}

class Internal
{
public:
	int A;
	float B;

	PROPERTIES(Internal, PROPERTY(A), PROPERTY(B));
};


enum class SType
{
	A,
	B,
	C
};

#define FIELDS(F) F(A) F(B) F(C)
GENERATE_ENUM(SType, FIELDS)
#undef KEYS

//template<>
//struct SJson::enum_mapper<SType>
//{
//	static const char* const map(SType v)
//	{
//		switch (v)
//		{
//		case SType::A:
//			return "A";
//		case SType::B:
//			return "B";
//		case SType::C:
//			return "C";
//		default:
//			break;
//		}
//		throw std::logic_error("Invalid enum type");
//	}
//};


class TestParent2
{
public:
	int		ParentAge2;

	PROPERTIES(TestParent2, PROPERTY(ParentAge2));
};

class TestParent
{
public:
	int		ParentAge;

	PROPERTIES(TestParent, PROPERTY(ParentAge));
};
class TestObject : public TestParent, public TestParent2
{
public:
	int			Age;
	float		Weight;

	PROPERTIES(TestObject, PROPERTY(Age), PROPERTY(Weight));
	constexpr static auto parents()
	{
		return std::make_tuple(
			BASECLASS(TestParent),
			BASECLASS(TestParent2)
		);
	}
};


int main()
{


	SJson::JsonNode node = SJson::object({
		{"Name", "DXTsT"},
		{"Age", 21},
		{"Company", "Microsoft"},
		{"Object", {1, 2, 3}},
		{"Test", SJson::array({1, 2, "123", false})},
		});
	printf("%s\n", SJson::GetValueTypeName(node.GetType()).c_str());

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

