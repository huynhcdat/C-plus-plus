#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <cctype>
#include <cstring> // strcasecmp
#include <sstream>
#include <stdio.h>
#include <cmath>

using namespace std;

class Task	{
	string	name, slots, passed, failed;
	Task	*passedTaskPtr, *failedTaskPtr;
	bool	bSink;
	int		inboundCount;
public:
	Task()
		: passedTaskPtr(nullptr), failedTaskPtr(nullptr),
		bSink(false), inboundCount(0)
	{}

	Task(string n, string s, string p, string f)
		: name(n), slots(s), passed(p), failed(f),
		  passedTaskPtr(nullptr), failedTaskPtr(nullptr),
		  inboundCount(0)
	{}

	// fluff: setters, and getters
	string& getName() 			{ return name; }		// returns reference. not const
	string& getPassed() 		{ return passed; }		// returns reference. not const
	string& getFailed() 		{ return failed; }		// returns reference. not const
	bool isSink() const			{ return passed.empty() && failed.empty(); }
	void incInboundCount()		{ inboundCount++; }
	bool isSource() const		{ return inboundCount == 0; }

	void print(ostream& os) const;
	void gengraphviz(ostream& os) const;
	
};

class TaskManager : public vector<Task> {
	vector<Task> taskList;
public:
	TaskManager()	{}
	
	Task* Find(string& name)
	{
		for (size_t i = 0u; i < taskList.size(); i++)
			if (name == taskList[i].getName())
				return &taskList[i];
		return nullptr;
	}
	
	void print(ostream& os);
	void runDot(std::string& filename);
	bool isInt (std::string s);
	bool isTaskName (std::string s);
	void gengraphviz(string filename);
	int parseTask(vector < vector<string> > csvData);
	int IntegrityCheck();
};