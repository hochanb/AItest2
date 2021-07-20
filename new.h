#ifndef _NEW_NEW_
#define _NEW_NEW_
#include <iostream>
#include <queue>
#include <vector>
#include <random>
using namespace std;

#define MAX_NUM 1000
namespace newai{


class Neuron{
    int index=0; //index of this neuron
    int sign=0; //weight of sign to propagate
    int buffer=0; //buffer for signal accumulation
    bool activated=false; //
    vector<Neuron*> nexts; //indexes of next neurons
    vector<Neuron*> prevs; //indexes of previous neurons

    static int RandomSign(int mean);
    static int EnhanceFunction(int n);
    public:
    Neuron(int _index); //constructor
    Neuron(int _index, int _sign); //constructor
    int GetIndex();
    int AddBuffer(int s); //add s to buffer
    void Enhance();
    void CheckActive(); //check if buffer is full enough (buffer>MAX_NUM)
    bool GetActivated();
    void Propagate(); //if activated, propagate the sign to next neurons
    void AddNextNeuron(Neuron* next); //add next neuron's index to 'nexts'
    void AddPrevNeuron(Neuron* prev); //add prev neuron's index to 'prevs'
    void ShowState();
};

class Brain{
    int num_neurons; //number of neurons
    int num_connections; //number of connections. decides average num of synapse of each neuron. [ n ~ n^2 ]. 
    int MSI;

    void Terminate();

    public:
    vector<Neuron*> neurons; //for convenience and speed, neurons of brain is set public
    Brain(int neurons,int connections);
    static void Mutate(Brain* brain, double mutate_rate);
    void Propagate(); //propagates one time
    void CheckActive(); //check
    void ShowStatus();
    void Initialize();
    void Update(); //input->check->propagate->output->input->...
    //~Brain();
};

//////////////////////////////////////
class Object{
    protected:
    //World* world; //world that object is belong to

    public:
    int x;
    int y;

    //color dimensions. 0~255
    int r;
    int g;
    int b;
};

//////////////////////////////////////
class Body : Object{
    void GetInput();
    void MoveBody(int right, int left, int up, int down); //0 or 1
    public:
    Brain* brain;   
    Body(int x, int y);
    void Update(); //get input -> update brain -> move body
};


//////////////////////////////////////
class World{
    int width;
    int height;
    //Object*** world; //2d array  of object pointer


};


}
#endif