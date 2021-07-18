#include <iostream>
#include <queue>
#include <vector>
#include <random>
using namespace std;

#define MAX_NUM 100

int main(){

    cout<<"hello~~"<<endl;
        
}
//////////////////////////////////////
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
    int AddBuffer(int s); //add s to buffer
    void CheckActive(); //check if buffer is full enough (buffer>MAX_NUM)
    void Propagate(); //if activated, propagate the sign to next neurons
    void AddNextNeuron(Neuron* next); //add next neuron's index to 'nexts'
    void AddPrevNeuron(Neuron* prev); //add prev neuron's index to 'prevs'
};

Neuron::Neuron(int _index){
    index=_index;
}
Neuron::Neuron(int _index,int _sign){
    index=_index;
    sign=RandomSign(_sign);
}

int Neuron::RandomSign(int mean){
    int c=MAX_NUM-mean;
    std::random_device rd;
    // random_device 를 통해 난수 생성 엔진을 초기화 한다.
    std::mt19937 gen(rd());
    // 0 부터 99 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
    std::uniform_int_distribution<int> dis(0, 1); //p=1/2

    for(int i=0;i<c;i++){
        if(dis(gen)==0)
            mean++;
        else
            mean--;
    }
    return mean;
}

int Neuron::AddBuffer(int s){
    buffer+=s;
}
void Neuron::CheckActive(){
    activated = buffer>=MAX_NUM;
    if(activated) buffer-=MAX_NUM;
}
void Neuron::Propagate(){
    if(!activated) return;

    int size=nexts.size();
    for(int i=0;i<size;i++)
        nexts[i]->AddBuffer(sign);

    activated=false;
}
void Neuron::AddNextNeuron(Neuron* next){
    nexts.push_back(next);
}
void Neuron::AddPrevNeuron(Neuron* prev){
    prevs.push_back(prev);
}

//////////////////////////////////////
class Brain{
    int num_neurons; //number of neurons
    int num_connections; //number of connections. decides average num of synapse of each neuron. [ n ~ n^2 ]. 
    
    vector<Neuron*> neurons;

    public:
    Brain(int neurons,int connections);
    void Initialize();
    void Run(); //input->check->propagate->output->input->...
    void GetInputSignal(); //sets input signals to some neurons
    void CheckActive(); //check
    void Propagate(); //propagates one time
    void SetOutputSignal();
    //외부에 대한 신호의 입출력 프로토콜이 필요.
};

Brain::Brain(int _num_neurons, int _num_connections){
    num_neurons=_num_neurons;
    num_connections=_num_connections;
}

void Brain::Initialize(){
    //initialize neurons
    int sign = MAX_NUM * num_neurons / num_connections;
    for(int i=0;i<num_neurons;i++){
        neurons.push_back(new Neuron(i,sign)); //create new neuron;
        /* !! new Neuron MUST be deleted !! */
    }
    
    //make connections
    //random func from https://modoocode.com/304
    std::random_device rd;
    // random_device 를 통해 난수 생성 엔진을 초기화 한다.
    std::mt19937 gen(rd());
    // 0 부터 99 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
    std::uniform_int_distribution<int> dis(0, num_neurons-1);

    for(int i=0;i<num_connections;i++){
        int from = dis(gen);
        int to = dis(gen);
        //중복의 경우는 일단 무시
        neurons[from]->AddNextNeuron(neurons[to]);
        neurons[to]->AddPrevNeuron(neurons[from]);
    }
}

//////////////////////////////////////
class World{
    int width;
    int height;
    Object** world; //2d array


};

class Object{
    int x;
    int y;

};

//////////////////////////////////////
class Body : Object{
    Brain* brain;    

};