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
#define crossroad_chance 0.4           //if frand is less than this and deg cell <= 2, return
#define crossroad_chance2 0.15         //if frand is less than this and deg cell <= 3, return
// Note: This is **not** how you do a multiple-file program in C++.
using namespace std;

class invoke_srand
{
public:
     invoke_srand()
     {
         srand(time(0));
     }
};

static invoke_srand do_invoke_srand;

int VISITED[MAXN+1][MAXN+1]={};

float pheromones[MAXN+1][MAXN+1]={};

maze123 M(100,100);

float total_score(unordered_set<int>query)     //calculate total score from query set
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

void total_score_vocative(int n, int m, unordered_set<int>query)     //calculate total score from query set
{
    float ts=0;
    int MPSZ = M.paths.size();
    cout<<"Nodes in query: ";
    for(auto u: query)cout<<u<<" ";
    cout<<endl;
    f(i,0,MPSZ-1)
    {
        int sum=0;
        int MPISZ=M.paths[i].size();
        cout<<"Good nodes in query: ";
        f(j,0,MPISZ-1)
        {
            if(query.find(M.paths[i][j])!=query.end())cout<<M.paths[i][j]<<" ", sum++;
        }
        cout<<endl;
        ts=max(ts,((float)(2*sum))/M.lengths[i]);
    }
    return;
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
unordered_set<int>rng(int n, int m)
{
    unordered_set<int>guess_sequence={};
    while(guess_sequence.size()<GUESSES)                    //if stack overflow happens here, check if N*M>=GUESSES
    {
        guess_sequence.insert(encode(rand()%n,rand()%m));
    }
    return guess_sequence;
}
vector<array<int,2>>rng_vec(int n, int m)
{
    unordered_set<int>guess_sequence=rng(n,m);
    vector<array<int,2>>guesses={};
    for(auto u: guess_sequence)
    {
        guesses.pb(decode(u));
    }
    random_shuffle(guesses.begin(),guesses.end());
    return guesses;
}
float pso(int n, int m) //the 'particles' are possible solutions.
{
    vector<array<int,2>>G(GUESSES,{-1,-1}); //global best swarm position
    vector<array<int,2>>x[PARTICLES]; //positions of particles
    vector<array<int,2>>x_best[PARTICLES]; //best positions of particles
    vector<array<float,2>>v[PARTICLES]; //velocities of particles
    f(i,0,PARTICLES-1)
    {
        x[i] = rng_vec(n,m);
        x_best[i] = x[i];
        if(total_score(convert_vii_to_us(x_best[i]))>total_score(convert_vii_to_us(G)))
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
            if(total_score(convert_vii_to_us(x[i]))>total_score(convert_vii_to_us(x_best[i])))
            {
                x_best[i]=x[i];
                if(total_score(convert_vii_to_us(x_best[i]))>total_score(convert_vii_to_us(G)))
                {
                    G=x_best[i];
                }
            }
        }
    }
    auto GG = convert_vii_to_us(G);
    /*for(auto uwu: GG)
    {
        cout<<uwu<<" "<<decode(uwu)[0]<<" "<<decode(uwu)[1]<<"\n";
    }*/
    return total_score(GG);
}

