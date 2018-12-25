//
//  SmartRecommandation.cpp
//  pro4
//
//  Created by Peihao Yuan on 10/22/17.
//  Copyright © 2017 Peihao Yuan. All rights reserved.
//

#include "SmartRecommandation.hpp"
#include <string>
#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
using namespace std;

//helper functions
void printSet(set<string> customer){
    set<string>::iterator sit;
    sit = customer.begin();
    while(sit != customer.end()){
        cout << *sit << endl;
        ++sit;
    }
}

void print_inner_Map(map<string, double> movie_grade){
    map<string, double>::iterator mit;
    mit = movie_grade.begin();
    while(mit != movie_grade.end()){
        cout << mit->first << " || " << mit->second << endl;
        ++mit;
    }
}

void print_outter_Map(map< string, map<string, double> > big_map){
    map< string, map<string, double> >::iterator bit;
    bit = big_map.begin();
    while(bit != big_map.end()){
        cout << "------------" << endl;
        cout << "User Name: " << bit->first << endl;
        cout << "------------" << endl;
        print_inner_Map(bit->second);
        cout << "------------" << endl;
        ++bit;
    }
}

set<string> creatSet(string filename){
    //cout << "reading user information" << endl;
    set<string> customer;
    ifstream ifile(filename);
    string line;
    while(getline(ifile, line)){
        stringstream ss;
        ss << line;
        string token;
        getline(ss, token, ',');
        customer.insert(token);
    }
    ifile.close();
    //cout << "user information reading done" << endl;
    return customer;
}

map< string, map<string, double> > createMap(set<string> customer, string filename){
    map< string, map<string, double> > big_map;
    ifstream ifile;
    string line;
    set<string>::iterator sit;
    sit = customer.begin();
    while(sit != customer.end()){
        string cn = *sit;
        map<string, double> movie_grade;
        ifile.open(filename);
        while(getline(ifile, line)){
            stringstream ss;
            ss << line;
            string movie_name;
            string user_name;
            string movie_score;
            getline(ss, movie_name, ',');
            getline(ss, user_name, ',');
            getline(ss, movie_score, ',');
            if(user_name == cn){
                movie_grade[movie_name] = stod(movie_score);
            }
        }
        ifile.close();
        big_map[cn] = movie_grade;
        ++sit;
    }
    return big_map;
}

int cmp_asc(const pair<string, double>& x, const pair<string, double>& y){
    return x.second < y.second;
}

int cmp_des(const pair<string, double>& x, const pair<string, double>& y){
    return x.second > y.second;
}

void sortMapByValue(map<string, double>& tMap, vector< pair<string, double> >& tVector, int order){
    for (map<string, double>::iterator curr = tMap.begin(); curr != tMap.end(); curr++){
        tVector.push_back(make_pair(curr->first, curr->second));
    }
    if(order == 1){
        sort(tVector.begin(), tVector.end(), cmp_asc);
    }else{
        sort(tVector.begin(), tVector.end(), cmp_des);
    }
}

void Reco(string name, map< string, map<string, double> > big_map, map<string, double> client_map){
    map<string, double> movie_grade = big_map[name];
    //print_inner_Map(movie_grade);
    map<string, double>::iterator it = movie_grade.begin();
    map<string, double>::iterator iit = client_map.begin();
    set<string> mset;
    set<string>::iterator sit;
    while(it != movie_grade.end()){
        while(iit != client_map.end()){
            //cout << it->first << endl;
            //cout << iit->first << endl;
            if(it->first == iit->first){
                //cout << it->first << endl;
                mset.insert(it->first);
            }
            ++iit;
        }
        iit = client_map.begin();
        ++it;
    }
    for(sit = mset.begin(); sit != mset.end(); ++sit){
        //cout << *sit << endl;
        movie_grade.erase(*sit);
    }
    
    if(movie_grade.size() == 0){
        cout << "no valid recommandation(from Reco)" << endl;
        return;
    }
    
    vector< pair<string, double> > tVector;
    sortMapByValue(movie_grade, tVector, 2);
    for(int i=0; i < ( 5<tVector.size()?5:tVector.size() ); i++){
        cout << tVector[i].first << " ---> " << tVector[i].second <<endl;
    }
}

