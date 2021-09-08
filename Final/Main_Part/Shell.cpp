#include "Shell.h"
#include "Exceptions.h"
#include <cctype>
#include <regex>
#include <algorithm>
#include <istream>
#include <fstream>


using std::string;
using std::map;
using std::set;
using std::vector;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::istream;

#define NPOS -1
#define NULL_CHAR '@'
#define OPERATORS "+-*^!"


//------------------------------SELF USE FUNCTIONS---------------------------------------------------------
set<string> Prepare_Node_Set(string str);
vector<string>  Prepare_Arc (string str);
string Snip_Ends(string str);
bool equalParantheses(string str);
bool equalBrackets(string str);
//------------------------------------------------------------------------------------------------------------


Shell::Shell(map<string, Graph> startGraphs, string startCommand,
             vector<string> startCommandVec, string givenOut):
             symbolTable(startGraphs), command(startCommand),commandVec(startCommandVec), out(givenOut)
             {}


bool Shell::Is_Graph_Exist(const std::string& wantedGraph)
{
    return (this->symbolTable.find(wantedGraph) != symbolTable.end());
}

string Clip_Ends(string str){
    int firstNoSpace = str.find_first_not_of(' ');
    int lastNoSpace = str.find_last_not_of(' ');
    string temp = str.substr(0,firstNoSpace);
    return (temp.substr(0,lastNoSpace));
}
void Shell::Execute_Print(Graph target)
{
    for (const string& str : target.getNodes())
    {
        out += str +"\n";
    }
    out += "$\n";
    for (auto &ell : target.getArcs()) {
        set<string> connections = ell.second;
        for (const string &str : connections) {
              out += (ell.first + " " + str + "\n");
        }
    }


}

void Shell::Execute_Delete(string target)
{
    if (!(Is_Graph_Exist(target))) {
        throw GraphNotFound();
    }
    symbolTable.erase(target);
}

void Shell::Execute_Who()
{
    for (auto &ell : symbolTable)
    {
          out += (ell.first + "\n");
    }
}

void Shell::Execute_Reset()
{
    symbolTable.clear();
    commandVec.clear();
    command = "";
}


Graph Shell::Execute_Create(string createStr)
{
    if (createStr == "" || createStr =="|") {
        Graph g;
        return g;
    }

    Graph newGraph;
    auto splitter = createStr.find('|');
    if (splitter != string::npos) {
        if (*(createStr.end()- 1 ) != '>' && *(createStr.end()- 1 ) != '|') {
            throw InvalidCommand();
        }
        string nodeString = createStr.substr(0, splitter);
        createStr = createStr.substr(splitter + 1);
        string arcString = createStr.substr(0);
        set<string> receiveNames = Prepare_Node_Set(nodeString);
        Graph tempNewGraph(receiveNames);
        map<string, set<string>> tempArcs = tempNewGraph.getArcs();
        while (createStr.length() > 1) {
            vector<string> pairArcs = Prepare_Arc(createStr);
            Graph secondGraph(tempNewGraph);
            secondGraph.InsertArc(pairArcs[0], pairArcs[1]);
            if (secondGraph.getArcs() == tempNewGraph.getArcs()) {
                throw ArcAlreadyExists();
            }
            tempNewGraph.InsertArc(pairArcs[0], pairArcs[1]);
            createStr = pairArcs[2];
        }
        newGraph = tempNewGraph;
    } else {
        string nodeString = createStr.substr(0);
        set<string> receiveNames = Prepare_Node_Set(nodeString);
        Graph tempNewGraph(receiveNames);
        newGraph = tempNewGraph;
    }

    return newGraph;

}

