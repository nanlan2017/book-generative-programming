#ifndef COMMON_H
#define COMMON_H

//? if-else
template <bool Condition,typename Then, typename Else>
struct  IF
{
	typedef Then RET;
};

template <typename Then, typename Else>
struct  IF<false,Then,Else>
{
	typedef Else RET;
};


#endif
