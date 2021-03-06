// Copyright(c) 1999-2018 ASL author
// Licensed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef ASL_INIFILE_H
#define ASL_INIFILE_H

#include <asl/Map.h>
#include <asl/HashMap.h>

namespace asl {

/**
A utility to read and write configuration files in INI format. On destruction, if there was any variable added or modified
the file is automatically saved. When reading a variable name, it is read from the current section. But if the name
contains a `/` then it is considered a "section/name" pair.

Example:

~~~
[main]
color = white

[network]
num_retries = 3
~~~

Can be read and written with:

~~~
IniFile config("settings.ini");
int num_retries = config["network/num_retries"];
config["main/color"] = "black";
~~~

The file in that example will be saved if the original `color` in section `main` was not "black".

The file can be cheked for correct reading and whether a variable exists:

~~~
IniFile config("settings.ini");
if(!config.ok()) {
	printf("Error reading %s\n", *config.fileName());
	return;
}
int num_retries = 5;
if(config.has("network/num_retries"))
	num_retries = config["network/num_retries"];
~~~

That last part can also be written as:

~~~
int num_retries = config("network/num_retries", 5);
~~~
*/

class ASL_API IniFile
{
	String _currentTitle;
	String _filename;
	Array<String> _lines;
	bool _modified;
	bool _shouldwrite;
	bool _ok;
public:
	class Section
	{
		String _title;
		HashDic<String> _vars;
	public:
		Section() {}
		Section(const String& t) : _title(t) {}
		const String& title() const {return _title;}
		String& operator[](const String& k) {return _vars[k];}
		const HashDic<String>& vars() const {return _vars;}
		friend class IniFile;
	};
	Dic<Section> sections;

	/**
	Opens an INI file from the given file
	*/
	IniFile(const String& fname, bool shouldwrite = true);
	/**
	Destroys the object and save the file if there were modifications
	*/
	~IniFile();

	/**
	Returns true if the file was read correctly
	*/
	bool ok() const {return _ok;}

	/**
	Returns the file name of this IniFile
	*/
	const String& fileName() const {return _filename;} 

	/**
	Returns the value associated with the key `name` in the current section
	if the name is like `section/name` then the given section is used.
	*/
	String& operator[](const String& name);
	
	/**
	Returns the value associated with the key `name` or `defaultVal` if it was not found. 
	*/
	String operator()(const String& name, const String& defaultVal);

	void write(const String& fname="");

	/**
	Sets the current section to 'name'
	*/
	Section& section(const String& name);

	/**
	Returns true if the file contains a key named `name`. If the name is like
	`section/name` then function tests a variable `name` in section `section`.
	*/
	bool has(const String& name) const;

	/**
	Returns the length of an "array" named `name` as written by the Qt library and enables reading its values.
	*/
	int arraysize(const String& name);

	/**
	Returns the value associated with field `name` at the array position `index` of the array last specified
	with `arraysize()`.
	*/
	String array(const String& name, int index);
};
}
#endif
