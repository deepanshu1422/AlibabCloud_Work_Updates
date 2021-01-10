//Kadanes Algorithm
#include<iostream>
using namespace std;
int main()
{
    int n;
    cin>>n;
    int a[n];
    for (int i = 0; i < n; i++)
    {
        cin>>a[i];
    }

    int currentSum=0;
    int maxsum=INT_MIN;
    for (int i = 0; i < n; i++)
    {
        currentSum+=a[i];
        if(currentSum<0)
        {
            currentSum=0;
        }
        maxsum=max(maxsum,currentSum);
    }
    
    cout<<maxsum;
    
}