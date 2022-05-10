#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;
float line(int x1,int x2,int y1,int y2,int x)
{
    float slope=(float)(y2-y1)/(float)(x2-x1);
    float ax=slope*x2;
    float c=y2-ax;
    float y=slope*x+c;
    return y;
}
float centroidtriangle(vector<int>&v)
{
    int sum=0;
    for(int i=0; i<3; i++)
    {
        sum+=v[i];
    }
    return sum/3;
}
float centroidtrapezoidal(vector<int>&v)
{
    float area=0;
    int trapezoial[]= {0,1,1,0};
    for(int i=0; i<3; i++)
    {
        area+=((v[i]*trapezoial[i+1])-(v[i+1]*trapezoial[i]));
    }
    area*=0.5;
    float centroid;
    for(int i=0; i<3; i++)
    {
        centroid+=((v[i]+v[i+1])*(v[i]*trapezoial[i+1]-v[i+1]*trapezoial[i]));
    }
    centroid*=(1/(6*area));
    return centroid;
}
int main()
{
    ifstream infile;
    infile.open("input.txt");
    string type;
    int numberofsets,numberofinput;
    infile>>numberofinput;
    string s[numberofinput];
    int n[numberofinput];
    map<string,float>m;
    for(int k=0; k<numberofinput; k++)
    {
        infile>>s[k]>>n[k]>>numberofsets;
        string sett[numberofsets];
        vector<vector<int>>v(numberofsets);
        for(int i=0; i<numberofsets; i++)
        {
            infile>>sett[i]>>type;
            if(type=="trapezoidal")
            {
                for(int j=0; j<4; j++)
                {
                    int x;
                    infile>>x;
                    v[i].push_back(x);
                }
            }
            else if(type=="triangle")
            {
                for(int j=0; j<3; j++)
                {
                    int x;
                    infile>>x;
                    v[i].push_back(x);
                }
            }
        }
        int trapezoidal[4]= {0,1,1,0};
        int triangle[3]= {0,1,0};
        for(int p=0; p<numberofsets; p++)
        {
            if(v[p].size()==3)
            {
                ///see if it's in the set
                bool found=false;
                int idx=-1;
                for(int g=0; g<3; g++)
                {
                    if(v[p][g]==n[k])
                    {
                        found=true;
                        idx=g;
                        break;
                    }
                }
                if(found)
                {
                    m[sett[p]]=triangle[idx];
                    continue;
                }
                ///see if it's between two numbers
                idx=-1;
                int idx2=-1;
                for(int g=1; g<3; g++)
                {
                    if(n[k]<v[p][g]&&n[k]>v[p][g-1])
                    {
                        found=true;
                        idx=g-1;
                        idx2=g;
                    }
                }
                if(found)
                {
                    float t=line(v[p][idx],v[p][idx2],triangle[idx],triangle[idx2],n[k]);
                    m[sett[p]]=t;
                }
                else
                {
                    m[sett[p]]=0;
                }

            }
            else
            {
                bool found=false;
                int idx=-1;
                for(int g=0; g<4; g++)
                {
                    if(v[p][g]==n[k])
                    {
                        found=true;
                        idx=g;
                        break;
                    }
                }
                if(found)
                {
                    m[sett[p]]=trapezoidal[idx];
                    continue;
                }
                ///see if it's between two numbers
                idx=-1;
                int idx2=-1;
                for(int g=1; g<4; g++)
                {
                    if(n[k]<v[p][g]&&n[k]>v[p][g-1])
                    {
                        found=true;
                        idx=g-1;
                        idx2=g;
                    }
                }
                if(found)
                {
                    float t=line(v[p][idx],v[p][idx2],trapezoidal[idx],trapezoidal[idx2],n[k]);
                    m[sett[p]]=t;
                }
                else
                {
                    m[sett[p]]=0;
                }
            }

        }
    }
    cout<<"the membership of inputs \n";
    for(auto &[st,x]:m)
    {
        cout<<st<<" "<<x<<endl;
    }
    string output;
    int outputsets;
    infile>>output>>outputsets;
    string setsofOutput[outputsets];
    vector<vector<int>> out(outputsets);
    for(int i=0; i<outputsets; i++)
    {
        infile>>setsofOutput[i]>>type;
        if(type=="trapezoidal")
        {
            for(int j=0; j<4; j++)
            {
                int x;
                infile>>x;
                out[i].push_back(x);
            }
        }
        else if(type=="triangle")
        {
            for(int j=0; j<3; j++)
            {
                int x;
                infile>>x;
                out[i].push_back(x);
            }
        }
    }
    int numberofRules;
    map<string,float> outputmember;
    infile>>numberofRules;
    string line;
    for(int i=0; i<numberofRules; i++)
    {
        int num,j=0;
        float ans;
        bool operation=false;///false or---true and
        infile>>num>>line;
        while(line!="then")
        {
            infile>>line;
            if(line=="AND")
            {
                operation=true;
                continue;
            }
            else if(line=="OR")
            {
                operation=false;
                continue;
            }
            if(line=="="){
                infile>>line;
                if(j==0)
                {
                    ans=m[line];
                    j++;
                    continue;
                }
                if(operation)
                {
                    ans=min(ans,m[line]);
                }
                else
                {
                    ans=max(ans,m[line]);
                }
            }else if(line=="!="){
                infile>>line;
                if(j==0)
                {
                    ans=1-m[line];
                    j++;
                    continue;
                }
                if(operation)
                {
                    ans=min(ans,1-m[line]);
                }
                else
                {
                    ans=max(ans,1-m[line]);
                }
            }

        }
        infile>>line>>line>>line;
        outputmember[line]=ans;
    }
    cout<<"result of rules \n";
    for(auto &[st,x]:outputmember)
    {
        cout<<st<<" "<<x<<endl;
    }
    map<string,float> centroid;
    for(int i=0; i<outputsets; i++)
    {
        if(out[i].size()==3)
        {
            centroid[setsofOutput[i]]=centroidtriangle(out[i]);
        }
        else
        {
            centroid[setsofOutput[i]]=centroidtrapezoidal(out[i]);
        }
    }
    cout<<"the centroid for outputs\n";
    for(auto &[st,x]:centroid)
    {
        cout<<st<<" "<<x<<endl;
    }
    float numenator=0,denomenator=0,maxi=-20000;
    for(auto &[st,x]:outputmember)
    {
        numenator+=(x*centroid[st]);
        denomenator+=x;
        maxi=max(maxi,x);
    }
    cout<<"the final result\n"<<numenator/denomenator<<endl;
    for(auto &[st,x]:outputmember)
    {
        if(x==maxi)
        {
            cout<<st<<" ";
        }
    }
    return 0;
}
