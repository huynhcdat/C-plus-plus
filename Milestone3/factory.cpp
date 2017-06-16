#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include <cctype>
#include <cstring> // strcasecmp

#include "item.h"
#include "j.h"
#include "m.h"
#include "order.h"
#include "task.h"
#include "csvreader.cpp"

using namespace std;

class Factory {
	ItemManager		im;
	OrderManager	om;
	TaskManager		tm;
	vector<Job>		jobList;
	vector<Machine>	machineList;
public:
	
	Factory(
		ItemManager		&& IM,
		OrderManager	&& OM,
		TaskManager		&& TM
		)
		: im(move(IM)), om(move(OM)), tm(move(TM))
		{
			//////////////////////////////////////////////////
			// step 1 - load machines, one machine per task //
			//////////////////////////////////////////////////
			for(size_t m =0; m < tm.size(); m++) {
				machineList.push_back( Machine(tm.at(m), im) );
			}
			
			//////////////////////////////////
			// step 2 - find source machine //
			//////////////////////////////////
			enum {SOURCE_NOT_FOUND = -1};
			int sourceMachine = SOURCE_NOT_FOUND;
			for(size_t m = 0; m < machineList.size(); m++) {
				
				if(machineList.at(m).isSource()) {
					if(sourceMachine == -1) {
						// keep checking to see if there are duplicate sources
						sourceMachine = m;
					} else {
						throw string("cannot build factory: multiple source nodes, fix data and resubmit");	
					}
				}
			}
			//if(sourceMachine == SOURCE_NOT_FOUND)
				
			//	throw string("cannot build factory: NO source node, fix data and resubmit");
			cout << "Soure machine = " << sourceMachine << ": "; tm.at(sourceMachine).print(cout); cout << "\n";
			
			//////////////////////////////////////////////////////////////
			// step 3 - load jobs into soure machine, one job per order //
			//////////////////////////////////////////////////////////////
			for(size_t j = 0;  j < om.size(); j++) {
				machineList.at(sourceMachine) . push ( move(Job(om.at(j))) );
		    }
			
			////////////////////////
			// step 4 - Time Loop //
			////////////////////////
		while(true) {
				// look for incomplete jobs
				bool notFinished = false;
				for(size_t m = 0; m < machineList.size(); m++) {
					if(not machineList.empty()) {
						notFinished = true;
						break;
					}
				}
				if(not notFinished)
					break;	// all done. simulation over
			
			
			///////////////////////////
			// step 5 - Machine loop //
			///////////////////////////
			for(size_t m = 0; m < machineList.size(); m++) {
				machineList[m].Run(im);
			}
			break;	// TODO: delete this line - just for testing
		}
	}
	Factory
	(	
		ItemManager		& im,
		OrderManager	& om,
		TaskManager		& tm
	)
	{	
		int errorCount = 0;
		
		// for each item, check task installer + remover exist
		for(auto item = im.begin(); item < im.end(); item++) {
			if(tm.Find(item->getInstaller()) == nullptr) {
				errorCount++;
				cerr << "Item <" << item->getName() << "> needs installer <" << item->getInstaller() << ">"
					 << " but there is no task <" << item->getInstaller() << ">"
					 << "\n";
			}
			
			if(tm.Find(item->getRemover()) == nullptr) {
				errorCount++;
				cerr << "Item <" << item->getName() << "> needs remover <" << item->getRemover() << ">"
					 << " but there is no task <" << item->getRemover() << ">"
					 << "\n";
			}
		}
		
		// for each order, check each item ordered exists
		
		for(auto order = om.begin(); order < om.end(); order++) {
			
			// order points to an order
			
			//item list size then getItem(size)
			int i = order->getListSize();
			for(int item = 0; item < i; item++){
				//item points to a string
				if(im.Find(order->getItem(item)) == nullptr) {
					errorCount++;
					cerr << "Customer <" << order->getCustName() << "> tried to order <" << order->getItem(item) << ">"
					 << " but there is no item <" << order->getItem(item) << ">"
					 << "\n";
				}
			}
		}
		
	
	}	
};

