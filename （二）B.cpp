#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;
const int N=5e5+5;
int n;
double val,a[N],sum[N];
double ave1(int m,int t){//尽可能取小
	double c=sum[t]+sum[m+t]-sum[m-1];
	c=c/(2*t+1);
	return c;
}
double ave2(int m,int t){//尽可能取大 
	double c=sum[m]-sum[m-t-1]+sum[n]-sum[n-t];
	c=c/(2*t+1);
	return c;
}
int main(){
	scanf("%d",&n);
	for(int i=1;i<=n;i++){
		int x;scanf("%d",&x);
		a[i]=double(x);
		sum[i]=sum[i-1]+a[i];
	}
	if(n==2){printf("%.7f\n",0);return 0;}
	for(int i=2;i<=n-1;i++){
		//average<medium 
		int l=0,r=min(i-1,n-i)-1,mid;
		while(l<=r){
			mid=(l+r)/2;
			if(ave1(i,mid)>ave1(i,mid+1)) l=mid+1;
			else r=mid-1;
		}
		val=max(val,a[i]-ave1(i,l));
		//average>medium
		l=0,r=min(i-1,n-i)-1;
		while(l<=r){
			mid=(l+r)/2;
			if(ave2(i,mid)<ave2(i,mid+1)) l=mid+1;
			else r=mid-1;
		}
		val=max(val,ave2(i,l)-a[i]);
	}
	printf("%.7lf",val);
	return 0;
}
