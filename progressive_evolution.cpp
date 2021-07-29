#include "progressive_evolution.h"
using namespace newai;
//////////////////////////////////////


int main(){


    Brain b1(1000,5000);
    b1.Initialize();
    int c=1000;
    int cc=0;
    while(true){
        system("clear");
        if(cc++<50000)
        for(int i=0;i<10;i++)
            b1.neurons[i]->AddBuffer(500);
        //b1.neurons[0]->ShowState();
        // b1.neurons[1]->AddBuffer(100);
        //b1.Update();
        b1.CheckActive();
        c--;
            cout<<"*"<<cc<<endl;
            b1.ShowStatus();
        if(c<=0){
           cin>>c;
        }
        b1.Propagate();
    }


     
}

//////////////////////////////////////
Neuron::Neuron(int _index){
    index=_index;
}
int Neuron::GetIndex(){
    return index;
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
void Neuron::AddNextNeuron(Neuron* next, int s){
    nexts.push_back(next);
    weights.push_back(s);
}
void Neuron::AddPrevNeuron(Neuron* prev){
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
    switch(m){
        case ADD_NEURON:

            break;
        case  
    }
}
void Brain::AddNeuron(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, num_neurons); 

    //instantiate

    return;
}
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
    //input & output neurons
    for(int i=0;i<num_inputs+num_outputs;i++){
        // !! Neuron should be deleted !!
        neurons.push_back(new Neuron(i));
        num_neurons++;
    }

    //add basic neurons. 
    //min num of neuron(except for in/out neurons) is min(num_input,num_output)
    int num=num_inputs<num_outputs?num_inputs:num_outputs;
    for(int i=0;i<num;i++){
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
// void Brain::Mutate(Brain* brain,double mutate_rate){
//     int count=brain->num_neurons * mutate_rate;
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_int_distribution<int> dis(0, brain->num_neurons-1); 
//     for(int i=0;i<count;i++){
//         Neuron* oldn=brain->neurons[dis(gen)];
        
//     }

// }
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