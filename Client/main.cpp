#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "MessageUMenu.h"
#include <string>

using namespace std;

int main()
{
    const string SERVER_INFO_FILE_NAME = "server.info";

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

    MessageUMenu menu = MessageUMenu(serverAddress, serverPort);

    return menu.runMainMenu();
}
