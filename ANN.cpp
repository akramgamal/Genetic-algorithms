#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;
int m,l,n,numOfTrain;
int t=500;//number of iteration
double eta=0.5;
vector<double> weightsleft,weightsright;
ofstream f;
double sigmoid(double x)//return sigmoid function
{
    return (double)1/(1+exp(-x));
}
void Backpropagation(vector<double>&yout,double output[],vector<double>&hiddenOut,double input[])
{
    vector<double>errors1,errors2;//error1....error of y
    //error2......error of hidden layer
    for(int i=0; i<n; i++)
    {
        errors1.push_back((yout[i]*(1-yout[i]))*(output[i]-yout[i]));//derivative of sigmoid
    }
    int idx=0,c=0;//idx to iterate over all weights
    for(int i=0; i<l; i++) //to get errors of hidden layers
    {
        double sum=0,sum2=0;
        idx=c;
        sum+=hiddenOut[i]*(1-hiddenOut[i]);//derivative of sigmoid
        for(int j=0; j<n; j++)
        {
            sum2+=(errors1[j]*weightsright[idx]);
            idx+=l;//idx+l to iterate over the weights in that hidden layer
        }
        sum*=sum2;
        c++;
        errors2.push_back(sum);//store errors in vector
    }
    idx=0;
    for(int i=0; i<n; i++)//to change the weights on the right in t+1 iteration
    {
        for(int j=0; j<l; j++)
        {
            weightsright[idx]=weightsright[idx]+(eta*errors1[i]*hiddenOut[j]);
            idx++;
        }
    }
    idx=0;
    for(int i=0; i<l; i++)//to change the weights on the left in t+1 iteration
    {
        for(int j=0; j<m; j++)
        {
            weightsleft[idx]=weightsleft[idx]+(eta*errors2[i]*input[j]);
        }
    }

}
void feedforward(double input[],double output[])
{
    int idx=0;
    vector<double> hiddenIn,hiddenOut,yin,yout;
    for(int i=0; i<l; i++)//sum of product on hidden layer
    {
        double sum=0.0;
        for(int j=0; j<m; j++)
        {
            sum+=(weightsleft[idx]*input[j]);
            idx++;
        }
        hiddenIn.push_back(sum);
        hiddenOut.push_back(sigmoid(sum));
    }
    idx=0;
    for(int i=0; i<n; i++)//sum of product on output layer
    {
        double sum=0.0;
        for(int j=0; j<l; j++)
        {
            sum+=(weightsright[idx]*hiddenOut[j]);
            idx++;
        }
        yin.push_back(sum);
        yout.push_back(sigmoid(sum));
    }
    double MSE=0.0;
    for(int i=0; i<n; i++)//compute mean square error
    {
        MSE+=abs(output[i]-yout[i]);
    }
    MSE*=0.5;
    if(t==1){// if it's the last iteration
    cout<<MSE<<endl;
    for(int i=0;i<weightsleft.size();i++){
        f<<weightsleft[i]<<"     ";
    }
    f<<"\n";
    for(int i=0;i<n;i++){
        f<<yout[i]<<" ";
    }
    f<<"\n";
    for(int i=0;i<weightsright.size();i++){
        f<<weightsright[i]<<"     ";
    }
    f<<"\n";
    }
    Backpropagation(yout,output,hiddenOut,input); ///call back probagation
}

int main()
{
    srand((unsigned) time(0));
    float LO=-5.0,HI=5.0;//random number from -5 to 5
    ifstream infile;
    infile.open("input.txt");
    f.open("output.txt");
    infile>>m>>l>>n>>numOfTrain;
    double input[m],output[n];
    int c=1;
    for(int i=0; i<numOfTrain; i++)
    {
        t=500;
        f<<"case : "<<c<<"\n";
        c++;
        for(int j=0; j<m; j++)
        {
            infile>>input[j];
        }
        for(int j=0; j<n; j++)
        {
            infile>>output[j];
        }
        for(int i=0; i<l; i++)
        {

            for(int j=0; j<m; j++)
            {
                float random = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));//random number from -5 to 5
                weightsleft.push_back(random);

            }
        }
        for(int i=0; i<l; i++)
        {

            for(int j=0; j<n; j++)
            {
                float random = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
                weightsright.push_back(random);

            }
        }
        while(t--)
        {
            feedforward(input,output);
        }
        weightsleft.clear();
        weightsright.clear();
    }
    return 0;
}