int main(int argc, char**argv)
{
	// return codes:
	// 1. - 4 arguments were not supplied
	// 2. - some or all of the data files cannot be found
	// 3. - integrity check failed, duplicate tasks, missing tasks
	
	char delimiter = ',';
	string fileItem;
	string fileOrder;
	string fileTask;
	
	if(argc != 5) {
		std::cerr << "usage: " << argv[0] << " itemfile orderfile taskfile csv-char\n";
		return 11111;
	}
	
	fileItem	= argv[1];
	fileOrder	= argv[2];
	fileTask	= argv[3];
	delimiter 	= argv[4][0];
	
	try {
		int errors = 0;
		
		auto fileExists = [] (string f) { ifstream is(f); return is.is_open(); };
		if(not fileExists(fileItem)) { errors++; cerr << "cannot open item file <" << fileItem << ">\n";}
		if(not fileExists(fileOrder)) { errors++; cerr << "cannot open order file <" << fileOrder << ">\n";}
		if(not fileExists(fileTask)) { errors++; cerr << "cannot open task file <" << fileTask << ">\n";}
		if(errors) return 2;
		
		cout <<"+++++++++++++++++++++++++++++++++++\n";
		cout <<"+             Item                +\n";
		cout <<"+++++++++++++++++++++++++++++++++++\n";
		cout << "# file: " << fileItem << " delimiter -->" << delimiter << "<--\n";
		vector< vector<string> > csvItem;	// a 2d ragged array: csvData[line][field]
		csvread(fileItem, delimiter, csvItem);
		// printCSV( csvitem );
	
		ItemManager im;
		errors = im.parseItem(csvItem);
		if(errors)
			cout << "'" << fileItem << "' contains " << errors << " SYNTAX ERROS\n";
		// im.print(cout);
		im.gengraphviz(fileItem + ".gv");
	
	
		cout <<"+++++++++++++++++++++++++++++++++++\n";
		cout <<"+             Order               +\n";
		cout <<"+++++++++++++++++++++++++++++++++++\n";
		cout << "# file: " << fileOrder << " delimiter -->" << delimiter << "<--\n";
		vector< vector<string> > csvOrder;	// a 2d ragged array: csvOrder[line][field]
		csvread(fileOrder, delimiter, csvOrder);
		// printCSV( csvOrder );
	
		OrderManager om;
		errors = om.parseOrder(csvOrder);
		if(errors)
			cout << "'" << fileOrder << "' contains " << errors << " SYNTAX ERROS\n";
		// om.print(cout);
		om.gengraphviz(fileOrder + ".gv");
	
	
		cout <<"+++++++++++++++++++++++++++++++++++\n";
		cout <<"+             Task                +\n";
		cout <<"+++++++++++++++++++++++++++++++++++\n";
		cout << "# file: " << fileTask << " delimiter -->" << delimiter << "<--\n";
		vector< vector<string> > csvTask;	// a 2d ragged array: csvTask[line][field]
		csvread(fileTask, delimiter, csvTask);
		// printCSV( csvTask );
	
		TaskManager tm;
		errors = tm.parseTask(csvTask);
		if(errors)
			cout << "BAD DATA :'" << fileTask << "' contains " << errors << " SYNTAX ERROS\n";
		int errorCount = tm.IntegrityCheck();
		if(errorCount) cout << "BAD DATA: " << errorCount
							<< " erros either duplicates or unresolved task references\n";
		// tm.print(cout);
		tm.gengraphviz(fileTask + ".gv");
		
		cout <<"+++++++++++++++++++++++++++++++++++\n";
		cout <<"+ Item,Order,Task Integrity Check +\n";
		cout <<"+++++++++++++++++++++++++++++++++++\n";
		errors = 0; //im.IntegrityCheck() + tm.IntegrityCheck() + om.IntegrityCheck();
		if(errors) {
			cerr << "\n";
			cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
			cerr << "!!!!                                      !!!!\n";
			cerr << "!!!! Integrity errors --- cannot continue !!!!\n";
			cerr << "!!!!      Fix the data and resubmit       !!!!\n";
			cerr << "!!!!                                      !!!!\n";
			cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
			cerr << "\n";
			return 3;
		}
		
		cout << "********************************\n";
		cout << "**** Integrity check passed ****\n";
		cout << "**** GOOD TO GO             ****\n";
		cout << "********************************\n";
		
		Factory f( move(im), move(om), move(tm) );
		
	    } catch(exception& e) {
			cerr << "It threw a std exception: " << e.what() << "\n";		
		} catch(string& e) {
			cerr << "It threw: " << e << "\n";
		}
		return 0;
}
	

