#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
typedef long long ll;
const int N=5e5+5;
const ll mod=998244353;
struct Edge{
	int v,nxt;
	ll w;
}edge[N<<1];
int n,cnt,head[N],d[N];
ll g[N];//g[u]=以u为中点的三点链个数 
ll f[N];//f[u]=以u为根的子树内三点链的个数
ll ff[N];//ff[u]=以u为根的子树外三点链的个数
ll ans=0; 
void addedge(int u,int v,ll w){
	edge[++cnt].v=v;edge[cnt].w=w;edge[cnt].nxt=head[u];head[u]=cnt;
}
void dfs1(int u,int fa){
	f[u]=g[u]=1ll*d[u]*(d[u]-1)/2;
	for(int i=head[u];i;i=edge[i].nxt){
		int v=edge[i].v;
		if(v==fa) continue;
		dfs1(v,u);
		f[u]+=f[v];
	}
}
void dfs2(int u,int fa){
	for(int i=head[u];i;i=edge[i].nxt){
		int v=edge[i].v;
		if(v==fa) continue;
		ff[v]=ff[u]+f[u]-f[v];
		dfs2(v,u);
	}
}
int q[N],m;
ll w[N],sum[N],sub[N],Sub[N];
bool cmp(int u,int v){
	return w[u]<w[v];
}
void getans(int u,int fa){//讨论由u引出的边的贡献 
	for(int i=head[u];i;i=edge[i].nxt){
		int v=edge[i].v;
		if(v==fa) continue;
		getans(v,u);
	}
	m=0;
	for(int i=head[u];i;i=edge[i].nxt){
		int v=edge[i].v;
		q[++m]=v;
		w[v]=edge[i].w;
		sub[v]=(v==fa?ff[u]:f[v]);
		Sub[v]=(v==fa?f[u]:ff[v]);
		sum[u]=(sum[u]+edge[i].w)%mod;
	}
	sort(q+1,q+m+1,cmp);
	for(int i=1;i<=m;i++){
		ans=(ans+1ll*(m-1)*(m-2)%mod*w[q[i]]%mod)%mod;
		ans=(ans+1ll*(m-2)*(sum[u]+mod-w[q[i]])%mod)%mod;
		//"Y"型
		if(q[i]!=fa) ans=(ans+4ll*sub[q[i]]%mod*Sub[q[i]]%mod*w[q[i]]%mod)%mod;
		//q[i]作树边 
		ans=(ans+1ll*(sum[u]+mod-1ll*m*w[q[i]]%mod)%mod*sub[q[i]]%mod)%mod;
		//q[i]在三点链中 
	}
	int tot=0,pre=0;
    for(int i=1;i<=m;i++){
        int sS=i-1,sL=m-i;
        tot=(tot+1ll*(3*sL+sS)*w[q[i]])%mod;
    }
    for(int i=1;i<=m;i++){
        int ntot=tot;
        int sS=i-1,sL=m-i;
        int vS=pre,vL=(sum[u]+2ll*mod-pre-w[q[i]])%mod;
        ntot=(ntot+mod-1ll*(3*sL+sS)*w[q[i]]%mod)%mod;
        ntot=(ntot+4ll*mod-3ll*vS-vL)%mod;
        ans=(ans+1ll*ntot*sub[q[i]])%mod;
        pre=(pre+w[q[i]])%mod;
    }
    //q[i]不在三点链中 
    if(m>=4) ans=(ans+1ll*(m-2)*(m-3)/2%mod*tot)%mod;
    //"X"型 
}
int main(){
	scanf("%d",&n);
	for(int i=1;i<n;i++){
		int u,v;ll w;
		scanf("%d%d%lld",&u,&v,&w);
		addedge(u,v,w);
		addedge(v,u,w);
		d[u]++;d[v]++;
	}
	dfs1(1,0);
	dfs2(1,0);
	for(int i=1;i<=n;i++){
		f[i]%=mod;
		ff[i]%=mod;
		g[i]%=mod;
	}
	getans(1,0);
	printf("%lld\n",ans);
	return 0;
}
/*
4
1 2 1
1 3 2
1 4 3
*/
