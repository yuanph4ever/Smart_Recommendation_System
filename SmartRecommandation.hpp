//
//  SmartRecommandation.hpp
//  pro4
//
//  Created by Peihao Yuan on 10/22/17.
//  Copyright © 2017 Peihao Yuan. All rights reserved.
//
#include <string>
#include <map>
#include <set>
using namespace std;

class SmartRecommandation{
public:
    SmartRecommandation(string file_name_1, string file_name_2, string file_name_3);
    
    void doRecommandation(char *name, int alg);
    
private:
    map< string, map<string, double> > data_map;

    set<string> customer;
    
};
