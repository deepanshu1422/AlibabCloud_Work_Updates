#include<iostream>
using namespace std;
#include<set>
#include<bits/stdc++.h>
int main()
{
   int n;
   cin>>n;
   int arr[n];
   for(int i=0;i<n;i++)
   {
       cin>>arr[i];
   }
    sort(arr,arr+n);
    set<int> s;
    for(int i=0;i<n;i++){
        s.insert(arr[i]);
    }
    set<int>::iterator itr;
    itr=s.begin();
    for(int i=0;i<n;i++)
    {
        if(*itr != arr[i])
        {
            cout<<arr[i];
            return 0;
        }
        else 
        itr++;
    }
}
