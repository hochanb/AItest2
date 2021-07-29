#ifndef _NEW_NEW_
#define _NEW_NEW_
#include <iostream>
#include <queue>
#include <vector>
#include <random>
using namespace std;

#define MAX_NUM 1000
namespace newai{

enum MUTATION{
    ADD_NEURON =0,
    ADD_SYNAPSE=1,
    DEL_SYNAPSE=2
};

typedef struct Mutation_Log{
    MUTATION m;
    Neuron* neuron_selected; //대상 뉴런
    Neuron* neuron_prev; //대상 뉴런에 연결될 이전 뉴런 (뉴런 생성 시에만 필요)
    int weight_prev_sel; //sign from &prev to &sel
    Neuron* neuron_next; //대상 뉴런에 연결할 다음 뉴런
    int weight_sel_next; //sign from &sel to &next
    
}MLog;

class Neuron{
    int index=0; //index of this neuron
    int buffer=0; //buffer for signal accumulation
    bool activated=false; //
    vector<Neuron*> nexts; //indexes of next neurons
    vector<int> weights; //weight of sign to each next neurons. two vectors are pair
    vector<Neuron*> prevs; //indexes of previous neurons

    static int RandomSign(int mean);
    static int EnhanceFunction(int n);
    public:
    Neuron(int _index); //constructor
    int GetIndex();
    int AddBuffer(int s); //add s to buffer
    void Enhance();
    void CheckActive(); //check if buffer is full enough (buffer>MAX_NUM)
    bool GetActivated();
    void Propagate(); //if activated, propagate the sign to next neurons
    void AddNextNeuron(Neuron* next, int s); //add next neuron's index to 'nexts'
    void AddPrevNeuron(Neuron* prev); //add prev neuron's index to 'prevs'
    void ShowState();
};

class Brain{
    int num_inputs;
    int num_outputs;
    int num_neurons=0; //number of neurons
    int num_synapses=0; //number of synapses. decides average num of synapse of each neuron. [ n ~ n^2 ]. 
    //int MSI; //mean sign intensity

    void AddNeuron();
    void AddSynapse();
    void DelSynapse();
    void Terminate();

    public:
    vector<MLog> log;
    vector<Neuron*> neurons; //for convenience and speed, neurons of brain is set public
    Brain(int num_input,int num_output);
    static void Mutate(Brain* brain, double mutate_rate);
    void Propagate(); //propagates one time
    void CheckActive(); //check
    void ShowStatus();
    void Initialize();
    void Update(); //input->check->propagate->output->input->...
    //~Brain();
};
//////////////////////////////////////
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