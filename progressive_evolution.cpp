#include "progressive_evolution.h"
using namespace newai;
//////////////////////////////////////


namespace newai{

int randnnn(int start, int end){
    std::random_device rd;
    std::mt19937 gen(rd());
    if(end<start) return start;
    std::uniform_int_distribution<int> dis(start, end); 
    return dis(gen);
}

string MLogToString(MLog mlog){
    MUTATION mu=mlog.m;
    if(mu==ADD_NEURON) 
        return "[ADD    NEURON] #"+to_string(mlog.neuron_prev);
    else if(mu==ADD_SYNAPSE) 
        return "[ADD   SYNAPSE] #"+to_string(mlog.neuron_prev)+" -> #"+to_string(mlog.neuron_next)
                +" ["+to_string(mlog.weight)+"]";
    else if(mu==MOD_WEIGHT)
        return "[MODIFY WEIGHT] #"+to_string(mlog.neuron_prev)+" -> #"+to_string(mlog.neuron_next)
                +" ["+to_string(mlog.weight)+"]";
}

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
    string typeofneuron;
    if(type==INPUT_NEURON) typeofneuron="INPUT";
    else if(type==OUTPUT_NEURON) typeofneuron="OUTPUT";
    else typeofneuron="INTER";

    cout<<"===================================="<<endl;
    cout<<"#"<<index<<" "<<typeofneuron<<" buffer="<<buffer<<" activated="<<activated<<endl;
    
    cout<<"prev neurons ("<<prevs.size()<<"): ";
    for(auto neu : prevs)
        cout<<" #"<<neu->GetIndex();
    cout<<endl;

    cout<<"next neurons ("<<nexts.size()<<"): ";
    for(int i=0;i<nexts.size();i++)
        cout<<" #"<<nexts[i]->GetIndex()<<"["<<weights[i]<<"]";
    
    cout<<'\n'<<endl;

}

//////////////////////////////////////
Brain::Brain(string _name, int _num_input,int _num_output){
    name=_name;
    num_inputs=_num_input;
    num_outputs=_num_output;
}
void Brain::Initialize(){
    //initialize neurons

    if(num_neurons>num_inputs+num_outputs) return; //no more init 
    
    int count=0;
    cout<<endl;

    //input neurons
    for(int i=0;i<num_inputs;i++){
        cout<<" $init input neuron #"<<count<<endl;
        // !! Neuron should be deleted !!
        neurons.push_back(new Neuron(count++,INPUT_NEURON));
        num_neurons++;
    }

    //output neurons
    for(int i=0;i<num_outputs;i++){
        cout<<" $init output neuron #"<<count<<endl;
        // !! Neuron should be deleted !!
        neurons.push_back(new Neuron(count++,OUTPUT_NEURON));
        num_neurons++;
    }
    //add minimum neurons. 
    //min num of neuron(except for in/out neurons) is min(num_input,num_output)
    int min=num_inputs<num_outputs?num_inputs:num_outputs;
    for(int i=0;i<min;i++){
        Neuron* nn=new Neuron(num_neurons++);
        neurons.push_back(nn);
        cout<<" $init inter neuron #"<<num_neurons-1<<endl;
    }

    //initial connections
    for(auto neu : neurons){
        NEURON_TYPE t=neu->GetNeuronType();
        if(t==INPUT_NEURON || t==INTER_NEURON) 
            AddSynapse(neu,nullptr); 
        if(t==OUTPUT_NEURON || t==INTER_NEURON)
            AddSynapse(nullptr,neu);
        
    }
}



int Brain::RandomWeight(){
    int w=WEIGHT_BIAS;
    for(int i=0;i<WEIGHT_RANGE;i++)
        w+=randnnn(-1,1);
    return w;
}

int Brain::Mutate(MUTATION m){
    int success=1;
    switch (m){
    case ADD_NEURON:
        AddNeuron(nullptr,nullptr);
        break;
    case ADD_SYNAPSE:
        success=AddSynapse(nullptr,nullptr);
        
        break;
    case MOD_WEIGHT:
        ModWeight(nullptr,nullptr,0);
        break;
    default:
        break;
    } 
    return success;
}

void Brain::AddNeuron(Neuron* prev, Neuron* next){

    //instantiate new neuron object
    Neuron* nn=new Neuron(num_neurons++);
    neurons.push_back(nn);

    //log
    MLog addneuron={ADD_NEURON,nn->GetIndex(),-1,0};
    log.push_back(addneuron);

    //make initial connections
    AddSynapse(prev,nn);
    AddSynapse(nn,next);
    return;
}

int Brain::AddSynapse(Neuron* from, Neuron* to){
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

    //if connection already exists, return 0
    for( int i=0;i<n_from->nexts.size();i++)
        if(n_from->nexts[i]==n_to)
            return 0;
    
    //give random weight
    weight=RandomWeight();


    n_from->AddNext(n_to,weight);
    n_to->AddPrev(n_from);
    
    //add log
    MLog addsynapse={ADD_SYNAPSE,n_from->GetIndex(),n_to->GetIndex(),weight};
    log.push_back(addsynapse);
    
    return 1;
}

void Brain::ModWeight(Neuron* from, Neuron* to, int w){
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
            for(int i=0;i<n_to->prevs.size();i++)
                if(n_to->prevs[i]==n_from){
                    i_from=i;
                    break;
                }
        }
        else{ //from==nullptr
            if(to==nullptr)
                to=neurons[randnnn(num_inputs,num_neurons-1)]; //output and inter
            n_to=to;
            i_from=randnnn(0,n_to->prevs.size()-1);
            n_from=n_to->prevs[i_from];

            //find i_to
            for(int i=0;i<n_from->nexts.size();i++)
                if(n_from->nexts[i]==n_to){
                    i_to=i;
                    break;
                }
        }
    }
    while(n_from==nullptr || n_to==nullptr); //if unconnected neuron has been chosen, retry
    
    int weight=w?w:RandomWeight();
    n_from->weights[i_to]=weight;

    MLog modweight={MOD_WEIGHT,n_from->GetIndex(),n_to->GetIndex(),weight};
    log.push_back(modweight);
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

