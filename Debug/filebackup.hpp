/*!	\file	filebackup.hpp
	\author Sabrina Tessier
	\date	September 25, 2018
	\brief	Provide the declaration of the FileManager class which will be created to copy C++ files from a source directory to a destination directory
*/
#pragma once
#include <filesystem>
#include <iostream>

using ul = uintmax_t;
using namespace std::experimental::filesystem::v1;

/*!	\brief	The FileManager class
*/
class FileManager
{
//CONSTRUCTORS
public:
	FileManager() : sourceDirectory_(_FILESYSTEM_::current_path()), destinationDirectory_("C:\\backup")
	{
		//Initialize FileManager object's collectedFiles_ pointer which points to the vector returned by parsing the directory
		collectedFiles_ = std::make_shared<std::vector<File>>(parseDirectory(sourceDirectory_));
	}
	FileManager(path &sourcePath) : sourceDirectory_(sourcePath), destinationDirectory_("C:\\backup") 
	{
		collectedFiles_ = std::make_shared<std::vector<File>>(parseDirectory(sourceDirectory_));
	}
	FileManager(path &sourcePath, path &destinationPath) : sourceDirectory_(sourcePath), destinationDirectory_(destinationPath) 
	{
		collectedFiles_ = std::make_shared<std::vector<File>>(parseDirectory(sourceDirectory_));
	}

	//COPY CONSTRUCTOR
	FileManager(FileManager &orig)
	{
		//Assign the new FileManager's shared_ptr to the same vector that the original had
		collectedFiles_ = orig.collectedFiles_;
		//Assign the source and destination directory to the same as the original
		sourceDirectory_ = orig.sourceDirectory_;
		destinationDirectory_ = orig.destinationDirectory_;
		//Assign the file backup counter variables to the same as the original
		numberOfBackups_ = orig.numberOfBackups_;
		totalSize_ = orig.totalSize_;
		//Call backup in order to backup the files
		this->backup();
	}

	//COPY ASSIGNMENT OPERATOR
	FileManager& operator = (FileManager const &rhs)
	{
	std::cout << "Copy assignment....\n";
		//Copy all members of the FileManager on the right to the FileManager on the left
		collectedFiles_ = rhs.collectedFiles_;
		sourceDirectory_ = rhs.sourceDirectory_;
		destinationDirectory_ = rhs.destinationDirectory_;
		numberOfBackups_ = rhs.numberOfBackups_;
		totalSize_ = rhs.totalSize_;
		return *this;
	}

	//BLOCK THE MOVE CONSTRUCTOR AND MOVE ASSIGNMENT OPERATOR
	FileManager(FileManager &&) = delete;
	FileManager& operator =(FileManager &&) = delete;

public:
	/*!	\brief Declaration of the backup and report methods which are implemented in filebackup.cpp
	*/
	void backup();
	void report();

private:
	/*!	\brief	The File class, an internal class to the FileManager. Allows for the collection and storage of 'File' objects including the file's path and size in bytes. 
	*/
	class File
	{
		//CONSTRUCTOR
	public:
		File(path p, ul s) : path(p), size(s) {}

		/*!	\brief	Getters and setters for the File class' data members
		*/
		path getPath() const { return path; }
		void setPath(std::string &p) { path = p; }
		ul getSize() const { return size; }
		void setSize(ul &s) { size = s; }

	//DATA MEMBERS
	private:
		path path;
		ul size;
	};
	
//PRIVATE METHODS - ACCESSED FROM INSIDE THE FILEMANAGER CLASS
	/*!	\brief Takes a recursive directory iterator as an argument and returns the size of the file pointed to by the iterator by using the filesystem::file_size method
	*/
	ul getFileSize(recursive_directory_iterator &rit)
	{
		try {
			uintmax_t fileSize = file_size(rit->path());
			return fileSize;
		}
		catch (filesystem_error &ex)
		{
			std::cout << ex.what() << std::endl;
			exit(1);
		}
	}

	/*!	\brief Takes a path as an argument and parses through that directory by using a recursive directory iterator. Constructs File objects and places them into a vector. 
		Once the end of the directory has been reached, the vector filled with File objects is returned
	*/
	std::vector<File> parseDirectory(path &sourcePath)
	{
		//Recursive iterators to parse directory
		recursive_directory_iterator recursiveIt(sourcePath);
		recursive_directory_iterator end;
		//Vector to collect files
		std::vector<File> foundFiles;
		//Parse the directory and collect files that have .c, .cpp, .h, or .hpp extensions
		while (recursiveIt != end)
		{
			if (is_regular_file(recursiveIt->status()))
			{
				//Check if the file is a cpp file
				std::string extension = recursiveIt->path().extension().string();
				if (extension == ".c" || extension == ".cpp" || extension == ".h" || extension == ".hpp")
				{
					//Get the file size
					ul fileSize = getFileSize(recursiveIt);

					//Get the file name which will be used to determine if the file already exists in the vector
					path fileName = recursiveIt->path().filename();

					//Iterate through the foundFiles vector to make sure a file with name does not already exist
					bool fileExists = false;
					for (auto it = foundFiles.begin(); it != foundFiles.end(); ++it)
					{
						if (fileName == it->getPath().filename())
							fileExists = true;
						break;
					}
					if (!fileExists)
					{
						//Construct a File object and add to the vector
						File addFile(recursiveIt->path(), fileSize);
						foundFiles.push_back(addFile);
					}
				}
			}
			++recursiveIt;
		}//end while

		//Return the full vector
		return foundFiles;
	}//end method

	//A path for the source
	path sourceDirectory_;
	//A path for the destination
	path destinationDirectory_;
	//The total number of files that are backed up from the source directory
	ul numberOfBackups_ = 0;
	//The totalsize of the files that are backed up from the source directory
	ul totalSize_ = 0;
	//A shared_ptr to a vector that will hold the files found in the source directory. All copies of the FileManager object will point to the same vector(prevents the vector from having to copied)
	std::shared_ptr<std::vector<File>> collectedFiles_;
};

