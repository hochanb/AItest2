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

    public:
    Neuron(int _index); //constructor
    Neuron(int _index, int _sign); //constructor
    int GetIndex();
    int AddBuffer(int s); //add s to buffer
    void CheckActive(); //check if buffer is full enough (buffer>MAX_NUM)
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
    void GetInputSignal(); //sets input signals to some neurons
    void CheckActive(); //check
    void Propagate(); //propagates one time
    void SetOutputSignal();

    public:
    vector<Neuron*> neurons;
    Brain(int neurons,int connections);
    void Initialize();
    void Step(); //input->check->propagate->output->input->...
    //외부에 대한 신호의 입출력 프로토콜이 필요.
};

class World{
    int width;
    int height;
    //Object*** world; //2d array  of object pointer


};

class Object{
    int x;
    int y;

};

//////////////////////////////////////
class Body : Object{
    Brain* brain;    

};

}
#endif