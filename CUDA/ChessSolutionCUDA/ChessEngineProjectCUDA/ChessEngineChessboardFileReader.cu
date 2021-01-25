
#include <fstream>

#include "ExceptionsMacro.h"

#include "ChessEngineChessboardFileReader.h"

using namespace std;

ChessEngineChessboardFileReader::ChessEngineChessboardFileReader(const string& FileName, string(*ChessBoardStartData)[MaxChessSizeX][MaxChessSizeY])
{
	try
	{ 
		ifstream ChessBoardFile;

		ChessBoardFile.open(FileName);

		if (ChessBoardFile.is_open() == false)
			throw runtime_error("Can not open chess board file");

		string SeparationLine;

		PosType CX = 0;
		PosType CY = 0;

		for (string s; getline(ChessBoardFile, s, ChessBoardFieldSeparatorChar);)
		{
			if (s.empty() == false && s.front() == EndLineChar)
			{
				getline(ChessBoardFile, SeparationLine, ChessBoardFieldSeparatorChar);
				CY++;
				CX = 0;
			}
			else
			if (CX < MaxChessSizeX && CY < MaxChessSizeY)
			{
				(*ChessBoardStartData)[CX][CY] = s;
				CX++;
			}
		}

		ChessBoardFile.close();
	}
	CATCH_AND_THROW("reading start chess board")
}