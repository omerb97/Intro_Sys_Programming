#ifndef FINAL_PROJECT_GRAPH_H
#define FINAL_PROJECT_GRAPH_H

#include "Exceptions.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>



class Graph
    {
    private:
        std::set<std::string> nodes;
        std::map<std::string,std::set<std::string>> arcs;
    public:
    explicit Graph(std::set<std::string> givenNodes ={}, std::map<std::string,std::set<std::string>> givenArcs ={});
    ~Graph(); //D'tor
    Graph(const Graph& graph); // Copy C'tor
    Graph& operator= (const Graph&); // operator= overload
    Graph operator+ (const Graph&) const; // Union
    Graph operator^ (const Graph&) const; // Intersection
    Graph operator- (const Graph&) const; // Difference
    Graph operator* (const Graph&) const; // Product
    Graph operator! () const; //Complement
    void InsertArc (std::string src, std::string dest);
    void InsertArcChar (char*, char*);
    void InsertNode (std::string);
    void InsertNodeChar (char*);
    std::set<std::string> getNodes() const;
    std::map<std::string,std::set<std::string>>  getArcs() const;
    friend std::ostream& operator<<(std::ostream& os, const Graph& target);

    };
#endif
