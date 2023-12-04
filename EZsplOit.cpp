#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
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
    FILE* pipe = popen("/sbin/ip a s", "r");
    
    if (!pipe) {
        std::cerr << "Error opening pipe to command." << std::endl;
        return "";
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        std::cout << buffer;  // Print each line (optional)
        
        if (strstr(buffer, "inet") != nullptr && (buffer, "127.")==nullptr) {
            // Extract IP address (modify this as needed)
            char* start = strstr(buffer, "inet") + 5;
            char* end = strchr(start, '/');
            if (end != nullptr) {
                *end = '\0';
            }
            ipAddress = start;
            break;
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
