void print_neighbor(vector< pair<string,double> > tVector){
    if(tVector.size() != 0){
        for(int i=0; i < ( 5<tVector.size()?5:tVector.size() ); i++){
            cout << tVector[i].second << " ---> " << tVector[i].first <<endl;
        }
    }else{
        cout << "no valid neighbor" << endl;
    }
}

void print_recommendation(vector< pair<string,double> > tVector, map< string, map<string, double> > big_map, map<string, double> client_map){
    if(tVector.size() != 0){
        //cout << tVector[0].first << endl;
        Reco(tVector[0].first, big_map, client_map);
    }else{
        cout << "no valid recommendation(no neighbor to recommand)" << endl;
    }
}

void Recok(vector< pair<string,double> > tVector, map< string, map<string, double> > big_map, map<string, double> client_map, int alg){
    
    if(tVector.size() == 0){
        cout << "K Nearest Neighbors:" << endl;
        cout << "no valid neighbor and recommendation" << endl;
        return;
    }
    
    int k = alg - 3;
    
    map<string, double> tmp;
    vector< pair<string,double> > tv;
    
    double sum = 0;
    int i = 0;
    
    while(i < k){
        if(tVector[i].first.empty()){
            //cout << "get an error trigger" << endl;
            k += 1;
            i++;
        }else{
            sum += tVector[i].second;
            i++;
        }
    }
    i = 0;
    while(i < k){
        
        if(!tVector[i].first.empty()){
            tmp[tVector[i].first] = tVector[i].second / sum;
            i++;
        }else{
            i++;
        }
        
    }
    
    /*
    while(i < k){
       
            sum += tVector[i].second;
            i++;
        
    }
    i = 0;
    while(i < k){
        
            tmp[tVector[i].first] = tVector[i].second / sum;
            i++;
     
    }
    */
    //print_inner_Map(tmp);
    sortMapByValue(tmp, tv, 2);
    //tVector has the grade
    //tv has the percentage
    //WE NEED tv <name, percentage>
    cout << "K Nearest Neighbors:" << endl;
    
    //print_neighbor(tVector);
    
    for(i=0; i < k; i++){
        if(!tVector[i].first.empty()){
            cout << tVector[i].second << " ---> " << tVector[i].first <<endl;
        }
    }
   
    map< string, map<string, double> > new_map;//map <user <movie, grade>> for knn_user ->movie/grade
    
    //get movies into oop
    map< string, double >::iterator tit;
    map< string, map<string, double> >::iterator bit;
    set<string> mset;//used movie
    map<string, int> oop;//overlaped movie for knn users
    for(tit = tmp.begin(); tit != tmp.end(); ++tit){//users in knn map which is tmp
        for(bit = big_map.begin(); bit != big_map.end(); ++bit){//whole users
            if(tit->first == bit->first){//users in knn map
                map< string, double >::iterator sit;
                for(sit = bit->second.begin(); sit != bit->second.end(); ++sit){//get movie map for knn users
                    map<string, int>::iterator oit = oop.find(sit->first);
                    if(oit == oop.end()){
                        oop[sit->first] = 1;
                    }else{
                        int i = oop[sit->first];
                        i += 1;
                        oop[sit->first] = i;
                    }
                    map< string, double >::iterator cit;
                    for(cit = client_map.begin(); cit != client_map.end(); ++cit){//check if old movie
                        if(cit->first == sit->first){
                            mset.insert(cit->first);
                        }
                    }
                }
            }
        }
    }
    //oop get
    
    //remove old movies and non-overlapped movies
    //removed non-overlapped movie
    set<string> rset;
    map<string, int>::iterator rit;
    for(rit = oop.begin(); rit != oop.end(); ++rit){
        if(rit->second != 3){
            rset.insert(rit->first);
        }
    }
    set<string>::iterator rsit;
    for(rsit = rset.begin(); rsit != rset.end(); ++rsit){
        oop.erase(*rsit);
    }
    //remove old movies
    for(rsit = mset.begin(); rsit != mset.end(); ++rsit){
        oop.erase(*rsit);
    }
    //remove done
    
    //then map<string, int> oop has the recommanded movies for client
    //we just want the movie name in oop map which is the key
    map<string, double> rap;
    
    for(tit = tmp.begin(); tit != tmp.end(); ++tit){//user in knn map
        for(bit = big_map.begin(); bit != big_map.end(); ++bit){//whole users
            if(tit->first == bit->first){//user in knn map
                map< string, double >::iterator sit;
                map< string, double > smap;//map for recommaded movie
                //for each one user
                for(sit = bit->second.begin(); sit != bit->second.end(); ++sit){//get movie map for knn users
                    map<string, int>::iterator oit = oop.find(sit->first);
                    //if the movie is in the movie map of the user
                    //then we want to get the movie score and user name
                    if(oit != oop.end()){
                        smap[sit->first] = sit->second;
                    }
                }
                new_map[tit->first] = smap;
            }
        }
    }
    
    //print_outter_Map(new_map);
    
    map< string, map<string, double> >::iterator new_it;
    for(int i = 0; i < tv.size(); i++){
        new_it = new_map.find(tv[i].first);
        map<string, double> rmap = new_it->second;
        for(map<string, double>::iterator rit = rmap.begin(); rit != rmap.end(); ++rit){
            double new_score = rit->second * tv[i].second;
            rmap[rit->first] = new_score;
        }
        new_map[tv[i].first] = rmap;
    }
    
    //print_outter_Map(new_map);
    
    //now new_map < user_name <movie_name, grade after percentage> >
    map<string, int>::iterator movie_it;
    map<string, double>::iterator inner_it;
    map< string, map<string, double> >::iterator outer_it;
    map< string, double > final_movie_map;
    for(movie_it = oop.begin(); movie_it != oop.end(); ++movie_it){
        final_movie_map[movie_it->first] = 0;
    }
    
    //print_inner_Map(final_movie_map);
    
    for(outer_it = new_map.begin(); outer_it != new_map.end(); ++outer_it){
        for(inner_it = outer_it->second.begin(); inner_it != outer_it->second.end(); ++inner_it){
            final_movie_map[inner_it->first] += inner_it->second;
        }
    }
    
    //print_inner_Map(final_movie_map);
    cout << "Recommendations: " << endl;
    
    vector< pair<string, double> > ttVector;
    sortMapByValue(final_movie_map, ttVector, 2);
    
    if(ttVector.size() != 0){
        for(int i=0; i < ( 5<ttVector.size()?5:ttVector.size() ); i++){
            cout << ttVector[i].first << " ---> " << ttVector[i].second <<endl;
        }
    }else{
        cout << "no valid recommandation(from Recok)" << endl;
    }
    
}

