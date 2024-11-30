#include <bits/stdc++.h>
#include "maze_gen.hpp"
#define ll long long
#define f(i,L,R) for(int i=L; i<=R; ++i)
#define oo 666666666666666
#define pb push_back
#define pf push_front
#define DIR "tests/"
#define MAXN 250
#define MAXPr 2                     //default: Free-2 branching - really free, an RD path usually always works, suboptimal
#define MINPr 2                     //Pr means "paths random"
#define EXTRA_P 50
#define _E 0
#define _N 1
#define _W 2
#define _S 3
#define wall "#"
#define weak (0)                    //fine but could be better
#define strict (1)                  //bad for 1-path-mazes, quite rigid, nearly ideal for 2-path-mazes
#define gauss (0)                   //bad for 1-path-mazes, interesting structure, good for 2-path mazes
#define inverse_gauss (0)           //bad for 1-path mazes, a bit free for 2-paths
#define strict_bias ((double)0.1)
#define weak_bias ((double)0.4)
#define gauss_bias ((double)0.15)
#define gauss_weight (0.25)
#define frand() ((double)(rand())/(RAND_MAX))
#define GUESSES 100
#define POTIONS 100
// Note: This is **not** how you do a multiple-file program in C++.
using namespace std;
float total_score(int n, int m, maze123 M, unordered_set<int>query)     //calculate total score from query set
{
    float ts=0;
    int MPSZ = M.paths.size();
    f(i,0,MPSZ-1)
    {
        int sum=0;
        int MPISZ=M.paths[i].size();
        f(j,0,MPISZ-1)
        {
            if(query.find(M.paths[i][j])!=query.end())sum++;
        }
        ts=max(ts,((float)(2*sum))/M.lengths[i]);
    }
    return ts;
}
/*
float pso_core(int n, int m, maze123 M)
float pso(int n, int m, maze123 M)
{
    f(j,1,sqrt(POTIONS)+0.00000000001) //sqrt(W) initial configurations
    {
        unordered_set<int>guess_sequence={};
        while(guess_sequence.size()<100)
        {
            guess_sequence.insert(encode(rand()%n,rand()%m));
        }
        f(i,1,sqrt(POTIONS)+0.00000000001) //sqrt(W) timelines per initial configuration
        {

        }
    }
}*/
float random(int n, int m, maze123 M)
{
    float best_ts=0;
    f(i,1,POTIONS)
    {
        unordered_set<int>guess_sequence={};
        while(guess_sequence.size()<GUESSES)                    //if stack overflow happens here, check if N*M>=GUESSES
        {
            guess_sequence.insert(encode(rand()%n,rand()%m));
        }
        best_ts=max(total_score(n,m,M,guess_sequence),best_ts);
    }
    return best_ts;
}
int main()
{
    srand(time(0));
    maze123 M(100,100);
    auto start = std::chrono::high_resolution_clock::now();
    cout<<random(100,100,M)<<"\n";
    auto finish = std::chrono::high_resolution_clock::now();
    auto duration_ms = chrono::duration_cast<chrono::microseconds>(finish - start);
    cout<<"Time taken for 'random': "<<duration_ms.count()/1000<<" miliseconds\n";
    M.see();
    M.see_paths();
    return 0;
}