void new_random_dfs(int x, int y, bool &done, int& guess, vector<int>&current_path)
    {
        if(done)return;
        VISITED[x][y]=1;
        current_path.pb(encode(x,y));
        guess++;
        if(guess>=GUESSES)
        {
            done=1;
            return;
        }
        //randomise order of if statements
        vector<int>permute={1,2,3,4};
        random_shuffle(permute.begin(),permute.end());
        //loads of if statements for dfs
        for(auto u: permute)
        {
            if(u==1) //case 1
            {
                if(M.in_bounds(x+1,y)) //increase x <-> go south
                {
                    if(!VISITED[x+1][y])
                    {
                        if(!M.adj_matrix[x][y][_S]) //if there is no edge, continue
                        {
                            continue;
                        }
                        new_random_dfs(x+1,y,done,guess,current_path);
                    }
                }
            }
            else if(u==2)
            {
                if(M.in_bounds(x-1,y))
                {
                    if(!VISITED[x-1][y])
                    {
                        if(!M.adj_matrix[x][y][_N])
                        {
                            continue;
                        }
                        new_random_dfs(x-1,y,done,guess,current_path);
                    }
                }
            }
            else if(u==3)
            {
                if(M.in_bounds(x,y+1))
                {
                    if(!VISITED[x][y+1])
                    {
                        if(!M.adj_matrix[x][y][_E])
                        {
                            continue;
                        }
                        new_random_dfs(x,y+1,done,guess,current_path);
                    }
                }
            }
            else
            {
                if(M.in_bounds(x,y-1))
                {
                    if(!VISITED[x][y-1])
                    {
                        if(!M.adj_matrix[x][y][_W])
                        {
                            continue;
                        }
                        new_random_dfs(x,y-1,done,guess,current_path);
                    }
                }
            }
        }
        //endifs
        return;
    }

void new_crossroad_dfs(int x, int y, bool &done, int& guess, vector<int>&current_path)
    {
        if(done)return;
        VISITED[x][y]=1;
        current_path.pb(encode(x,y));
        guess++;
        if(guess>=GUESSES)
        {
            done=1;
            return;
        }
        //randomise order of if statements
        vector<int>permute={1,2,3,4};
        random_shuffle(permute.begin(),permute.end());
        //loads of if statements for dfs
        int degree=0;
        f(i,0,3)
        {
            if(M.adj_matrix[x][y][i])
            {
                degree++;
            }
        }
        float prob=frand();
        if(degree<=2&&prob<crossroad_chance)
        {
            return;
        }
        if(degree<=3&&prob<crossroad_chance2)
        {
            return;
        }
        //prioritise crossroads
        for(auto u: permute)
        {
            if(u==1) //case 1
            {
                if(M.in_bounds(x+1,y)) //increase x <-> go south
                {
                    if(!VISITED[x+1][y])
                    {
                        if(!M.adj_matrix[x][y][_S]) //if there is no edge, continue
                        {
                            continue;
                        }
                        new_crossroad_dfs(x+1,y,done,guess,current_path);
                    }
                }
            }
            else if(u==2)
            {
                if(M.in_bounds(x-1,y))
                {
                    if(!VISITED[x-1][y])
                    {
                        if(!M.adj_matrix[x][y][_N])
                        {
                            continue;
                        }
                        new_crossroad_dfs(x-1,y,done,guess,current_path);
                    }
                }
            }
            else if(u==3)
            {
                if(M.in_bounds(x,y+1))
                {
                    if(!VISITED[x][y+1])
                    {
                        if(!M.adj_matrix[x][y][_E])
                        {
                            continue;
                        }
                        new_crossroad_dfs(x,y+1,done,guess,current_path);
                    }
                }
            }
            else
            {
                if(M.in_bounds(x,y-1))
                {
                    if(!VISITED[x][y-1])
                    {
                        if(!M.adj_matrix[x][y][_W])
                        {
                            continue;
                        }
                        new_crossroad_dfs(x,y-1,done,guess,current_path);
                    }
                }
            }
        }
        //endifs
        return;
    }

