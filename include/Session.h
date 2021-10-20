#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <string>
#include <iostream>
#include "json.hpp"
#include <fstream>
#include "queue"
#include "../include/Agent.h"
#include "Tree.h"
#include "Graph.h"


class Tree;
class Agent;
using json = nlohmann::json ;
using namespace std;

enum TreeType{
    Cycle,
    MaxRank,
    Root
};


class Session{

public:
    Session(const std::string& path); // Constructor
    ~Session(); // Destructor
    void clear();
    Session(const Session &other); //Copy Constructor
    Session(Session &&other); //Move Constructor
    Session& operator=(const Session &other); //Copy Assignment Operator
    Session& operator=(Session &&other); // Move Assignment Operator

    void simulate();
    void addAgent(const Agent& agent);
    void setGraph(const Graph& graph);
    Graph getG() const;
    void isolateNode(int node);

    void enqueueInfected(int);
    int dequeueInfected();
    TreeType getTreeType() const;
    int getCycle() const;
    Tree* bfs (int rootLabel);
    void incrementCycle();
    bool checkTermination() ;
    bool queueIsEmpty(); //checks if infection Q is empty
    bool containsAgent(int nodeInd);

private:
    Graph g;
    TreeType treeType;
    std::vector<Agent*> agents;
    std::queue<int> infectionQueue;
    int cycle;
};

#endif

