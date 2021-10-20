//
// Created by spl211 on 03/11/2020.
//
#include "../include/Agent.h"


    // Agent
    Agent::Agent() {} // Constructor

    Agent::~Agent()  {}

    // ContactTracer
    ContactTracer::ContactTracer(): Agent() {}
    Agent * ContactTracer::clone() const {
        return new ContactTracer (*this);
    }
    int ContactTracer::getNodeInd() const {
        return -1;
    }


    void ContactTracer::act(Session &session) {
        if(!session.queueIsEmpty()){
            int ind = session.dequeueInfected(); // Get a node from infection queue.
            Tree* bfsTree= session.bfs(ind); // Create a tree to choose node.
            if (!bfsTree->getChildren().empty()) {
                int toIsolate=bfsTree->traceTree();
                session.isolateNode(toIsolate);
            }
            delete bfsTree;
        }

    }

    // Virus
    Virus::Virus(int nodeInd): Agent(), nodeInd(nodeInd) {}
    Agent * Virus::clone() const {
        return new Virus (*this);
    }


    int Virus::getNodeInd() const {
        return nodeInd;
    }


    void Virus::act(Session &session) {
        bool acted = false;
        Graph graph = session.getG();
        if (!graph.isInfected(nodeInd)){ // the Virus infects his own node
            graph.infectNode(nodeInd);
            session.enqueueInfected(nodeInd); // enqueue the nodeInd to the Infection Queue
        }
        for (int i = 0; !acted && i < (int) graph.getEdges().size(); i++) {
            if (graph.getEdges()[nodeInd][i]==1) { // Neighbours
                if (!graph.getInfected()[i] && !session.containsAgent(i)) { // Not Infected
                    session.addAgent(Virus(i));
                    acted = true;
                }
            }
        }
        session.setGraph(graph);
    }

