#include "../Hello World/_Common.h"

int fib(int n)
{
	int x = 1, y = 0;
	for (int i=1; i<n; ++i) {
		x = x + y;
		y = x - y;
	}
	return x;
}
//x-------------------------------------------------------------
struct Less
{
	template<int x, int y>
	struct Comp
	{
		enum { RET = x<y };
	};
};

template<typename Stat>
struct Stop
{
	using RET = Stat;
};

template<int from, typename Compare, int to, int by, typename Statement>
struct FOR
{
	using RET = typename IF<Compare::template Comp<from,to>::RET,
							FOR<from+by, Compare,to, by,
										typename Statement::template Code<from>::Next>,
							Stop<typename Statement::template Code<from>>
					>::RET::RET;
};


//x-------------------------------------------------------------
template<int x_,int y_>
struct FibStat
{
	template<int i>
	struct Code
	{
		enum { x = x_ };
		using Next = FibStat<x + y_, x>;
	};
};


