#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;



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

int scan(){
    
    cout << "Scanning..." << endl; 
    

    // Use platform-specific command to get local IP address
    string ipAddress;
    ifstream stream("/sbin/ip a s");
    string line;
    while (getline(stream, line)) {
        istringstream iss(line);
        if (line.find("inet") != string::npos) {
            iss >> ipAddress;
            break;
        }
    }
    cout <<ipAddress<<endl;
    string scanCommand = "nmap -sn "+ipAddress+"/24";
    int result = system(scanCommand,c_str());
    return result;

}



int main() {
  cout << R"(
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
)" <<endl;
int option;
option = menu();

if(option == 1 ){scan();}

    return 0;
}
















