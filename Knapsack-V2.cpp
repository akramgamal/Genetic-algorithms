#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;
const int M=100,N=1e6;
float pc=0.7,pm=0.1;
int c,n,d,t,T,b=5,idx=0;
float points[N][2];
float encode[M][N],newencode[M][N],totalfit=0,mini=1e5;
vector<float>v1,v2,best;
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
        for(int i=r1; i<l; i++)
        {
            v1.push_back(encode[c2][i]);
        }
        for(int i=0; i<r1; i++)
        {
            v2.push_back(encode[c2][i]);
        }
        for(int i=r1; i<l; i++)
        {
            v2.push_back(encode[c1][i]);
        }
    }
    else
    {
        for(int i=0; i<l; i++)
        {
            v1.push_back(encode[c1][i]);
        }
        for(int i=0; i<l; i++)
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
            float deltaL=v1[i]-(-10.0);
            float deltaU=10.0-v1[i];
            float r1=(float) rand() / RAND_MAX;
            float y;
            if(r1<=0.5)y=deltaL;
            else y=deltaU;
            float r2=(float) rand() / RAND_MAX;
            float deltaT=y*(pow(1-r2,pow((1-t)/T,b)));
            if(y==deltaL)v1[i]=v1[i]-deltaT;
            else v1[i]=v1[i]+deltaT;
        }
    }
    for(int i=0; i<l; i++)
    {
        float r=(float) rand() / RAND_MAX;
        if(r<=pm)
        {
            float deltaL=v2[i]-(-10.0);
            float deltaU=10.0-v2[i];
            float r1=(float) rand() / RAND_MAX;
            float y;
            if(r1<=0.5)y=deltaL;
            else y=deltaU;
            float r2=(float) rand() / RAND_MAX;
            float deltaT=y*(pow(1-r2,pow((1-t)/T,b)));
            if(y==deltaL)v2[i]=v2[i]-deltaT;
            else v2[i]=v2[i]+deltaT;
        }
    }
    if(idx<100)
    {
        for(int i=0; i<l; i++)
        {
            newencode[idx][i]=v1[i];
        }
        idx++;
    }
    if(idx<100)
    {
        for(int i=0; i<l; i++)
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
    infile.open("input.txt");
    float LO=-10.0,HI=10.0;
    int cases=1;
    infile>>c;
    while(c--)
    {
        mini=1e5;
        totalfit=0;
        idx=0;
        infile>>n>>d;
        for(int i=0; i<n; i++)
        {
            for(int j=0; j<2; j++)
            {
                infile>>points[i][j];
            }
        }
        for(int i=0; i<M; i++)
        {
            for (int j=0; j < d+1; j++)
            {
                float random = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
                encode[i][j]=random;
            }
        }
        vector<float>evaluate;
        vector<float>cumm;
        vector<float>inversefit;
        for(int i=0; i<M; i++)
        {
            float MSE=0.0;
            for(int j=0; j<n; j++)
            {
                float res=encode[i][0];//a0
                int p=1;
                for(int k=1; k<d+1; k++)
                {
                    res+=(encode[i][k]*pow(points[j][0],p));
                    p++;
                }
                res-=points[j][1];
                res=res*res;
                MSE+=res;
            }
            float x=(MSE/n);
            evaluate.push_back(x);
            inversefit.push_back(1/x);
            totalfit+=(1/x);
            if(x<mini)
            {
                mini=x;
                best.clear();
                for(int h=0; h<d+1; h++)
                {
                    best.push_back(encode[i][h]);
                }
            }
        }
        for(int i=0; i<M; i++)
        {
            cumm.push_back(inversefit[i]);
        }
        for(int i=1;i<M;i++){
            cumm[i]+=cumm[i-1];
        }
        t=1;
        T=7000;
        while(t<T)
        {
            idx=0;
            for(int i=0; i<100; i++)
            {
                int p1=selection(cumm);
                int p2=selection(cumm);
                crossover(d+1,p1,p2);
                mutation(d+1,p1,p2);

            }
            for(int i=0; i<M; i++)
            {
                for(int j=0; j<d+1; j++)
                {
                    encode[i][j]=newencode[i][j];
                }
            }
            evaluate.clear();
            cumm.clear();
            inversefit.clear();
            totalfit=0;
            for(int i=0; i<M; i++)
            {
                float MSE=0.0;
                for(int j=0; j<n; j++)
                {
                    float res=encode[i][0];
                    int p=1;
                    for(int k=1; k<d+1; k++)
                    {
                        res+=(encode[i][k]*pow(points[j][0],p));
                        p++;
                    }
                    res-=points[j][1];
                    res=res*res;
                    MSE+=res;
                }
                float x=(MSE/n);
                evaluate.push_back(x);
                inversefit.push_back(1/x);
                totalfit+=(1/x);
                if(x<mini)
                {
                    mini=x;
                    best.clear();
                    for(int h=0; h<d+1; h++)
                    {
                        best.push_back(encode[i][h]);
                    }
                }
            }
            for(int i=0; i<M; i++)
            {
                cumm.push_back(inversefit[i]);
            }
            for(int i=1;i<M;i++){
            cumm[i]+=cumm[i-1];
             }
            t++;
        }

        cout<<"Case "<<cases<<" : ";
        cases++;
        for(int i=0; i<d+1; i++)
        {
            cout<<best[i]<<" ";
        }
        cout<<" ERROR : "<<mini<<endl;
    }
    return 0;
}
/*1
4 2
1 5
2 8
3 13
4 20
*/