void runReco(char *name, map< string, map<string, double> > big_map, int alg){
    //print_outter_Map(big_map);
    map<string, double> mht_map;
    string client_name;
    stringstream ss;
    ss << name;
    ss >> client_name;
    map< string, map<string, double> >::iterator bit;
    bit = big_map.find(client_name);
    if(bit == big_map.end()){
        cout << "No information for this user" << endl;
        return;
    }
    map<string, double> client_map;
    client_map = big_map[client_name];
    big_map.erase(client_name);
    bit = big_map.begin();
    while(bit != big_map.end()){
        //Manhattan & Euclidean
        double result = 0;
        //PCC & CS
        map<double, double> map_xy;
        bool overlap = 0;
        string user_name = bit->first;
        map<string, double> movie_grade = bit->second;
        map<string, double>::iterator cit;
        cit = client_map.begin();
        while(cit != client_map.end()){
            map<string, double>::iterator mit;
            mit = movie_grade.begin();
            while(mit != movie_grade.end()){
                if(cit->first == mit->first){
                    overlap = 1;
                    if(alg == 0){
                        //Manhattan
                        result += fabs(cit->second - mit->second);
                    }else if(alg == 1){
                        //Euclidean
                        result += fabs(cit->second - mit->second) * fabs(cit->second - mit->second);
                    }else if(alg == 2){
                        //PCC
                        map_xy[cit->second] = mit->second;
                    }else if(alg == 3){
                        //CS
                        map_xy[cit->second] = mit->second;
                    }else{
                        //KNN
                        map_xy[cit->second] = mit->second;
                    }
                    
                }
                ++mit;
            }
            ++cit;
        }
        
        if(alg == 0){
            //Manhanttan
        }else if(alg == 1){
            //Euclidean
            double tmp = sqrt(result);
            result = tmp;
        }else if(alg == 2){
            //PCC
            map<double, double>::iterator it_xy;
            int n = 0;
            double sum_xy = 0;
            double sum_x = 0;
            double sum_y = 0;
            double sum_xx = 0;
            double sum_yy = 0;
            for(it_xy = map_xy.begin(); it_xy != map_xy.end(); ++it_xy){
                n += 1;
                sum_xy += it_xy->first * it_xy->second;
                sum_x += it_xy->first;
                sum_y += it_xy->second;
                sum_xx += it_xy->first * it_xy->first;
                sum_yy += it_xy->second * it_xy->second;
            }
            result = ( sum_xy - sum_x * sum_y / n ) / ( sqrt(sum_xx - sum_x * sum_x / n) * sqrt(sum_yy - sum_y * sum_y / n) );
        }else if(alg > 2){
            //CS
            map<double, double>::iterator it_xy;
            double sum_xy = 0;
            double sum_xx = 0;
            double sum_yy = 0;
            for(it_xy = map_xy.begin(); it_xy != map_xy.end(); ++it_xy){
                sum_xy += it_xy->first * it_xy->second;
                sum_xx += it_xy->first * it_xy->first;
                sum_yy += it_xy->second * it_xy->second;
            }
            result = sum_xy / (sqrt(sum_xx) * sqrt(sum_yy));
        }
        
        if(!isnan(result) && overlap){
            mht_map[user_name] = result;
        }
        ++bit;

    }
    
    if(alg == 0){
        cout << "Manhattan Nearest Neighbors:" << endl;
        
        vector< pair<string,double> > tVector;
        sortMapByValue(mht_map,tVector, 1);
        
        /*
        for(int i=0; i < ( 5<tVector.size()?5:tVector.size() ); i++){
            cout << tVector[i].second << " ---> " << tVector[i].first <<endl;
        }
        */
        print_neighbor(tVector);
        
        cout << "Recommendations: " << endl;
        
        //Reco(tVector[0].first, big_map, client_map);
        
        print_recommendation(tVector, big_map, client_map);
        
    }else if(alg == 1){
        cout << "Euclidean Nearest Neighbors:" << endl;
        
        vector< pair<string,double> > tVector;
        sortMapByValue(mht_map,tVector, 1);
        /*
        for(int i=0; i < ( 5<tVector.size()?5:tVector.size() ); i++){
            cout << tVector[i].second << " ---> " << tVector[i].first <<endl;
        }
        */
        print_neighbor(tVector);
        
        cout << "Recommendations: " << endl;
        
        //Reco(tVector[0].first, big_map, client_map);
        print_recommendation(tVector, big_map, client_map);
    }else if(alg == 2){
        cout << "Pearson Nearest Neighbors:" << endl;
        
        vector< pair<string,double> > tVector;
        sortMapByValue(mht_map,tVector, 2);
        /*
        for(int i=0; i < ( 5<tVector.size()?5:tVector.size() ); i++){
            cout << tVector[i].second << " ---> " << tVector[i].first <<endl;
        }
        */
        print_neighbor(tVector);
        cout << "Recommendations: " << endl;
        print_recommendation(tVector, big_map, client_map);
        //Reco(tVector[0].first, big_map, client_map);
    }else if(alg == 3){
        cout << "Cosine Nearest Neighbors:" << endl;
        
        vector< pair<string,double> > tVector;
        sortMapByValue(mht_map,tVector, 2);
        /*
        for(int i=0; i < ( 5<tVector.size()?5:tVector.size() ); i++){
            cout << tVector[i].second << " ---> " << tVector[i].first <<endl;
        }
        */
        print_neighbor(tVector);
        cout << "Recommendations: " << endl;
        print_recommendation(tVector, big_map, client_map);
        //Reco(tVector[0].first, big_map, client_map);
    }else if(alg > 3){
        //KNN
        vector< pair<string,double> > tVector;
        sortMapByValue(mht_map, tVector, 2);
        Recok(tVector, big_map, client_map, alg);
        
    }
    
}

