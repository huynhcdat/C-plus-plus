#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <exception>
#include <cctype>
#include <cstring> // strcasecmp
#include <sstream>
#include <stdio.h>
#include <cmath>
#include <queue>

#include "item.h"
#include "order.h"
#include "task.h"
#include "j.h"

class Machine : public Task {
	
	// Task			t;
	bool			bInstaller;
	bool			bRemover;
	bool			bSink;
	std::queue<Job>	inputQ;
	std::string		name;
	Job				job;
	
public:
	Machine(Task& tt, ItemManager& im);
	void push(Job&& j);
	
	void print(std::ostream& os);
	void Classify(ItemManager& im);
	void Run(ItemManager& im);
};