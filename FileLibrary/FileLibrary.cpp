#include "FileLibrary.h"

#include <exception>
#include <fstream>
#include <string>
#include <algorithm>
#include <list>

namespace FileLibrary
{
	File::File()
	{
		isError_ = false;
		isLoaded_ = false;
		fileFormat_ = Format::KeyValue;
	}

	File::File(const std::string & filePath)
	{
		isError_ = false;
		isLoaded_ = false;
		fileFormat_ = Format::KeyValue;
		filePath_ = filePath;

		if (std::ifstream(filePath_))
		{
			std::cout << "File already exists" << std::endl;
			load(filePath_);
		}
	}

	void File::add(const std::string & key, const std::string & value)
	{
		size_t foundSpace;

		if ((foundSpace = key.find(' ')) == std::string::npos) //if key doesn't contain
		{
			fileKeyValueStorage_.push_back(std::make_pair(key, value));
		}
		else
		{
			std::cout << "Bad enter in the key '" + key + "'. Read rules to right adding for that format of the file!" << std::endl;
		}
	}

	void File::deleteByKey(const std::string & key)
	{
		int count = 0;

		for (const auto& i : fileKeyValueStorage_)
		{
			if (i.first == key)
			{
				fileKeyValueStorage_.erase(fileKeyValueStorage_.begin() + count);
				break;
			}
			else
			{
				++count;
			}
		}
	}

	void File::deleteByValue(const std::string & value)
	{
		int count = 0;

		for (const auto& i : fileKeyValueStorage_)
		{
			if (i.second == value)
			{
				fileKeyValueStorage_.erase(fileKeyValueStorage_.begin() + count);
				break;
			}
			else
			{
				++count;
			}
		}
	}

	void File::deleteKeyValue(const std::string & key, const std::string & value)
	{
		int count = 0;

		for (const auto& i : fileKeyValueStorage_)
		{
			if (i.first == key && i.second == value)
			{
				fileKeyValueStorage_.erase(fileKeyValueStorage_.begin() + count);
				break;
			}
			else
			{
				++count;
			}
		}
	}

	bool File::load(std::string filePath)
	{
		std::ifstream file(filePath, std::ios::in);
		std::vector<std::string> fileData;
		std::string strFile;
		isLoaded_ = true;

		if (!file.is_open() || !file.good())
		{
			isError_ = true;
			isLoaded_ = false;
			throw(std::runtime_error("Doesn't exist or can't open file " + filePath + "."));
		}

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

		fileFormat_ = determineFileFormat(fileData, isLoaded_);

		if (fileFormat_ == Format::KeyValue)
		{
			//setFilePath(filePath);
			loadDataFromFile(fileData);

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
		if (!filePath_.length())
		{
			isError_ = true;
			throw(std::logic_error("No file path specified."));
		}

		return load(filePath_);
	}

	bool File::save()
	{
		/*if (isLoaded_ == false)
		{
		fileFormat_ = Format::KeyValue;
		}*/

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
		// Check that filename is specified
		if (!filePath_.length())
		{
			isError_ = true;
			throw(std::logic_error("No file path specified."));
		}

		std::ifstream file(filePath_, std::ios::in);

		if (!file.is_open() || !file.good())
		{
			isError_ = true;
			throw(std::runtime_error("Doesn't exist or can't open file path " + filePath_ + "."));
		}

		std::streampos fileSize = file.tellg(); // The file pointer is currently at the beginning   

		file.seekg(0, std::ios::end);			  // Place the file pointer at the end of file

		fileSize = file.tellg() - fileSize;
		file.close();

		static_assert(sizeof(fileSize) >= sizeof(long long), "Size of the file is too big. Is is impossible to handle that file with path.");

		return fileSize;
	}

	bool File::formatKeyValue(const std::vector<std::string>& fileData)
	{
		int foundAssign;
		size_t foundSpace;
		std::string key;

		//Checks that key contains spaces or not and strings of file contains symbol '=' or not
		for (const auto& i : fileData)
		{
			foundAssign = i.find('=');

			if (foundAssign != std::string::npos)
			{
				key = i.substr(0, foundAssign);

				if (((foundSpace = key.find(' ')) == std::string::npos) ? true : false)
				{
					if (i == fileData.back())
					{
						return true;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}

		return false;
	}

	bool File::saveToKeyValueFile(std::vector<std::pair<std::string, std::string>> fileKeyValueStorage)
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

	const Format& File::determineFileFormat(const std::vector<std::string>& fileData, bool isLoaded)
	{
		Format a;

		if (formatKeyValue(fileData) == true)
		{
			a = Format::KeyValue;
		}
		else
		{
			if (isLoaded == false)
			{
				a = Format::NotLoaded;
			}
			else
			{
				a = Format::Error;
			}
		}

		return a;
	}

	void File::loadDataFromFile(const std::vector<std::string>& fileData)
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

					fileKeyValueStorage_.push_back(std::make_pair(key, value));
				}
			}
		}
	}

	void File::print()
	{
		if (fileKeyValueStorage_.empty() == true)
		{
			isError_ = true;
			throw(std::logic_error("The file " + filePath_ + " is an empty."));
		}

		std::cout << "===========================================" << std::endl;;
		std::cout << "The file " + filePath_ + " contains: " << std::endl;

		int numb = 0;
		for (const auto& i : fileKeyValueStorage_)
		{
			++numb;
			std::cout << numb << ". Key: " << i.first << ", value: " << i.second << std::endl;
		}

		std::cout << "===========================================" << std::endl;;
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
		if (fileKeyValueStorage_.empty() == true)
		{
			return true;
		}

		return false;
	}

	void File::clear()
	{
		if (fileKeyValueStorage_.empty() == false)
		{
			fileKeyValueStorage_.clear();
		}
	}
}



