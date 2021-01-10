#include<iostream>
#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n;
    cin>>n;
    int arr[n];

    for(int i=0;i<n;i++)
    {
        cin>>arr[i];
    
    }

    int answer=2;
    int pd=arr[1]-arr[0] ;
    int j=2;
    int curr=2;

    while(j<n)
    {
        if(pd==arr[j]-arr[j-1])
        {
            curr++;
        }
        else
        {
            pd=arr[j]-arr[j-1];
            curr=2;
        }
        answer=max(answer,curr);
        j++;
    }


    return 0;



}