#include "filebackup.hpp"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	//Locale
	locale here{ "" };
	cout.imbue(here);

	//Determine which constructor should be called, based on the number of arguments

	//One argument - run default implementation
	if (argc == 1)
	{
		//Create a FileManager object using the default constructor
		FileManager fileMan;
		//Call backup which copies files from user's CWD to C:\backup
		fileMan.backup();
		fileMan.report();
	}
	else if (argc == 2)
	{
		path sourcePath = argv[1];
		FileManager fileMan(sourcePath);
		//Use a copy constructor to create a copy of the FileManager object which then performs the backup on the new copy, into C:\backup
		FileManager copy(fileMan);
		copy.report();
	}
	else if (argc == 3)
	{
		path sourcePath = argv[1];
		path destinationPath = argv[2];
		FileManager fileMan(sourcePath, destinationPath);
		//Call backup on fileMan, which calls report from within
		fileMan.backup();
		fileMan.report();
	}
	//Else, the user has entered an incorrect number of inputs. Tell the user this and tell the user the appropriate use of the program
	else
	{
		cerr << "Incorrect number of arguments for filebackup.exe\nUse: filebackup [SourceDirectory] [backupDirectory] " << endl;
	}
}