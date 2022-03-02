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
    const string CLIENT_INFO_FILE_NAME = "my.info";

    Logger::initLogger(LOG_FILE_NAME);

    string serverAddress;
    string serverPort;

    fstream serverInfoFile;
    fstream myInfoFile;
    ConsoleUI* consoleUI;

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

        serverInfoFile.close();
    }
    else
    {
        cout << "server.info file doesn't exist!\n";
        system("pause");
        return 1;
    }

    myInfoFile.open(CLIENT_INFO_FILE_NAME, ios::in);
    if (myInfoFile.is_open())
    {
        cout << "Load client info from my.info file:" << endl;
        string clientName;
        string clientId;
        getline(myInfoFile, clientName);
        getline(myInfoFile, clientId);
        cout << "Client name: " << clientName << endl;
        cout << "Client id: " << clientId << endl;

        string line;
        string base64PrivateKey;
        while (getline(myInfoFile, line))
        {
            base64PrivateKey += line;
        }
        cout << "Private key: " << base64PrivateKey << endl;
        myInfoFile.close();

        consoleUI = new ConsoleUI(serverAddress, serverPort, clientName, clientId, base64PrivateKey);
        system("pause");
    }
    else
    {
        consoleUI = new ConsoleUI(serverAddress, serverPort);
    }

    int exit = consoleUI->run();
    delete consoleUI;
    return exit;
}