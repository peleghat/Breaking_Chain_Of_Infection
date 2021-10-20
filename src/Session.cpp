//
// Created by spl211 on 03/11/2020.
//
#include "../include/Session.h"





    Session::Session(const std::string &path): g(),treeType(MaxRank), agents(vector<Agent*>(0)), infectionQueue(queue<int>()), cycle(0) { // Constructor

     std::ifstream file(path);
        json j;
        file >> j ;
      g=Graph(j["graph"]);
      string Ttype = (string)j["tree"];
      if (Ttype == "M")
          treeType=MaxRank;
      else if (Ttype == "C")
          treeType=Cycle;
      else
          treeType=Root;


     for (auto& elem: j["agents"]) {
         Agent* newAgent;
         if (elem[0]=="V")
             newAgent = new Virus(elem[1]);
         else { // we need to create a ContactTracer
             newAgent = new ContactTracer();
         }
         agents.push_back(newAgent);
     }
 }


    Session::~Session() {// Destructor
       clear();
    }

    void Session::clear() {
        for(Agent* x :agents){
            if (x) {
                delete x;
            }
        }
        while (!agents.empty()) {
            agents.pop_back(); // Making the Vector empty.
        }
    }

    Session::Session(const Session &other): g(other.g),treeType(other.treeType), agents(vector<Agent*>(0)),infectionQueue(other.infectionQueue), cycle(other.cycle){ // Copy Constructor
        for (int i=0; i< (int) other.agents.size(); i++) {
            Agent* newAgent = other.agents[i]->clone();
            agents.push_back(newAgent);
        }
    }

    Session::Session(Session &&other): g(other.g), treeType(other.treeType) , agents(vector<Agent*>(0)), infectionQueue(other.infectionQueue), cycle(other.cycle) { // Move Constructor
       for (int i=0; i< (int) other.agents.size(); i++) {
           agents.push_back(other.agents[i]);
       }
       for (int i=0; i< (int) other.agents.size(); i++) {
           other.agents[i]= nullptr;
       }
    }


    Session & Session::operator=(const Session &other) { // Copy Assignment Operator
        if(this==&other){ // Check for self assignment
            return *this;
        }
        clear();
        g=other.g;
        treeType=other.treeType;
        infectionQueue=other.infectionQueue;
        cycle=other.cycle;

        for (int i=0; i< (int) other.agents.size(); i++) {
            Agent* newAgent = other.agents[i]->clone();
            agents.push_back(newAgent);
        }
       return *this;
    }

    Session & Session::operator=(Session &&other) { // Move Assignment Operator
        if(this == &other) {//check for self assignment
            return *this;
        }
        else {
            clear();
            for (int i = 0; i < (int) other.agents.size() ; i++) {
                agents.push_back(other.agents[i]);
                other.agents[i]= nullptr;
            }
            g=other.g;
            treeType=other.treeType;
            infectionQueue=other.infectionQueue;
            cycle=other.cycle;
        }
        return *this;
    }


    Graph Session::getG() const {
        return g;
    }
    void Session::setGraph(const Graph &graph){
        g=graph;
    }
    int Session::getCycle() const {
        return cycle;
    }
    void Session::incrementCycle() {
        cycle=cycle+1;
    }
    TreeType Session::getTreeType() const {
        return treeType;
    }
    Tree* Session::bfs(int rootLabel) {

        std::vector<std::vector<int>> adj = g.getEdges();
        std::queue<Tree*> q; //create empty queue.
        std::vector<bool> discovered(adj.size());
        for (int i=0; i< (int) discovered.size() ; i++) { // initialize discovered.
            discovered[i] = false;
        }

        Tree* bfsTree = Tree::createTree(*this,rootLabel);
        q.push(bfsTree);
        discovered[rootLabel]=true;



        while (!q.empty()) {
            Tree* v = q.front();
            q.pop();

            for (int i = 0; i < (int) adj.size(); i++) {
                if (adj[v->getNode()][i] == 1) {
                    if (!discovered[i]) {
                        Tree* tmpTree = Tree::createTree(*this,i);
                        q.push(tmpTree);
                        v->addChild(tmpTree);
                        discovered[i]=true;
                    }
                }
            }
        }
        return bfsTree;
    }

    void Session::isolateNode(int node) { // Method that will be used by contact tracer.
        std::vector<std::vector<int>> adj=g.getEdges();
        for (int i = 0; i < (int) adj.size(); ++i) { // Remove all neighbors of node.
            adj[node][i]=0;
            adj[i][node]=0;
        }
        g.setEdges(adj);
    }

    void Session::enqueueInfected(int nodeInd) {
        infectionQueue.push(nodeInd);
    }
    bool Session::queueIsEmpty(){
        return infectionQueue.empty();
}

    int Session::dequeueInfected() {
        int  output = infectionQueue.front();
        infectionQueue.pop();
        return output;
    }

    void Session::addAgent(const Agent& agent) {
        Agent* newAgent = agent.clone();
        agents.push_back(newAgent);
    }

    void Session::simulate() {
        //First Iteration
        int s = agents.size();
        for (int i=0; i< s; i++) {
            agents[i]->act(*this);
        }
        incrementCycle();

        while (!checkTermination() || s < (int) agents.size()) {
            s = agents.size();
            for (int i=0; i<s; i++) {
                agents[i]->act(*this);

            }
            incrementCycle();
        }

        std::vector<int> whoGotInfected;
        for (int i = 0; i < (int) g.getInfected().size(); i++) {
            if (g.getInfected()[i]) {
                whoGotInfected.push_back(i);
            }
        }
        json  j; // Create Json output.
        j["graph"]=g.getEdges();
        j["infected"]=whoGotInfected;
        std::ofstream i("./output.json");
        i << j;
    }


    bool Session::checkTermination() { // Checks if termination conditions are satisfied.
    bool satisfied=true;
    std::vector<std::vector<int>> adj=g.getEdges();
    std::vector<bool> inf =g.getInfected();
        for (int i = 0; i < (int) adj.size() && satisfied ; i++) {
            for (int j = 0; j < (int) adj.size() && satisfied; j++) {
                if(adj[i][j]==1 && inf[i]!=inf[j]){ // If two neighbors but different state-not satisfied.
                    satisfied=false;
                }
            }
        }
        return satisfied;
    }

    bool Session::containsAgent(int nodeInd) {
        bool output = false;
        for(Agent* x:agents){
            if(x->getNodeInd()==nodeInd) {
                output = true;
                return output;
            }
        }
        return output;
    }
