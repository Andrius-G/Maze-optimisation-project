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
#define pso_phi_p 2.5                  //cognitive coefficient in [1,3]
#define pso_phi_g 1.1                  //social coefficient in [1,3]
#define pso_inertia_weight 0.1         //less than 1
#define PARTICLES 30
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
unordered_set<int>convert_v_to_us(vector<int>v)
{
    unordered_set<int>s;
    for(auto u: v)s.insert(u);
    return s;
}
unordered_set<int>convert_vii_to_us(vector<array<int,2>>v)
{
    unordered_set<int>s;
    for(auto u: v)s.insert(encode(u[0],u[1]));
    return s;
}
unordered_set<int>rng(int n, int m, maze123 M)
{
    unordered_set<int>guess_sequence={};
    while(guess_sequence.size()<GUESSES)                    //if stack overflow happens here, check if N*M>=GUESSES
    {
        guess_sequence.insert(encode(rand()%n,rand()%m));
    }
    return guess_sequence;
}
vector<array<int,2>>rng_vec(int n, int m, maze123 M)
{
    unordered_set<int>guess_sequence=rng(n,m,M);
    vector<array<int,2>>guesses={};
    for(auto u: guess_sequence)
    {
        guesses.pb(decode(u));
    }
    random_shuffle(guesses.begin(),guesses.end());
    return guesses;
}
float pso(int n, int m, maze123 M) //the 'particles' are possible solutions.
{
    vector<array<int,2>>G(GUESSES,{-1,-1}); //global best swarm position
    vector<array<int,2>>x[PARTICLES]; //positions of particles
    vector<array<int,2>>x_best[PARTICLES]; //best positions of particles
    vector<array<float,2>>v[PARTICLES]; //velocities of particles
    f(i,0,PARTICLES-1)
    {
        x[i] = rng_vec(n,m,M);
        x_best[i] = x[i];
        if(total_score(n,m,M,convert_vii_to_us(x_best[i]))>total_score(n,m,M,convert_vii_to_us(G)))
        {
            G=x_best[i];
        }
        f(j,1,GUESSES)
        {
            float v_x=((frand())*(2*n))-n;
            float v_y=((frand())*(2*m))-m;
            v[i].pb({v_x,v_y});
        }
    }
    f(k,0,POTIONS-PARTICLES)
    {
        f(i,0,PARTICLES-1)
        {
            f(d,0,GUESSES-1)
            {
                float& v_x = v[i][d][0];
                float& v_y = v[i][d][1];
                float r_p = frand(), r_g = frand(); //random cognitive and social parameters
                //update velocity <-> move towards 'best' pos
                v_x=(pso_inertia_weight*v_x)+(pso_phi_p*r_p*(x_best[i][d][0]-x[i][d][0]))+(pso_phi_g*r_g*(G[d][0]-x[i][d][0]));
                v_y=(pso_inertia_weight*v_y)+(pso_phi_p*r_p*(x_best[i][d][1]-x[i][d][1]))+(pso_phi_g*r_g*(G[d][1]-x[i][d][1]));
                //update particle position
                x[i][d][0]+=v_x;
                if(x[i][d][0]<0)x[i][d][0]=0;
                if(x[i][d][0]>=n)x[i][d][0]=n-1;
                x[i][d][1]+=v_y;
                if(x[i][d][1]<0)x[i][d][1]=0;
                if(x[i][d][1]>=n)x[i][d][1]=n-1;
            }//update each dimension of particle
            if(total_score(n,m,M,convert_vii_to_us(x[i]))>total_score(n,m,M,convert_vii_to_us(x_best[i])))
            {
                x_best[i]=x[i];
                if(total_score(n,m,M,convert_vii_to_us(x_best[i]))>total_score(n,m,M,convert_vii_to_us(G)))
                {
                    G=x_best[i];
                }
            }
        }
    }
    auto GG = convert_vii_to_us(G);
    for(auto uwu: GG)
    {
        cout<<uwu<<" "<<decode(uwu)[0]<<" "<<decode(uwu)[1]<<"\n";
    }
    return total_score(n,m,M,GG);
}
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
    M.see();          //comment if laggy
    //M.see_paths();
    {
        auto start = std::chrono::high_resolution_clock::now();
        cout<<random(100,100,M)<<"\n";
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration_ms = chrono::duration_cast<chrono::microseconds>(finish - start);
        cout<<"Time taken for 'random': "<<duration_ms.count()/1000<<" miliseconds\n";
    }
    {
        auto start = std::chrono::high_resolution_clock::now();
        cout<<pso(100,100,M)<<"\n";
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration_ms = chrono::duration_cast<chrono::microseconds>(finish - start);
        cout<<"Time taken for 'pso': "<<duration_ms.count()/1000<<" miliseconds\n";
    }
    return 0;
}
