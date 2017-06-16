#pragma once

#include<iostream>
#include<string>
#include<vector>
#include<exception>
#include <fstream>
#include <cctype>
#include <sstream>
#include <string>

using namespace std;

class CustomerOrder {
	string custName, prodName;
	vector<string> itemList;
public:
	CustomerOrder()	{}

	CustomerOrder(string cn, string pn, vector<string> il)
		: custName(cn), prodName(pn), itemList(std::move(il)) {}
    
	
		
	// Item getter functions
	string& getCustName() 			{ return custName;}
	string& getProdName()			{ return prodName;}
    string& getItem(int i)			{ return itemList[i];}
	
    int getListSize()               { return itemList.size();}
	
	void print(ostream& os) const;
	void gengraphviz(ostream& os) const;
	
};


class OrderManager : public vector<CustomerOrder> {
public:
	OrderManager() {}
	
	CustomerOrder* Find(string& name) {
		int i = 0;
		for(auto it = begin(); it != end(); it++) {
			
            for(int y = 0; y < it->getListSize(); y++) {
				//cout << "test y-" << y << "\n";
				//cout << "listsize -" << it->getListSize() << "\n";
                if (name == it->getItem(y)) {
                    //cout << "finding ->" << it->getItem(y) << " at " << y << "\n";
                    return &at(i);
                }
            }
			i++;
		}
		return nullptr;
	}
	
	void print(ostream& os);
	void gengraphviz(string filename);
	bool isItemName(std::string s);
	void runDot(std::string& filename);
	bool isTaskName(std::string s);
	int parseOrder(vector < vector<string> > csvData);
	int IntegrityCheck();
	
	
	
};