void new_single_dfs(int x, int y, bool &done, int& guess, int& potions, float& maxTotal, vector<int>&current_path)
    {
        if(done)return;
        VISITED[x][y]=1;
        current_path.pb(encode(x,y));
        guess++;
        if(guess>=GUESSES)
        {
            guess=0;
            unordered_set<int>guess_sequence={};
            for(auto u: current_path)
            {
                guess_sequence.insert(u);
            }
            current_path.clear();
            maxTotal=max(maxTotal,total_score(guess_sequence));
            potions++;
        }
        if(potions>=POTIONS)
        {
            done=1;
            return;
        }
        //randomise order of if statements
        vector<int>permute={1,2,3,4};
        random_shuffle(permute.begin(),permute.end());
        //loads of if statements for dfs
        for(auto u: permute)
        {
            if(u==1) //case 1
            {
                if(M.in_bounds(x+1,y)) //increase x <-> go south
                {
                    if(!VISITED[x+1][y])
                    {
                        if(!M.adj_matrix[x][y][_S]) //if there is no edge, continue
                        {
                            continue;
                        }
                        new_single_dfs(x+1,y,done,guess,potions,maxTotal,current_path);
                    }
                }
            }
            else if(u==2)
            {
                if(M.in_bounds(x-1,y))
                {
                    if(!VISITED[x-1][y])
                    {
                        if(!M.adj_matrix[x][y][_N])
                        {
                            continue;
                        }
                        new_single_dfs(x-1,y,done,guess,potions,maxTotal,current_path);
                    }
                }
            }
            else if(u==3)
            {
                if(M.in_bounds(x,y+1))
                {
                    if(!VISITED[x][y+1])
                    {
                        if(!M.adj_matrix[x][y][_E])
                        {
                            continue;
                        }
                        new_single_dfs(x,y+1,done,guess,potions,maxTotal,current_path);
                    }
                }
            }
            else
            {
                if(M.in_bounds(x,y-1))
                {
                    if(!VISITED[x][y-1])
                    {
                        if(!M.adj_matrix[x][y][_W])
                        {
                            continue;
                        }
                        new_single_dfs(x,y-1,done,guess,potions,maxTotal,current_path);
                    }
                }
            }
        }
        //endifs
        return;
    }

void improve_ts(int x, int y, bool &done, int& guess, float& maxTotal, vector<int>&current_path)
    {
        if(done)return;
        VISITED[x][y]=1;
        current_path.pb(encode(x,y));
        guess++;
        if(guess>=GUESSES)
        {
            done=1;
            return;
        }
        //randomise order of if statements
        vector<int>permute={1,2,3,4};
        int best_direction=-1;
        random_shuffle(permute.begin(),permute.end());
        //loads of if statements for dfs
        for(auto u: permute)
        {
            if(u==1) //case 1
            {
                if(M.in_bounds(x+1,y)) //increase x <-> go south
                {
                    if(!VISITED[x+1][y])
                    {
                        if(!M.adj_matrix[x][y][_S]) //if there is no edge, continue
                        {
                            continue;
                        }
                        if(best_direction==-1)best_direction=_S;
                        vector<int>vvv=current_path;
                        vvv.pb(encode(x+1,y));
                        float newTs=total_score(convert_v_to_us(vvv));
                        if(newTs>maxTotal)
                        {
                            maxTotal=newTs;
                            best_direction=_S;
                        }
                    }
                }
            }
            else if(u==2)
            {
                if(M.in_bounds(x-1,y))
                {
                    if(!VISITED[x-1][y])
                    {
                        if(!M.adj_matrix[x][y][_N])
                        {
                            continue;
                        }
                        if(best_direction==-1)best_direction=_N;
                        vector<int>vvv=current_path;
                        vvv.pb(encode(x-1,y));
                        float newTs=total_score(convert_v_to_us(vvv));
                        if(newTs>maxTotal)
                        {
                            maxTotal=newTs;
                            best_direction=_N;
                        }
                    }
                }
            }
            else if(u==3)
            {
                if(M.in_bounds(x,y+1))
                {
                    if(!VISITED[x][y+1])
                    {
                        if(!M.adj_matrix[x][y][_E])
                        {
                            continue;
                        }
                        if(best_direction==-1)best_direction=_E;
                        vector<int>vvv=current_path;
                        vvv.pb(encode(x,y+1));
                        float newTs=total_score(convert_v_to_us(vvv));
                        if(newTs>maxTotal)
                        {
                            maxTotal=newTs;
                            best_direction=_E;
                        }
                    }
                }
            }
            else
            {
                if(M.in_bounds(x,y-1))
                {
                    if(!VISITED[x][y-1])
                    {
                        if(!M.adj_matrix[x][y][_W])
                        {
                            continue;
                        }
                        if(best_direction==-1)best_direction=_W;
                        vector<int>vvv=current_path;
                        vvv.pb(encode(x,y-1));
                        float newTs=total_score(convert_v_to_us(vvv));
                        if(newTs>maxTotal)
                        {
                            maxTotal=newTs;
                            best_direction=_W;
                        }
                    }
                }
            }
        }
        if(best_direction==_S)
        {
            improve_ts(x+1,y,done,guess,maxTotal,current_path);
        }
        if(best_direction==_N)
        {
            improve_ts(x-1,y,done,guess,maxTotal,current_path);
        }
        if(best_direction==_E)
        {
            improve_ts(x,y+1,done,guess,maxTotal,current_path);
        }
        if(best_direction==_W)
        {
            improve_ts(x,y-1,done,guess,maxTotal,current_path);
        }
        //endifs
        return;
    }

