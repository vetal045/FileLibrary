#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <map>

//
// Declaration of the namespace FilesFormalLib.
//

namespace FileLibrary
{
	//
	// Declaration of the enum Formal.
	//

	enum class Format
	{
		Error,
		NotLoaded,
		KeyValue
	};

	//
	// Declaration of the class File.
	//

	class File
	{

		//
		// Construction and destruction.
		//

	public:
		File();
		File(const std::string& fileName);

		//
		// Public methods.
		//

	public:

		//! Adds string with parameters such as key and value to data of the file
		void add(const std::string& key, const std::string& value);

		//! Deletes the string from file by specified key
		void deleteByKey(const std::string& key);

		//! Deletes the string from file by specified value
		void deleteByValue(const std::string& value);

		//! Loads an file from a given file path
		//! @Returns true if the file was successfully loaded and contained normal format of data
		bool load(const std::string& filePath);

		//! @Returns true if the file was successfully loaded and contained normal format of data
		bool load();

		//! Saves an file to a given file path
		//! @Returns true if the file was successfully saved
		bool save();

		//! Sets path of file
		void setFilePath(std::string filePath);

		//! Returns path of file
		const std::string& getFilePath() const;

		//! Returns size of current file
		long long getFileSize();

		//! Prints content of the file
		void print(std::ostream& os);

		//! Clears current data of the file
		void clear();

		//! Changes value by key
		void set(const std::string& key, const std::string& value);

		//! Returns certain note by key
		std::string get(const std::string& key);

		//! Returns the true if file contains the note by specified key
		bool contains(const std::string& key);

		//
		// Private methods.
		//

	private:
		//! Checks format of the file
		//! @Returns true if the file had format like KeyValue
		bool formatKeyValue(const std::vector<std::string>& fileData);

		//! Saves data to KeyValue format of the file
		bool saveToKeyValueFile(const std::map<std::string, std::string>& fileKeyValueStorage);

		//! @Returns an object with normal format of the file
		const Format& determineFileFormat(const std::vector<std::string>& fileData);

		//! Load all data from file to vector of the pair strings 
		void loadDataFromKeyValueFile(const std::vector<std::string>& fileData);

		//! @Returns the error flag
		bool errorOccured();

		//! Assigns the error flag to a false
		void cleanErrorFlag();

		//! Returns true if the file is clear
		bool isClear();

		//! Defines the size of current file
		long long defineFileSize(std::ifstream& file);

		//
		// Private data members.
		//

	private:
		std::string filePath_;
		int fileSize_;
		bool isError_;
		Format fileFormat_;
		//std::vector<std::pair<std::string, std::string>> fileKeyValueStorage_;
		std::map<std::string, std::string> fileKeyValueStorage_;
	};
}