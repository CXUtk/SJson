#include "SRefl.hpp"

#include <bitset>
#include <ctime>
#include <chrono>
#include <iostream>

class TestObject
{
public:
	SREFL_TYPE_HEADER(TestObject);

	int					Age;
	float				Weight;
	bool				Male;

private:
	int					m_privateValue;
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
			SREFL_FIELD(m_privateValue)
		);
	}
};

enum class TestType
{
	A123,
	B234,
	CACC,
	DBDF
};

template<>
struct SRefl::EnumInfo<TestType>
{
	SREFL_TYPEINFO_HEADER(TestType);
	constexpr static auto _ENUMLIST()
	{
		return std::make_tuple(
			SREFL_ENUM_TERM(A123),
			SREFL_ENUM_TERM(B234),
			SREFL_ENUM_TERM(CACC),
			SREFL_ENUM_TERM(DBDF)
		);
	}
#define LISTFUNC(F) F(A123) F(B234) F(CACC) F(DBDF) 
	GENERATE_ENUM_MAPPING(TestType, LISTFUNC)
#undef LISTFUNC
};
template<unsigned long N>
void printBitset(std::bitset<N> const& bs)
{
	std::cout << bs.to_string<char, std::char_traits<char>,
		std::allocator<char>>();
}

int main()
{
	SRefl::ForEachField<TestObject>([](const auto& field) {
		std::cout << field.Name << std::endl;
		});
	SRefl::ForEachEnumItem<TestType>([](const auto& item) {
		std::cout << (int)item.Value << ", " << item.Name << std::endl;
		});
	return 0;
}