void new_skip_dfs(int x, int y, bool &done, int& guess, vector<int>&current_path, int direction_back)
    {
        if(done)return;
        VISITED[x][y]=1;
        current_path.pb(encode(x,y));
        guess++;
        if(guess>=GUESSES)
        {
            done=1;
            return;
        }
        if(direction_back>=0&&!M.adj_matrix[x][y][direction_back])
        {
            //direction_back is the direction one has to go to go back to the cell where the dfs came from.
            //so this basically tests if our checker strategy skipped too many nodes and hence has to backtrack

            //not backtracking and just returning is bad (score \approx 0.01)
            //so backtrack to one less

            // since (x,y) and (x+2dx,y+2dy) exist, (x+dx,y+dy) also exists where dx,dy\in\{-1,0,1\}

            if(direction_back==_N)
            {
                new_skip_dfs(x-1,y,done,guess,current_path,-1);
            }
            if(direction_back==_S)
            {
                new_skip_dfs(x+1,y,done,guess,current_path,-1);
            }
            if(direction_back==_E)
            {
                new_skip_dfs(x,y+1,done,guess,current_path,-1);
            }
            if(direction_back==_W)
            {
                new_skip_dfs(x,y-1,done,guess,current_path,-1);
            }
            return;
        }
        //randomise order of if statements
        vector<int>permute={1,2,3,4};
        random_shuffle(permute.begin(),permute.end());
        //loads of if statements for dfs
        for(auto u: permute)
        {
            if(u==1) //case 1
            {
                if(M.in_bounds(x+2,y)) //increase x <-> go south
                {
                    if(!VISITED[x+2][y])
                    {
                        if(!M.adj_matrix[x][y][_S]) //if there is no edge, continue
                        {
                            continue;
                        }
                        new_skip_dfs(x+2,y,done,guess,current_path,_N);
                    }
                }
            }
            else if(u==2)
            {
                if(M.in_bounds(x-2,y))
                {
                    if(!VISITED[x-2][y])
                    {
                        if(!M.adj_matrix[x][y][_N])
                        {
                            continue;
                        }
                        new_skip_dfs(x-2,y,done,guess,current_path,_S);
                    }
                }
            }
            else if(u==3)
            {
                if(M.in_bounds(x,y+2))
                {
                    if(!VISITED[x][y+2])
                    {
                        if(!M.adj_matrix[x][y][_E])
                        {
                            continue;
                        }
                        new_skip_dfs(x,y+2,done,guess,current_path,_W);
                    }
                }
            }
            else
            {
                if(M.in_bounds(x,y-2))
                {
                    if(!VISITED[x][y-2])
                    {
                        if(!M.adj_matrix[x][y][_W])
                        {
                            continue;
                        }
                        new_skip_dfs(x,y-2,done,guess,current_path,_E);
                    }
                }
            }
        }
        //endifs
        return;
    }

