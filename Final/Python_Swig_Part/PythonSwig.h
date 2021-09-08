#ifndef FINALPROJECT_SWIG_PYTHONSWIG_H
#define FINALPROJECT_SWIG_PYTHONSWIG_H

#include "Graph.h"


Graph* create();
void destroy (Graph* target);

Graph* addVertex(Graph* target, char* v);
Graph* addEdge(Graph* target, char* v1, char* v2);
void disp(Graph* target);

Graph* graphUnion (Graph* graph_in1, Graph* graph_in_2, Graph* graph_out);
Graph* graphIntersection (Graph* graph_in1, Graph* graph_in_2, Graph* graph_out);
Graph* graphDifference (Graph* graph_in1, Graph* graph_in_2, Graph* graph_out);
Graph* graphProduct (Graph* graph_in1, Graph* graph_in_2, Graph* graph_out);
Graph* graphComplement (Graph* graph_in, Graph* graph_out);
#endif
