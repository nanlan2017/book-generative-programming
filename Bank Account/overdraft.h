#include "account.h"

//? 不可透支的账户
//x 修改debit()用款函数的行为
template<typename Base>
class NoCredit : public Base
{
public: // 取出想使用的Component
	using Config = typename Base::Config;
	using Owner = typename Base::Owner;
	using NumericType = typename Base::NumericType;

	NoCredit(const Owner& own):Base(own){ }

	// @override
	void debit(const NumericType& amount)
	{
		assert(amount > 0);
		if (Base::balance()-amount<0) {
			throw "No credit Available!";
		}else {
			Base::debit(amount);
		}
	}
};

//X------------------------------------------------------------------------
//? 可透支的账户（但透支额有限制）
//x 增加limit(透支限额)成员； 重写debit()行为
template<typename Base>
class Credit: public Base
{
public: // 取出想使用的Component
	using Config = typename Base::Config;
	using Owner = typename Base::Owner;
	using NumericType = typename Base::NumericType;

	Credit(Owner& own):Base(own),limit_(0){ }

	NumericType& limit() const { return limit_; } //TODO 只读方法 能返回 其成员的非const引用 ？？？
	void limit(const NumericType& newLimit) { limit_ = newLimit; }

	// @override
	void debit(const NumericType& amount)
	{
		assert(amount > 0);
		if (Base::balance()-amount< -limit_) {
			throw "Account overdrawn!";
		}else {
			Base::debit(amount);
		}
	}
private:
	NumericType limit_;
};

