#include <iostream>
#include <queue>
#include <vector>
#include <random>
#include "new.h"
using namespace std;
using namespace newai;
#define MAX_NUM 100


int main(){

    cout<<"hello~~"<<endl;
    Brain b1(1000,5000);
    b1.Initialize();
    b1.neurons[0]->ShowState();
    
        
}