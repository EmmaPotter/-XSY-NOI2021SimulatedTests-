#include<iostream>
#include<cstdio>
#include<algorithm>
#define int long long
using namespace std;
const int N=5e5+5;
const int mod=998244353;
int read(){
    int x=0,f=1;
    char ch=getchar();
    while(ch<'0'||ch>'9'){if(ch=='-')f=-1;ch=getchar();}
    while(ch>='0'&&ch<='9'){x=x*10+ch-'0';ch=getchar();}
    return x*f;
}
int add(int a,int b){return (a+b)%mod;} 
int dec(int a,int b){return ((a-b)%mod+mod)%mod;}
void Add(int &a,int b){a=add(a,b);}
struct Edge{
	int v,w,nxt;
}e[N<<1];
int n,d[N],head[N],cnt=0,ans;
int f[N],g[N];
//f[i]：i子树内三点链的个数
//g[i]：i子树外三点链的个数
int addedge(int u,int v,int w){
	e[++cnt].v=v;e[cnt].w=w;e[cnt].nxt=head[u];head[u]=cnt;
}
void dfs(int u,int ff){
    f[u]=1ll*d[u]*(d[u]-1)/2%mod;
    for(int i=head[u];i;i=e[i].nxt){
    	if(e[i].v!=ff){
    		dfs(e[i].v,u);
            Add(f[u],f[e[i].v]);
		}
	}
}
int to[N],val[N],Sw[N];
int su[N],sv[N];
int pre[N],suf[N];
bool cmp(int a,int b){
	return val[a]<val[b];
}
void work(int u,int ff){
    for(int i=head[u];i;i=e[i].nxt){
    	int v=e[i].v;
    	if(v==ff) continue;
    	work(v,u);
	}
    int tot=0;
    for(int i=head[u];i;i=e[i].nxt){
    	int v=e[i].v;
    	to[++tot]=v;
    	val[v]=e[i].w; 
    	Add(Sw[u],e[i].w);
    	su[v]=v==ff?f[u]:g[v];
    	sv[v]=v==ff?g[u]:f[v];
	}
    sort(to+1,to+tot+1,cmp);
    pre[0]=suf[tot+1]=0; 
    for(int i=1;i<=tot;i++) pre[i]=add(pre[i-1],sv[to[i]]);
    for(int i=tot;i;i--) suf[i]=add(suf[i+1],sv[to[i]]);
    for(int i=1;i<=tot;i++){
        int v=to[i],du=d[u],dv=d[v],w=val[v];
        //处理这条边为相交的三点链做的贡献 
        Add(ans,1ll*(tot-i)*(tot-i-1)*(tot-i-2)/6*9%mod*w%mod);//以u为中心的X型，当前边为第1小边 
        Add(ans,1ll*(i-1)*(tot-i)*(tot-i-1)/2*7%mod*w%mod);//以u为中心的X型，当前边为第2小边 
        Add(ans,1ll*(i-1)*(i-2)/2*(tot-i)*5%mod*w%mod);//以u为中心的X型，当前边为第3小边 
        Add(ans,1ll*(i-1)*(i-2)*(i-3)/6*3%mod*w%mod);//以u为中心的X型，当前边为第4小边 
        Add(ans,1ll*(tot-1)*(tot-2)/2*4%mod*w%mod);//以u为中心的Y型
        if(v!=ff) Add(ans,1ll*(du-1)*(dv-1)*2%mod*w%mod+add(1ll*(Sw[u]-w)*(dv-1)%mod,1ll*(Sw[v]-w)*(du-1)%mod));//以u为中心的Z型 
        //处理这条边为不相交的三点链做的贡献 
        if(v!=ff) Add(ans,1ll*(sv[v]-(dv-1))*(su[v]-(du-1))*4%mod*w%mod);//这条边在树上最短路径中，注意if 
        Add(ans,1ll*dec(su[v],1ll*du*(du-1)/2%mod)*(tot-2)%mod*w%mod); 
        Add(ans,1ll*dec(su[v],pre[i-1]+1ll*du*(du-1)/2%mod)*(tot-i-1)%mod*2*w%mod);
        Add(ans,1ll*dec(su[v],suf[i+1]+1ll*du*(du-1)/2%mod)*(tot-i)%mod*2*w%mod);//树上最短路径连中点u
		Add(ans,(1ll*(tot-1)*3*w%mod+1ll*(Sw[u]-w))*(sv[v]-(dv-1))%mod);//树上最短路径连端点v 
    }
}
signed main(){
    n=read();
    for(int i=1;i<n;i++){
    	int u=read(),v=read(),w=read();
    	addedge(u,v,w);
    	addedge(v,u,w);
    	d[u]++;d[v]++;
	}
    dfs(1,0);
    for(int i=1;i<=n;i++) g[i]=f[1]-f[i];
    work(1,0);
    cout<<dec(ans,0)<<endl;
    return 0;
}