bool check_key(string key, map<string, string> id_name){
    map<string, string>::iterator it;
    it = id_name.find(key);
    if(it != id_name.end()){
        return 1;
    }else{
        return 0;
    }
}

void print_inner_map_book(map<string, string> bmap){
    cout << "print book map" << endl;
    map<string, string>::iterator it;
    for(it = bmap.begin(); it != bmap.end(); ++it){
        cout << it->first << "--->" << it->second;
    }
}

map< string, map<string, double> > create_book_map(string f2, string f3){
    cout << "reading data from " << f2 << endl;
    map< string, map<string, double> > big_map;
    map< string, string > id_name;
    ifstream ifile;
    string line;
    ifile.open(f2);
    while(getline(ifile, line)){
        stringstream ss;
        ss << line;
        string movie_id;
        string movie_name;
        getline(ss, movie_id, ';');
        getline(ss, movie_name, ';');
        //cout << movie_id << "--->" << movie_name << endl;
        id_name[movie_id] = movie_name;
    }
    ifile.close();
    cout << "reading done" << endl;
    
    
    //print_inner_map_book(id_name);
    
    
    cout << "reading data from " << f3 << endl;
    
    ifile.open(f3);
    string last_name = "";
    map<string, double> movie_grade;
    int tag = 0;
    while(getline(ifile, line)){
        stringstream ss;
        ss << line;
        string movie_id;
        string user_name;
        string movie_score;
        getline(ss, user_name, ';');
        getline(ss, movie_id, ';');
        getline(ss, movie_score, ';');
        if(last_name != user_name){
            if(tag == 0){
                tag = 1;
                last_name = user_name;
            }else{
                big_map[last_name] = movie_grade;
                movie_grade.clear();
                movie_grade[id_name[movie_id]] = stod(movie_score);
                last_name = user_name;
            }
        }else{
            movie_grade[id_name[movie_id]] = stod(movie_score);
            last_name = user_name;
        }
    }
    
    cout << "reading done" << endl;
    
    //print_outter_Map(big_map);
    
    return big_map;
}

//class functions
SmartRecommandation::SmartRecommandation(string file_name_1, string file_name_2, string tag){
    if(tag == "-m"){
        customer = creatSet(file_name_1);
        data_map = createMap(customer, file_name_2);
    }else if(tag == "-b"){
        //customer = creatSet(file_name_1);
        data_map = create_book_map(file_name_1, file_name_2);
    }else{
        cout << "invalid tag" << endl;
        cout << "main people.csv ratings.csv -m / main BBooks.csv BRatings.csv -b" << endl;
        exit(0);
    }
}

void SmartRecommandation::doRecommandation(char *name, int alg){
    runReco(name, data_map, alg);
}
