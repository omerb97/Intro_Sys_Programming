#include "graph.h"
#include "Exceptions.h"
#include <iostream>

Graph* create ()
{
    return new Graph();
}

void destroy (Graph* target)
{
    if (target == nullptr){
        std::cout << "Error: This is an empty pointer" << std::endl;
        return;
    }
    delete target;
}

Graph* addVertex(Graph* target, char* v)
{
    if (target == nullptr){
        std::cout << "Error: This is an empty pointer"  << std::endl;
        return nullptr;
    }
    try{
        target->InsertNodeChar(v);
    }
    catch(IllegalNodeName &e){
        std::cout << e.what()  << std::endl;
    }
    return target;
}

Graph* addEdge(Graph* target, char* v1, char* v2)
{
    if (target == nullptr){
        std::cout << "Error: This is an empty pointer"  << std::endl;
        return nullptr;
    }
    try{
        target->InsertArcChar(v1,v2);
    }
    catch(ArcException &e){
        std::cout << e.what()  << std::endl;
    }
    return target;
}

void disp(Graph* target)
{
    if (target == nullptr){
        std::cout << "Error: This is an empty pointer"  << std::endl;
        return;
    }
    std::cout << (*target);
}

Graph* graphUnion (Graph* graph_in1, Graph* graph_in_2, Graph* graph_out)
{
    if (graph_in1 == nullptr || graph_in_2 == nullptr || graph_out == nullptr) {
        std::cout << "Error: There is an nullptr operand"  << std::endl;
        return nullptr;
    }
    *graph_out = (*graph_in1) + (*graph_in_2);
    return graph_out;
}

Graph* graphIntersection (Graph* graph_in1, Graph* graph_in_2, Graph* graph_out)
{
    if (graph_in1 == nullptr || graph_in_2 == nullptr || graph_out == nullptr) {
        std::cout << "Error: There is an nullptr operand"  << std::endl;
        return nullptr;
    }
    *graph_out = (*graph_in1) ^ (*graph_in_2);
    return graph_out;
}

Graph* graphDifference (Graph* graph_in1, Graph* graph_in_2, Graph* graph_out)
{
    if (graph_in1 == nullptr || graph_in_2 == nullptr || graph_out == nullptr) {
        std::cout << "Error: There is an nullptr operand"  << std::endl;
        return nullptr;
    }
    *graph_out = (*graph_in1) - (*graph_in_2);
    return graph_out;
}

Graph* graphProduct (Graph* graph_in1, Graph* graph_in_2, Graph* graph_out)
{
    if (graph_in1 == nullptr || graph_in_2 == nullptr || graph_out == nullptr) {
        std::cout << "Error: There is an nullptr operand"  << std::endl;
        return nullptr;
    }
    *graph_out = (*graph_in1) * (*graph_in_2);
    return graph_out;
}

Graph* graphComplement (Graph* graph_in, Graph* graph_out)
{
    if (graph_in == nullptr || graph_out == nullptr) {
        std::cout << "Error: There is an nullptr operand"  << std::endl;
        return nullptr;
    }
    *graph_out = !(*graph_in);
    return graph_out;
}