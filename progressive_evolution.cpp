#include "progressive_evolution.h"
using namespace newai;
//////////////////////////////////////



int randnnn(int start, int end){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(start, end); //p=1/2
    return dis(gen);
}

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
void Neuron::ShowStatus(){
    cout<<"===================================="<<endl;
    cout<<"#"<<index<<" buffer="<<buffer<<" activated="<<activated<<endl;
    
    cout<<"prev neurons ("<<prevs.size()<<"): "<<endl;
    for(auto neu : prevs)
        cout<<" #"<<neu->GetIndex();
    cout<<endl;

    cout<<"next neurons ("<<nexts.size()<<"): "<<endl;
    for(int i=0;i<nexts.size();i++)
        cout<<" #"<<nexts[i]->GetIndex()<<"["<<weights[i]<<"]"<<endl;
    
    cout<<'\n'<<endl;

}

//////////////////////////////////////
Brain::Brain(int _num_input,int _num_output){
    num_inputs=_num_input;
    num_outputs=_num_output;
}
int Brain::RandomWeight(){
    int w=WEIGHT_BIAS;
    for(int i=0;i<WEIGHT_RANGE;i++)
        w+=randnnn(-1,1);
    return ;
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
    
    Neuron* n_from;
    Neuron* n_to;
    int weight;

    if(from==nullptr){
        int i_from;
        do{
            i_from=randnnn(0, num_neurons-1);
        }while(neurons[i_from]->GetNeuronType()==OUTPUT_NEURON);
        n_from=neurons[i_from];
    }
    else n_from=from;

    if(to==nullptr){
        int i_to;
        do{
            i_to=randnnn(0, num_neurons-1);
        }while(neurons[i_to]->GetNeuronType()==INPUT_NEURON);
        n_to=neurons[i_to];
    }
    else n_to=to;

    //if connection already exists, return
    for( int i=0;i<n_from->nexts.size();i++)
        if(n_from->nexts[i]==n_to)
            return;
    
    //give random weight
    weight=RandomWeight();


    n_from->AddNext(n_to,weight);
    n_to->AddPrev(n_from);
    
    //add log
    MLog addsynapse={ADD_SYNAPSE,n_from,n_to,weight};
    log.push(addsynapse);
}
void Brain::ModWeight(Neuron* from, Neuron* to, int add){
    //four cases:
    //1. from->to : but not sure two are connected
    //2. rand->to
    //3. from->rand
    //4. rand->rand

    int i_from, i_to; //n_from==n_to->prevs[i_from] / n_to==n_from->nexts[i_to]
    Neuron* n_from;
    Neuron* n_to;
    do{
        if(from != nullptr){
            n_from = from;
            bool connected=false;
            if(to != nullptr){
                //check if two are connected
                for(int i=0;i<from->nexts.size();i++)
                    if(from->nexts[i]==to){
                        connected=true;
                        n_to=to;
                        i_to=i;
                        break;
                    }
            }
            if(!connected){
                i_to=randnnn(0,from->nexts.size()-1);
                n_to=from->nexts[i_to];
            }
            //find i_from
            for(int i=0;i<to->prevs.size();i++)
                if(to->prevs[i]==from){
                    i_from=i;
                    break;
                }
        }
        else{ //from==nullptr
            if(to==nullptr)
                to=neurons[randnnn(num_inputs+num_outputs,num_neurons-1)]; //interneuron only
            n_to=to;
            i_from=randnnn(0,to->prevs.size()-1);
            n_from=to->prevs[i_from];

            //find i_to
            for(int i=0;i<from->nexts.size();i++)
                if(from->nexts[i]==to){
                    i_to=i;
                    break;
                }
        }
    }
    while(n_from==nullptr || n_to==nullptr); //if unconnected neuron has been chosen, retry
    
    int w=RandomWeight();
    n_from->weights[i_to]=w;

    MLog modweight={MOD_WEIGHT,n_from,n_to,w};
    log.push(modweight);
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