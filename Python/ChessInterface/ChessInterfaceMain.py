from ChessInterfaceConstants import ChessInterfaceConstants

import ChessInterfaceFiles
import ChessInterfaceFrame

#from tkinter import *
import sys
import argparse
import subprocess
import platform

if __name__ == "__main__":
    ChessInterfaceFiles.FileConstants.ChessInterfaceConfig_xml += "_" + platform.system() + ".xml"

    ChessInterfaceFiles.ReadAndPrintChessInterfaceConfigFile()

    ArgumentsParser = argparse.ArgumentParser(description = "Short sample app")

    ArgumentsParser.add_argument("-s", dest = "server", action = "store_true", default = False)

    ArgumentsParser.add_argument("-e", dest = "use_engine", action = "store_true", default = False)
    ArgumentsParser.add_argument("-i", dest = "invisible", action = "store_true", default = False)

    ArgumentsParser.add_argument("-c", dest = "client", action = "store_true", default = False)

    args = ArgumentsParser.parse_args()
    NumberOfArguments = len(sys.argv[1:])
    if NumberOfArguments == 0:
        if platform.system() == "Windows":
            ChessInterfaceServerProcess = subprocess.Popen("python ChessInterfaceMain.py -s -e -i", stderr = subprocess.PIPE)
            ChessInterfaceClientProcess = subprocess.Popen("python ChessInterfaceMain.py -c", stderr = subprocess.PIPE)
            #ChessInterfaceServerProcess = subprocess.Popen("python ChessInterfaceMain.py -s -e -i")
            #ChessInterfaceClientProcess = subprocess.Popen("python ChessInterfaceMain.py -c")
        else:
            print("Lack of arguments. Process closed!")
            exit(0)

    print(args)

    if args.server:
        ChessInterfaceFrameObject = ChessInterfaceFrame.ChessInterfaceFrame(Mode = ChessInterfaceConstants.server, UseEngine = args.use_engine, InvisibleWindow = args.invisible)
    else:
        ChessInterfaceFrameObject = ChessInterfaceFrame.ChessInterfaceFrame(Mode = ChessInterfaceConstants.client, UseEngine = args.use_engine, InvisibleWindow = False)