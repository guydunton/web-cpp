#pragma once
#include <type_traits>


template <typename T, typename... Args>
struct pack_contains_t : std::disjunction<std::is_same<T, Args>...>
{};

template <typename... Args>
struct contains_duplicates_impl;

template <typename T>
struct contains_duplicates_impl<T> : std::false_type {};

template <typename Arg, typename... Args>
struct contains_duplicates_impl<Arg, Args...>
{
	constexpr static const bool value = pack_contains_t<Arg, Args...>::value || contains_duplicates_impl<Args...>::value;
};

template<typename... Args>
struct contains_duplicates : contains_duplicates_impl<Args...>
{};
