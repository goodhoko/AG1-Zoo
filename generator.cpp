//
//  main.cpp
//  ht
//
//  Created by Danil Luzin on 05/11/2016.
//  Copyright © 2016 Danil Luzin. All rights reserved.
//

#include <iostream>
#include <vector>
#include <time.h>       /* time */
#include <algorithm>
#include <fstream>

int main(int argc, const char * argv[]) {
    srand(time(NULL));

    std::ofstream input;
    input.open ("input.txt");
    std::ofstream output;
    output.open ("output.txt");

    int entCnt = 1000;            //NUMBER OF ENTRIES
    int r = 5;                      //PROBOBILITY OF DELETION
    unsigned long int roof = 50;  //MAX ID
    int fr = roof/20;               //MAX STRENGTH
    const int www = 10;             //NUMBER OF TRUCKS

    struct g{
        g(unsigned long int h, unsigned long int r){y=h;w=r;}
        unsigned long int y;
        unsigned long int w;
        bool operator < (const g & oth) const{
            if (y == oth.y){
                return w < oth.w;
            }
            return y>oth.y;
        }
    };

    std::vector<g> wtf [www+1] ;

    struct less_than_key
    {
        inline bool operator() (const g& s1, const g& s2)
        {
            if (s1.y == s2.y){
                return s1.w < s2.w;
            }
            return s1.y < s2.y;
        }
    };

    struct more_than_key
    {
        inline bool operator() (const g& s1, const g& s2)
        {
            if (s1.y == s2.y){
                return s1.w < s2.w;
            }
            return s1.y > s2.y;
        }
    };


    std::vector<g> v;
    for (int t=0; t<entCnt ; t++) {
        unsigned long int y = (rand() % (roof/fr)) + 1;
        unsigned long int w = rand() % roof;
        int dest = (rand() % www)+1;
        std::cout << 0 << " " << dest <<" " << w << " " << y <<"\n";

//        std::cout << 0 << " " << 1 <<" " << w << " " << 1 <<"\n";
        input << 0 << " " << dest <<" " << w << " " << y <<"\n";
//        input << 0 << " " << 1 <<" " << w << " " << 1 <<"\n";

        wtf[dest].push_back(g(y,w));
        while (rand()%r == 0 || rand()%r == 1 ){

            int rnddest =(rand()%www)+1;
            int ra = rand()%2 + 1;

            if(ra == 1 ) { //getmax
                std::cout << 1 << " "<<rnddest <<"\n";
                input << 1 << " "<<rnddest <<"\n";
                std::sort(wtf[rnddest].begin(), wtf[rnddest].end(),more_than_key());
                if (wtf[rnddest].empty()){
                    output<<"empty\n";
                }else{
                    output << wtf[rnddest].front().w <<"\n";
                    wtf[rnddest].erase(wtf[rnddest].begin());
                }
            }
            if(ra == 2 ) { //getmin
                std::cout << 2 << " "<<rnddest <<"\n";
                input << 2 << " "<<rnddest <<"\n";
                std::sort(wtf[rnddest].begin(), wtf[rnddest].end(),less_than_key());
                if (wtf[rnddest].empty()){
                    output<<"empty\n";
                }else{
                    output << wtf[rnddest].front().w <<"\n";
                    wtf[rnddest].erase(wtf[rnddest].begin());
                }
            }
        }
//        v.push_back(g(1,w));

    }
//    for (int t=0; t<remCnt ; t++) {
//        std::cout << rand() % 2 + 1 << " "<<1 <<"\n";
//    }

    for (int q=1; q <= www ; q++) {
        for(int u=0;u<=wtf[q].size();u++){
            int ra = rand()%2 + 1;

            if(ra == 1 ) { //getmax
                std::cout << 1 << " "<<q <<"\n";
                input << 1 << " "<<q <<"\n";
                std::sort(wtf[q].begin(), wtf[q].end(),more_than_key());
                if (wtf[q].empty()){
                    output<<"empty\n";
                }else{
                    output << wtf[q].front().w <<"\n";
                    wtf[q].erase(wtf[q].begin());
                }
            }
            if(ra == 2 ) { //getmin
                std::cout << 2 << " "<<q <<"\n";
                input << 2 << " "<<q <<"\n";
                std::sort(wtf[q].begin(), wtf[q].end(),less_than_key());
                if (wtf[q].empty()){
                    output<<"empty\n";
                }else{
                    output << wtf[q].front().w <<"\n";
                    wtf[q].erase(wtf[q].begin());
                }
            }
        }
    }
    input << "4" << "\n";

    output.close();
    input.close();
    return 0;
}