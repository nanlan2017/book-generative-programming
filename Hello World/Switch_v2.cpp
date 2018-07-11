#include "_Common.h"
#include <iostream>
using namespace std;

struct stop
{
	enum { RET = 1 };
};

template<int n>
struct Factorial
{
	using PreviousFactorial = IF<n == 0, stop, Factorial<n - 1>>;
	enum
	{
		RET = (n==0)
				? PreviousFactorial::RET 
				: PreviousFactorial::RET*n	,
	};
};

//X------------------------------------------------------------------------------
struct A {static void execute() { cout<<"A"<<endl ; } };
struct B {static void execute() { cout<<"B"<<endl ; } };
struct D {static void execute() { cout<<"Default"<<endl ; } };

const int DEFAULT = ~(~0u >> 1);

struct NilCase{};

template<int tag_,typename Type_, typename Next_ = NilCase>
struct CASE
{
	enum { tag = tag_ };
	using Type = Type_;
	using Next = Next_;
};

template<int tag,typename Case>
class SWITCH
{
	using NextCase = typename Case::Next;
	enum
	{
		caseTag = Case::tag,
		found = (caseTag== tag || caseTag==DEFAULT)  //? ★★★
	};
public:
	//? ★★ 这个迭代必须在 Case-tag为Default（其NextCase为NilCase—— xu） 时
	//?  不再继续要求下一个 SWITCH<tag, NilCase> <—— 这个会出错，因为未特化版的NilCase进去会求Next，逻辑上没这个东西
	using RET =typename  IF<found,
							typename Case::Type,
							typename SWITCH<tag, NextCase>::RET
						 >::RET;
};

template<int tag>
class SWITCH<tag,NilCase>  //? ★★★
{
public:
	using RET = NilCase;
};

/********************************************************************/

void main_6()
{
	// 测试
	using T00 = typename
	SWITCH<(10),
		CASE<DEFAULT, D
		>>::RET;

	using T0 = typename
	SWITCH<(10),
		CASE<10, A,
		CASE<DEFAULT, D
		>>>::RET;

	// D
	using T1 = typename
	SWITCH<(1 + 1 - 2),
		CASE<1, A,
		CASE<2, B,
		CASE<DEFAULT, D
		>>>>::RET;

	// A
	using T2 = typename
	SWITCH<(1 + 1 - 1),
		CASE<1, A,
		CASE<2, B,
		CASE<DEFAULT, D
		>>>>::RET;

	// B
	using T3 = typename
	SWITCH<(1 + 1),
		CASE<1, A,
		CASE<2, B,
		CASE<DEFAULT, D
		>>>>::RET;
}