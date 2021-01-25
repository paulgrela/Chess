from ChessInterfaceConstants import ChessInterfaceConstants

import ChessInterfaceFiles

from socket import socket, AF_INET, SOCK_STREAM
from threading import Thread
from time import sleep

class NetworkConfig:
    ServerHost = ""
    ClientHost = "localhost"
    Port = 50005

def PrintReceivedData(ReceivedData):
    if len(ReceivedData) > 13:
        if ReceivedData[14:16] == "o0":
            if ReceivedData[8] == "c":
                print(ReceivedData[2:4].upper(), "->", ReceivedData[8:10].upper(), "(0-0-0)")
            else:
                print(ReceivedData[2:4].upper(), "->", ReceivedData[8:10].upper(), "(0-0)")
        else:
            print(ReceivedData[2:4].upper(), "->", ReceivedData[8:10].upper(), "#" + ReceivedData[14:16].upper())
    else:
        print(ReceivedData[2:4].upper(), "->", ReceivedData[8:10].upper())

def SelectChessEngine(ChessInterfaceFrameObject, ReceivedData, GameText):

    ChessInterfaceFrameObject.SelectedChessEngineIndex = int(ReceivedData[len(GameText) + 1:len(GameText) + 1 + ReceivedData[len(GameText) + 1:len(ReceivedData):1].find("|"):1])

    if ChessInterfaceFrameObject.SelectedChessEngineIndex < 0:
        ChessInterfaceFrameObject.SelectedChessEngineIndex = 0
    if ChessInterfaceFrameObject.SelectedChessEngineIndex >= ChessInterfaceFrameObject.ListOfEnginesListbox.size():
        ChessInterfaceFrameObject.SelectedChessEngineIndex = ChessInterfaceFrameObject.ListOfEnginesListbox.size() - 1

    ChessInterfaceFrameObject.ListOfEnginesListbox.selection_clear(0, "end")
    ChessInterfaceFrameObject.ListOfEnginesListbox.selection_set(ChessInterfaceFrameObject.SelectedChessEngineIndex)
    ChessInterfaceFrameObject.SelectedChessEngineLabel.config(text = ChessInterfaceFrameObject.ListOfEnginesAndConfigsToRun[ChessInterfaceFrameObject.SelectedChessEngineIndex][2])

def ReceiveCommand(SocketConnection, ChessInterfaceFrameObject):
    ReceivedData = None
    try:
        ReceivedData = SocketConnection.recv(1024)
    except OSError:
        print("CLOSED CONNECTION AFTER RECEIVE")
        ChessInterfaceFrameObject.CloseThisProcess()

    if ReceivedData:
        ReceivedData = ReceivedData.decode()

        while len(ReceivedData) > 1:
            if ReceivedData[0:len(ChessInterfaceConstants.NEW_GAME)] == ChessInterfaceConstants.NEW_GAME:
                print(ReceivedData.upper())
                SelectChessEngine(ChessInterfaceFrameObject, ReceivedData, ChessInterfaceConstants.NEW_GAME)
                ChessInterfaceFrameObject.NewGame()
            elif ReceivedData[0:len(ChessInterfaceConstants.STOP_GAME)]  == ChessInterfaceConstants.STOP_GAME:
                print(ReceivedData.upper())
                ChessInterfaceFrameObject.StopGame()
            elif ReceivedData[0:len(ChessInterfaceConstants.START_GAME)] == ChessInterfaceConstants.START_GAME:
                print(ReceivedData.upper())
                SelectChessEngine(ChessInterfaceFrameObject, ReceivedData, ChessInterfaceConstants.START_GAME)
                ChessInterfaceFrameObject.StartGame()
            elif ReceivedData[0:len(ChessInterfaceConstants.CLOSE_ALL_APPLICATIONS)] == ChessInterfaceConstants.CLOSE_ALL_APPLICATIONS:
                print("RECEIVED EXIT SIGNAL")
                ChessInterfaceFrameObject.RepeatMainLoop = False
                ChessInterfaceFrameObject.TkRoot.destroy()
            else:
                print(ReceivedData)
                PrintReceivedData(ReceivedData)

                if ReceivedData.find("#") > 13:
                    CheckIfThereIsEndOfGameResult = ChessInterfaceFrameObject.MakeOpponentMove(ReceivedData[2:4], ReceivedData[8:10], ReceivedData[14:16])
                else:
                    CheckIfThereIsEndOfGameResult = ChessInterfaceFrameObject.MakeOpponentMove(ReceivedData[2:4], ReceivedData[8:10], None)

                if CheckIfThereIsEndOfGameResult is False or (CheckIfThereIsEndOfGameResult is True and ChessInterfaceFrameObject.TournamentStopped is True):
                    ChessInterfaceFrameObject.UpdateCurrentPlayerColorData()
                    if (ChessInterfaceFrameObject.UseEngineVar.get() == 1) and (ChessInterfaceFrameObject.GameStopped == False):
                        ChessInterfaceFiles.UseEngine(ChessInterfaceFrameObject)

            ReceivedData = ReceivedData[ReceivedData.find("#") + 1:len(ReceivedData):1]
            if len(ReceivedData) > 1:
                print("NEW RECEIVED DATA =", ReceivedData)

    sleep(0.1)

def SendMoveGeneral(SocketConnection, ChessMove):
    if SocketConnection:
        ChessMoveStr = str(ChessMove)
        try:
            SocketConnection.send((ChessMoveStr + "#").encode())
        except OSError:
            print("CLOSED CONNECTION AFTER SEND")
            quit(0)
    else:
        raise NameError("No connection")

class ChessNetSocketServer(Thread):

    def __init__(self, ChessInterfaceFrame):
        self.ChessInterfaceFrameObject = ChessInterfaceFrame
        self.SocketConnection = None
        self.NetSocket = socket(AF_INET, SOCK_STREAM)
        self.NetSocket.bind((NetworkConfig.ServerHost, NetworkConfig.Port))
        self.NetSocket.listen(1)
        Thread.__init__(self)

    def run(self):
        print("Server is running")

        self.SocketConnection, SocketAddress = self.NetSocket.accept()
        print("Connection = ", self.SocketConnection)
        print("Address IP = ", SocketAddress)
        IPAddressString, Socket = SocketAddress
        print(IPAddressString, Socket)
        print("Client has connected to the server - information from server!")

        while self.ChessInterfaceFrameObject.RepeatMainLoop is True:
            ReceiveCommand(self.SocketConnection, self.ChessInterfaceFrameObject)

    def SendChessMove(self, ChessMove):
        SendMoveGeneral(self.SocketConnection, ChessMove)


class ChessNetSocketClient(Thread):

    def __init__(self, ChessInterfaceFrame):
        self.ChessInterfaceFrameObject = ChessInterfaceFrame
        self.SocketConnection = socket(AF_INET, SOCK_STREAM)
        self.SocketConnection.connect((NetworkConfig.ClientHost, NetworkConfig.Port))
        Thread.__init__(self)

    def run(self):
        print("Client has connected to the server - information from client!")

        while self.ChessInterfaceFrameObject.RepeatMainLoop is True:
            ReceiveCommand(self.SocketConnection, self.ChessInterfaceFrameObject)

        self.SocketConnection.close()

    def SendChessMove(self, ChessMove):
       SendMoveGeneral(self.SocketConnection, ChessMove)