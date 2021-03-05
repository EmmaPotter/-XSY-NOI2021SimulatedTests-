#include<iostream>
#include<cstdio>
#include<map>
#include<algorithm>
#include<vector>
#include<queue>
using namespace std;
const int N=3e5+5;
struct data{
    int v,nxt;
}edge[N<<1];
vector<int> vex[N],vid[N];
int n,m,cnt,head[N],ind,dfn[N],sz[N],son[N],fa[N][21];
int low[N],slow[N],g[N][21];
int st[N<<2];
int fl[N],hk[N];
//fl[u]=删掉u后，有几个u的子节点会被与连通图断开 
//hk[v]=删掉fa[v][0]后，v会与连通图断开 
map<int,int> mp[N];
//mp[a][b]=1表示为保持图连通，若删了a，则b不能被删 
bool fla[N],flagg[N];
int rt[N];
priority_queue<int> q[N];
queue<int> que;
void add(int u,int v){
    edge[++cnt].v=v;
    edge[cnt].nxt=head[u];
    head[u]=cnt;
}
void merge(int a,int b){
    int p[11];
    p[1]=low[a];p[2]=slow[a];p[3]=low[b];p[4]=slow[b];
    sort(p+1,p+5);
    low[a]=p[1];
    slow[a]=n+1;
    for(int i=2;i<=4;i++){
        if(p[i]!=p[i-1]){
            slow[a]=p[i];break;
        }
    }
}
inline void dfs(int u){
    ind++;
    dfn[u]=ind;
    for(int i=head[u];i;i=edge[i].nxt){
        if(!dfn[edge[i].v]){
            fa[edge[i].v][0]=u;
            dfs(edge[i].v);
    	}    
    }
}
void dfs_tree(int u){//判low直接与high连通+low通过mid与high连通 
	sz[u]=1;
    low[u]=dfn[u];slow[u]=n+1;
    for(int i=head[u];i;i=edge[i].nxt){
    	int v=edge[i].v;
        if(v==fa[u][0]) continue;
        if(fa[v][0]==u){
            son[u]++;
            dfs_tree(v);
            merge(u,v);
            sz[u]=sz[u]+sz[v];
            if(low[v]>=dfn[u]){
        		fl[u]++;hk[v]++;
    		}
			else if(slow[v]>=dfn[u]) mp[u][low[v]]=1;
        }
		else{
            if(dfn[v]==low[u]) continue;
            if(low[u]>dfn[v]){
                slow[u]=low[u];
                low[u]=dfn[v];
            }else if(slow[u]>dfn[v]) slow[u]=dfn[v];
        }
    }
}
//此时的线段树用于统计low的最小值 
inline void update(int u,int l,int r,int x,int v){
    if(l>x||r<x)return;
    if(l==r){
        st[u]=v;
        return;
    }
    int mid=(l+r)/2;
    update(u<<1,l,mid,x,v);
    update(u<<1|1,mid+1,r,x,v);
    st[u]=min(st[u<<1],st[u<<1|1]);
}
inline int query(int u,int l,int r,int ql,int qr){
    if(l>qr||r<ql)return n+1;
    if(ql<=l&&r<=qr)return st[u];
    int mid=(l+r)/2;
    int res1=query(u<<1,l,mid,ql,qr);
    int res2=query(u<<1|1,mid+1,r,ql,qr);
    return min(res1,res2);
}
//此时的线段树用于维护区间是否可选 
inline void modify(int u,int l,int r,int ql,int qr){
    if(!flagg[u]){
        que.push(u);
        flagg[u]=1;
    }
    if(l>qr||r<ql)return;
    if(ql<=l&&r<=qr){
        st[u]=1;
        return;
    }
    int mid=(l+r)/2;
    modify(u<<1,l,mid,ql,qr);
    modify(u<<1|1,mid+1,r,ql,qr);
    st[u]=st[u<<1]&st[u<<1|1];
}
inline bool ask(int u,int l,int r,int x){
    if(l>x||r<x) return 0;
    if(st[u]) return st[u];
    if(l==r) return st[u];
    int mid=(l+r)/2;
    return ask(u<<1,l,mid,x)|ask(u<<1|1,mid+1,r,x);
}
inline void get_g(int u){//判mid不通过low直接与high连通  
    for(int i=1;i<=20;i++){
        fa[u][i]=fa[fa[u][i-1]][i-1];
        g[u][i]=min(g[u][i-1],g[fa[u][i-1]][i-1]);
    }
    int sum=0,tot=0,s=n+1;
    for(int i=head[u];i;i=edge[i].nxt){
    	int v=edge[i].v;
        if(v==fa[u][0]) continue;
        if(fa[v][0]==u) sum++;
        else s=min(s,dfn[v]);
    }
    for(int i=head[u];i;i=edge[i].nxt){
    	int v=edge[i].v;
        if(fa[v][0]==u){
            tot++;
            update(1,1,sum,tot,low[v]);
        }
    }
    tot=0;
    for(int i=head[u];i;i=edge[i].nxt){
    	int v=edge[i].v;
        if(fa[v][0]==u){
            tot++;
            g[v][0]=min(min(query(1,1,sum,1,tot-1),query(1,1,sum,tot+1,sum)),s);
        }
    }
    for(int i=head[u];i;i=edge[i].nxt){
    	int v=edge[i].v;
        if(fa[v][0]==u) get_g(v);
    }
}
int solve(int u,int v){
    int s=n+1;
    for(int i=20;i>=0;i--){
        if(dfn[fa[v][i]]>dfn[u]){
            s=min(s,g[v][i]);
            v=fa[v][i];
        }
    }
    return s;
}
int mergeq(int a,int b){
    if(q[a].size()<q[b].size()) swap(a,b);
    while(!q[b].empty()){
        q[a].push(q[b].top());
        q[b].pop();
    }
    return a;
}
inline void getans(int u){//判mid通过low与high连通 
    for(int i=head[u];i;i=edge[i].nxt){
    	int v=edge[i].v;
    	if(fa[v][0]==u) getans(v);
	}	
    rt[u]=u;
    for(int i=head[u];i;i=edge[i].nxt){
    	int v=edge[i].v;
        if(v==fa[u][0])continue;
        if(fa[v][0]==u){
            int l=low[v],r=0;
            while(!q[rt[v]].empty()){
                if(q[rt[v]].top()>=dfn[u]) q[rt[v]].pop();
                else{
                    r=q[rt[v]].top();
                    break;
                }
            }
            modify(1,1,n,l+1,r-1);
			//对于非树边(u,y)（dfn[y]<dfn[u]）,若dfn[y]在[l+1,r-1]的区间内，则删去此边后(u,y)边的mid部分仍与high连通 
            rt[u]=mergeq(rt[v],rt[u]);
        }
		else{
            q[rt[u]].push(dfn[v]);
        }
    }
    for(int i=0;i<vex[u].size();i++){
        int x=u;
        int y=vex[u][i];
        int z=vid[u][i];
        if(dfn[y]>dfn[x])continue;
        if(x==1||y==1)continue;
        if(y==fa[x][0])continue;
        if(!ask(1,1,n,dfn[y]))fla[z]=1;
    }
    while(!que.empty()){
        flagg[que.front()]=0;
        st[que.front()]=0;
        que.pop();
    }//恢复 
}
char check(int id,int a,int b){
    if(dfn[a]>dfn[b]) swap(a,b);
    if(a==1){
        if(fa[b][0]==a){
            if(son[a]>2) return '1';
            if(son[a]>1&&sz[b]>1) return '1';
            if(son[b]>1) return '1';
            return '0';
        }
        if(son[a]>1) return '1';
        if(mp[b][dfn[a]]==0&&fl[b]==0) return '0';
        return '1';
    }
    if(fa[b][0]==a){
        if(sz[b]==1&&hk[b]==1){
            if(fl[a]<=1&&fl[b]==0&&mp[b][dfn[a]]==0) return '0';
        }
		else{
            if(fl[a]==0&&fl[b]==0&&mp[b][dfn[a]]==0) return '0';
        }
        return '1';
    }
    if(fl[a]||fl[b]||mp[b][dfn[a]]) return '1';
    int s=solve(a,b);
    if(s>=dfn[a]&&fla[id]) return '1';
    return '0';
}
int main(){
    scanf("%d%d",&n,&m);
    for(int i=1;i<=m;i++){
    	int x,y;
        scanf("%d%d",&x,&y);
        add(x,y);add(y,x);
        vex[x].push_back(y);
        vex[y].push_back(x);
        vid[x].push_back(i);
        vid[y].push_back(i);
    }
    dfs(1);
    dfs_tree(1);
    get_g(1);
    for(int i=1;i<=n*4;i++) st[i]=0;
    getans(1);
    for(int i=1;i<=m;i++)
        putchar(check(i,edge[i*2-1].v,edge[i*2].v));
}
