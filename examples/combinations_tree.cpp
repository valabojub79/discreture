#include "Discreture/CombinationsTree.hpp"
#include <cstdlib>
#include <sstream>
#include <string>

int n = 5; //Global variable for exposition purposes. Do not use global variables in real code.
int k = 3;

using std::cout;
using std::cerr;
using std::endl;
using dscr::combinations_tree;
using dscr::operator<<;

// Just sets global variables n and k.
void parse_command_line(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	std::ios_base::sync_with_stdio(false); //this makes terminal output a bit faster.
	
	parse_command_line(argc,argv);
	
	for (auto& x : combinations_tree(n,k))
		cout << x << endl;
	
	return 0;
}

void parse_command_line(int argc, char* argv[])
{
	std::stringstream usage;
	usage	<< "Usage: combinations_tree n k\n"
			<< "Print to STDOUT the set of combinations of size k of a subset of size n,\n"
			<< "where n and k are integers, in lexicographic order.\n"
			<< "Example:\n"
			<< argv[0] << " " << n << " " << k << "\n";
	
	if (argc != 3)
	{
		cout << "Wrong number of arguments!\n";
		cout << usage.str();
		return;
	}
	
	std::vector<std::string> arguments(argv + 1, argv + argc);

	try
	{
		int ntemp = std::stoi(arguments[0]);
		int ktemp = std::stoi(arguments[1]);
		
		if (ktemp < 0)
			throw;
		//only set if there were no errors
		n = ntemp;
		k = ktemp;
	}
	catch (...)
	{
		cout << "Arguments must be numbers!\n";
		cout << usage.str();
		return;
	}
	
}
