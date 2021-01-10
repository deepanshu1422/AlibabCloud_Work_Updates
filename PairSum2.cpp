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

    cout <<"Enter the sum";
    int sum;
    cin>>sum;

    int start=0;
    int end=n;
    int flag;
    for(int i=0;i<n;i++)
    {
        if(a[start]+a[end]==sum)
        {
            cout<<start<<" "<<end;
            flag=1;
            break;
        }
        if(a[start]+a[end]>sum)
        {
            end--;
        }
        if(a[start]+a[end]<sum)
        {
            start++;
        }
    }
   if(flag!=1)
   cout<<"Not Found";
}