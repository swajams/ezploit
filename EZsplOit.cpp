#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <algorithm>
using namespace std;

#define RESET_COLOR "\033[0m"
#define RED_COLOR "\033[31m"

int menu() {
    int choice;
    bool validInput = false;

    do {
        std::cout << "What would you like to do?" << std::endl;
        std::cout << "1. Scan" << std::endl;
        std::cout << "2. Targets" << std::endl;
        std::cout << "3. Attacks" << std::endl;

        string input;
        char option;
        std::cin >> input;
        if(input.length() == 1){
             option = input[0];
        }
        else{
            option = 0;
        }

        switch (option) {
            case '1':
                choice = 1;
                validInput = true;
                break;
            case '2':
                choice = 2;
                validInput = true;
                break;
            case '3':
                choice = 3;
                validInput = true;
                break;
            default:
                std::cout << "INVALID INPUT! Please enter a valid option." << std::endl;
        }
    } while (!validInput);

    return choice;

}


std::string getLocalIPAddress() {
    // Use platform-specific command to get local IP address
    std::string ipAddress;

    // Open a pipe to the command and read its output
    FILE* pipe = popen("/sbin/ip -o -4 a s | awk '{print $4}' | cut -d/ -f1", "r");

    if (!pipe) {
        std::cerr << "Error opening pipe to command." << std::endl;
        return "";
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        std::cout << buffer;  // Print each line (optional)

        // Extract IP address (modify this as needed)
        ipAddress = buffer;

        // Erase newline characters
        ipAddress.erase(std::remove_if(ipAddress.begin(), ipAddress.end(),
                                       [](char c) { return c == '\n' || c == '\r'; }),
                        ipAddress.end());

        // Skip the loopback address (127.0.0.1)
        if (ipAddress != "127.0.0.1") {
            break;  // Assuming you want the first non-loopback IPv4 address found
        }
    }

    pclose(pipe);

    return ipAddress;
}


int scan() {
    cout << "Scanning..." << endl;

    // Get local IP address
    string localIPAddress = getLocalIPAddress();

    cout << "Local IP Address: " << localIPAddress << endl;

    // Formulate the nmap command using the local IP address
    string scanCommand = "nmap -sn " + localIPAddress + "/24";

    // Run the nmap command
    int result = system(scanCommand.c_str());

    return result;
}



int main() {
  cout <<RED_COLOR<<R"(
       __             _,-^~^"-.
     _// )      _,-^~`         `.
   ." ( /`"-,-"`                 ;
  / @            EZsplOit         ;
 /           ,             ,-"     ;
(,__.--.      \           /        ;
       /`-.\   |          |        `._________
   _.-'_/`  )  )--...,,,___\     \-----------,)
 ((("~` _.-'.-'           __`-.   )         //
       ((("`             (((---~"`         //
                                          ((________________
                                          `----^^^^^^^^''''
)" <<RESET_COLOR<<endl;
int option;
option = menu();

if(option == 1 ){scan();}

    return 0;
}
