void Shell::Parse_Command()
{

    //old parsing

    //regex parsing
    //string regCheck = "[a-zA-Z0-9][ ]+[a-zA-Z0-9]"; //checks if there are spaces in names
//    std::regex regCheck("[a-zA-Z0-9]+[ ]+[a-zA-Z0-9]+");
//    std::regex regCheck222("[a-zA-Z0-9][ ]+[a-zA-Z0-9]");
//    bool result = std::regex_match(command, regCheck);
//    bool res2 = std::regex_match(command, regCheck222);
//    if(std::regex_match(command, regCheck, std::regex_constants::match_any)){
//        throw IllegalNameWithSpace();
//    }

    command = Snip_Ends(command);

    int it =0;
    while (it < int(command.length()))
    {
        if (isspace(command[it]) && isalnum(command[it-1]))
        {
            int temp = it;
            while (temp < int(command.length()))
            {

                if (isalnum(command[temp])) {
                    throw IllegalNameWithSpace();
                }
                if (isspace(command[temp])) {
                    temp++;
                }
                if (ispunct(command[temp])) {
                    break;
                }
            }
        }
        it++;
    }

    int startCutPos = command.find_first_not_of(' ');
    int count = 0;
    while ((startCutPos+count) <= int(command.length()))
    {
        if ((command[startCutPos + count] == '=') || isspace(command[startCutPos + count]) || (command[startCutPos + count] == '+') ||
                (command[startCutPos + count] == '-') || (command[startCutPos + count] == '^') || (command[startCutPos + count] == '{') || (command[startCutPos + count] == '}') ||
                (command[startCutPos + count] == '(') || (command[startCutPos + count] == ')') || (command[startCutPos + count] == '!') ||
                (command[startCutPos + count] == ','))
        {
            string temp = command.substr(startCutPos,count);
            commandVec.push_back(temp);
            startCutPos += count;
            commandVec.push_back(command.substr(startCutPos,1));
            startCutPos += 1;
            count = 0;
        }
        else if ((startCutPos+count) == int(command.length()))
        {
            string temp = command.substr(startCutPos,count);
            commandVec.push_back(temp);
            count++;
        }else {
            count++;
        }
    }


    Clean_Up_Vector();
    commandVec.shrink_to_fit();
    Are_Terms_Legal();
    //old parsing

    string tempCommand;
    for (string str : commandVec){
        tempCommand+=str;
    }
    command=tempCommand;

    int equalSign = command.find('=');
    if (equalSign == -1){
        if (command == "who" && command.find("who") == 0){
            Execute_Who();
        }
        if (command =="reset" && command.find("reset") == 0)
        {
            Execute_Reset();
        }
        if(command.find("delete") != string::npos && command.find("delete") == 0){
            int open = command.find("(");
            string term = command.substr(open+1, command.length()-open-2);
            Execute_Delete(term);
        }
        if (command.find("print") != string::npos && command.find("print") == 0){
            int open = command.find("(");
            string term = command.substr(open+1, command.length()-open-2);
            Graph target = Parse_Recursion(term);
            Execute_Print(target);
        }
        if (command.find("save") != string::npos && command.find("save") == 0){
            int open = command.find("(");
            int splitter = command.find_last_of(",");
            string term = command.substr(open+1, splitter - open-1);
            string fileName = command.substr(splitter+1, command.length()-splitter-2);
            Graph saveGraph = Parse_Recursion(term);
            Execute_Save_Graph(saveGraph, fileName);
        }
    }else{
        string splitLhs = command.substr(0,equalSign);
        string splitRhs = command.substr(equalSign+1);
        if (Is_Graph_Exist(splitLhs)){
            symbolTable.find(splitLhs)->second = Parse_Recursion(splitRhs);
        }else {
            if (!Legal_Graph_Name(splitLhs)){
                throw IllegalGraphName();
            }
            symbolTable.insert({(splitLhs), Parse_Recursion(splitRhs)});
        }
    }
}

void Shell::Insert_New_Command(std::string newCommand)
{
    this->command.clear();
    this->out.clear();
    commandVec.clear();
    newCommand = Snip_Ends(newCommand);
    this->command = newCommand;
    Parse_Command();
}

set<string> Prepare_Node_Set(string str)
{
    string nodeName;
    set<string> sendingNames;
    for (char c :str)
    {
        if (c != ','){
            nodeName += c;
        } else{
            if (sendingNames.find(nodeName) != sendingNames.end()){
                throw NodeAlreadyExists();
            }
            sendingNames.insert(nodeName);
            nodeName ="";
        }
    }

    if (sendingNames.find(nodeName) != sendingNames.end()){
        throw NodeAlreadyExists();
    }
    sendingNames.insert(nodeName);
    return sendingNames;
}

