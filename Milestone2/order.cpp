

#include "order.h"

using namespace std;


void CustomerOrder::print(ostream& os) const {
		os  << "customer name=" << custName << "\n"
            << "product name=" << prodName << "\n"
            << "number of items=" << itemList.size() << "\n";
        for(int i = 0; i < itemList.size(); i++) {
            os << "item:" << i << " " << itemList[i] << "\n";
        }

			os << "\n";
	} // print

void CustomerOrder::gengraphviz(ostream& os) const {
		string n = "\"" + custName + "\\n" + prodName + "\"";
		
		for(auto e : itemList) {
			//draw node/vertex
			os << "\"" << e << "\"" << " [fillcolor=yellow, shape=box];\n";
			//draw edge/arc
			os << "\"" << e << "\"" << "->" << n << " [color=green];\n";
		}
		os << "\n";
} // gengraphviz



void OrderManager::print(ostream& os) {
		for (auto it = begin(); it != end(); it++) {
			it->print(os);
		}
	}



void OrderManager::gengraphviz(string filename) {
	ofstream of(filename.c_str());
	if (of.is_open()) {
		of << "digraph task {\n";
		of << R"(node[style="filled", fillcolor="grey"]
		)";
		for (auto t = begin(); t != end(); t++) {
			t->gengraphviz(of);
		}
/*
of << R"foo(
label = "
			
)foo";
of << filename;
of << R"foo(
Task Relation Diagram
drawn by Graphviz";
fontsize=30;
)foo";*/

		of << "}\n";
		of.close();

		runDot(filename);
	}
}

void OrderManager::runDot(std::string& filename)
{
	// std::string pgm = "neato";	// nodes overlap
	// std::string pgm = "fdp";		// edges cross
	// std::string pgm = "twopi";	// edges cross
	// std::string pgm = "circo"; 	// edges cross
	std::string pgm = "dot";	// best looking graphs
	std::string type = "png";	// SVG files are tiny. The colours are better in PNG.
	std::string cmd = pgm + " -T" + type + " " + filename + ">" + filename + "." + pgm + "." + type;
	std::cout << "Command " << cmd << " returned " << system(cmd.c_str()) << "\n";
}
bool OrderManager::isTaskName(std::string s)
{
	if (s.empty()) return false;
	// names start with an alpha or a '_', continued with space, alpha, digit or '-', '_'

	if (not(isalpha(s[0]) or s[0] == '_'))
		return false;

	for (unsigned i = 1; i < s.size(); i++) {
		if (not(isalpha(s[i]) or s[i] == ' ' or s[i] == '-' or s[i] == '_'))
			return false;
	}
	return true;
}
int OrderManager::parseOrder(vector < vector<string> > csvData) {
	int errors = 0;
	int lineNumber = 0;
	auto printError = [&errors, &lineNumber](string message, string expected, string found) {
		errors++;
		cerr << "line " << lineNumber << " SYNTAX ERROR:  " << message << " expected " << expected
			<< " found -->" << found << "<--\n";
	};

	for (auto line : csvData) {
		lineNumber++;
		string custName, prodName;
		vector<string> itemList;
		
		if (line.size() < 3) {
			cout << "LINE: " << lineNumber << " SYNTAX ERROR - need at least 3 fields\n";
			continue;
		}      
		if (isTaskName(line[0])) {
			custName = line[0];
		}
		if (isTaskName(line[1])) {
			prodName = line[1];
		}
		
		for(int ii = 2; ii < line.size(); ii++) {
			if (isTaskName(line[ii])) {
				itemList.push_back(line[ii]);
			}
		}

	   
		emplace_back(custName, prodName, std::move(itemList));
	}
	return errors;
} // parseTask


int OrderManager::IntegrityCheck() {
	int errorCount = 0;

	for (auto it = begin(); it != end(); it++) {
		CustomerOrder* pCustomerOrder;
				   
		if(it->getListSize() > 0) {
			for(int x = 0; x < it->getListSize(); x++ ) {
		   
				if(it->getListSize() != 0)
			
					pCustomerOrder = Find(it->getItem(x));
				if(pCustomerOrder) {
				
				}
				else {
					errorCount++;
					cout << "'item' not found: "; it->print(cout);
				}
			}
		}

	}
	
	
	
	return errorCount;
} // integritycheck


