#include "_Common.h"
#include <iostream>

using namespace std;

//? ¸¡µãÊý
template <int mantissa_, int exp_>
struct Float
{
	enum
	{
		mantissa = mantissa_,
		exp = exp_
	};
	operator const float() const {
		return mantissa_ * pow(10, exp_);
	}

	static const float v = Float<mantissa_, exp_>();
};

/***************************************************************/
//? Sqrt()
template<typename Float>
struct Sqrt
{
	enum
	{
		val = sqrtf(Float()),
	};

	typedef Float<val,0> RET;
};

//x***************************************************************

int main(int argc, char* argv[])
{

	IF<(1 + 2 > 4), short, int>::RET i;

	typedef Float<25, -1> F2p5;
	float v3 = 8.0;
	static const float v0 = v3 + F2p5();
	typedef typename Sqrt<F2p5>::RET RESULT;
	static const int v1 = 8 + RESULT();
	cout << v0 << v1;

}

const int v5 = 8.0 + Float<25, -1>::v;
