#include "new.h"
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
Neuron::Neuron(int _index,int _sign){
    index=_index;
    sign=RandomSign(_sign);
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
        nexts[i]->AddBuffer(sign);

    //activated=false;
}
void Neuron::AddNextNeuron(Neuron* next){
    nexts.push_back(next);
}
void Neuron::AddPrevNeuron(Neuron* prev){
    prevs.push_back(prev);
}
void Neuron::ShowState(){
    cout<<"//////////////////////////////"<<endl;
    cout<<"#"<<index<<" sign="<<sign<<" buffer="<<buffer<<" activated="<<activated<<endl;
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
Brain::Brain(int _num_neurons, int _num_connections){
    num_neurons=_num_neurons;
    num_connections=_num_connections;
    MSI=MAX_NUM * num_neurons / num_connections;
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
    for(int i=0;i<num_neurons;i++){
        neurons.push_back(new Neuron(i,MSI)); //create new neuron;
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