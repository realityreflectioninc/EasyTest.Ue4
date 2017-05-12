// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <map>
#include <tuple>
#include <type_traits>
#include "AutomationTest.h"
#define SIMPLE_FUNCTION_TEST(name, function, ...) \
IMPLEMENT_SIMPLE_AUTOMATION_TEST(name ## _AutoTest, #name, EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter) \
bool name ## _AutoTest::RunTest(const FString& Parameters)\
{\
	return function(__VA_ARGS__);\
}

#define FUNCTION_TEST(name, function) \
class name##_AutoTestBase : public FAutomationTestBase\
{\
public:\
	using FuncType = decltype(function);\
	name##_AutoTestBase(const FString& InName, bool bIsComplex)\
		: FAutomationTestBase(InName, bIsComplex)\
	{\
	}\
\
protected:\
	static TestCaseTypeGetter<FuncType>::Type TestCases;\
};\
TestCaseTypeGetter<decltype(function)>::Type name##_AutoTestBase::TestCases;\
IMPLEMENT_CUSTOM_COMPLEX_AUTOMATION_TEST(name##_AutoTest, name##_AutoTestBase, #name, EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)\
bool name##_AutoTest::RunTest(const FString& Parameters) \
{\
	return DoExecuteTuple(function, TestCases.Table[Parameters]);\
}\
void name##_AutoTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray <FString>& OutTestCommands) const

#define ADD_TEST_CASE(name, ...)\
OutBeautifiedNames.Add(#name);\
OutTestCommands.Add(#name);\
TestCases.Table[ #name ] = TestCaseTypeGetter<FuncType>::MakeTuple(__VA_ARGS__)

template <int N>
struct TupleUnpacker
{
	template <class... FuncArgs, class... TupleArgs, class... Args>
	static bool DoExecute(bool f(FuncArgs...), const std::tuple<TupleArgs...>& targ, Args&&... args)
	{
		return TupleUnpacker<N - 1>::DoExecute(f, targ, std::get<N - 1>(targ), std::forward<Args>(args)...);
	}
};

template <>
struct TupleUnpacker<0>
{
	template <class... FuncArgs, class... TupleArgs, class... Args>
	static bool DoExecute(bool f(FuncArgs...), const std::tuple<TupleArgs...>& targ, Args&&... args)
	{
		return f(std::forward<Args>(args)...);
	}
};

template <class... FuncArgs, class... TupleArgs>
bool DoExecuteTuple(bool f(FuncArgs...), std::tuple<TupleArgs...> const& targ)
{
	return TupleUnpacker<sizeof...(TupleArgs)>::DoExecute(f, targ);
}

template<typename... Args>
struct TestCase
{
public:
	static std::map<FString, std::tuple<Args...>> Table;
};

template<typename... Args>
std::map<FString, std::tuple<Args...>> TestCase<Args...>::Table;

template<typename T>
struct TestCaseTypeGetter;

template<typename Result, typename... Args>
struct TestCaseTypeGetter<Result(Args...)>
{
	static std::tuple<std::decay_t<Args>...> MakeTuple(std::decay_t<Args>&&... args)
	{
		return std::make_tuple<std::decay_t<Args>...>(std::forward<std::decay_t<Args>>(args)...);
	}

	using Type = TestCase<std::decay_t<Args>...>;
};