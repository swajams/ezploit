#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <array>
#include <algorithm>
#include <vector>
using namespace std;


#define RESET_COLOR "\033[0m"
#define RED_COLOR "\033[31m"


int userInput(int limit) {
    std::string rawInput;
    bool loop = true;
    char value;

    while (loop) {
        std::cout << "Please enter a number between 1 and " << limit << ": ";
        std::cin >> rawInput;

        if (rawInput.length() == 1) {
            value = rawInput[0];
            if (std::isdigit(static_cast<unsigned char>(value))) {
                int option = value - '0';
                if (option > 0 && option <= limit) {
                    loop = false;
                    return option; // Return the validated integer
                }else {std::cout << "Invalid value. Please enter a valid option" << std::endl;}
            }else {std::cout << "Invalid value. Please enter a valid integer!" << std::endl;}
        }else {std::cout << "Invalid value. Please enter one character only!" << std::endl;}
    }
    // If somehow the loop exits without returning a value, which shouldn't happen:
    return -1;
}

int menu() {
    std::cout << "What would you like to do?" << std::endl;
    std::cout << "1. Scan" << std::endl;
    std::cout << "2. Targets" << std::endl;
    std::cout << "3. Attacks" << std::endl;
    std::cout << "4. Quit" << std::endl;
    int choice = userInput(4);
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
    int firstOption = userInput(i);
    cout << "Enter Second Target (1-" << i << ") or enter "<< i+1 <<"if no second Target, press any other key to exit. " << endl;
    int secondOption = userInput(i);
    if (secondOption == i+1){return {Targets[firstOption - 1],0};}
    else{return {Targets[firstOption - 1], Targets[secondOption - 1]};};  // Adjust index to match vector indexing
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
    for (const std::string& ips : activeIPs) {
        std::cout << ips << std::endl;
    }
    return activeIPs;
}

void portScan(string IP){
    string portScanCommand = "nmap -sV " + IP;
    int result = system(portScanCommand.c_str());
    if (result != 0){
        cerr << "Error executing Nmap command" << endl;
    }
    
    cout << "This is working" << endl;
    
}

int Attacks(std::vector<string>& vec){
    cout << "1. Port Scan Target"<< endl;
    cout << "2. Man In The Middle (MITM)"<< endl;
    cout << "3. Denial of Service (DoS)"<< endl;
    cout << "5. DeAuthenticate"<< endl;
    cout << "6. Evil Twin AP"<< endl;
    string IP = vec.front();
    int attackChoice = userInput(6);
    if (attackChoice == 1){
        portScan(IP);
    } 
    return 0;
    
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

    std::vector<std::string> TargetIPs; // Declare TargetIPs in a scope accessible to both option cases

    do {
        option = menu();

        if (option == 1) {
            targets = scan();
        } else if (option == 2) {
            TargetIPs = target(targets); // Assign the result to TargetIPs
            // for (const std::string& ips : TargetIPs) {
                // std::cout << ips << std::endl;
            // }
        } else if (option == 3) {
            Attacks(TargetIPs); // Pass TargetIPs to the Attacks function
        }

    } while (option != 0);

    return 0;
}















