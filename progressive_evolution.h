#ifndef _NEW_NEW_
#define _NEW_NEW_
#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <random>
using namespace std;

#define MAX_NUM 100
#define WEIGHT_BIAS 10 //bias, mean
#define WEIGHT_RANGE 200 //+- range

namespace newai{



class Neuron; //declaration

enum MUTATION{
    ADD_NEURON =0,
    ADD_SYNAPSE=1,
    //DEL_SYNAPSE=2
    MOD_WEIGHT=2
};
enum NEURON_TYPE{
    INTER_NEURON=0, //inter neuron
    INPUT_NEURON=1, //~sensory neuron
    OUTPUT_NEURON=2 //~motor neuron
};

typedef struct Mutation_Log{
    MUTATION m;
    //Neuron* neuron_prev; //시냅스 전뉴런
    //Neuron* neuron_next; //시냅스 후뉴런
    int neuron_prev; //전뉴런 ind. empty=-1
    int neuron_next; //후뉴런 ind. empty=-1
    int weight; //가중치 empty=0
    
}MLog;

string MLogToString(MLog mlog);

class Neuron{
    int index=0; //index of this neuron
    int buffer=0; //buffer for signal accumulation
    bool activated=false; //
    NEURON_TYPE type;

    static int EnhanceFunction(int n);

    public:
    vector<int> weights; //weight of sign to each next neurons. two vectors are pair
    vector<Neuron*> nexts; //indexes of next neurons
    vector<Neuron*> prevs; //indexes of previous neurons
    Neuron(int _index); //default type is interneuron
    Neuron(int _index,NEURON_TYPE t); //for in/out neurons
    int GetIndex();
    NEURON_TYPE GetNeuronType();
    int AddBuffer(int s); //add s to buffer
    void Enhance();
    void CheckActive(); //check if buffer is full enough (buffer>MAX_NUM)
    bool GetActivated();
    void Propagate(); //if activated, propagate the sign to next neurons
    void AddNext(Neuron* next, int s); //add next neuron's index to 'nexts'
    void AddPrev(Neuron* prev); //add prev neuron's index to 'prevs'
    void ShowStatus();

};

class Brain{
    int num_inputs;
    int num_outputs;
    int num_neurons=0; //number of neurons
    string name;
    //int num_synapses=0; //number of synapses. decides average num of synapse of each neuron. [ n ~ n^2 ]. 
    //int MSI; //mean sign intensity

    static int RandomWeight();
    void ShowStatus();
    void AddNeuron(Neuron* prev, Neuron* next); //nullptr->random sel
    int AddSynapse(Neuron* from, Neuron* to); //nullptr->random sel 0:fail 1:success
    //void DelSynapse(Neuron* from, Neuron* to); //nullptr->random sel
    void ModWeight(Neuron* from, Neuron* to, int w); //nullptr, 0->random sel
    //void Terminate();
    // void Run();
    int Mutate(MUTATION m);
    int GetTotalSynapse();

    public:
    Brain(string name, int num_input,int num_output);
    vector<MLog> log;
    vector<Neuron*> neurons; //for convenience and speed, neurons of brain is set public
    void ManualControl();
    void Initialize();
    void SetInput(int* inputs); //get input values from int array. array size must be num_inputs
    void GetOutput(int* outputs); //set output values to int array. array size must be num_outputs
    void Propagate(); //propagates one time
    void CheckActive(); //check
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

    //color dimensions. 0~255 -> 0~63
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