void new_skip_aco_dfs(int x, int y, bool &done, int& guess, float& maxTotal, vector<int>&current_path)
    {
        if(done)return;
        VISITED[x][y]=1;
        current_path.pb(encode(x,y));
        //the aco step
        float ff=total_score(convert_v_to_us(current_path));
        if(ff>maxTotal)
        {
            maxTotal=ff;
            pheromones[x][y]=(1.0+pheromones[x][y])/2;
        }
        else
        {
            pheromones[x][y]/=2;
        }
        guess++;
        if(guess>=GUESSES)
        {
            done=1;
            return;
        }
        //randomise order of if statements
        vector<pair<float,int>>permute;
        if(M.in_bounds(x+1,y))permute.pb({pheromones[x+2][y],1});
        if(M.in_bounds(x-1,y))permute.pb({pheromones[x-2][y],2});
        if(M.in_bounds(x,y+1))permute.pb({pheromones[x][y+2],3});
        if(M.in_bounds(x,y-1))permute.pb({pheromones[x][y-2],4});
        sort(permute.begin(),permute.end());
        //loads of if statements for dfs
        for(auto u: permute)
        {
            if(u.second==1) //case 1
            {
                if(M.in_bounds(x+1,y)) //increase x <-> go south
                {
                    if(!VISITED[x+1][y])
                    {
                        if(!M.adj_matrix[x][y][_S]) //if there is no edge, continue
                        {
                            continue;
                        }
                        new_skip_aco_dfs(x+1,y,done,guess,maxTotal,current_path);
                    }
                }
            }
            else if(u.second==2)
            {
                if(M.in_bounds(x-1,y))
                {
                    if(!VISITED[x-1][y])
                    {
                        if(!M.adj_matrix[x][y][_N])
                        {
                            continue;
                        }
                        new_skip_aco_dfs(x-1,y,done,guess,maxTotal,current_path);
                    }
                }
            }
            else if(u.second==3)
            {
                if(M.in_bounds(x,y+1))
                {
                    if(!VISITED[x][y+1])
                    {
                        if(!M.adj_matrix[x][y][_E])
                        {
                            continue;
                        }
                        new_skip_aco_dfs(x,y+1,done,guess,maxTotal,current_path);
                    }
                }
            }
            else
            {
                if(M.in_bounds(x,y-1))
                {
                    if(!VISITED[x][y-1])
                    {
                        if(!M.adj_matrix[x][y][_W])
                        {
                            continue;
                        }
                        new_skip_aco_dfs(x,y-1,done,guess,maxTotal,current_path);
                    }
                }
            }
        }
        //endifs
        return;
    }

void new_greedy_dfs(int x, int y, bool &done, int& guess, vector<int>&current_path)
    {
        if(done)return;
        VISITED[x][y]=1;
        current_path.pb(encode(x,y));
        guess++;
        if(guess>=GUESSES)
        {
            done=1;
            return;
        }
        //greedily sort if statements
        vector<array<int,2>>permute = {
            {M.adj_matrix[x][y][_S],1},{M.adj_matrix[x][y][_N],2},
            {M.adj_matrix[x][y][_E],3},{M.adj_matrix[x][y][_W],4}
            };
        sort(permute.begin(),permute.end());
        //loads of if statements for dfs
        //sorted by connectivity, so 0's will be skipped
        for(auto u: permute)
        {
            if(u[1]==1) //case 1
            {
                if(M.in_bounds(x+1,y)) //increase x <-> go south
                {
                    if(!VISITED[x+1][y])
                    {
                        if(!M.adj_matrix[x][y][_S]) //if there is no edge, continue
                        {
                            continue;
                        }
                        new_greedy_dfs(x+1,y,done,guess,current_path);
                    }
                }
            }
            else if(u[1]==2)
            {
                if(M.in_bounds(x-1,y))
                {
                    if(!VISITED[x-1][y])
                    {
                        if(!M.adj_matrix[x][y][_N])
                        {
                            continue;
                        }
                        new_greedy_dfs(x-1,y,done,guess,current_path);
                    }
                }
            }
            else if(u[1]==3)
            {
                if(M.in_bounds(x,y+1))
                {
                    if(!VISITED[x][y+1])
                    {
                        if(!M.adj_matrix[x][y][_E])
                        {
                            continue;
                        }
                        new_greedy_dfs(x,y+1,done,guess,current_path);
                    }
                }
            }
            else
            {
                if(M.in_bounds(x,y-1))
                {
                    if(!VISITED[x][y-1])
                    {
                        if(!M.adj_matrix[x][y][_W])
                        {
                            continue;
                        }
                        new_greedy_dfs(x,y-1,done,guess,current_path);
                    }
                }
            }
        }
        //endifs
        return;
    }

