#include "../include/Graph.h"


    //Constructors
    Graph::Graph(std::vector<std::vector<int>> matrix): edges(matrix), infected(std::vector<bool>(0)) { // Constructor
        for (int i = 0; i <(int) matrix.size(); i++) { // Initialize the infected vector elements to "F"
            infected.push_back(false);
        }
    } // Constructor
    Graph::Graph(): edges(std::vector<std::vector<int>>(0)), infected(std::vector<bool>(0)) {}  //Default Constructor

    //Getters
    std::vector<std::vector<int>> Graph::getEdges() const {
        return edges;
    }
    std::vector<bool> Graph::getInfected() const {
    return infected;
}

    //Setters
    void Graph::setEdges(std::vector<std::vector<int>> newEdges) {
    edges=newEdges;
}
    void Graph::setInfected(std::vector<bool> newInfected) {
    infected=newInfected;
}

    //Other Methods
    void Graph::infectNode(int nodeInd) {
        infected[nodeInd]=true;
    }
    bool Graph::isInfected(int nodeInd) {
       return infected[nodeInd];
    }

