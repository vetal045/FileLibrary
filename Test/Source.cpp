#include <iostream>
#include <string>

#include "FileLibrary.h"


void main()
{
	try
	{
		FileLibrary::File f("C://Git/123.txt");

		f.add("1", "13");
		f.add("2", "123");
		
		f.set("2", "5");

		f.save();

		f.print(std::cout);
	}
	catch (std::exception a)
	{
		std::cout << a.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Error" << std::endl;
	}
	system("pause");
}