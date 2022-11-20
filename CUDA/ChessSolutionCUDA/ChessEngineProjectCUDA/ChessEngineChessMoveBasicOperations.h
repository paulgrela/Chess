#pragma once

#ifndef _CHESS_ENGINE_MOVE_BASIC_OPERATIONS_H
#define _CHESS_ENGINE_MOVE_BASIC_OPERATIONS_H

#include <string>
#include <vector>

#include "ChessEngineTypes.h"

std::string GetChessMoveAsString(const PosType cx1, const PosType cy1, const PosType cx2, const PosType cy2);
std::string GetChessMovesPathAsString(const std::vector<ChessMove>& MovesPath);

#endif