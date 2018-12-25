//
//  main.cpp
//  pro4
//
//  Created by Peihao Yuan on 10/17/17.
//  Copyright © 2017 Peihao Yuan. All rights reserved.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <math.h>
#include "SmartRecommandation.hpp"
using namespace std;

string usage = "main people.csv ratings.csv -m / main BBooks.csv BRatings.csv -b";

int main(int argc, const char * argv[]) {
    
    if(argc != 4){
        cout << usage << endl;
        return -1;
    }
    
    SmartRecommandation myR(argv[1], argv[2], argv[3]);
    
    while(1){
        
        cout << "Enter Name: " ;
        char name[100];
        cin.getline(name, 100);
        if(strlen(name) != 0){
            if(strcmp(name, "exit") == 0){
                exit(0);
            }else{
                myR.doRecommandation(name, 0);
                cout << endl;
                myR.doRecommandation(name, 1);
                cout << endl;
                myR.doRecommandation(name, 2);
                cout << endl;
                myR.doRecommandation(name, 3);
                cout << endl;
                myR.doRecommandation(name, 6);
                cout << endl;
            }
        }
    }
    
    return 0;
}
