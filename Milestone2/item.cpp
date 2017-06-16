
#include "item.h"


using namespace std;


void Item::print(ostream& os) const
{
	os	<< "name=" << name
		<< " installer=" << installer
		<< " remover=" << remover
		<< " sequentialCode=" << sequentialCode
		;
		
	if (not description.empty()) os << " description=" << description;
	os << "\n";
}	// print

void Item::gengraphviz(ostream& os) const
{
	string quote(R"(")");
	string n = quote + "ITEM\\n"		+ name		+ quote;
	string i = quote + "INSTALLER\\n"	+ installer + quote;
	string r = quote + "REMOVER\\n"		+ remover	+ quote;

	os << "  " << n << " [shape=box][color=blue]\n";
	os << "  " << i << " [color=green]\n";
	os << "  " << r << " [color=red]\n";

	os << "  " << n << "->" << i << " [color=green]\n";
	os << "  " << n << "->" << r << " [color=red]\n";

}	// gengraphviz

void ItemManager::print(ostream& os)
{
	for(auto i : itemList){
		i.print(os);
	}
	
// code to print 	
	
	
}

void ItemManager::runDot(std::string& filename)
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

void ItemManager::gengraphviz(string filename)
{
	ofstream of(filename.c_str());
	if (of.is_open())	{
		of << "digraph task {\n";

		of << R"(node[style="filled", fillcolor="grey"])";

		for (auto t : itemList){
			t.gengraphviz(of);
		}

		of << R"foo(
	label = ")foo";
		of << filename;
		of << R"foo( Task Relation Diagram drawn by Graphviz"; fontsize=30;)foo";

		of << "}\n";
		of.close();

		// run the dot graphviz program
		runDot(filename);

	}
}	// gengraphviz

bool ItemManager::isItemName(std::string s)
{
	if (s.empty()) return false;
}

int ItemManager::parseItem(vector < vector<string> > csvData) // return error count
{
	int errors = 0;
	int lineNumber = 0;
	auto printError = [&errors, &lineNumber](string message, string expected, string found)
	{ errors++; cerr << "line " << lineNumber << " SYNTAX ERROR: " << message << " expected " << expected << " found -->" << found << "<--\n"; };

	for (auto line : csvData) {
		lineNumber++;
		string name, installer, remover, sequentialCode, description;
		switch (line.size()){
		case 5:	// name, installer, remover, sequentialCode, description
			if (!(line[4]).empty()){
				description = line[4];
			
			}
			else {
				printError("'description' field name", "alpha", line[4]);
				
			}
		case 4:	// name, installer, remover, sequentialCode
			if (!(line[3]).empty()){
				sequentialCode = line[3];
			}
			else {
				printError("'Sequential Code' field name", "alpha", line[3]);
			}
		case 3:	// name, installer, remover
			if (!(line[2]).empty()){
				remover = line[2];
			}
			else {
				printError("'remover' field name", "alpha", line[2]);
			}
		case 2:	// name, installer
			if (!(line[1]).empty()){
				installer = line[1];
			}
			else {
				printError("'installer' field name", "alpha", line[1]);
			}
		case 1:	// name
			if (!(line[0]).empty()){
				name = line[0];
			}
			else {
				printError("'name' field name", "alpha", line[0]);
			}
			break;
		default:
			printError("?", "1,2,3,4, or 5 fields", to_string(line.size()) + "fields");
			continue;
		}


		itemList.emplace_back(name, installer, remover, sequentialCode, description);
	}


}

int ItemManager::IntegrityCheck()	//	return number of erros
{
	int errorCount = 0;
	//for each item, check task installer + remove exist
	for (auto item = itemList.begin(); item < itemList.end(); item++) {

		Item* pItem;
		pItem = Find(item->getInstaller());

		if (item->getInstaller().empty()){
			if (pItem){

			}
			else {
				errorCount++;
				cerr << "Item <" << item->getName() << "> needs installer <" << item->getInstaller() << ">"
					<< " but there is no task <" << item->getInstaller() << ">"
					<< "\n";
			}
		}
	

		if (item->getRemover().empty()) {
			errorCount++;
			cerr << "Item <" << item->getName() << "> needs remover <" << item->getRemover() << ">"
				<< " but there is no task <" << item->getRemover() << ">"
				<< "\n";
		}
	}
}



