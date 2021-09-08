#include "Graph.h"
#include <string>
#include <vector>
#include <iostream>
using std::set;
using std::string;
using std::map;
using std::cout;
using std::endl;
using std::vector;

//------------------------------------------------------------
//Static functions
static bool FindPair (const map<string,set<string>> givenArcs, string src, string dest);
static set<string> UnionSets (const set<string> set1, const set<string> set2);
static set<string> IntersectSets (const set<string> set1, const set<string> set2);
static bool LegalNodeName(const string name);
//-------------------------------------------------------------

//Constructor
Graph::Graph(set<string> givenNodes, map<string,set<string>> givenArcs)
{
    for (string str : givenNodes)
    {
        if (!LegalNodeName(str)){
            throw IllegalNodeName();
        }
    }
    nodes = givenNodes;
    for (auto &ell : givenArcs)
    {
        if (nodes.find(ell.first) == nodes.end()){
            throw IllegalArcNoEndpoint();
        }
        for (string str : ell.second)
        {
            if (nodes.find(str) == nodes.end()){
                throw IllegalArcNoEndpoint();
            }
        }
        for (string secondNode : ell.second){
            if (secondNode == ell.first){
                throw IllegalArcNoEndpoint();
            }
        }
    }
    arcs = givenArcs;
}

// D'tor

Graph::~Graph()
{
    nodes.clear();
    arcs.clear();
}

//Copy C'tor
Graph::Graph(const Graph& original):
nodes(original.getNodes()), arcs(original.getArcs())
{}

Graph &Graph::operator=(const Graph& rhs)
{
    if (this == &rhs){
        return *this;
    }
    this->nodes=rhs.getNodes();
    this->arcs = rhs.getArcs();
    return *this;
}

//Union of graphs
Graph Graph::operator+ (const Graph& rhs) const
{
    set<string> rhsNodes = rhs.getNodes();
    set<string> newNodes = UnionSets(this->nodes,rhsNodes);
    map<string,set<string>> newArcs;
    map<string,set<string>> oldArcs1 = this->arcs;
    for (auto &ell : oldArcs1)
    {
        newArcs.insert(ell);
    }
    map<string,set<string>> oldArcs2 = rhs.getArcs();
    for (auto &ell : oldArcs2)
    {
        auto it = newArcs.find(ell.first);
        if (it != newArcs.end()){ // if the key exists than it merges the two sets of arcs
            it->second = UnionSets(it->second, ell.second);
        } else{ // if it doesn't it makes a new one
            newArcs.insert(ell);
        }
    }
    Graph unionGraph(newNodes,newArcs);
    return unionGraph;
}

//Intersection
Graph Graph::operator^ (const Graph& rhs) const
{
    set<string> rhsNodes = rhs.getNodes();
    set<string> newNodes = IntersectSets(this->nodes,rhsNodes);
    map<string,set<string>> newArcs;
    map<string,set<string>> oldArcs1 = this->arcs;
    map<string,set<string>> oldArcs2 = rhs.getArcs();
    for (auto &ell : oldArcs1)
    {
        auto it = oldArcs2.find(ell.first);
        if (it != oldArcs2.end()){
            newArcs.insert({ell.first,IntersectSets(ell.second,it->second)});
        }
    }
//    for (auto &ell : newArcs) // cleans up the arcs if there is an empty set
//    {
//        if (ell.second.empty()) {
//            newArcs.erase(ell.first);
//        }
//    }
    vector<map<string,set<string>>::iterator> removeIt;
    for (auto it = newArcs.begin(); it != newArcs.end(); it++){
        if (it->second.empty()){
            removeIt.push_back(it);
        }
    }
    for (auto it : removeIt){
        newArcs.erase(it);
    }
    Graph newGraph(newNodes, newArcs);
    return newGraph;
}

//Difference sets

Graph Graph::operator-(const Graph& rhs) const
{
    set<string> rhsNodes = rhs.getNodes();
    set<string> newNodes;
    for (const string& ell : this->getNodes())
    {
        if (rhsNodes.find(ell)==rhsNodes.end()){ //if it is found in the first nodes but not in the second
            newNodes.insert(ell);
        }
    }

    map<string,set<string>> newArcs;
    map<string,set<string>> rhsArcs = rhs.getArcs();
    for(auto &ell : this->getArcs())
    {
        string currentNodeSrc = ell.first;
        if (newNodes.find(currentNodeSrc) != newNodes.end()){
            set<string> newNodeDest;
            set<string> currentNodeDest = ell.second;
            for (const string& setEll : currentNodeDest)
            {
                if (newNodes.find(setEll) != newNodes.end()){
                    newNodeDest.insert(setEll);
                }
            }
            if (!newNodeDest.empty()){
                newArcs.insert({currentNodeSrc,newNodeDest});
            }
        }
    }
    Graph newGraph(newNodes, newArcs);
    return newGraph;
}

//Complement Graph
Graph Graph::operator!() const
{
    set<string> newNodes = this->getNodes();
    map<string,set<string>> newArcs;
    Graph complementGraph(newNodes,newArcs);
    map<string,set<string>> oldArcs = this->getArcs();
    for (const string& src : newNodes)
    {
        for (const string& dest : newNodes)
        {
            if (!FindPair(oldArcs, src, dest) && src != dest){
                complementGraph.InsertArc(src,dest);
            }
        }
    }
    return complementGraph;
}

