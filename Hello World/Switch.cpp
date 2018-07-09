#include "_Common.h"
#include <iostream>
using namespace std;
/*
switch()
{
	case1:
	case2:
	...
}
*/
//? Case类，其每个实例是一种Case
struct A {static void execute() { cout<<"A"<<endl ; } };
struct B {static void execute() { cout<<"B"<<endl ; } };
struct D {static void execute() { cout<<"Default"<<endl ; } };
struct DDD {static void execute() { cout<<"DDD"<<endl ; } };

const int DEFAULT = 100;
struct End
{
	
};

struct DefaultCase
{

}; 

/********************************************************************/
//? 一般的Case实例
template<int val,typename T, typename NextCase = End>
struct CASE
{
	enum
	{
		VAL = val,
	};
	using TYPE = T;
	using Next = NextCase;
};

//? default的那个case实例
template<typename T, typename NextCase = End>
struct CASE<DEFAULT,T, NextCase>
{
	enum
	{
		VAL = DEFAULT,
	};
	using TYPE = T;
	using Next = DefaultCase;
};
/********************************************************************/
//? 递归过程
template<int Condition , typename CaseList >
struct SWITCH
{
	using RET = typename IF<(Condition == CaseList::VAL),
							typename CaseList::TYPE,
							typename SWITCH<Condition, typename CaseList::Next>::RET
							>::RET;
				
};
//? 递归最后一步： SWITCH<  Condition, CASE<DEFAULT,D,DefaultCase>  >
//?   下一步：求 SWITCH<Condition, DefaultCase::Next>
//?   需要：遇到这种情况时：改写SWITCH,直接返回 CaseList::TYPE
template<int Condition>
struct SWITCH<Condition,CASE<DEFAULT,D,End>>
{
	using RET = DDD;
};
/********************************************************************/

void main_5()
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