#pragma once

#ifndef _CHESS_ENGINE_CHESS_MOVE_BASIC_OPERATIONS_H
#define _CHESS_ENGINE_CHESS_MOVE_BASIC_OPERATIONS_H

#include <string>
#include <vector>

#include "ChessEngineTypes.h"

std::string GetChessMoveAsString(const PosType cx1, const PosType cy1, const PosType cx2, const PosType cy2);
std::string GetChessMovesPathAsString(const std::vector<ChessMove>& MovesPath);
std::string GetChessMovesPathAsString(const ChessScoredMovesPath& MovesPathObject);

void CopyChessMovesPathsVector(const LengthType DeepLevelPathLength, ChessMove* const OneMovesPath, const ChessMove* const ActuallyInvestigatedMovesPath);
std::vector<ChessMove> GetChessMovesPathVector(const LengthType DeepLevelPathLength, const ChessMove* const ActuallyInvestigatedMovesPath);

#endif