#include "FileLibrary.h"

#include <exception>
#include <fstream>
#include <string>
#include <algorithm>
#include <list>

namespace FileLibrary 
{
	File::File() : isError_(false), fileFormat_(Format::KeyValue), fileSize_(0)
	{ }

	File::File(const std::string & filePath) : isError_(false),  
		fileFormat_ (Format::KeyValue), filePath_ (filePath), fileSize_(0)
	{
		load(filePath_);
	}

	void File::add(const std::string & key, const std::string & value)
	{
		if (key.find(' ') == std::string::npos) //if key doesn't contain
		{
			//fileKeyValueStorage_.push_back(std::make_pair(key, value));
			fileKeyValueStorage_.insert(std::pair<std::string, std::string>(key, value));
		}
		else
		{
			std::cout << "Bad enter in the key '" + key + "'. Read rules to right adding for that format of the file!" << std::endl;
		}
	}

	void File::deleteByKey(const std::string & key)
	{
		std::map<std::string, std::string>::iterator it = fileKeyValueStorage_.find(key);

		fileKeyValueStorage_.erase(it);
	}

	void File::deleteByValue(const std::string & value)
	{
		std::map<std::string, std::string>::iterator it = fileKeyValueStorage_.begin();

		for (const auto& i : fileKeyValueStorage_)
		{
			if (i.second == value)
			{
				fileKeyValueStorage_.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}
	}


	bool File::load(const std::string& filePath)
	{
		std::ifstream file(filePath, std::ios::in);
		std::vector<std::string> fileData;
		std::string strFile;

		if (!file.is_open() || !file.good())
		{
			isError_ = true;
			fileFormat_ = Format::NotLoaded;
			throw(std::runtime_error("Doesn't exist or can't open file " + filePath + "."));
		}

		fileSize_ = defineFileSize(file);

		/*if (filePath_ == getFilePath())
		{
		isError_ = true;
		std::cout << "File " + filePath_ + " is already loaded." << std::endl;

		return true;
		}*/

		while (std::getline(file, strFile))
		{
			fileData.push_back(strFile);
		}

		fileFormat_ = determineFileFormat(fileData);

		if (fileFormat_ == Format::KeyValue)
		{
			//setFilePath(filePath);
			loadDataFromKeyValueFile(fileData);

			return true;
		}
		else if (fileFormat_ == Format::NotLoaded)
		{
			std::cout << "Type of the file " + filePath + ": " << "NotLoaded" << std::endl;
			return false;
		}
		else
		{
			std::cout << "Type of the file " + filePath + ": " << "Error. The file is not correct format." << std::endl;
			return false;
		}
	}

	bool File::load()
	{
		// Check that filename is specified
		if (filePath_.empty())
		{
			isError_ = true;
			throw(std::logic_error("No file path specified."));
		}

		return load(filePath_);
	}

	bool File::save()
	{

		if (fileFormat_ == Format::KeyValue)
		{
			saveToKeyValueFile(fileKeyValueStorage_);

			return true;
		}

		return false;
	}

	void File::setFilePath(std::string filePath)
	{
		filePath_ = filePath;
	}

	const std::string & File::getFilePath() const
	{
		return filePath_;
	}

	long long File::getFileSize()
	{
		return fileSize_;
	}

	bool File::formatKeyValue(const std::vector<std::string>& fileData)
	{
		int foundAssign;
		std::string key;

		for (const auto& i : fileData)
		{
			foundAssign = i.find('=');

			if (foundAssign == std::string::npos)
			{
				return false;
			}

			key = i.substr(0, foundAssign);

			if (key.find(' ') != std::string::npos)
			{
				return false;
			}
		}

		return true;
	}

	bool File::saveToKeyValueFile(const std::map<std::string, std::string>& fileKeyValueStorage)
	{
		std::ofstream file;

		file.open(filePath_, std::ofstream::out);

		if (!file.is_open())
		{
			isError_ = true;
			throw(std::runtime_error("Can't open the file '" + filePath_ + "' to save changes.\n"));
		}

		for (const auto& i : fileKeyValueStorage_)
		{
			file << i.first + "=" + i.second << std::endl;
		}

		file.close();

		return true;
	}

	const Format& File::determineFileFormat(const std::vector<std::string>& fileData)
	{
		Format a;

		if (formatKeyValue(fileData) == true)
		{
			a = Format::KeyValue;
		}
		else
		{
			if (fileFormat_ == Format::NotLoaded)
			{
				return fileFormat_;
			}
			else
			{
				a = Format::Error;
			}
		}

		return a;
	}

	void File::loadDataFromKeyValueFile(const std::vector<std::string>& fileData)
	{
		int foundAssign, foundSpace;
		std::string key, value;

		if (fileKeyValueStorage_.empty() == false)
		{
			fileKeyValueStorage_.clear();
		}

		for (const auto& i : fileData)
		{
			foundAssign = i.find('=');

			if (foundAssign != std::string::npos)
			{
				key = i.substr(0, foundAssign);

				if ((foundSpace = key.find(' ') == std::string::npos) ? true : false)
				{
					value = i.substr(foundAssign + 1);

					fileKeyValueStorage_.insert(std::pair<std::string, std::string>(key, value));
				}
			}
		}
	}

	void File::print(std::ostream& os)
	{
		if (fileKeyValueStorage_.empty() == true)
		{
			isError_ = true;
			os<< "The file " + filePath_ + " is an empty.";
		}
		else
		{
			os << "===========================================" << std::endl;;
			os << "The file " + filePath_ + " contains: " << std::endl;

			int numb = 0;
			for (const auto& i : fileKeyValueStorage_)
			{
				++numb;
				os << numb << ". Key: " << i.first << ", value: " << i.second << std::endl;
			}
			
			os << "===========================================" << std::endl;
		}
	}

	bool File::errorOccured()
	{
		return isError_;
	}

	void File::cleanErrorFlag()
	{
		isError_ = false;
	}

	bool File::isClear()
	{
		return fileKeyValueStorage_.empty();
	}

	long long File::defineFileSize(std::ifstream & file)
	{
		std::streampos fileSize = file.tellg(); // The file pointer is currently at the beginning   

		file.seekg(0, std::ios::end);			  // Place the file pointer at the end of file

		fileSize = file.tellg() - fileSize;
		file.close();

		static_assert(sizeof(fileSize) >= sizeof(long long), "Size of the file is too big. Is is impossible to handle that file with path.");

		return fileSize;
	}

	void File::clear()
	{
		if (fileKeyValueStorage_.empty() == false)
		{
			fileKeyValueStorage_.clear();
		}
	}

	void File::set(const std::string & key, const std::string& value)
	{
		std::map<std::string, std::string>::iterator it = fileKeyValueStorage_.find(key);

		if (it != fileKeyValueStorage_.end())
			it->second = value;
	}

	std::string File::get(const std::string & key)
	{
		std::string value;

		for (const auto& i : fileKeyValueStorage_)
		{
			if (i.first == key)
			{
				value = i.second;
				break;
			}
		}

		return value;
	}

	bool File::contains(const std::string & key)
	{
		std::map<std::string, std::string>::iterator it = fileKeyValueStorage_.find(key);

		if (it == fileKeyValueStorage_.end())
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}



