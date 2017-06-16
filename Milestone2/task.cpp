#include "task.h"


using namespace std;

void Task::print(ostream& os) const
{
	os	<< "name=" << name
		<< " slots=" << slots;
	if (!passed.empty()) os << " passed=" << passed;
	if (!failed.empty()) os << " failed=" << failed;
	if (isSource() && isSink()){
		os << " SINGLETON";
	}
	else{
		if (isSink())
			os << " SINK";
		if (isSource())
			os << " Source";
	}
	os << "\n";
	
}	// print

void Task::gengraphviz(ostream& os) const
{
	auto makeName = [](string n) { return string("\"TASK\\n") + n + "\""; };

	if (isSource() && isSink()){
		os << "  " << makeName(name) << " [fillcolor=yellow, shape=box3d];\n";
	} else if (isSource()){
		os << "  " << makeName(name) << " [fillcolor=green, style=\"rounded,filled\", shape=rectangle];\n";
	} else if (isSink()){
		os << "  " << makeName(name) << " [fillcolor=green, style=\"rounded,filled\", shape=rectangle];\n";
	}
	if (!passed.empty()){
		os << "  " << makeName(name) << "->" << makeName(passed) << " [color=green];\n";
	} 
	if (!failed.empty()){
		os << "  " << makeName(name) << "->" << makeName(failed) << " [color=red];\n";
	}
}	// gengraphviz


void TaskManager::print(ostream& os)
{
	for (auto t : taskList) {
		t.print(os);
	}

// print out source/sink status
	int sourceCount = 0;
	int sinkCount = 0;
	int singletonCount = 0;
	for (auto it = taskList.begin(); it < taskList.end(); it++){
		if (it->isSink() && it->isSource()){
			singletonCount++;
			cout << it->getName() << " is a SINGLETON - isolated node\n";
		}
		else {
			if (it->isSink()){
				sinkCount++;
				cout << it->getName() << " is a SINK\n";
			}
			if (it->isSource()){
				sourceCount++;
				cout << it->getName() << " is a SOURCE\n";
			}
		}
	}
	cout << "There are"; 
	

}	// print

void TaskManager::runDot(std::string& filename)
{
	// std::string pgm = "neato";	// nodes overlap
	// std::string pgm = "fdp";		// edges cross
	// std::string pgm = "twopi";	// edges cross
	// std::string pgm = "circo"; 	// edges cross
	std::string pgm 	= "dot";	// best looking graphs
	std::string type	= "png";	// SVG files are tiny. The colours are better in PNG.
	std::string cmd		= pgm + " -T" + type + " " + filename + ">" + filename + "." + pgm + "." + type;
	std::cout << "Command " << cmd << " returned " << system(cmd.c_str()) << "\n";
}

bool TaskManager::isInt (std::string s)
{
if(s.empty()) return false;
for(size_t i = 0; i < s.size(); i++)
	if(not isdigit(s[i])) return false;
return true;
}

bool TaskManager::isTaskName (std::string s)
{
	if(s.empty()) return false;
	// names start with an alpha or a '_', continued with space, alpha, digit or '-', '_'
	
	if(not (isalpha(s[0]) or s[0] == '_') )
		return false;
	
	for(unsigned i=1; i < s.size(); i++) {
		if(not (isalpha(s[i]) or s[i] == ' ' or s[i] == '-' or s[i] == '_'))
			return false;
	}
	return true;
}

void TaskManager::gengraphviz(string filename)
{
	ofstream of(filename.c_str());
	if (of.is_open())	{
		of << "digraph task {\n";

		of << R"(node[style="filled", fillcolor="grey"])";

		for (auto t : taskList){
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

int TaskManager::parseTask(vector < vector<string> > csvData) // return error count
{
	int errors = 0;
	int lineNumber = 0;
	auto printError = [&errors, &lineNumber](string message, string expected, string found)
	{ errors++; cerr << "line " << lineNumber << " SYNTAX ERROR: " << message << " expected " << expected << " found -->" << found << "<--\n"; };

	for (auto line : csvData) {
		lineNumber++;
		string name, slots("1"), passed, failed;
		switch (line.size()) {
		case 4:	// name, slots, passed, failed
			if (isTaskName(line[3])) {
				failed = line[3];
			}
			else{
				printError("'failed' field name", "alpha", line[3]);
				continue;
			}
		case 3:	// name, slots, passed
			if (isTaskName(line[2])) {
				passed = line[2];
			}
			else {
				printError("'passed' field name", "alpha", line[2]);
				continue;
			}
		case 2:	// name, slots
			if (isInt(line[1])) {
				slots = line[1];
			}
			else {
				printError("'slots' count", "number", line[1]);
				continue;
			}
		case 1: // name
			if (isTaskName(line[0])) {
				name = line[0];
			}
			else {
				printError("'name' field", "alpha", line[0]);
				continue;
			}
			break;
		default:
			printError("?", "1,2,3, or 4 fields", to_string(line.size()) + "fields");
			continue;
		}
		// it parsed!	store the fields.
		// taskList.push_back( move(Task(name, slots, passed, failed)) );
		taskList.emplace_back(name, slots, passed, failed);
	}
	return errors;
}	// parseTask



int TaskManager::IntegrityCheck()	//	return number of erros
{
	int errorCount = 0;
	//	look for duplicates
	for (auto it1 = taskList.begin(); it1 < taskList.end(); it1++) {
		for (auto it2 = it1+1; it2 < taskList.end(); it2++){
			if ( ::strcasecmp(it1->getName().c_str(), it2->getName().c_str()) == 0){
				errorCount++;
				cout << "Duplicate Task 1: "; it1->print(cout);
				cout << "Duplicate Task 2: "; it2->print(cout);
			}
		}
	}

	for (auto it = taskList.begin(); it < taskList.end(); it++) {
		Task* pTask;
		if (!it->getPassed().empty()) {
			pTask = Find(it->getPassed());
			if (pTask) {
				pTask->incInboundCount();
			}
			else {
				errorCount++;
				cout << "'passed' task not found: "; it->print(cout);
			}
		}
		if (!it->getFailed().empty()) {
			pTask = Find(it->getFailed());
			if (pTask) {
				pTask->incInboundCount();
			}
			else {
				errorCount++;
				cout << "'failed' task not found: "; it->print(cout);
			}
		}
	}
}




