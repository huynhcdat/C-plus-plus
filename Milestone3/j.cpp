// j.cpp - job file
#include "j.h"

Job::Job(CustomerOrder& oo)
: CustomerOrder(oo)	// base class constructor
{
	for(size_t i = 0; i < getListSize(); i++)
		installed.push_back(false);
}

bool Job::isComplete()
{
	for(auto e : installed)
		if(not e) return false;	// not complete
	return true;
}