float multi_dfs(int n, int m)
{
    float best_ts=0;
        f(q,1,POTIONS)
        {
            vector<int>path={};
            unordered_set<int>guesses={};
            f(i,0,MAXN-1)
            f(j,0,MAXN-1)
            {
                VISITED[i][j]=0;
            }
            bool b=0;
            int c=0;
            new_random_dfs(0,0,b,c,path);
            for(auto u: path)
            {
                guesses.insert(u);
            }
            best_ts=max(best_ts,total_score(guesses));
        }
    return best_ts;
}

float random(int n, int m)
{
    float best_ts=0;
    f(i,1,POTIONS)
    {
        unordered_set<int>guess_sequence={};
        while(guess_sequence.size()<GUESSES)                    //if stack overflow happens here, check if N*M>=GUESSES
        {
            guess_sequence.insert(encode(rand()%n,rand()%m));
        }
        best_ts=max(total_score(guess_sequence),best_ts);
    }
    return best_ts;
}

float catalan(int n, int m)
{
    float best_ts=0;
    f(i,1,POTIONS)
    {
        vector<char>v;
        f(i,1,50)v.pb('S');
        f(i,1,50)v.pb('E');
        int x=0, y=0;
        unordered_set<int>guess_sequence={};
        random_shuffle(v.begin(),v.end());
        f(i,1,100)
        {
            guess_sequence.insert(encode(x,y));
            if(v[i]=='S')x+=2;
            else y+=2;          //v[i]=='E'
            if(x>=100)x=99;
            if(y>=100)y=99;
        }
        best_ts=max(total_score(guess_sequence),best_ts);
    }
    return best_ts;
}

float pattern(int n, int m, unsigned int x, unsigned int y)
{
    float best_ts=0;
    f(i,1,POTIONS)
    {
        unordered_set<int>guess_sequence={};
        //35+35+30=100
        //top-left 1/4 x 1/4 bit
        while(guess_sequence.size()<x)                    //if stack overflow happens here, check if N*M>=GUESSES
        {
            guess_sequence.insert(encode(rand()%(n/4),rand()%(m/4)));
        }
        while(guess_sequence.size()<y)                    //if stack overflow happens here, check if N*M>=GUESSES
        {
            guess_sequence.insert(encode((n/4)+(rand()%(n/2)),(m/4)+(rand()%(m/2))));
        }
        while(guess_sequence.size()<100)                   //if stack overflow happens here, check if N*M>=GUESSES
        {
            guess_sequence.insert(encode((3*(n/4))+(rand()%(n/4)),(3*(m/4))+(rand()%(m/4))));
        }
        best_ts=max(total_score(guess_sequence),best_ts);
    }
    return best_ts;
}

float single_dfs(int n, int m)
{
    f(i,0,MAXN-1)
    f(j,0,MAXN-1)
    {
        VISITED[i][j]=0;
    }
    bool b=0;
    int g=0;
    int p=0;
    float best_ts=0;
    vector<int>v={};
    new_single_dfs(0,0,b,g,p,best_ts,v);
    return best_ts;
}

