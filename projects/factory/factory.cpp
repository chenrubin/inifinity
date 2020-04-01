#include "factory.hpp"

namespace ilrd
{
factory::BadCreate::BadCreate()
	: std::runtime_error("Bad create")
{}

factory::BadKey::BadKey()
	: std::logic_error("Bad key")
{}
}