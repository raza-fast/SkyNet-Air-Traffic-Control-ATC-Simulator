#include "SkyNet.h"
#include <iostream>
using namespace std;

int main() {
    cout << "Initializing SkyNet Air Traffic Control System...\n";
    
    SkyNet* skynet = new SkyNet();
    skynet->run();
    
    delete skynet;
    
    return 0;
}

