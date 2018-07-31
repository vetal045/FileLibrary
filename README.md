# FileLibrary

A simple C++ library for reading and writing files of certain format.

Current supported formats:

- KeyValue

The format of KeyValue is sent to the file in the form: 

key=value

where:

key - value of letters and numbers without spaces,
value - anything.


# Author

FileLibrary is written and maintained by vetal045.


# Settings  

I. Creating a C ++ console application in Visual Studio, referring to a static library

1. On the menu bar, click File, New, Project.

2. In the left pane, in the Visual C++ category, select Win32.

3. In the central area, select Win32 Console Application.

4. Specify the project name, for example Source, in the Name field. Click OK.

5. On the Overview page of the Win32 Application Wizard dialog box, click Next.

6. On the Application Settings page, in the Application Type field, select Console Application.

7. Click Finish to create the project.

II. Settings to using the functionality from the static library in the application

1. On Property Pages, go to Сonfiguration properties->Catalogies VC++->Include Directories and provide the path, where the header file of the library that you want to use is located.

2. Then go to Сonfiguration properties->Catalogies VC++->Library Directories and specify the path, where your .lib file is located.

3. Finally, go to Linker->Input->Additional Dependencies and add the name of the file containing your library together with its extension (e.g. example_library.lib). 

4. At the end, click apply and OK.

5. To use static library write(for example, in Source.cpp file) so:

#include "FileLibrary.h"

# Usage

# Create an File object:

#include "FileLibrary.h"

FileLibrary::File f;

# Load a file:

f.load("C:\\files\\fileName");

Or

f.load(); // if current path of file object is specified

# Add new string of KeyValue format

f.add("qwerty","123456");

# Save changes to file

f.save();

# Print results of loaded file

f.print();

# Delete any string from data of file

f.deleteByKey("qwerty"); //using key

f.deleteByValue("123456"); //using value

f.deleteKeyValue("qwerty","123456"); //using both

# Set file path

f.setFilePath("C:\\files\\fileName");

# Get file path

f.getFilePath();

# Get size of the file

f.getFileSize();

# Set new value by key to data of file

f.setValueByKey("qwerty","changedValue");

# Get value by get from file

f.getValueByKey("qwerty");


# Versions

Test version - 5th May 2018


# License
Copyright (c) 2017 vetal045

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see http://www.gnu.org/licenses/..
