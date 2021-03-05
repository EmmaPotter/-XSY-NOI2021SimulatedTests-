#include<iostream>
#include<cstdio>
using namespace std;
typedef long long ll;
ll read(){
	ll x=0,f=1ll;
	char ch=getchar();
	while(ch<'0'||ch>'9'){if(ch=='-')f=-1ll;ch=getchar();}
	while(ch>='0'&&ch<='9'){x=x*10+ch-'0';ch=getchar();}
	return x*f;
}
int T;
ll a,b;
struct data{ll a,b,x;};
int main(){
	scanf("%d",&T);
	for(int o=1;o<=T;o++){
		a=read();b=read();
		if(b<0){a=-a;b=-b;}
		ll t=0;
		while(a){
			if(a>0){
				swap(a,b);a=-a;
				t++;
			}
			else{
				if(b<(-a)&&(-a)<2*b){
					ll x=-a-b;
					ll k=(b-1)/x;
					a+=x*k;
					b-=x*k;
					t+=3*k;
				}
				else{
					ll k=(-a-1)/b+1;
                	a+=b*k;
                	t+=k;
				}
			}
		}
		printf("%lld\n",t);
	}
	return 0;
}
/*
5
0 1
1 1
-1 2
-2 4
-8 -5
*/