vector<string>  Prepare_Arc (string str)
{
    if (str[0] != '<'){
        throw IllegalArcStructure();
    }
    str = str.substr(1);
    string rhsArc;
    while (str[0] != ','){
        rhsArc+=str[0];
        str = str.substr(1);
    }
    if (str[0] != ','){
        throw IllegalArcStructure();
    }
    str = str.substr(1);
    string lhsArc;
    while (isalnum(str[0])){
        lhsArc+=str[0];
        str = str.substr(1);
    }
    if (str[0] != '>'){
        throw IllegalArcStructure();
    }
    if (str.length()>=2) {
        str = str.substr(2);
    }
    vector<string> pairing = {rhsArc,lhsArc,str};
    return pairing;
}

void Shell::Clean_Up_Vector()
{
    int count = 0;
    auto i = commandVec.begin();
    while (i<commandVec.end())
    {
        if (commandVec[count].empty() || commandVec[count] == " " || iscntrl(command[count])) {
            commandVec.erase(i);
        } else{
            i++;
            count++;
        }
    }
}
void Shell::Are_Terms_Legal()
{
    if(!equalParantheses(command)){
        throw IllegalParentheses();
    }
    if(!equalBrackets(command)){
        throw IllegalBrackets();
    }
    bool flag = false;
    if (command.find('=') == string::npos) {
        string term = commandVec[0];
        if (term == "print" && command.find("(") != string::npos && command.find(")") != string::npos){
            if (commandVec[1] == "("){
                flag = true;
            }

        }
        if (term == "delete" && command.find("(") != string::npos && command.find(")") != string::npos ){
            if (commandVec[1] == "(" && commandVec[3] == ")"){
                flag = true;
            }
        }
        if (term =="save" && command.find("(") != string::npos && command.find(")") != string::npos)
        {
            if (commandVec[1] == "("){
                flag = true;
            }
        }
        if (term == "who" || term == "reset") {
            if  (commandVec.size()==1){
                flag = true;
            }
        }
    } else{
        if (commandVec[1] != "="){
            throw IllegalGraphName();
        }
        flag = true;
    }
    if (!flag){
        throw InvalidCommand();
    }
}

std::string Shell::getOutput()
{
    return this->out;
}

bool Shell::Legal_Graph_Name(std::string name)
{
    if (!isalpha(name[0])){
        return false;
    }
    if (name == "print" || name == "delete" || name == "reset" || name == "quit" || name == "who" || name == "save" || name == "load"){
        return false;
    }
    for (char c : name){
        if (!isalnum(c)){
            return false;
        }
    }
    return true;
}

Graph Shell::Execute_Load_Graph(string fileName)
{
    ifstream loadFile;
    loadFile.open(fileName, std::ios_base::binary);
    if (!loadFile){
        throw FileNotFound();
    }

    int numOfNodes=0;
    int numOfArcs=0;
    loadFile.read((char*) &numOfNodes, sizeof(int)); //get the number of nodes and puts into int variable
    loadFile.read((char*) &numOfArcs, sizeof(int)); //get the number of arcs and puts into int variable
    set<string> prepareNodes;
    for (int i=0; i<numOfNodes; i++){
        int nodeNameLength = 0;
        loadFile.read((char*) &nodeNameLength, sizeof(int)); //get LENGTH of node name
        string nodeName;
        for (int j=0; j<nodeNameLength; j++){
            char c;
            loadFile.read((char*) &c, sizeof(char)); //read char-by-char
            nodeName += c;
        }
        prepareNodes.insert(nodeName);
    }
    Graph loadedGraph(prepareNodes);
    for (int i=0; i<numOfArcs; i++) {
        int srcNameLength;
        loadFile.read((char*) &srcNameLength, sizeof(int));
        string srcName;
        for (int j = 0; j < srcNameLength; j++) {
            char c;
            loadFile.read((char*) &c, sizeof(char));
            srcName += c;
        }
        int destNameLength;
        loadFile.read((char*) &destNameLength, sizeof(int));
        string destName;
        for (int j = 0; j < destNameLength; j++) {
            char c;
            loadFile.read((char*) &c, sizeof(char));
            destName += c;
        }
        loadedGraph.InsertArc(srcName, destName);
    }

    loadFile.close();
    return loadedGraph;

}

