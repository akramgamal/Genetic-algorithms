#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;
const int M=100,N=50;
float pc=0.7,pm=0.1;
int c,n,s,maxi=-1;
int devices[N][2];
int encode[M][N],newencode[M][N],idx=0,totalfit=0;
vector<int>v1,v2;
vector<int>best;
bool valid(vector<int> &v)
{
    int res=0;
    for(int i=0; i<n; i++)
    {
        res+=v[i]*devices[i][0];///0110
    }
    return res<=s;
}
int selection(vector<float> &v)
{
    float random=(float) rand() / RAND_MAX;///from 0 to 1
    if(random<v[0])
    {
        return 0;
    }
    else
    {
        for(int i=0; i<M-1; i++)
        {
            if(random>=v[i]&&random<v[i+1])
            {
                return i+1;
            }
        }
    }
}
void crossover(int l,int c1,int c2)
{
    int r1 = (rand() % l-1)+1;
    float r2=(float) rand() / RAND_MAX;
    v1.clear();
    v2.clear();
    if(r2<=pc)
    {
        for(int i=0; i<r1; i++)
        {
            v1.push_back(encode[c1][i]);
        }
        for(int i=r1; i<n; i++)
        {
            v1.push_back(encode[c2][i]);
        }
        for(int i=0; i<r1; i++)
        {
            v2.push_back(encode[c2][i]);
        }
        for(int i=r1; i<n; i++)
        {
            v2.push_back(encode[c1][i]);
        }
    }
    else
    {
        for(int i=0; i<n; i++)
        {
            v1.push_back(encode[c1][i]);
        }
        for(int i=0; i<n; i++)
        {
            v2.push_back(encode[c2][i]);
        }
    }
}
void mutation(int l,int c1,int c2)
{
    for(int i=0; i<l; i++)
    {
        float r=(float) rand() / RAND_MAX;
        if(r<=pm)
        {
            if(v1[i]==0)
                v1[i]=1;
            else
                v1[i]=0;
        }
    }
    for(int i=0; i<l; i++)
    {
        float r=(float) rand() / RAND_MAX;
        if(r<=pm)
        {
            if(v2[i]==0)
                v2[i]=1;
            else
                v2[i]=0;
        }
    }
    if(valid(v1)&&idx<100)
    {
        for(int i=0; i<n; i++)
        {
            newencode[idx][i]=v1[i];
        }
        idx++;
    }
    if(valid(v2)&&idx<100)
    {
        for(int i=0; i<n; i++)
        {
            newencode[idx][i]=v2[i];
        }
        idx++;
    }
}
int main()
{
    srand((unsigned) time(0));
    ifstream infile;
    infile.open("input_example.txt");
    int d=1;
    infile>>c;
    while(c--)
    {
        maxi=-1;
        infile>>n>>s;
        for(int i=0; i<n; i++)
        {
            for(int j=0; j<2; j++)
            {
                infile>>devices[i][j];
            }
        }

        for(int i=0; i<M; i++)
        {
            vector<int> v;
            for (int j=0; j < n; j++)
            {
                v.push_back(rand() % 2);
            }
            while(!valid(v))
            {
                v.clear();
                for (int j=0; j < n; j++)
                {
                    v.push_back(rand() % 2);
                }
            }
            for(int k=0; k<n; k++)
            {
                encode[i][k]=v[k];
            }
        }
        vector<int>evaluate;
        vector<float>cumm;
        for(int i=0; i<M; i++)
        {
            int res=0;
            for(int j=0; j<n; j++)
            {
                res+=encode[i][j]*devices[j][1];///0110
            }
            evaluate.push_back(res);
            totalfit+=res;
            if(res>maxi)
            {
                maxi=res;
                best.clear();
                for(int k=0; k<n; k++)
                {
                    best.push_back(encode[i][k]);
                }
            }
        }
        for(int i=0; i<M; i++)
        {
            cumm.push_back((float)evaluate[i]/totalfit);
        }
        for(int i=1; i<M; i++)
        {
            cumm[i]+=cumm[i-1];
        }
        int z=0;
        while(z<3000)
        {
            idx=0;
            for(int i=0; i<100; i++)
            {
                int p1=selection(cumm);
                int p2=selection(cumm);
                crossover(n,p1,p2);
                mutation(n,p1,p2);
            }
            for(int i=0; i<M; i++)
            {
                for(int j=0; j<n; j++)
                {
                    encode[i][j]=newencode[i][j];
                }
            }
            evaluate.clear();
            cumm.clear();
            totalfit=0;
            bool t=true;
            for(int i=0; i<M; i++)
            {
                int res=0;
                for(int j=0; j<n; j++)
                {
                    res+=encode[i][j]*devices[j][1];
                }
                evaluate.push_back(res);
                totalfit+=res;
                if(res>maxi)
                {
                    maxi=res;
                    z++;
                    best.clear();
                    for(int k=0; k<n; k++)
                    {
                        best.push_back(encode[i][k]);
                    }
                    t=false;
                }
            }
            for(int i=0; i<M; i++)
            {
                cumm.push_back((float)evaluate[i]/totalfit);
            }
            for(int i=1; i<M; i++)
            {
                cumm[i]+=cumm[i-1];
            }
            if(t)
                z++;
            else
                z=0;
        }

        cout<<"Case "<<d<<" : ";
        d++;
        cout<<maxi<<endl;
        int cnt=0;
        for(int i=0; i<n; i++)
        {
            if(best[i]==1)
                cnt++;
        }
        cout<<cnt<<endl;
        for(int i=0; i<n; i++)
        {
            if(best[i]==1)
            {
                for(int j=0; j<2; j++)
                {
                    cout<<devices[i][j]<<" ";
                }
                cout<<endl;
            }
        }
    }
    return 0;
}
