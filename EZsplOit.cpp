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
        std::cout << "4. Quit" << std::endl;

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
            case '4':
                choice = 0;
                validInput = true;
                break;
            default:
                std::cout << "INVALID INPUT! Please enter a valid option." << std::endl;
        }
    } while (!validInput);

    return choice;

}

vector<string> target(const vector<string>& Targets) {
    cout << "\nTargets: " << endl;
    int i = 0;

    if (Targets.empty()) {
        cout << "No Targets found. Perform a Scan" << endl;
        return {};  // Return an empty vector to indicate no valid targets
    }

    for (const string& ip : Targets) {
        cout << ++i << ". " << ip << endl;
    }

    cout << "Select First Target (1-" << i << ") or press any other key to exit. " << endl;
    int firstOption;
    cin >> firstOption;

    if (firstOption >= 1 && firstOption <= i) {
        cout << "Enter Second Target (1-" << i << ") or press any other key to exit. " << endl;
        int secondOption;
        cin >> secondOption;

        if (secondOption >= 1 && secondOption <= i) {
            // Return both selected targets' IP addresses
            return {Targets[firstOption - 1], Targets[secondOption - 1]};  // Adjust index to match vector indexing
        } else {
            // Return only the first selected target's IP address
            return {Targets[firstOption - 1]};
        }
    }

    return {};  // Return an empty vector to indicate no valid targets
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
    istringstream iss(nmapOutput);
    string line;
    while (std::getline(iss, line)) {

        if (line.find("Nmap scan report") != std::string::npos) {
            size_t start = line.find("for ") + 4;
            size_t end = line.find(" ", start);
            //if (end != std::string::npos) {
                std::string ip = line.substr(start, end - start);
                activeIPs.push_back(ip);
           // }
        }
    }

    return activeIPs;
}

vector<string> scan() {
    cout << "Scanning..." << endl;

    string localIPAddress = getLocalIPAddress();

    cout << "IP Address: " << localIPAddress << endl;

    string outputFileName = "nmap_output.txt";
    string scanCommand = "nmap -sn " + localIPAddress + "/24 > " + outputFileName;

    // Execute nmap command and redirect output to a file
    int result = system(scanCommand.c_str());
    if (result != 0) {
        cerr << "Error executing nmap command" << endl;
        return {};  // Return an empty vector in case of an error
    }

    // Read the contents of the output file into a string
    string nmapOutput;
    ifstream outputFileStream(outputFileName);
    if (outputFileStream.is_open()) {
        string line;
        while (getline(outputFileStream, line)) {
            nmapOutput += line + "\n";
        }
        outputFileStream.close();
    } else {
        cerr << "Error opening nmap output file" << endl;
        return {};  // Return an empty vector in case of an error
    }

    // Remove the temporary output file
    remove(outputFileName.c_str());

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
    vector<string> targets;

    do {
        option = menu();

        if (option == 1) {
            targets = scan();
        } else if (option == 2) {
            vector <string> TargetIPs = target(targets);
            for (const string& ips : TargetIPs) {
                cout << ips << endl;
            }
        }

    } while (option != 0);

    return 0;
}
















