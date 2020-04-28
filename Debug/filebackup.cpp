/*!	\file	filebackup.cpp
	\author Sabrina Tessier
	\date	September 25, 2018
	\brief	Provide the implementation of the FileManager class by accepting user arguments to the program and constructing FileManager objects to perform the backup operation
*/

#include "filebackup.hpp"
#include <iostream>
#include <locale>
#include <map>
#include <iomanip>
using namespace std;


/*!	\brief	Implementation of the FileManager::backup method. Iterates through a vector of collected files and copies them to the destination directory
*/
	void FileManager::backup()
	{
		//Create the directory where the backup will go
		create_directory(destinationDirectory_);

		//Iterate through the vector of File objects. For every one, first check if there's an existing file with that name in the destination.
		//If there isn't, copy the file.
		for (File & a : (*collectedFiles_))
		{
			path fileName = a.getPath().filename();
			bool duplicateFound = false;
			//String that will be used to construct a new path
			string newPath = "";
			recursive_directory_iterator destinationIt(destinationDirectory_);
			recursive_directory_iterator end;
			while (destinationIt != end)
			{
				if (fileName == destinationIt->path().filename())
				{
					duplicateFound = true;
					break;
				}

				++destinationIt;
			}//end while

			//If a duplicate was not found, copy the file to the destination and then add a '+' to the path property contained within the current File
			if (!duplicateFound)
			{
				try
				{
					copy(a.getPath(), destinationDirectory_);
				}
				catch (filesystem_error &ex)
				{
					cout << "Unable to copy file: " << a.getPath().filename() << endl;
					cout << ex.what() << endl;
					exit(1);
				}
				newPath = "+" + fileName.string();
			}
			//If a duplicate was found, don't copy the file just add '-' to the front of the path for the purpose of the report function
			else
			{
				newPath = "-" + fileName.string();
			}

			//Modify the File object
			a.setPath(newPath);
		}//end for
	}//end backup

	void FileManager::report()
	{
		//Header for report
		cout << "Root Directory: " << sourceDirectory_ << endl;
		cout << "Destination Directory: " << destinationDirectory_ << "\n\n";
		cout << right << setw(10) << "Status" << setw(17) << "Filename" << setw(27) << "Size\n" << endl;

		//Iterate through the vector of File objects, printing the filename and size properties
		for (File const& a : (*collectedFiles_))
		{
			//Data to be printed
			string print = a.getPath().string();
			//If the file was copied, add the size to the total size and increment the number of backups
			if (print.at(0) == '+')
			{
				++numberOfBackups_;
				totalSize_ += a.getSize();
			}
		
			cout << right << setw(7) << print.at(0) << right << setw(12) << " " << left<< setw(30) << print.substr(1) << left << setw(15) << a.getSize() << endl;
		}
		cout << setfill('=') << setw(60) << "\n" << endl;
		cout << setfill(' ') << "Backup total : " << left << setw(20) << numberOfBackups_ << "Total Size : " << totalSize_ << endl;
		cout << right << setfill('=') << setw(60) << ' ' << endl;
		
		
	}

