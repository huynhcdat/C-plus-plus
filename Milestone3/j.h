#pragma once

// j.h - job header file, derived from Order

#include "order.h"

class Job : public CustomerOrder {
	std::vector<bool> installed;	// true if o.getItemsOrdered[i] is installed
public:
	Job() {}
	Job(CustomerOrder& oo);
	bool isComplete();
	void Install(int i) { installed[i]	= true;  }
	void Remove(int i)	{ installed[i]	= false; }
	
};