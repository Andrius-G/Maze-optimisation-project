#include <bits/stdc++.h>
#define ll long long
#define f(i,L,R) for(int i=L; i<=R; ++i)
#define oo 666666666666666
#define pb push_back
#define pf push_front
#define DIR "tests/"
#define MAXN 250
#define MAXP 2
#define MINP 2
#define _E 0
#define _N 1
#define _W 2
#define _S 3
#define wall "#"
#define weak (0)
#define strict (1)
#define strict_bias ((double)0.1)
#define weak_bias ((double)0.4)
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
struct maze123
{
    int N, M;
    int paths_count;
    int visited[MAXN][MAXN];
    int adj_matrix[MAXN][MAXN][4];
    vector<vector<int>>paths;
    vector<int>lengths;
    maze123(int n, int m)
    {
        N = n;
        M = m;
        paths_count=0;
        f(i,0,N-1)
        f(j,0,M-1)
        {
            adj_matrix[i][j][_N]=0;
            adj_matrix[i][j][_E]=0;
            adj_matrix[i][j][_W]=0;
            adj_matrix[i][j][_S]=0;
        }
        regen();
        return;
    }
    void see()
    {
        f(i,0,N-1)
        {
            string s1="";
            string s2="";
            string s3="";
            string s4="";
            f(j,0,M-1)
            {
                if(i==0&&j==0)
                {
                    s1+=wall;
                    s2+=wall;
                    s3+=wall;
                    s4+=wall;
                    s1+="   ";
                    s2+="   ";
                    s3+="   ";
                    s4+="   ";
                    continue;
                }
                s1+=wall;
                if(in_bounds(i-1,j)&&adj_matrix[i][j][_N]) //north
                {
                    s1+=" ";
                    s1+=to_string(adj_matrix[i][j][_N]);
                    s1+=" ";
                }
                else s1+=wall, s1+=wall, s1+=wall;
                if(in_bounds(i,j-1)&&adj_matrix[i][j][_W]) //west
                {
                    s2+=" ";
                    s3+=to_string(adj_matrix[i][j][_W]);
                    s4+=" ";
                }
                else s2+=wall, s3+=wall, s4+=wall;
                s2+="   ";
                s3+="   ";
                s4+="   ";
            }
            s1+=wall;
            s2+=wall;
            s3+=wall;
            s4+=wall;
            cout<<s1<<"\n"<<s2<<"\n"<<s3<<"\n"<<s4<<"\n";
        }
        f(i,0,4*M-4)
        {
            cout<<wall;
        }
        cout<<"   "<<wall<<endl;
        return;
    }
    void see_paths()
    {
        ll K=paths.size();
        f(i,1,K)
        {
            cout<<"Length of path "<<i<<": "<<lengths[i-1]<<".\nThere are "<<paths[i-1].size()<<" nodes in the path. List of the nodes:\n";
            for(auto u: paths[i-1])
            {
                cout<<u<<" ";
            }
            cout<<"\n";
        }
        cout<<endl;
        return;
    }
    bool in_bounds(int x, int y)
    {
        if(x<0||y<0||x>=N||y>=M)return 0;
        return 1;
    }
    void dfs(int x, int y, int xf, int yf, int len, vector<int>&current_path, bool&found_end)
    {
        visited[x][y]=1;
        current_path.pb(encode(x,y));
        if(x==xf&&y==yf)
        {
            paths.pb(current_path);
            lengths.pb(len);
            found_end=1;
            return;
        }
        //weak branching: if we already have a path, we have a chance to stop
        if(weak&&found_end&&frand()<weak_bias)
        {
            return;
        }
        //strict branching: prioritise branching near the start
        if(strict&&found_end&&frand()*strict_bias<(double)current_path.size()/(N*M))
        {
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
                if(in_bounds(x+1,y)) //increase x <-> go south
                {
                    if(!visited[x+1][y])
                    {
                        if(!adj_matrix[x][y][_S]) //if there is no edge, build it
                        {
                            adj_matrix[x][y][_S]=(rand()%3)+1;
                            adj_matrix[x+1][y][_N]=adj_matrix[x][y][_S];
                        }
                        dfs(x+1,y,xf,yf,len+adj_matrix[x+1][y][_N],current_path,found_end);
                    }
                    //only one path per iteration of dfs; place walls
                    //regen() takes care of placing zeroes at the beginning
                    //in case a previous path went through x,y <-> x+1,y, we don't reset the edge weight here
                }
            }
            else if(u==2)
            {
                if(in_bounds(x-1,y))
                {
                    if(!visited[x-1][y])
                    {
                        if(!adj_matrix[x][y][_N])
                        {
                            adj_matrix[x][y][_N]=(rand()%3)+1;
                            adj_matrix[x-1][y][_S]=adj_matrix[x][y][_N];
                        }
                        dfs(x-1,y,xf,yf,len+adj_matrix[x-1][y][_S],current_path,found_end);
                    }
                }
            }
            else if(u==3)
            {
                if(in_bounds(x,y+1))
                {
                    if(!visited[x][y+1])
                    {
                        if(!adj_matrix[x][y][_E])
                        {
                            adj_matrix[x][y][_E]=(rand()%3)+1;
                            adj_matrix[x][y+1][_W]=adj_matrix[x][y][_E];
                        }
                        dfs(x,y+1,xf,yf,len+adj_matrix[x][y+1][_W],current_path,found_end);
                    }
                }
            }
            else
            {
                if(in_bounds(x,y-1))
                {
                    if(!visited[x][y-1])
                    {
                        if(!adj_matrix[x][y][_W])
                        {
                            adj_matrix[x][y][_W]=(rand()%3)+1;
                            adj_matrix[x][y-1][_E]=adj_matrix[x][y][_W];
                        }
                        dfs(x,y-1,xf,yf,len+adj_matrix[x][y-1][_E],current_path,found_end);
                    }
                }
            }
        }
        //endifs
        current_path.pop_back();
        return;
    }
    void regen()
    {
        paths_count = (rand() % (MAXP-MINP+1)) + MINP;
        paths.clear();
        lengths.clear();
        f(i,0,N-1)
        f(j,0,M-1)
        {
            adj_matrix[i][j][_N]=0;
            adj_matrix[i][j][_E]=0;
            adj_matrix[i][j][_W]=0;
            adj_matrix[i][j][_S]=0;
        }
        f(i,1,paths_count)
        {
            f(i,0,N-1)
            f(j,0,M-1)
            {
                visited[i][j]=0;
            }
            vector<int>v={};
            bool b=0;
            dfs(0,0,N-1,M-1,0,v,b);
        }
        return;
    }
};
int main()
{
    srand(time(0));
    maze123 M(20,20);
    M.see();
    M.see_paths();
    return 0;
}
