#pragma once

#ifndef _CHESS_ENGINE_CHESS_BOARD_FILE_READER_H_
#define _CHESS_ENGINE_CHESS_BOARD_FILE_READER_H_

#include <string>

#include "ChessEngineTypes.h"
#include "ChessEngineConstants.h"

class ChessEngineChessboardFileReader
{
public:
	ChessEngineChessboardFileReader(const std::string& FileName, std::string (*ChessBoardStartData)[MaxChessSizeX][MaxChessSizeY]);
};

#endif