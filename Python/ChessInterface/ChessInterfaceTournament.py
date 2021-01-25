from ChessInterfaceConstants import ChessInterfaceConstants
import ChessInterfaceFiles

from distutils.dir_util import copy_tree

import os

class ChessInterfaceTournament:

    @classmethod
    def WriteTournamentResultsAndStatisticsToFileAndListbox(cls, ChessInterfaceFrameObject, EnginesVictories):

        with open(os.path.join(ChessInterfaceFiles.GetActualTournamentStartDateTimeFromFile(ChessInterfaceFrameObject.Mode), ChessInterfaceFiles.FileConstants.this_tournament_results_and_statistics_history_txt), "w") as ThisTournamentResultsAndStatisticsHistoryFile:

            ThisTournamentResultsAndStatisticsHistoryFile.write("TOURNAMENT RESULTS:\n")

            for EngineVictories in EnginesVictories:
                ThisTournamentResultsAndStatisticsHistoryFile.write("\n")
                ThisTournamentResultsAndStatisticsHistoryFile.write("Engine name = " + ChessInterfaceFrameObject.ListOfEnginesAndConfigsToRun[EngineVictories[0]][2] + "\n\n")
                ThisTournamentResultsAndStatisticsHistoryFile.write("Number of victories as white = " + str(EngineVictories[1]) + " Percent of victories as white = " + "{0:0.2f}".format(EngineVictories[2]) + "\n")
                ThisTournamentResultsAndStatisticsHistoryFile.write("Number of victories as black = " + str(EngineVictories[3]) + " Percent of victories as black = " + "{0:0.2f}".format(EngineVictories[4]) + "\n")
                ThisTournamentResultsAndStatisticsHistoryFile.write("Total number of victories = " + str(EngineVictories[5]) + " Percent of total number of victories = " + "{0:0.2f}".format(EngineVictories[6]) + "\n")

            ThisTournamentResultsAndStatisticsHistoryFile.write("\n\n")

            print(str(len(ChessInterfaceFrameObject.TournamentResultsList)))

            GameNumber = 1
            for TournamentResult in ChessInterfaceFrameObject.TournamentResultsList:
                ThisTournamentResultsAndStatisticsHistoryFile.write("Game " + str(GameNumber) + " Result : " + TournamentResult[0] + "\n")
                ThisTournamentResultsAndStatisticsHistoryFile.write("Engine Local (White) = " + ChessInterfaceFrameObject.ListOfEnginesAndConfigsToRun[TournamentResult[1]][2] + " " + ChessInterfaceFrameObject.ListOfEnginesAndConfigsToRun[TournamentResult[1]][3] + "\n")
                ThisTournamentResultsAndStatisticsHistoryFile.write("Engine Partner (Black) = " + ChessInterfaceFrameObject.ListOfEnginesAndConfigsToRun[TournamentResult[2]][2] + " "  + ChessInterfaceFrameObject.ListOfEnginesAndConfigsToRun[TournamentResult[2]][3] + "\n")
                ThisTournamentResultsAndStatisticsHistoryFile.write("Game index = " + str(TournamentResult[3]) + "\n")
                ThisTournamentResultsAndStatisticsHistoryFile.write("\n")
                GameNumber += 1

        ChessInterfaceFrameObject.TournamentResultsListbox.delete(0, "end")

        with open(os.path.join(ChessInterfaceFiles.GetActualTournamentStartDateTimeFromFile(ChessInterfaceFrameObject.Mode), ChessInterfaceFiles.FileConstants.this_tournament_results_and_statistics_history_txt), "r") as ThisTournamentResultsAndStatisticsHistoryFile:

            ThisTournamentResultsAndStatisticsHistoryFileLines = ThisTournamentResultsAndStatisticsHistoryFile.readlines()
            ThisTournamentResultsAndStatisticsHistoryLinesIndex = 1
            for ThisTournamentStatisticsAndGamesHistoryFileLine in ThisTournamentResultsAndStatisticsHistoryFileLines:
                ChessInterfaceFrameObject.TournamentResultsListbox.insert(ThisTournamentResultsAndStatisticsHistoryLinesIndex, ThisTournamentStatisticsAndGamesHistoryFileLine)
                ThisTournamentResultsAndStatisticsHistoryLinesIndex += 1

    @classmethod
    def MakeEndOfTournamentAndCountTournamentStatistics(cls, ChessInterfaceFrameObject):

        print(ChessInterfaceFrameObject.TournamentResultsList)

        ChessInterfaceFrameObject.EngineNumberForOpponentEntrybox["state"] = "normal"
        ChessInterfaceFrameObject.NewTournamentButton["state"] = "normal"
        ChessInterfaceFrameObject.FirstMoveInNewGameFromEngineVar.set(0)
        ChessInterfaceFrameObject.TournamentStopped = True

        EnginesVictories = []
        for EngineIndex in range(ChessInterfaceFrameObject.ListOfEnginesListbox.size()):

            TournamentTotalNumberOfVictoriesResultIndex = 0
            TournamentNumberOfVictoriesAsWhiteResultIndex = 0
            TournamentNumberOfVictoriesAsBlackResultIndex = 0

            for TournamentResult in ChessInterfaceFrameObject.TournamentResultsList:

                if TournamentResult[1] == EngineIndex and TournamentResult[2] != EngineIndex and TournamentResult[0] == ChessInterfaceConstants.MATE_WHITE_WINS:
                    TournamentTotalNumberOfVictoriesResultIndex += 1
                    TournamentNumberOfVictoriesAsWhiteResultIndex += 1
                if TournamentResult[2] == EngineIndex and TournamentResult[1] != EngineIndex and TournamentResult[0] == ChessInterfaceConstants.MATE_BLACK_WINS:
                    TournamentTotalNumberOfVictoriesResultIndex += 1
                    TournamentNumberOfVictoriesAsBlackResultIndex += 1

            EnginesVictories.append((EngineIndex, TournamentNumberOfVictoriesAsWhiteResultIndex, TournamentNumberOfVictoriesAsWhiteResultIndex / len(ChessInterfaceFrameObject.TournamentResultsList), TournamentNumberOfVictoriesAsBlackResultIndex, TournamentNumberOfVictoriesAsBlackResultIndex / len(ChessInterfaceFrameObject.TournamentResultsList), TournamentTotalNumberOfVictoriesResultIndex, TournamentTotalNumberOfVictoriesResultIndex / len(ChessInterfaceFrameObject.TournamentResultsList)))

        print(EnginesVictories)

        cls.WriteTournamentResultsAndStatisticsToFileAndListbox(ChessInterfaceFrameObject, EnginesVictories)

    @classmethod
    def SendNewGameCommandToOpponentPartner(cls, ChessInterfaceFrameObject):

        ChessInterfaceFrameObject.TournamentStatusLabel.config(text = "Local = " + str(ChessInterfaceFrameObject.SelectedChessEngineIndex) + " Partner = " + str(ChessInterfaceFrameObject.TournamentPartnerEngineIndex) + " Repeat = " + str(ChessInterfaceFrameObject.TournamentGameIndex))

        ChessInterfaceFrameObject.ChessInterfaceEngineObject.ChessNetSocketObject.SocketConnection.send((ChessInterfaceConstants.STOP_GAME + "#").encode())
        ChessInterfaceFrameObject.StopGame()
        ChessInterfaceFrameObject.ChessInterfaceEngineObject.ChessNetSocketObject.SocketConnection.send((ChessInterfaceConstants.NEW_GAME + "|" + ChessInterfaceFrameObject.EngineNumberForOpponentEntryboxVar.get() + "|" + "#").encode())
        ChessInterfaceFrameObject.NewGame()
        ChessInterfaceFrameObject.ChessInterfaceEngineObject.ChessNetSocketObject.SocketConnection.send((ChessInterfaceConstants.START_GAME + "|" + ChessInterfaceFrameObject.EngineNumberForOpponentEntryboxVar.get() + "|" + "#").encode())
        ChessInterfaceFrameObject.StartGame()

    @classmethod
    def UpdateTournamentStatus(cls, ChessInterfaceFrameObject, MateStr):

        if ChessInterfaceFrameObject.AreEnginesDifferentInGameInTournament() is True:
            NewGameDirectory = os.path.join(ChessInterfaceFiles.GetActualTournamentStartDateTimeFromFile(ChessInterfaceFrameObject.Mode), ChessInterfaceFiles.GetActualGameStartDateTimeFromFile(ChessInterfaceFrameObject.Mode).split(os.sep)[-1])
            os.makedirs(NewGameDirectory)
            copy_tree(ChessInterfaceFiles.GetActualGameStartDateTimeFromFile(ChessInterfaceFrameObject.Mode), NewGameDirectory)

            ChessInterfaceFrameObject.TournamentResultsList.append((MateStr, ChessInterfaceFrameObject.SelectedChessEngineIndex, ChessInterfaceFrameObject.TournamentPartnerEngineIndex, ChessInterfaceFrameObject.TournamentGameIndex))

        if ChessInterfaceFrameObject.TournamentGameIndex < int(ChessInterfaceFrameObject.UseEngineRepeatNumberInOneTournamentEntryboxVar.get()):
            ChessInterfaceFrameObject.TournamentGameIndex += 1

            if ChessInterfaceFrameObject.AreEnginesDifferentInGameInTournament() is True:
                cls.SendNewGameCommandToOpponentPartner(ChessInterfaceFrameObject)
                return
            else:
                cls.UpdateTournamentStatus(ChessInterfaceFrameObject, MateStr)
        else:
            ChessInterfaceFrameObject.TournamentGameIndex = 1
            if ChessInterfaceFrameObject.TournamentPartnerEngineIndex < ChessInterfaceFrameObject.ListOfEnginesListbox.size() - 1:
                ChessInterfaceFrameObject.TournamentPartnerEngineIndex += 1
                ChessInterfaceFrameObject.EngineNumberForOpponentEntryboxVar.set(ChessInterfaceFrameObject.TournamentPartnerEngineIndex)

                if ChessInterfaceFrameObject.AreEnginesDifferentInGameInTournament() is True:
                    cls.SendNewGameCommandToOpponentPartner(ChessInterfaceFrameObject)
                    return
                else:
                    cls.UpdateTournamentStatus(ChessInterfaceFrameObject, MateStr)
            else:
                ChessInterfaceFrameObject.TournamentPartnerEngineIndex = 0
                ChessInterfaceFrameObject.EngineNumberForOpponentEntryboxVar.set(ChessInterfaceFrameObject.TournamentPartnerEngineIndex)
                if ChessInterfaceFrameObject.SelectedChessEngineIndex < ChessInterfaceFrameObject.ListOfEnginesListbox.size() - 1:
                    ChessInterfaceFrameObject.SelectedChessEngineIndex += 1
                    ChessInterfaceFrameObject.ListOfEnginesListbox.selection_clear(0, "end")
                    ChessInterfaceFrameObject.ListOfEnginesListbox.selection_set(ChessInterfaceFrameObject.SelectedChessEngineIndex)
                    ChessInterfaceFrameObject.SelectedChessEngineLabel.config(text = ChessInterfaceFrameObject.ListOfEnginesAndConfigsToRun[ChessInterfaceFrameObject.SelectedChessEngineIndex][2])

                    if ChessInterfaceFrameObject.AreEnginesDifferentInGameInTournament() is True:
                        cls.SendNewGameCommandToOpponentPartner(ChessInterfaceFrameObject)
                        return
                    else:
                        cls.UpdateTournamentStatus(ChessInterfaceFrameObject, MateStr)
                else:
                    cls.MakeEndOfTournamentAndCountTournamentStatistics(ChessInterfaceFrameObject)