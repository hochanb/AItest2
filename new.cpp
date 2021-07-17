#include <iostream>
#include <queue>
#include <vector>
using namespace std;

#define MAX_NUM 512

int main(){
    cout<<"hello~~"<<endl;
        
}


class Neuron{
    int index=0; //index of this neuron
    int sign=0; //weight of sign to propagate
    int buffer=0; //buffer for signal accumulation
    bool activate=false; //
    queue<int> nexts; //indexes of next neurons
    queue<int> prevs; //indexes of previous neurons

    public:
    Neuron(int _index); //constructor
    int AddBuffer(int s); //add s to buffer
    void Activate(); //if activated, propagate the sign to next neurons
    void AddNextNeuron(int _index); //add next neuron's index to 'nexts'
    void AddPrevNeuron(int _index); //add prev neuron's index to 'prevs'
};

class Brain{
    int num_neurons; //number of neurons
    int num_connections; //number of connections. decides average num of synapse of each neuron. [ n ~ n^2 ]

    vector<Neuron> neurons;


};

