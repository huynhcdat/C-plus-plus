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

using namespace std;

class Item {
	string name, installer, remover, sequentialCode, description;

public:
	Item() {}

	Item(string n, string i, string r, string sc, string d)
		:name(n), installer(i), remover(r), sequentialCode(sc), description(d) {}
		
	string& getName()			{ return name; }
	string& getInstaller()		{ return installer; }
	string& getRemover()		{ return remover; }
	string& getSequentialCode()	{ return sequentialCode; }
	string& getDescription()	{ return description; }

	
	void print(ostream& os) const;
	void gengraphviz(ostream& os) const;
};



class ItemManager : public vector<Item> {
	vector <Item> itemList;
public:
		ItemManager()	{}
		
		Item* Find(string& name)
		{
			for (size_t i = 0u; i < itemList.size(); i++)
			if (name == itemList[i].getName())
				return &itemList[i];
			return nullptr;
		}
		
		
		void print(ostream& os);
		void runDot(std::string& filename);
		void gengraphviz(string filename);
		bool isItemName(std::string s);
		int parseItem(vector < vector<string> > csvData);
		int IntegrityCheck();
};	
		
		
		
		
		
		