#pragma once
#include <vector>
#include <tuple>
#include <type_traits>
#include <cstdio>
#include <functional>

namespace SRefl
{
	template<typename T>
	struct TypeInfo { using _Type = T; };

	template<typename Class, typename T>
	struct FieldImpl
	{
		constexpr FieldImpl(T Class::* member, const char* name) : MemberPtr{ member }, Name{ name } {}

		using _Type = T;
		using _Class = Class;

		T Class::*	MemberPtr;
		const char*	Name;
	};

	template<typename Class, typename T>
	constexpr auto _field(T Class::* member, const char* name)
	{
		return FieldImpl<Class, T>{member, name};
	}

	template<typename T, typename = void>
	struct has_fields : public std::false_type {};

	template<typename T>
	struct has_fields<T, std::void_t<decltype(TypeInfo<T>::_FIELDLIST())>> : std::true_type {};

	template<typename T>
	constexpr bool has_fields_v = has_fields<T>::value;

	template<typename T, typename = void>
	struct has_bases : public std::false_type {};

	template<typename T>
	struct has_bases<T, std::void_t<decltype(TypeInfo<T>::_BASELIST())>> : std::true_type {};

	template<typename T>
	constexpr bool has_bases_v = has_fields<T>::value;


	template <typename T, T... S, typename F>
	inline constexpr void for_sequence(std::integer_sequence<T, S...>, F&& f)
	{
		(static_cast<void>(f(std::integral_constant<T, S>{})), ...);
	}

	template<typename T, typename F>
	inline constexpr void ForEachField(F&& func)
	{
		constexpr auto nTypes = std::tuple_size<decltype(TypeInfo<T>::_FIELDLIST())>::value;
		for_sequence(std::make_index_sequence<nTypes>{}, [&](auto i) {
			// get the field
			constexpr auto field = std::get<i>(TypeInfo<T>::_FIELDLIST());

			// apply function to the field
			func(field);
		});
	}

	template<typename T, typename F>
	inline constexpr void ForEachBase(F&& func)
	{
		constexpr auto nTypes = std::tuple_size<decltype(TypeInfo<T>::_BASELIST())>::value;
		for_sequence(std::make_index_sequence<nTypes>{}, [&](auto i) {
			// get the base type
			constexpr auto base = std::get<i>(TypeInfo<T>::_BASELIST());

			// apply function to the base type
			func(base);
		});
	}

#define SREFL_TYPE_HEADER(Type) friend struct SRefl::TypeInfo<Type>
#define SREFL_TYPEINFO_HEADER(Type) using _Type = Type;
#define SREFL_FIELD(member) SRefl::_field(&_Type::member, #member)
#define SREFL_FIELDLIST(Type, ...) constexpr static auto _FIELDLIST()\
{\
	return std::make_tuple(__VA_ARGS__);\
}\

};