float skip_dfs(int n, int m)
{
    float best_ts=0;
        f(q,1,POTIONS)
        {
            vector<int>path={};
            unordered_set<int>guesses={};
            f(i,0,MAXN-1)
            f(j,0,MAXN-1)
            {
                VISITED[i][j]=0;
            }
            bool b=0;
            int c=0;
            new_skip_dfs(0,0,b,c,path,-1);
            for(auto u: path)
            {
                guesses.insert(u);
            }
            best_ts=max(best_ts,total_score(guesses));
        }
    return best_ts;
}

float greedy(int n, int m)
{
    float best_ts=0;
        f(q,1,POTIONS)
        {
            vector<int>path={};
            unordered_set<int>guesses={};
            f(i,0,MAXN-1)
            f(j,0,MAXN-1)
            {
                VISITED[i][j]=0;
            }
            bool b=0;
            int c=0;
            new_greedy_dfs(0,0,b,c,path);
            for(auto u: path)
            {
                guesses.insert(u);
            }
            best_ts=max(best_ts,total_score(guesses));
        }
    return best_ts;
}

float improve(int n, int m)
{
    float best_ts=0;
        f(q,1,POTIONS/4)
        {
            vector<int>path={};
            unordered_set<int>guesses={};
            f(i,0,MAXN-1)
            f(j,0,MAXN-1)
            {
                VISITED[i][j]=0;
            }
            bool b=0;
            int c=0;
            float ts=0;
            improve_ts(0,0,b,c,ts,path);
            for(auto u: path)
            {
                guesses.insert(u);
            }
            best_ts=max(best_ts,total_score(guesses));
        }
    return best_ts;
}

float checker(int n, int m)
{
    float best_ts=0;
    f(k,1,POTIONS)
    {
        unordered_set<int>guess_sequence={};
        for(int i=0; (i<=100-k)&&guess_sequence.size()<GUESSES/2; ++i)
            for(int j=0; (j<=k-1)&&guess_sequence.size()<GUESSES/2; ++j)
                {
                    if((i+j)%2)continue;
                    guess_sequence.insert(encode(i,j));
                }
        for(int i=99; (i>=100-k)&&guess_sequence.size()<GUESSES/2; --i)
            for(int j=99; (j>=k)&&guess_sequence.size()<GUESSES/2; --j)
                {
                    if((i+j)%2)continue;
                    guess_sequence.insert(encode(i,j));
                }
        best_ts=max(total_score(guess_sequence),best_ts);
    }
    return best_ts;
}

float crossroad(int n, int m)
{
    float best_ts=0;
        f(q,1,POTIONS)
        {
            vector<int>path={};
            unordered_set<int>guesses={};
            f(i,0,MAXN-1)
            f(j,0,MAXN-1)
            {
                VISITED[i][j]=0;
            }
            bool b=0;
            int c=0;
            new_crossroad_dfs(0,0,b,c,path);
            for(auto u: path)
            {
                guesses.insert(u);
            }
            best_ts=max(best_ts,total_score(guesses));
        }
    return best_ts;
}

float aco(int n, int m) //aco: use pheromone trails to prioritise good paths
{
    float best_ts=0;
    f(i,0,n-1)
    f(j,0,m-1)
    {
        pheromones[i][j]=0;
    }
    f(i,1,POTIONS)
    {
        vector<int>path={};
        unordered_set<int>guesses={};
        f(i,0,MAXN-1)
        f(j,0,MAXN-1)
        {
            VISITED[i][j]=0;
        }
        bool b=0;
        int c=0;
        float ff=0;
        new_skip_aco_dfs(0,0,b,c,ff,path);
        for(auto u: path)
        {
            guesses.insert(u);
        }
        best_ts=max(best_ts,total_score(guesses));
    }
    return best_ts;
}

