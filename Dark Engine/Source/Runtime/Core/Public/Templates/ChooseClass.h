#pragma once


#include "CoreTypes.h"

template<bool Predicate, typename TrueClass, typename FalseClass>
class TChooseClass;


template<typename TrueClass, typename FalseClass>
class TChooseClass<true, TrueClass, FalseClass>
{
public:
	typedef TrueClass Result;
};


template<typename TrueClass, typename FalseClass>
class TChooseClass<false, TrueClass, FalseClass>
{
public:
	typedef FalseClass Result;
};