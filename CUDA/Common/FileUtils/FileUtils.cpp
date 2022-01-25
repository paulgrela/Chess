
#include <fstream>
#include <sstream>

#include "ExceptionsMacro.h"

#include "FileUtils.h"

using namespace std;

namespace FileUtils
{
	void RewriteTextToFile(const char* FileName, const char* TextToAppend)
	{
		try
		{
			ofstream FileToWriteText;
			FileToWriteText.open(FileName, ios_base::out | ios_base::trunc);
			if (FileToWriteText.is_open() == false)
				throw runtime_error("Can not open file to rewrite text");
			FileToWriteText << string(TextToAppend) << endl;
			FileToWriteText.close();
		}
		CATCH_AND_THROW("rewriting text to file")
	}

	void AppendTextToFile(const char* FileName, const char* TextToAppend)
	{
		try
		{
			ofstream FileToWriteText;
			FileToWriteText.open(FileName, ios_base::app);
			if (FileToWriteText.is_open() == false)
				throw runtime_error("Can not open file to append text");
			FileToWriteText << string(TextToAppend) << endl;
			FileToWriteText.close();
		}
		CATCH_AND_THROW("appending text to file")
	}

	string ReadFirstLineFromFile(const char* FileName)
	{
		string LineStr = "";

		try
		{

			ifstream FileToReadText;
			FileToReadText.open(FileName);
			if (FileToReadText.is_open() == false)
				throw runtime_error("Can not open file to read first line");
			getline(FileToReadText, LineStr);
			FileToReadText.close();
		}
		CATCH_AND_THROW("reading first line from file")

		return LineStr;
	}
}