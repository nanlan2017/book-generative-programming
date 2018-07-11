#ifndef CONT_S_H
#define CONT_S_H

namespace intimate 
{
//? 不使用 模板偏特化 实现的 IF<>           （把成员模板封装在不同的struct中，然后由完全特化匹配其中一个struct）
struct SelectThen
{
	template<typename ThenType,typename ElseType>
	struct Result
	{
		using RET = ThenType;
	};
};

struct SelectElse
{
	template<typename ThenType,typename ElseType>
	struct Result
	{
		using RET = ElseType;
	};
};
/*****************************************************/
template<bool Condition>
struct ChooseSelector
{
	using RET = SelectThen;
};
template<>
struct ChooseSelector<false>
{
	using RET = SelectElse;
};
/*****************************************************/
template<bool Condition,typename ThenType, typename ElseType>
class IF
{
	using selector = typename ChooseSelector<Condition>::RET;
public:
	using RET = typename selector::template Result<ThenType, ElseType>::RET;
};


}
#endif

