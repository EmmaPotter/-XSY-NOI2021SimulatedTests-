#include<iostream>
#include<cstdio>
using namespace std;
typedef long long ll;
const int N=5e5+5;
const ll mod=998244353;
const ll div2=499122177;
struct Edge{
	int v,nxt;
}edge[N<<1];
int n,m,cnt,head[N];ll a[N];
int dep[N],fa[N],sz[N],son[N],top[N],dfn[N],ind;
ll sum[N];//sum[u]表示以u为根的子树中苹果个数总和
ll ssum[N];//ssum[u]表示u的轻儿子的sum和
ll ans[N];//ans[u]表示u的轻儿子的不合法方案数两倍总和 
ll c[N];//c[i]表示dfn序在[1,i]的点现持有的苹果个数总和 
ll S,res;
void add(int u,int v){
	edge[++cnt].v=v;
	edge[cnt].nxt=head[u];
	head[u]=cnt;
}
void dfs1(int u,int f){
	dep[u]=dep[f]+1;
	fa[u]=f;
	sz[u]=1;
	sum[u]=a[u];
	for(int i=head[u];i;i=edge[i].nxt){
		int v=edge[i].v;
		if(v==f) continue;
		dfs1(v,u);
		sz[u]+=sz[v];
		sum[u]=(sum[u]+sum[v])%mod;
		if(sz[v]>sz[son[u]]) son[u]=v;
	}
}
void dfs2(int u,int tp){
	if(u==tp){//说明u不是fa[u]的轻儿子 
		ssum[fa[u]]=(ssum[fa[u]]+sum[u])%mod;
		ans[fa[u]]=(ans[fa[u]]+sum[u]*(sum[u]+mod-1)%mod)%mod;
	}
	dfn[u]=++ind;
	top[u]=tp;
	if(!son[u]) return;
	dfs2(son[u],tp);
	for(int i=head[u];i;i=edge[i].nxt){
		int v=edge[i].v;
		if(v!=fa[u]&&v!=son[u]) dfs2(v,v);
	}
}
int lowbit(int x){
	return x&(-x);
}
void add(int x,ll y){
	for(int i=x;i<=n;i+=lowbit(i)) c[i]=(c[i]+y)%mod;
}
ll query(int x){
	ll res=0;
	for(int i=x;i;i-=lowbit(i)) res=(res+c[i])%mod;
	return res;
}
void update(int x,ll y){//只维护轻儿子的影响 
	for(x=top[x];x;x=top[fa[x]]){
		ans[fa[x]]=(ans[fa[x]]+mod-sum[x]*(sum[x]+mod-1)%mod)%mod;
		sum[x]=(sum[x]+y)%mod;
		ssum[fa[x]]=(ssum[fa[x]]+y)%mod;
		ans[fa[x]]=(ans[fa[x]]+sum[x]*(sum[x]+mod-1)%mod)%mod;
	}
}
int main(){
	//freopen("slayer2.in","r",stdin);
	//freopen("slayer2.out","w",stdout);
	scanf("%d%d",&n,&m);
	for(int i=1;i<=n;i++)
		scanf("%lld",&a[i]),S=(S+a[i])%mod;
	for(int i=1;i<n;i++){
		int u,v;
		scanf("%d%d",&u,&v);
		add(u,v);add(v,u);
	}
	dfs1(1,0);
	dfs2(1,1);
	for(int i=1;i<=n;i++) add(dfn[i],a[i]);
	int opt,x;ll y;
	for(int o=1;o<=m;o++){
		scanf("%d",&opt);
		if(opt==1){
			scanf("%d%lld",&x,&y);
			S=(S+y)%mod;
			a[x]=(a[x]+y)%mod;
			add(dfn[x],y);
			update(x,y);
		}
		else if(opt==2){
			scanf("%d",&x);
			res=S*(S+mod-1)%mod;
			res=(res+mod-ans[x])%mod;
			ll h=son[x]?(query(dfn[son[x]]+sz[son[x]]-1)+mod-query(dfn[x]))%mod:0;
			if(son[x]) res=(res+mod-h*(h+mod-1)%mod);
			h=((h+ssum[x])%mod+a[x])%mod;
			h=(S+mod-h)%mod;
			res=(res+mod-h*(h+mod-1)%mod)%mod;
			res=res*div2%mod;
			printf("%lld\n",res);
		}
	}
	return 0;
}
/*
5 6
2 2 2 2 2
1 2
1 3
2 4
2 5
2 2
2 1
1 2 3
2 2
1 4 1
2 2
*/