int main()
{
    //M.see();          //comment if laggy
    //M.see_paths();
    {
        auto start = std::chrono::high_resolution_clock::now();
        cout<<random(100,100)<<"\n";
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration_ms = chrono::duration_cast<chrono::microseconds>(finish - start);
        cout<<"Time taken for 'random': "<<duration_ms.count()/1000<<" miliseconds\n";
    }
    /*{
        auto start = std::chrono::high_resolution_clock::now();
        cout<<pso(100,100)<<"\n";
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration_ms = chrono::duration_cast<chrono::microseconds>(finish - start);
        cout<<"Time taken for 'pso': "<<duration_ms.count()/1000<<" miliseconds\n";
    }*/
    {
        auto start = std::chrono::high_resolution_clock::now();
        cout<<multi_dfs(100,100)<<"\n";
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration_ms = chrono::duration_cast<chrono::microseconds>(finish - start);
        cout<<"Time taken for 'multi-dfs': "<<duration_ms.count()/1000<<" miliseconds\n";
    }
    {
        auto start = std::chrono::high_resolution_clock::now();
        cout<<catalan(100,100)<<"\n";
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration_ms = chrono::duration_cast<chrono::microseconds>(finish - start);
        cout<<"Time taken for 'catalan': "<<duration_ms.count()/1000<<" miliseconds\n";
    }
    {
        auto start = std::chrono::high_resolution_clock::now();
        cout<<pattern(100,100,35,65)<<"\n";
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration_ms = chrono::duration_cast<chrono::microseconds>(finish - start);
        cout<<"Time taken for 'pattern': "<<duration_ms.count()/1000<<" miliseconds\n";
    }
    {
        auto start = std::chrono::high_resolution_clock::now();
        cout<<pattern(100,100,50,50)<<"\n";
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration_ms = chrono::duration_cast<chrono::microseconds>(finish - start);
        cout<<"Time taken for 'corners': "<<duration_ms.count()/1000<<" miliseconds\n";
    }
    {
        auto start = std::chrono::high_resolution_clock::now();
        cout<<single_dfs(100,100)<<"\n";
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration_ms = chrono::duration_cast<chrono::microseconds>(finish - start);
        cout<<"Time taken for 'single-dfs': "<<duration_ms.count()/1000<<" miliseconds\n";
    }
    {
        auto start = std::chrono::high_resolution_clock::now();
        cout<<skip_dfs(100,100)<<"\n";
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration_ms = chrono::duration_cast<chrono::microseconds>(finish - start);
        cout<<"Time taken for 'skip-dfs': "<<duration_ms.count()/1000<<" miliseconds\n";
    }
    {
        auto start = std::chrono::high_resolution_clock::now();
        cout<<greedy(100,100)<<"\n";
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration_ms = chrono::duration_cast<chrono::microseconds>(finish - start);
        cout<<"Time taken for 'greedy': "<<duration_ms.count()/1000<<" miliseconds\n";
    }
    /*{
        auto start = std::chrono::high_resolution_clock::now();
        cout<<improve(100,100)<<"\n";
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration_ms = chrono::duration_cast<chrono::microseconds>(finish - start);
        cout<<"Time taken for 'improve': "<<duration_ms.count()/1000<<" miliseconds\n";
    }*/
    {
        auto start = std::chrono::high_resolution_clock::now();
        cout<<crossroad(100,100)<<"\n";
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration_ms = chrono::duration_cast<chrono::microseconds>(finish - start);
        cout<<"Time taken for 'crossroad': "<<duration_ms.count()/1000<<" miliseconds\n";
    }
    {
        auto start = std::chrono::high_resolution_clock::now();
        cout<<checker(100,100)<<"\n";
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration_ms = chrono::duration_cast<chrono::microseconds>(finish - start);
        cout<<"Time taken for 'checker': "<<duration_ms.count()/1000<<" miliseconds\n";
    }
    {
        auto start = std::chrono::high_resolution_clock::now();
        cout<<aco(100,100)<<"\n";
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration_ms = chrono::duration_cast<chrono::microseconds>(finish - start);
        cout<<"Time taken for 'aco': "<<duration_ms.count()/1000<<" miliseconds\n";
    }
    return 0;
}
