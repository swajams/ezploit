#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <vector>
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
    std::string ipAddress;

    FILE* pipe = popen("/sbin/ip -o -4 a s | awk '{print $4}' | cut -d/ -f1", "r");

    if (!pipe) {
        std::cerr << "Error opening pipe to command." << std::endl;
        return "";
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {

        ipAddress = buffer;
        ipAddress.erase(std::remove_if(ipAddress.begin(), ipAddress.end(),
                                       [](char c) { return c == '\n' || c == '\r'; }),
                        ipAddress.end());

        if (ipAddress != "127.0.0.1") {
            break;
        }
    }

    pclose(pipe);

    return ipAddress;
}


std::vector<std::string> getActiveIPs(const std::string& nmapOutput) {
    std::vector<std::string> activeIPs;

    std::istringstream iss(nmapOutput);
    std::string line;
    while (std::getline(iss, line)) {
        // Process each line of nmap output to identify active IP addresses
        // Example: Check if the line contains "Nmap scan report" (modify this as needed)
        if (line.find("Nmap scan report") != std::string::npos) {
            size_t start = line.find("for ") + 4;
            size_t end = line.find(" ", start);
            if (end != std::string::npos) {
                std::string ip = line.substr(start, end - start);
                activeIPs.push_back(ip);
            }
        }
    }

    return activeIPs;
}

std::vector<string> scan() {
    cout << "Scanning..." << endl;

    string localIPAddress = getLocalIPAddress();

    cout << "IP Address: " << localIPAddress << endl;

    string scanCommand = "nmap -sn " + localIPAddress + "/24";

    FILE* pipe = popen(scanCommand.c_str(), "r");
    if (!pipe) {
        cerr << "Error opening pipe to nmap command" << endl;
        return {};  // Return an empty vector in case of an error
    }

    char buffer[128];
    string nmapOutput;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        nmapOutput += buffer;
        cout << buffer;
    }

    pclose(pipe);

    vector<string> activeIPs = getActiveIPs(nmapOutput);

    return activeIPs;
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

if(option == 1 ){vector<string> targets = scan();}

    return 0;
}
















