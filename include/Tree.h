/*#ifndef TREE_H_
#define TREE_H_

#include <vector>
#include "../include/Session.h"
#include "queue"

class Session;

class Tree{

public:
    //Constructors
    Tree(int rootLabel); // Constructor
    Tree(); // Default Constructor
    virtual Tree* clone() const=0;

    //Getters
    int getNode() const ;
    std::vector<Tree*> getChildren() const ;

    //Setters
    void setNode (int newNode);
    void setChildren (std::vector<Tree*> newChildren);

    //Other Methods
    virtual int traceTree()=0; // Pure Virtual Method
    static Tree* createTree(const Session& session, int rootLabel);
    void addChild(Tree* child);
    void addChild(const Tree& child);

    // Rule of Five
    // Copy Constructor
    // Destructor
    // Copy Assignment Operator
    // Move Constructor
    // Move Assignment Operator

private:
    int node;
    std::vector<Tree*> children;
};

class CycleTree: public Tree{
public:
    CycleTree(int rootLabel, int currCycle);
    virtual Tree* clone() const;
    virtual int traceTree();
private:
    int currCycle;
};

class MaxRankTree: public Tree{
public:
    MaxRankTree(int rootLabel);
    virtual Tree* clone() const;
    virtual int traceTree();
};

class RootTree: public Tree{
public:
    RootTree(int rootLabel);
    virtual Tree* clone() const;
    virtual int traceTree();
};

#endif
*/

#ifndef TREE_H_
#define TREE_H_

#include <vector>
#include "../include/Session.h"
#include "queue"
class Session;

class Tree{

public:
    //Constructors
    Tree(int rootLabel); // Constructor
    Tree(); // Default Constructor

    // Rule of Five
    Tree(const Tree &other); //Copy Constructor
    virtual ~Tree(); // Destructor
    void clear();
    Tree & operator=(const Tree &aTree); //Copy Assignment Operator
    Tree(Tree &&other); //Move Constructor
    Tree& operator=(Tree &&other); //Move Assignment Operator

    virtual Tree* clone() const=0;

    //Getters
    int getNode() const ;
    std::vector<Tree*> getChildren() const ;

    //Setters
    void setNode (int newNode);
    void setChildren (std::vector<Tree*> newChildren);

    //Other Methods
    virtual int traceTree()=0; // Pure Virtual Method
    static Tree* createTree(const Session& session, int rootLabel);
    void addChild(Tree* child);
    void addChild(const Tree& child);



private:
    int node;
    std::vector<Tree*> children;
};

class CycleTree: public Tree{
public:
    CycleTree(int rootLabel, int currCycle);
    virtual Tree* clone() const;
    virtual int traceTree();
    virtual ~CycleTree();
private:
    int currCycle;
};

class MaxRankTree: public Tree{
public:
    MaxRankTree(int rootLabel);
    virtual Tree* clone() const;
    virtual int traceTree();
    virtual ~MaxRankTree();
};

class RootTree: public Tree{
public:
    RootTree(int rootLabel);
    virtual Tree* clone() const;
    virtual int traceTree();
    virtual ~RootTree();
};

#endif