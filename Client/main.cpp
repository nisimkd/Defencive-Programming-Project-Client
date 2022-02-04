#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "ConsoleUI.h"
#include "Logger.h"
#include <string>

using namespace std;

int main()
{
    //TODO Move to other place
    const string LOG_FILE_NAME = "Client.log";
    const string SERVER_INFO_FILE_NAME = "server.info";

    Logger::initLogger(LOG_FILE_NAME);    

    string serverAddress;
    string serverPort;

    fstream serverInfoFile;

    //TODO Create a function that load from file the server details
    serverInfoFile.open(SERVER_INFO_FILE_NAME, std::ios::in);// open read from file
    if (serverInfoFile.is_open())
    {
        //TODO Create a function that getting the address and the port
        vector<string> serverInfoVec;
        string serverInfoStr;
        while (getline(serverInfoFile, serverInfoStr, ':'))
        {
            serverInfoVec.push_back(serverInfoStr);
        }

        serverAddress = serverInfoVec[0];
        serverPort = serverInfoVec[1];
    }

    ConsoleUI consoleUI = ConsoleUI(serverAddress, serverPort);

    return consoleUI.run();
}
