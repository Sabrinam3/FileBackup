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
		//Iterate through the vector of files, for each of the paths, perform a copy operation
		for (File & a : (*collectedFiles_))
		{
			bool wasCopied = copy_file(a.getPath(), destinationDirectory_ / a.getPath().filename(), copy_options::skip_existing);
			//If the file was copied, change the status member of the file object to true
			if (wasCopied)
			{
				a.setStatus(true);
			}
		}//end for
	}//end backup

	void FileManager::report()
	{
		//Header for report
		cout << left<< setw(25) << "Root Directory: " << sourceDirectory_ << endl;
		cout << left << setw(25) << "Destination Directory: " << destinationDirectory_ << "\n\n";
		cout << right << setw(10) << "Status" << setw(17) << "Filename" << setw(27) << "Size\n" << endl;

		//Iterate through the vector of File objects, printing the filename and size properties
		for (File const& a : (*collectedFiles_))
		{
			//If the file was copied, add the size to the total size and increment the number of backups. Also print a '+'
			if (a.getStatus())
			{
				++numberOfBackups_;
				totalSize_ += a.getSize();
				cout << right << setw(7) << "+";
			}
			else
			{
				cout << right << setw(7) << "-";
			}
			cout << right << setw(12) << " " << left<< setw(30) << a.getPath().filename() << left << setw(15) << a.getSize() << endl;
		}
		cout << setfill('=') << setw(60) << "\n" << endl;
		cout << setfill(' ') << "Backup total : " << left << setw(20) << numberOfBackups_ << "Total Size : " << totalSize_ << endl;
		cout << right << setfill('=') << setw(60) << ' ' << endl;
			
	}//end report

