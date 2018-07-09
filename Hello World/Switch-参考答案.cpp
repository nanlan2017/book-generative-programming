#include "_Common.h"
#include <iostream>
using namespace std;

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
		found = (caseTag== tag || caseTag==DEFAULT)  //? °Ô°Ô°Ô
	};
public:
	using RET =typename  IF<found,
							typename Case::Type,
							typename SWITCH<tag, NextCase>::RET
						 >::RET;
};

template<int tag>
class SWITCH<tag,NilCase>  //? °Ô°Ô°Ô
{
public:
	using RET = NilCase;
};

/********************************************************************/

void main_6()
{
	// ≤‚ ‘
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