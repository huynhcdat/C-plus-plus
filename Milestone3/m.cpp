// m.cpp - machine
#include "item.h"
#include "m.h"

Machine::Machine(Task& tt, ItemManager& im)
: Task(tt), // base class constructor
  bInstaller(false), bRemover(false), bSink(false)
{
	name = tt.getName();
	Classify(im);
}

void Machine::push(Job&& j) { inputQ.push( move(j) ); }

void Machine::print(std::ostream& os)
{
	os << "Machine " << name << "\n";
}

void Machine::Classify(ItemManager& im) {
	for(auto it = im.begin(); it != im.end(); it++) {
		if(name == it->getInstaller()) 	bInstaller 	= true;
		if(name == it->getRemover())	bRemover 	= true;
	}
	if(bInstaller && bRemover)
		throw name + " is both an installer and a remover. Fix item data and resubmit.";
}

void Machine::Run(ItemManager& im)
{
	cout << "Machine::Run: our name is " << name << "\n";
	if(inputQ.empty())
		return;				// nothing to do
	
	job = move(inputQ.front());
	inputQ.pop();
	
	cout << "Job="; job.print(cout);
	
	if(bSink) {
		if(job.isComplete()) {	// all done with job
		cout << "'" << job.getProdName() << "'"
			 << " complete for customer '" << job.getCustName() << "'"
			 << "\n";
		} else {
			throw std::string("job at sink node ") + name + " but job is not complete";
		}
	}
	
	if(bInstaller) {
		cout << job.getListSize() << " items ordered\n";
		// For each item ordered and compare our name with an isntaller
		for(size_t item = 0; item < job.getListSize(); item++) {
			// compare installer field with our name
			string itemName = job.getItem(item);	// item name, EG I7
			Item* itemPtr 	= im.Find( itemName );			// item
			if(itemPtr && itemPtr->getInstaller() == name) {// installer field name
				job.Install(item);
			}
			else cout << "item " << itemName << " needs installer " << itemPtr->getInstaller() << ", (not us)\n";
		}
	}
	
	if(bRemover) {
		cout << job.getListSize() << " items ordered\n";
		// Look at items ordered and figure out if an item needs a remover....
		//if not installed, nothing to do
		for(size_t item = 0; item < job.getListSize(); item++) {
			// compare remover field with our name
			string 	itemName 	= job.getItem(item);
			Item*	itemPtr2	=	im.Find( itemName );
			if(itemPtr2 && itemPtr2->getRemover() == name) {
				job.Remove(item);
			}
			else cout << "item " << itemName << " needs remover " << itemPtr2->getRemover() << ", (not us)\n";
		}
		
	}
	
}