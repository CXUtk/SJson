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
	return 0;
}