#include "progressive_evolution.h"
using namespace newai;
//////////////////////////////////////


int main(){



     
}

//////////////////////////////////////
Neuron::Neuron(int _index){
    index=_index;
    type=INTER_NEURON;
}
Neuron::Neuron(int _index,NEURON_TYPE t){
    index=_index;
    type=t;
}
int Neuron::GetIndex(){
    return index;
}
NEURON_TYPE Neuron::GetNeuronType(){
    return type;
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
    return buffer;
}
int Neuron::EnhanceFunction(int n){
    int y=0;
    return y;
}
void Neuron::Enhance(){
    if(activated){

    }
}
void Neuron::CheckActive(){
    activated = buffer>=MAX_NUM;
    if(activated) buffer-=MAX_NUM;
}
bool Neuron::GetActivated(){
    return activated;
}
void Neuron::Propagate(){
    if(!activated) return;

    int size=nexts.size();
    for(int i=0;i<size;i++)
        nexts[i]->AddBuffer(weights[i]);

    //activated=false;
}
void Neuron::AddNext(Neuron* next, int s){
    nexts.push_back(next);
    weights.push_back(s);
}
void Neuron::AddPrev(Neuron* prev){
    prevs.push_back(prev);
}
void Neuron::ShowState(){
    cout<<"//////////////////////////////"<<endl;
    cout<<"#"<<index<<" buffer="<<buffer<<" activated="<<activated<<endl;
    cout<<"prev neurons ("<<prevs.size()<<"): "<<endl;
    for(auto neu : prevs)
        cout<<" #"<<neu->GetIndex();
    cout<<endl;
    cout<<"next neurons ("<<nexts.size()<<"): "<<endl;
    for(auto neu : nexts)
        cout<<" #"<<neu->GetIndex();
    cout<<'\n'<<endl;

}

//////////////////////////////////////
Brain::Brain(int _num_input,int _num_output){
    num_inputs=_num_input;
    num_outputs=_num_output;
}
void Brain::Mutate(MUTATION m){
    switch (m)
    {
    case ADD_NEURON:
        AddNeuron();
        break;
    case ADD_SYNAPSE:
        AddSynapse(nullptr,nullptr);
        break;
    // case DEL_SYNAPSE:
    //     DelSynapse();
    //     break;
    default:
        break;
    }   
}
void Brain::AddNeuron(){

    //instantiate new neuron object
    Neuron* nn=new Neuron(num_neurons++);
    neurons.push_back(nn);

    //log
    MLog addneuron={ADD_NEURON,nn,nullptr,0};
    log.push(addneuron);

    //make initial connections
    AddSynapse(nullptr,nn);
    AddSynapse(nn,nullptr);
    return;
}
void Brain::AddSynapse(Neuron* from, Neuron* to){
    //select two random neurons and connect them
    //input neurons are not selected for next neuron
    //output neurons are not selected for prev neuron
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, num_neurons-1); 
    
    Neuron* n_from;
    Neuron* n_to;
    int weight=WEIGHT_BIAS;

    if(from==nullptr){
        int i_from;
        do{
            i_from=dis(gen);
        }while(neurons[i_from]->GetNeuronType()==OUTPUT_NEURON);
        n_from=neurons[i_from];
    }
    else n_from=from;

    if(to==nullptr){
        int i_to;
        do{
            i_to=dis(gen); 
        }while(neurons[i_to]->GetNeuronType()==INPUT_NEURON);
        n_to=neurons[i_to];
    }
    else n_to=to;
    
    std::uniform_int_distribution<int> dis2(-1,1);
    for(int i=0;i<WEIGHT_RANGE;i++)
        weight+=dis2(gen);
    


    n_from->AddNext(n_to,weight);
    n_to->AddPrev(n_from);
    
    //add log
    MLog addsynapse={ADD_SYNAPSE,from,to,weight};
    log.push(addsynapse);
}
// void Brain::DelSynapse(Neuron* from, Neuron* to){
    
// }
void Brain::ShowStatus(){
    int count=0;
    cout<<"\n\n=========================================";
    cout<<"  ";
    for(int i=0;i<num_neurons;i++){
        if(i%100==0)
            cout<<"\n  ";
        if(neurons[i]->GetActivated()){
            count++;
            cout<<"● ";
        }
        else
            cout<<"○ ";
    }
    cout<<"\n>activated: "<<count<<endl;
}
void Brain::Initialize(){
    //initialize neurons

    int count=0;
    
    //input neurons
    for(;count<num_inputs;count++){
        // !! Neuron should be deleted !!
        neurons.push_back(new Neuron(count,INPUT_NEURON));
        num_neurons++;
    }

    //output neurons
    for(;count<num_outputs;count++){
        // !! Neuron should be deleted !!
        neurons.push_back(new Neuron(count,OUTPUT_NEURON));
        num_neurons++;
    }
    //add minimum neurons. 
    //min num of neuron(except for in/out neurons) is min(num_input,num_output)
    int min=num_inputs<num_outputs?num_inputs:num_outputs;
    for(int i=0;i<min;i++){
        AddNeuron();
    }
}
void Brain::CheckActive(){
    for(auto neu : neurons){
        neu->CheckActive();
    }
}
void Brain::Propagate(){
    for(auto neu : neurons){
        neu->Propagate();
    }
}
void Brain::Update(){
    //GetInputSignal();
    CheckActive();
    Propagate();
    //SetOuptputSignal();
}

//////////////////////////////////////
Body::Body(int _x, int _y){
    x=_x;
    y=_y;
    r=127;
    g=127;
    b=127;
}
void Body::GetInput(){

}
void Body::Update(){
    GetInput();
    brain->Update();
    //MoveBody();
}