#include<iostream>
using namespace std;
int main()
{
    int n;
    cin>>n;

    int a[n];
    for(int i=0;i<n;i++)
    {
        cin>>a[i];
    }
    cout<<"sum";
    int s;
    cin>>s;

    int sum=0;
    int start=-1;
    int end=-1;
    int i=0;
    int j=0;

    while(j<n && sum+a[j]<=s)
    {
        sum+=a[j];
        j++;
    }

    if(sum==s)
    {
        cout<<i+1<<" "<<j <<endl;
        return 0;
    }

    while(j<n)
    {
        sum+=a[j];
        while(sum>s)
        {
            sum-=a[i];
            i++;
        }

        if(sum==s)
        {
            start=i+1;
            end=j+1;
            break;
        }
        j++;
    }
cout<<start<<" "<<end;
    return 0;

}