//Product

Graph Graph::operator*(const Graph& rhs) const
{
    set<string> newNodes;
    set<string> rhsNodes = rhs.getNodes();
    for (const string& ell1 : this->nodes)
    {
        for (const string& ell2 : rhsNodes)
        {
            string productNode = "[" + ell1 + ";" + ell2 + "]";
            newNodes.insert(productNode);
        }
    }
    map<string,set<string>> newArcs;
    map<string,set<string>> rhsArcs = rhs.getArcs();
    Graph productGraph(newNodes,newArcs);
    for (auto &ell1 : this->arcs)
    {
        for (const string& dest1 : ell1.second)
        {
            for (auto &ell2 : rhsArcs)
            {
                for (const string& dest2 : ell2.second)
                {
                    string newSrc = "[" + ell1.first + ";" + ell2.first + "]";
                    string newDest = "[" + dest1 + ";" + dest2 + "]";
                    productGraph.InsertArc(newSrc,newDest);
                }
            }
        }
    }
    return productGraph;

}

//Get Nodes
set<string> Graph::getNodes() const
{
    return this->nodes;
}

//Get Arcs
map<string,set<string>>  Graph::getArcs() const
{
    return this->arcs;
}

void Graph::InsertArc(string src, string dest)
{
    auto it = this->arcs.find(src);
    if (src == dest){
        throw IllegalArcSrcDest();
    }
    if (nodes.find(src) == nodes.end()){
        throw IllegalArcNoEndpoint();
    }
    if (nodes.find(dest) == nodes.end()){
        throw IllegalArcNoEndpoint();
    }
    if (it == this->arcs.end()){ //if src is not found
        set<string> newSet;
        newSet.insert(dest);
        this->arcs.insert({src,newSet});
    } else{
        it->second.insert(dest);
    }
}

void Graph::InsertArcChar (char* src, char* dest)
{
    auto it = this->arcs.find(src);
    if (src == dest){
        throw IllegalArcSrcDest();
    }
    if (nodes.find(src) == nodes.end()){
        throw IllegalArcNoEndpoint();
    }
    if (nodes.find(dest) == nodes.end()){
        throw IllegalArcNoEndpoint();
    }
    if (it == this->arcs.end()){ //if src is not found
        set<string> newSet;
        newSet.insert(dest);
        this->arcs.insert({src,newSet});
    } else{
        it->second.insert(dest);
    }
}

void Graph::InsertNode(const string givenNode)
{
    if (!LegalNodeName(givenNode)){
        throw IllegalNodeName();
    }
    this->nodes.insert(givenNode);
}

void Graph::InsertNodeChar (char* givenNode)
{
    if (!LegalNodeName(givenNode)){
        throw IllegalNodeName();
    }
    this->nodes.insert(givenNode);
}

std::ostream &operator<<(std::ostream &os, const Graph &target)
{
    for (const string& str : target.getNodes())
    {
        os << str << endl;
    }
    os << "$" << endl;
    for (auto &ell : target.getArcs()) {
        set<string> connections = ell.second;
        for (const string &str : connections) {
            os << ell.first << " " << str << endl;
        }
    }
    return os;
}








//--------------------------------------STATIC FUNCTIONS -------------------------------------------------------------

//Union sets
static set<string> UnionSets (set<string> set1, set<string> set2)
{
    set<string> newSet;
    for (string element1 : set1)
    {
        newSet.insert(element1);
    }
    for (string element2: set2)
    {
        newSet.insert(element2);
    }
    return newSet;
}

//Intersects sets
static set<string> IntersectSets (const set<string> set1, const set<string> set2)
{
    set<string> newSet;
    for (string element1 : set1)
    {
        set<string>::iterator it = set2.find(element1);
        if (it != set2.end()){
            newSet.insert(element1);
        }
    }
    return newSet;
}

static bool FindPair (map<string,set<string>> givenArcs, string src, string dest)
{
    map<string,set<string>>:: iterator it = givenArcs.find(src);
    if(it != givenArcs.end()){
        set<string>::iterator it2 = it->second.find(dest);
        if (it2 != it->second.end()){
            return true;
        }
    }
    return false;
}

static bool LegalNodeName(const string name)
{
    if (name == ""){
        return false;
    }

    for (char c : name) {
        if (!isalnum(c) && c !='[' && c !=']' && c !=';') {
            return false;
        }
    }

    int pCount = 0;
    for (char c : name)
    {
        if (c == '['){
            pCount++;
        }
        if (c == ']'){
            pCount--;
        }
        if(pCount < 0){
            return false;
        }
    }
    if (pCount != 0){
        return false;
    }
    if (name.find(";") != string::npos) {
        for (char c : name) {
            if (c == '[') {
                pCount++;
            }
            if (c == ']') {
                pCount--;
            }
            if (c == ';' && pCount == 0) {
                return false;
            }
            if (pCount < 0) {
                return false;
            }
        }
    }
    return true;


}