void Shell::Execute_Save_Graph(Graph target, string fileName)
{
    ofstream graphFile(fileName);
    unsigned int numOfNodes = target.getNodes().size();
    graphFile.write((const char *) &(numOfNodes), sizeof(int));
    int numOfArcs = 0;
    for (const auto &ell : target.getArcs()) {
        numOfArcs += ell.second.size();
    }
    graphFile.write((const char *) &(numOfArcs), sizeof(int));

    for (const string &ell : target.getNodes()) {
        unsigned int nameLength = ell.length();
        graphFile.write((const char *) &(nameLength), sizeof(int));
        for (char c : ell) {
            graphFile.write((const char *) &(c), sizeof(char));
        }
    }

    for (const auto &ell : target.getArcs()) {
        for (const string &destStr : ell.second) {
            unsigned int srcNameLength = ell.first.length();
            graphFile.write((const char *) &(srcNameLength), sizeof(int));

            for (char c : ell.first) {
                graphFile.write((const char *) &(c), sizeof(char));
            }
            unsigned int destNameLength = destStr.length();
            graphFile.write((const char *) &(destNameLength), sizeof(int));
            for (char c : destStr) {
                graphFile.write((const char *) &(c), sizeof(char));
            }
        }
    }
    graphFile.close();

}

int Find_Next_Op (const string& str){
    return str.find_last_of(OPERATORS);
}

int Is_Operator_Inside_Parantheses(string str, int pos){
    int count=0;
    for (int i=0; i < pos; i++){
        if(str[i] == '('){
            count++;
        }
        if(str[i] ==  ')'){
            count--;
        }
    }
    return (count == 0);
}

