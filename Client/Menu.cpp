#include "Menu.h"
#include <iostream>

Menu::Menu()
{
}

void test()
{
    int userChoise;

    do
    {
        system("cls");

        std::cout << "Hello";

        std::cin >> userChoise;

        switch (userChoise)
        {
        case 1:
            break;
        }
    } while (userChoise != 9);
}

int Menu::runMainMenu()
{
    Menu::mainMenuOptions userSelection;

    do
    {
        system("cls");

        Menu::displayMainMenuOptions();

        userSelection = (Menu::mainMenuOptions)Menu::getInput();

        switch (userSelection)
        {
        case Menu::mainMenuOptions::user_register:
            test();
            break;
        case Menu::mainMenuOptions::request_clients_list:
            break;
        case Menu::mainMenuOptions::request_public_key:
            break;
        case Menu::mainMenuOptions::request_waiting_messages:
            break;
        case Menu::mainMenuOptions::send_text_message:
            break;
        case Menu::mainMenuOptions::request_symmetric_key:
            break;
        case Menu::mainMenuOptions::send_symmetric_key:
            break;
        case Menu::mainMenuOptions::exit:
            break;
        default:
            break;
        }
    } while (userSelection != Menu::mainMenuOptions::exit);

    return EXIT_SUCCESS;
}

int Menu::getInput()
{
    int choise;
    std::cin >> choise;
    return choise;
}

void Menu::displayMainMenuOptions()
{
    std::cout << "MessageU client at your service." << std::endl << std::endl;
    std::cout << "110) Register" << std::endl;
    std::cout << "120) Request for clients list" << std::endl;
    std::cout << "130) Request for public key" << std::endl;
    std::cout << "140) Request for waiting messages" << std::endl;
    std::cout << "150) Send a text message" << std::endl;
    std::cout << "151) Send a request for symmetric key" << std::endl;
    std::cout << "152) Send your symmetric key" << std::endl;
    std::cout << "0) Exit client" << std::endl;
}
