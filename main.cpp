
#include <iostream>
#include <cstdlib>
#include <vector>
#include <experimental/filesystem>
#include <unistd.h>
#include <cstring>
#include <climits>
#include <sys/stat.h>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      //Black
#define RED     "\033[31m"      //Red
#define GREEN   "\033[32m"      //Green
#define YELLOW  "\033[33m"      //Yellow
#define BLUE    "\033[34m"      //Blue
#define MAGENTA "\033[35m"      //Magenta
#define CYAN    "\033[36m"      //Cyan
#define WHITE   "\033[37m"      //White
#define BOLDBLACK   "\033[1m\033[30m"      //Bold Black
#define BOLDRED     "\033[1m\033[31m"      //Bold Red
#define BOLDGREEN   "\033[1m\033[32m"      //Bold Green
#define BOLDYELLOW  "\033[1m\033[33m"      //Bold Yellow
#define BOLDBLUE    "\033[1m\033[34m"      //Bold Blue
#define BOLDMAGENTA "\033[1m\033[35m"      //Bold Magenta
#define BOLDCYAN    "\033[1m\033[36m"      // Bold Cyan
#define BOLDWHITE   "\033[1m\033[37m"      // Bold White

using namespace std;
typedef unsigned int uint;

/**
 * @brief Connects to an openVPN server.
 * Currently uses system calls, but eventually should use C++17's `std::filesystem`.
 * Currently does not take program options like -o.
 * @param args A copy of the program arguments passed to it from main.
 * @param output The output file for logs. Currently set to a default of ~/.vpns/logs/most_recent.log Will
 *  eventually be specified by the -o argument.
 */
void connect(const char** args, const char* output = strcat(getenv("HOME"), "/.vpns/logs/most_recent.log")) {

    //eventually will add command line args, not right now

    string name = args[2];
    string home = getenv("HOME");

    pid_t pID = fork();

    if (pID == 0) {
        //is child process == run VPN through here
        std::ostringstream oss;
        oss << "openvpn " << home << "/.vpns/" << name << ".ovpn > " << output << " &";
        cout << "test" << endl;
        system(oss.str().c_str());
        cout << "test" << endl;
        exit(0);

    } else {
        //is parent process == kill after reporting child's pid
        cout << "Your VPN is running through process" << BOLDGREEN << pID << RESET << endl;
        exit(0);
    }
}

/**
 * @brief Adds a profile.
 * Uses system calls `system()` to copy, move, and rename files.
 * Eventually should use C++17's `std::filesystem`, but it is not supported by cmake at this time.
 * @param args A copy of program arguments, passed to it from main.
 * @return 0 if successful, 1 if unsuccessful.
 */
int addProfile(const char** args) {

    string name = args[2];
    string file = args[3];
    string home = getenv("HOME");
    ostringstream oss;
    if (file[0] == '/') {
        oss << "cp " << file << " " << home << "/.vpns/" << name << ".ovpn";
    } else {
        char cwd[PATH_MAX + 1]; // +1 required because c strings require a null-terminator
        getcwd(cwd, sizeof(cwd));
        oss << "cp " << cwd << "/" << file << " " << home << "/.vpns/" << name << ".ovpn";
    }
    if (system(oss.str().c_str()) == 0) {
        cout << "Profile " << name << " created successfully in ~/.vpns/" << name << ".ovpn" << endl;
    } else {
        cerr << "Program failed to create VPN profile. Diagnosing..." << endl;
        oss.str("");
        oss << home << "/.vpns";
        struct stat buf;
        if (stat(oss.str().c_str(), &buf) == 0) {
            cerr << "~/.vpns exists..." << endl;
            cerr << "Does your profile exist? Checking..." << endl;
            oss.str("");
            if (file[0] == '/') {
                struct stat buf2;
                if (stat(file.c_str(), &buf2) == 0) {
                    cerr << "Your file exists..." << endl;
                    cerr << "I can't determine the error. Try checking your journal: " << endl;
                    cerr << "journalctl" << endl;
                } else {
                    cerr << "Error identified." << endl;
                    cerr << BOLDRED << "The VPN profile does not exist." << RESET << endl;
                    return -2;
                }
            } else {
                char cwd[PATH_MAX + 1]; // +1 required because c-strings require a null-terminator
                getcwd(cwd, sizeof(cwd));
                oss << cwd << "/" << file;
                struct stat buf2;
                if (stat(oss.str().c_str(), &buf2) == 0) {
                    cerr << "Your file exists..." << endl;
                    cerr << "I can't determine the error. Try checking your journal: " << endl;
                    cerr << "journalctl" << endl;
                } else {
                    cerr << "Error identified." << endl;
                    cerr << BOLDRED << "The VPN profile does not exist." << RESET << endl;
                    return -2;
                }
            }
        } else {
            cerr << BOLDYELLOW << "WARN" << RESET << "The folder ~/.vpns does not exist. Creating..." << endl;
            oss.str("");
            oss << "mkdir " << home << "/.vpns";
            system(oss.str().c_str());
        }
    }

    return 0;
}

void disconnect() {
    system("killall openvpn");
    system("systemctl restart NetworkManager");
}

/**
 * @brief The main function. Program base that takes all input.
 * @param argc Argument count
 * @param argv Argument values
 * @return Exit codes
 */
int main(int argc, const char* argv[]) {
    if (argc <= 1) {
        cout << BOLDRED << "Not enough arguments." << RED << endl << "See vpnmanip(1) for help." << endl << RESET;
        exit(1);
    }

    if (strncmp(argv[1], "add", 3) == 0) {
        return addProfile(argv);
    }

    if(strncmp(argv[1], "connect", 7) == 0) {
        connect(argv);
    }
    if (strncmp(argv[1], "disconnect", 10) == 0) {
        disconnect();
    }
    return 0;
}