Graph Shell::Parse_Recursion (string cmd)
{
    if (cmd.empty()){
        throw InvalidCommand();
    }
    int nextOperation=0;
    //check parantheses
    int lastOperation = cmd.find_last_of(OPERATORS);
    string copyCommand = cmd;
    bool wentInsideLoop = false;
    while (!Is_Operator_Inside_Parantheses(copyCommand,lastOperation) && lastOperation != NPOS)
    {
        copyCommand[lastOperation] = NULL_CHAR;
        lastOperation = copyCommand.find_last_of(OPERATORS);
        wentInsideLoop = true;
    }
    if (int(cmd.size()) > 0 && cmd[0] == '(') {
        int openingPos = 0;
        int closingPos = cmd.find_last_of(')');

        if (closingPos == -1  ||  closingPos == openingPos + 1) {
            throw InvalidCommand();
        }
        string sendingSub = cmd.substr(1, closingPos - 1);

        if (cmd[0] == '(' && wentInsideLoop) {
            return Parse_Recursion(sendingSub);
        }
        if (cmd[0] == '(' && !wentInsideLoop) {
            return Parse_Recursion(sendingSub);
        }
    }
    nextOperation = Find_Next_Op(copyCommand);
    if (cmd.find("load") == 0){
        int closingParentheses = cmd.find_first_of(')');
        Graph loaded = Execute_Load_Graph(cmd.substr(5,closingParentheses-5));
        nextOperation = Find_Next_Op(cmd.substr(closingParentheses+1));
        string sendOn = cmd.substr(closingParentheses+1);
        if (!sendOn.empty()) {
            if (sendOn[0] == '+') {
                return loaded + Parse_Recursion(sendOn);
            }
            if (sendOn[0] == '-') {
                return loaded - Parse_Recursion(sendOn);
            }
            if (sendOn[0] == '*') {
                return loaded * Parse_Recursion(sendOn);
            }
            if (sendOn[0]== '^') {
                return loaded ^ Parse_Recursion(sendOn);
            }
        }
        else{
            return loaded;
        }
    }

    if (nextOperation == -1)
    {
        if (cmd[0] == '{')
        {
            return Execute_Create(cmd.substr(1,cmd.find_last_not_of('}')));
        }
        if(isalpha(cmd[0])){
            if (!Is_Graph_Exist(cmd)){
                throw GraphNotFound();
            } else {
                return symbolTable.find(cmd) -> second;
            }
        }
        throw InvalidCommand();
    }

    if (cmd[nextOperation] == '!')
    {

        string temp1 = cmd.substr(0,nextOperation);
        int nextOperation2 = Find_Next_Op(temp1);
        string temp2 = cmd.substr(nextOperation+1);
        Graph before_all = Parse_Recursion(temp2);
        if (nextOperation2 == -1){
            return (!(Parse_Recursion(temp2)));
        }
        if (temp1[nextOperation2] == '+'){
            return (Parse_Recursion(temp1.substr(0,nextOperation2)) + (!(Parse_Recursion(temp2))));
        }
        if (temp1[nextOperation2] == '-'){
            return Parse_Recursion(temp1.substr(0,nextOperation2)) - (!(Parse_Recursion(temp2)));
        }
        if (temp1[nextOperation2] == '*'){
            return Parse_Recursion(temp1.substr(0,nextOperation2)) * (!(Parse_Recursion(temp2)));
        }
        if (temp1[nextOperation2] == '^'){
            return Parse_Recursion(temp1.substr(0,nextOperation2)) ^ (!(Parse_Recursion(temp2)));
        }
        if (temp1[nextOperation2] == '!'){
            int temp = nextOperation2-1;

            while (temp1[temp] == '!'){
                temp--;
                before_all = !before_all;
            }
            return before_all;
        }

    }

    if (cmd[nextOperation] == '{'){
        int closingBracket =cmd.find_last_not_of('}');
        Graph literal = Execute_Create(cmd.substr(1,cmd.find_last_not_of('}')));
        if (closingBracket == (int(cmd.size())-1)){
            return literal;
        }
        nextOperation = Find_Next_Op(cmd.substr(closingBracket+1));

        if (cmd[nextOperation] == '+'){
            return literal + Parse_Recursion(cmd.substr(closingBracket + 1));
        }
        if (cmd[nextOperation] == '-'){
            return literal - Parse_Recursion(cmd.substr(closingBracket + 1));
        }
        if (cmd[nextOperation] == '*'){
            return literal * Parse_Recursion(cmd.substr(closingBracket + 1));
        }
        if (cmd[nextOperation] == '^'){
            return literal ^ Parse_Recursion(cmd.substr(closingBracket + 1));
        }
    }

    if (nextOperation != -1) {
        string leftSubStr = cmd.substr(0, nextOperation);
        if (nextOperation == (int(cmd.size()) - 1)) {
            throw InvalidCommand();
        }
        string rightSubStr = cmd.substr(nextOperation + 1);


        if (cmd[nextOperation] == '+') {
            return Parse_Recursion(leftSubStr) + Parse_Recursion(rightSubStr);
        }
        if (cmd[nextOperation] == '-') {
            return Parse_Recursion(leftSubStr) - Parse_Recursion(rightSubStr);
        }
        if (cmd[nextOperation] == '*') {
            return Parse_Recursion(leftSubStr) * Parse_Recursion(rightSubStr);
        }
        if (cmd[nextOperation] == '^') {
            return Parse_Recursion(leftSubStr) ^ Parse_Recursion(rightSubStr);
        }
    }
    throw InvalidCommand();
}

string Snip_Ends(string str)
{
    int count = 0;
    bool flag = false;
    while (!flag){
        if (isblank(str[count]) || iscntrl(str[count]) || isblank(str[count])){
            count++;
        } else {
            flag = true;
        }
    }
    str = str.substr(count);
    flag = false;
    count = str.length()-1;
    while (!flag){
        if (isblank(str[count])){
            count--;
        } else {
            flag = true;
        }
    }
    str = str.substr(0,count+1);
    return str;
}

bool equalParantheses(string str)
{
    int count = 0;
    for (char c : str)
    {
        if (c == '('){
            count++;
        }
        if (c == ')'){
            count--;
        }
    }
    return count == 0;
}

bool equalBrackets(string str)
{
    int count = 0;
    for (char c : str)
    {
        if (c == '{'){
            count++;
        }
        if (c == '}'){
            count--;
        }
    }
    return count == 0;
}


