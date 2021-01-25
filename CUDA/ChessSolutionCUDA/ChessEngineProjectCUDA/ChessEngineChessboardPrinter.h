#pragma once

#ifndef _CHESS_BOARD_PRINTER_H_
#define _CHESS_BOARD_PRINTER_H_

#include <string>
#include <ostream>

#include <functional>

class ChessEngineAllPossibleMovesGenerator;

class ChessEngineChessboardPrinter
{
private:
    ChessEngineAllPossibleMovesGenerator& ChessEngineAllPossibleMovesGeneratorLocalReference;
public:
    ChessEngineChessboardPrinter(ChessEngineAllPossibleMovesGenerator& ChessEngineAllPossibleMovesGeneratorLocalReferenceParameter) : ChessEngineAllPossibleMovesGeneratorLocalReference(ChessEngineAllPossibleMovesGeneratorLocalReferenceParameter)
    {
    }
private:
    static std::mutex CoutForPrintingChessBoardInColorsMutexObject;
private:
    void CleanScreen() const noexcept;
    void PrintChessBoardLetters(std::ostream& StreamObject) const noexcept;
    void PrintChessBoardHorizontalSeparationLine(std::ostream& StreamObject) const noexcept;
    void PrintChessBoardAndPieces(std::ostream& StreamObject, bool LogColors) const noexcept;
    std::ostream& PrintActualChessBoardState(std::ostream&& StreamObject, bool LogColors) const;
public:
    void LogPrintActualChessBoardStateToStreams(std::function<void(LoggersManager*, const std::stringstream & Message)> LogFunction, uint64_t EndLineCounter1);
};

#endif