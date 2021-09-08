#ifndef FINAL_PROJECT_SHELL_H
#define FINAL_PROJECT_SHELL_H

#include "Graph.h"
#include <map>
#include <string>
#include <vector>

class Shell
{
private:
    std::map<std::string,Graph> symbolTable;
    std::string command;
    std::vector<std::string> commandVec;
    std::string out;

    //--------------------------------------Self Use Functions -----------------------------------------------
    void Parse_Command();
    bool Legal_Graph_Name(std::string name);
    bool Is_Graph_Exist(const std::string& wantedGraph);
    Graph Execute_Create(std::string createStr);
    void Execute_Print(Graph target);
    void Execute_Delete(std::string target);
    void Execute_Who();
    void Execute_Reset();
    Graph Execute_Load_Graph(std::string source);
    static void Execute_Save_Graph(Graph target, std::string fileName);
    void Are_Terms_Legal();
    void Clean_Up_Vector();
    Graph Parse_Recursion (std::string cmd);

public:
    explicit Shell(std::map<std::string,Graph> startGraphs ={}, std::string startCommand="", std::vector<std::string> startCommandVec={}, std::string givenOut="");
    ~Shell() = default;
    void Insert_New_Command(std::string newCommand);
    std::string getOutput ();


};
#endif //FINAL_PROJECT_SHELL_H
