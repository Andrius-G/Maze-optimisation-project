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
using namespace std;
int encode(int n, int m)
{
    return (n*MAXN)+m;
}
array<int,2> decode(int n)
{
    return {n/MAXN,n-(n/MAXN)};
}

