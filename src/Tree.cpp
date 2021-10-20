//
// Created by spl211 on 03/11/2020.
//
#include "../include/Tree.h"


    //Constructors
    Tree::Tree(int rootLabel): node(rootLabel), children(vector<Tree*>(0)) {} // Constructor
    Tree::Tree() : node(-1), children(vector<Tree*>(0))  {} // Default Constructor
    Tree::~Tree() {
        clear();
    }

    // Rule of Five
    void Tree::clear() { // Recursive method to act in Destructor.
        if(!children.empty()){
            while(!children.empty()){
                children[children.size()-1]->clear();
                delete children[children.size()-1];
                children.pop_back();
            }
        }
    }

    Tree::Tree(const Tree &other): node(other.node), children(vector<Tree*>(0)) { // Copy Constructor
        for (Tree* x :other.children) {
            children.push_back(x->clone());
        }
    }

    Tree::Tree(Tree &&other): node(other.node), children(other.children) { // Move Constructor
        other.children=std::vector<Tree*>();
    }

    Tree & Tree::operator=(const Tree &other) { // Copy Assignment Operator
        if(this == &other) { // Check for Self Assignment
            return *this;
        }
        else {
            clear();
            node = other.node;
            for (Tree* x : other.children){
                children.push_back(x->clone());
            }
           return *this;
        }
    }

    Tree & Tree::operator=(Tree &&other) { // Move Assignment Operator
        if(this == &other) { // Check for Self Assignment
            return *this;
        }
        else {
            clear();
            node = other.node;
            children = other.children;
            for (int i=0; i< (int) other.children.size(); i++) {
                other.children[i]= nullptr;
            }
            while((int)other.children.size()>0){
                other.children.pop_back();
            }
            return *this;
        }
    }

    //Getters
    int Tree::getNode() const {
        return node;
    }
    std::vector<Tree*> Tree::getChildren() const {
        return children;
    }

    //Setters
    void Tree::setNode(int newNode) {
        node=newNode;
    }
    void Tree::setChildren(std::vector<Tree *> newChildren) {
        children=newChildren;
    }

    //Other Methods
    Tree* Tree::createTree(const Session &session, int rootLabel) {
        Tree* output;

        if (session.getTreeType()==Cycle)
            output = new CycleTree(rootLabel,session.getCycle());
        else if (session.getTreeType()==MaxRank)
            output = new MaxRankTree(rootLabel);
        else // (session.getTreeType()==Root)
            output = new RootTree(rootLabel);

        return output;
    }
    void Tree::addChild(Tree* child) {
        children.push_back(child);
    }
    void Tree::addChild(const Tree &child){
        Tree* newTree = child.clone();
        children.push_back(newTree);
    }

    //CycleTree
    CycleTree::CycleTree(int rootLabel, int currCycle = 0): Tree(rootLabel), currCycle(currCycle) {}
    Tree * CycleTree::clone() const {
        if(getChildren().empty()){ // if node is a leaf,duplicate
            return new CycleTree(this->getNode(),this->currCycle);
        }
        else{//if not empty, recursively clone each child.
            Tree* output= new CycleTree(this->getNode(),this->currCycle);
            for(Tree* x : getChildren()){
                output->addChild(x->clone());
            }
            return output;
        }
    }
    CycleTree::~CycleTree() {// Recursive Destructor, deletes a tree only when it has no children.
        clear();
    }
    int CycleTree::traceTree() {
        int output=this->getNode();
        Tree* currNode = this;
        if (currCycle==0) {
            return output;
        }
        else {
            for (int i = 0; i < currCycle ; i++) {
                if (currNode->getChildren().empty()) {
                    return output;
                }
                else{
                    currNode=currNode->getChildren()[0];
                    output=currNode->getNode();
                }
            }
            return output;
        }
    }

    //MaxRankTree
    MaxRankTree::MaxRankTree(int rootLabel): Tree(rootLabel) {
    }

    MaxRankTree::~MaxRankTree(){// Recursive Destructor, deletes a tree only when it has no children.
        clear();
    }

    Tree * MaxRankTree::clone() const{ //clones the tree recursively
        if(getChildren().empty()){ // if node is a leaf,duplicate
            return new MaxRankTree(this->getNode());
        }
        else{ //if not empty, recursively clone each child.
            Tree* output= new MaxRankTree(this->getNode());
            for(Tree* x : getChildren()){
                output->addChild(x->clone());
            }
            return output;
        }
    }

    int MaxRankTree::traceTree() {//iterative bfs scan.
        std::queue<Tree*> q; //create empty queue.
        int output = this->getNode();
        int maxdeg = this->getChildren().size();
        q.push(this);
        while (!q.empty()) {
            Tree* v = q.front();
            q.pop();
            for (Tree* x : v->getChildren()) {
                if ((int) x->getChildren().size() > maxdeg) {
                    output=x->getNode();
                    maxdeg=x->getChildren().size();
                }
                q.push(x);
            }
        }
        return output;
    }

    //RootTree
    RootTree::RootTree(int rootLabel): Tree(rootLabel) {}
    Tree * RootTree::clone() const{
        if(getChildren().empty()){// if node is a leaf,duplicate
            return new RootTree(this->getNode());
        }
        else{ //if not empty, recursively clone each child.
            Tree* output= new RootTree(this->getNode());
            for(Tree* x : getChildren()){
                output->addChild(x->clone());
            }
            return output;
        }
    }
    RootTree::~RootTree(){ // Recursive Destructor, deletes a tree only when it has no children.
        clear();
    }
    int RootTree::traceTree() {
        return this->getNode();
    }