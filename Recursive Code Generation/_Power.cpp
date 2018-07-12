#include <iostream>
using namespace std;

// 求m^n
namespace dynamic_code 
{
inline int power(int&m, int n)
{
	int r = 1;
	for(; n>0; n--) {
		r *= m;
	}
	return r;
}	
}

//x---------------------------------------------------
//? Power<>
template<int n>
inline 
int power(const int& m)
{
	return power<n - 1>(m)*m;	// m^n   ——————————>     m^(n-1) * m
}

//? power<>特化： 1
template<>
inline
int power<1>(const int& m)
{
	return m;
}

//? power<>特化： 0
template<>
inline
int power<0>(const int& m)
{
	return 1;
}

//x---------------------------------------------------
void main_power()
{
	power<3>(2);  // 2^3 ————>   2 *2 *2
}