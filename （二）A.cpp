#include<iostream>
#include<cstdio>
using namespace std;
typedef long long ll;
ll read(){
	ll x=0,f=1;
	char ch=getchar();
	while(ch<'0'||ch>'9'){if(ch=='-')f=-1;ch=getchar();}
	while(ch>='0'&&ch<='9'){x=x*10+ch-'0';ch=getchar();}
	return x*f;
}
const int N=5e5+5;
int n,q,a[N];ll sum[N];
int query(ll x,ll y){
	int tot=0;
	int l=1,r=min(1ll*n,y),mid,ans=0;
	while(l<=r){
		mid=(l+r)/2;
		if(x+sum[mid]-y*mid>=0){ans=mid;l=mid+1;}
		else r=mid-1;
	}
	if(ans&&x+sum[ans]-y*ans==0) tot++;
	l=min(1ll*n,y)+1,r=n,ans=0;
	while(l<=r){
		mid=(l+r)/2;
		if(x+sum[mid]-y*mid<=0){ans=mid;l=mid+1;}
		else r=mid-1;
	}
	if(ans&&x+sum[ans]-y*ans==0) tot++;
	return tot;
}
int main(){
	n=read();q=read();
	for(int i=1;i<=n;i++){
		a[i]=read();
		sum[i]=sum[i-1]+1ll*a[i];
	}
	ll x,y;
	for(int o=1;o<=q;o++){
		x=read();y=read();
		if(y<=n&&x+sum[y]-y*y==0){
			int tot=1;
			if(x+sum[y+1]-y*(y+1)==0) tot++;
			if(x+sum[y-1]-y*(y-1)==0) tot++;
			printf("%d\n",tot);
		}
		else printf("%d\n",query(x,y));
	}
	return 0;
}