void Brain::SetInput(int * inputs){
    for(int i=0;i<num_inputs;i++)
        neurons[i]->AddBuffer(inputs[i]);
}
void Brain::GetOutput(int* outputs){
    for(int i=0;i<num_outputs;i++){
        outputs[i]=neurons[num_inputs+i]->GetActivated(); //output format is 0 or 1 
    }
}
void Brain::Update(){
    //GetInputSignal();
    CheckActive();
    Propagate();
    //SetOuptputSignal();
}

int Brain::GetTotalSynapse(){
    int c=0;
    for(auto neu:neurons){
        c+=neu->nexts.size();
    }
    return c;
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

void Brain::ManualControl(){
    bool refresh=true;
    bool run=true;
    int* inputArray=new int[num_inputs];
    while(run){
        if(refresh){
            int ts=GetTotalSynapse();
            int nn=num_neurons;
            cout<<"\n\n########################################################\n";
            cout<<"#[ "<<name<<" ]\n";
            cout<<"# >neurons(in/out/inter/total): "<<num_inputs<<" / "<<num_outputs<<" / "<<num_neurons-num_inputs-num_outputs
                <<" / "<<num_neurons<<"\n";
            cout.precision(4);
            cout<<"# >total synapses: "<<ts<<"  ("
                <<(double)ts/(double)nn<<" | "<<100*(double)ts/(double)nn/(double)nn<<"%)\n";
            cout<<"# >logs: "<<log.size()<<"\n";
            cout<<"#"<<endl;
            cout<<"# 0)initialize  1)run  2)show neuron  3)show all neurons\n";
            cout<<"# 10)add neuron  11)add synapse  12)modify weight  13)random mutation\n";
            cout<<"# 20)show log  21)show recent(10) logs  22)show all logs\n";
            cout<<"# 30)set inputs\n";
            cout<<"# 100)refresh  999)exit\n";
            cout<<"########################################################\n";
            cout<<endl;
            refresh=false;
        }

        cout<<">> ";
        int command;
        cin>>command;
        int ind;
        switch (command){
        case 999:
            run=false;
            break;
        case 100:
            refresh=true;
            break;
        case 0:{
            Initialize();
            cout<<" $initialize done"<<endl;
            refresh=true;
            break;
        }
        case 1:{
            int count=0;
            do{
                cout<<"count(exit=-1): ";
                cin>>count;
                for(int i=0;i<count;i++){
                    system("clear");
                    SetInput(inputArray);
                    CheckActive();
                    ShowStatus();
                    Propagate();
                    //GetOutput(outputArray);
                }
            }while(count!=-1);
            refresh=true;
            break;
        }
        case 2:{
            cout<<"index: ";
            cin>>ind;
            if(ind <0 || ind>=num_neurons){
                cout<<" $invalid index"<<endl;
                break;
            }
            neurons[ind]->ShowStatus();
            break;
        }
        case 3:{
            for(auto neu:neurons)
                neu->ShowStatus();
            break;
        }
        case 10:{
            Mutate(ADD_NEURON);
            cout<<" $"<<MLogToString(log[log.size()-3])<<endl;
            cout<<" $"<<MLogToString(log[log.size()-2])<<endl;
            cout<<" $"<<MLogToString(log[log.size()-1])<<endl;
            break;
        }
        case 11:{
            if(Mutate(ADD_SYNAPSE))
                cout<<" $"<<MLogToString(log.back())<<endl;
            break;
        }
        case 12:{
            Mutate(MOD_WEIGHT);    
            cout<<" $"<<MLogToString(log.back())<<endl;
            break;
        }
        case 13:{
            MUTATION r=(MUTATION)randnnn(0,2);
            int success=Mutate(r);
            if(r==ADD_NEURON){
                cout<<" $"<<MLogToString(log[log.size()-3])<<endl;
                cout<<" $"<<MLogToString(log[log.size()-2])<<endl;
                cout<<" $"<<MLogToString(log[log.size()-1])<<endl;
            }
            else if(success)
                cout<<" $"<<MLogToString(log.back())<<endl;
            break;
        }
        case 20:{
            cout<<"index: ";
            cin>>ind;
            if(ind<0 || ind>=log.size()) {
                cout<<" $invalid index"<<endl;
                break;
            }
            cout<<" #"<<ind<<"  "<<MLogToString(log[ind])<<endl;
            break;
        }
        case 21:{
            cout<<" $Logs:"<<endl;
            int s=log.size();
            int min= s<10 ? s:10;
            for(int i=0;i<min;i++)
                cout<<" #"<<s+i-min<<"  "<<MLogToString(log[s+i-min])<<endl;
            break;
        }
        case 22:{
            cout<<" $Logs:"<<endl;
            for(int i=0;i<log.size();i++)
                cout<<" #"<<i<<"  "<<MLogToString(log[i])<<endl;
            break;
        }
        case 30:{
            cout<<" $current input:\n  > ";
            for(int i=0;i<num_inputs;i++)
                cout<<i<<")"<<inputArray[i]<<"  ";
            cout<<endl;
            for(int i=0;i<num_inputs;i++){
                cout<<" "<<i<<": ";
                cin>>inputArray[i];
            }
            cout<<" $changed inputs"<<endl;
            break;
        }
        }
    }

    delete[] inputArray;
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