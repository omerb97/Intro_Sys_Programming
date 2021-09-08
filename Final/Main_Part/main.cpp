#include <iostream>
#include "Graph.h"
#include "Shell.h"
#include <string>
#include <fstream>
#include <istream>
using std::ifstream;
using std::ofstream;
using std::istream;
using std::set;
using std::map;
using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::vector;

int main(int argc, char** argv)
{
    string command;
    Shell cmd;
    ofstream to;
    ifstream from;
    if (argc > 1){
         from.open(argv[1]);
        if (!from){
            return 1;
        }
        to.open(argv[2]);
        if (!to){
            return 1;
        }
    }

    while (command != "quit") {
        if (argc == 1) {
            cout << "Gcalc> ";
            if (!(getline(cin, command))){
                command="quit";
            }
        } else{
            if (!from.eof()) {
                getline(from, command);
            } else{
                command = "quit";
            }
        }

        if (command == "quit" || command.empty()) {
            continue;
        }

        try {
            cmd.Insert_New_Command(command);
            string strOutput = cmd.getOutput();
            if (argc == 1){
                cout << strOutput;
            } else{
                to << strOutput;
            }
        }
        catch (GraphNotFound &e) {
            if (argc == 1) {
                cout << e.what();
                cout << endl;
            }else{
                to << e.what();
                to << endl;
            }
            continue;
        }
        catch (IllegalNodeName &e) {
            if (argc == 1) {
                cout << e.what();
                cout << endl;
            } else{
                to << e.what();
                to << endl;
            }
            continue;
        }
        catch (NodeAlreadyExists &e) {
            if (argc == 1) {
                cout << e.what();
                cout << endl;
            } else{
                to << e.what();
                to << endl;
            }
            continue;
        }
        catch (InvalidCommand &e) {
            if (argc == 1) {
                cout << e.what();
                cout << endl;
            } else{
                to << e.what();
                to << endl;
            }
            continue;
        }

        catch (IllegalGraphName &e) {
            if (argc == 1) {
                cout << e.what();
                cout << endl;
            } else{
                to << e.what();
                to << endl;
            }
            continue;
        }
        catch (FileNotFound &e) {
            if (argc == 1) {
                cout << e.what();
                cout << endl;
            } else{
                to << e.what();
                to << endl;
            }
            continue;
        }
        catch (IllegalParentheses &e) {
            if (argc == 1) {
                cout << e.what();
                cout << endl;
            } else{
                to << e.what();
                to << endl;
            }
            continue;
        }
        catch (IllegalBrackets &e) {
            if (argc == 1) {
                cout << e.what();
                cout << endl;
            } else{
                to << e.what();
                to << endl;
            }
            continue;
        }
        catch (std::bad_alloc &e) {
            std::cerr << "Error: Fatal Error";
            return 1;
        }
        catch (IllegalNameWithSpace &e) {
            if (argc == 1) {
                cout << e.what();
                cout << endl;
            } else{
                to << e.what();
                to << endl;
            }
            continue;
        }
        catch (ArcException &e) {
            if (argc == 1) {
                cout << e.what();
                cout << endl;
            } else{
                to << e.what();
                to << endl;
            }
            continue;
        }

    }
    if (argc > 1 ){
        from.close();
        to.close();
        return 0;
    }
    return 0;
}