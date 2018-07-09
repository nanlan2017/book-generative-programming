#ifndef IF_H_
#define IF_H_

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

/***************************************************************/
#endif
