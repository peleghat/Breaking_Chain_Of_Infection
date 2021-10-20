#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>

class Graph{
public:

    //Constructors
    Graph(std::vector<std::vector<int>> matrix); // Constructor
    Graph(); //Default Constructor

    //Getters
    std::vector<std::vector<int>> getEdges() const;
    std::vector<bool> getInfected() const;

    //Setters
    void setEdges (std::vector<std::vector<int>> newEdges);
    void setInfected (std::vector<bool> newInfected);

    //Other Methods
    void infectNode(int nodeInd);
    bool isInfected(int nodeInd);


private:
    std::vector<std::vector<int>> edges;
    std::vector<bool> infected;


};

#endif
