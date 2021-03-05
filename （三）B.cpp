#include<iostream>
#include<cstdio>
#include<vector>
using namespace std;
const int N=1e5+5;
const int mod=998244353;
int len,rev[N<<1],inv[N<<1],w[20][N<<1],iw[20][N<<1];
int n,m,rt; 
int fl[N<<1],fr[N<<1],pl[N<<1],pr[N<<1],sl[N<<1],sr[N<<1];
int nf[N<<1],np[N<<1],ns[N<<1];
vector<int> f[N<<2],p[N<<2],s[N<<2];
int add(int a,int b){
	a=a+b;
	if(a>mod) return a-mod;
    return a;
}
int dec(int a,int b){
	a=a-b;
	if(a<0) return a+mod;
	return a;
}
int mul(int a,int b){
    return 1ll*a*b%mod;
}
int power(int a,int b){
    int res=1;
    while(b){   
        if(b&1){
            res=mul(res,a);
        }
        b>>=1;
        a=mul(a,a);
    }
    return res;
}
void init(int len){
    inv[1]=1;
    for(int i=2;i<=len;i++) 
        inv[i]=mul(inv[mod%i],dec(mod,mod/i));
    for(int i=1,t=0;i<len;i<<=1,t++){
        int wn=power(3,(mod-1)/i/2),iwn=power(wn,mod-2);
        for(int j=0;j<len;j+=(i<<1)){
            int w=1,iw=1;
            for(int k=j;k<j+i;k++,w=mul(w,wn),iw=mul(iw,iwn)){
                ::w[t][k]=w;
                ::iw[t][k]=iw;
            }
        }
    }
}
void NTT(int a[],int len,int op){//op=0:系数转点值,op=1:点值转系数 
    for(int i=0;i<len;i++){
        rev[i]=(rev[i>>1]>>1)|((i&1)*(len>>1));
        if(i<rev[i])
            swap(a[i],a[rev[i]]);
    }
    for(int i=1,t=0;i<len;i<<=1,t++){
        for(int j=0;j<len;j+=(i<<1)){
            int x,y;
            for(int k=j;k<j+i;k++){
                x=a[k];
                if(!op) y=mul(w[t][k],a[k+i]);
                else if(op) y=mul(iw[t][k],a[k+i]);
                a[k]=add(x,y);
                a[k+i]=dec(x,y);
            }
        }
    }
    if(op){
        for(int i=0;i<len;i++)
            a[i]=mul(a[i],inv[len]);
    } 
}
void solve(int u,int l,int r){
	if(l==r){
		if(l<n-1){
			int a=(l+1)+((l+1)>=rt);
			f[u].push_back(dec(1,a));
			f[u].push_back(dec(a,n+1));  
		}
		else{
			f[u].push_back(1);
			f[u].push_back(0);  
		}
		int a=(l-1)+((l-1)>=rt);
		p[u].push_back(dec(0,a));
		p[u].push_back(dec(a,n));
		s[u].push_back(mul(f[u][0],p[u][0]));
		s[u].push_back(add(mul(f[u][1],p[u][0]),mul(f[u][0],p[u][1])));
		s[u].push_back(mul(f[u][1],p[u][1]));
		return;
	}
	int mid=(l+r)>>1,ls=u<<1,rs=u<<1|1;
	solve(ls,l,mid);
	solve(rs,mid+1,r);
    for(int i=0,sz=f[ls].size();i<sz;i++) fl[i]=f[ls][i];
	for(int i=0,sz=f[rs].size();i<sz;i++) fr[i]=f[rs][i];
	for(int i=0,sz=p[ls].size();i<sz;i++) pl[i]=p[ls][i];
	for(int i=0,sz=p[rs].size();i<sz;i++) pr[i]=p[rs][i];    
    for(int i=0,sz=s[ls].size();i<sz;i++) sl[i]=s[ls][i];
	for(int i=0,sz=s[rs].size();i<sz;i++) sr[i]=s[rs][i];
	int len=1; 
   	while(len<=(r-l+2)) len<<=1;
	NTT(fl,len,0);NTT(fr,len,0);NTT(pl,len,0);NTT(pr,len,0);NTT(sl,len,0);NTT(sr,len,0);
	for(int i=0;i<len;i++){
		nf[i]=mul(fl[i],fr[i]);
		np[i]=mul(pl[i],pr[i]);
		ns[i]=add(mul(sl[i],fr[i]),mul(pl[i],sr[i]));
	}
	NTT(nf,len,1);NTT(np,len,1);NTT(ns,len,1);
	for(int i=0;i<=(r-l+1);i++) f[u].push_back(nf[i]);
	for(int i=0;i<=(r-l+1);i++) p[u].push_back(np[i]);
	for(int i=0;i<=(r-l+2);i++) s[u].push_back(ns[i]);
	for(int i=0;i<len;i++)
        fl[i]=fr[i]=pl[i]=pr[i]=sl[i]=sr[i]=nf[i]=np[i]=ns[i]=0;
}
int main(){
	scanf("%d%d%d",&n,&rt,&m);
	if(n==1){
		printf("%d\n",1);
		return 0;
	}
	int len=1;
	while(len<=n) len<<=1;
    init(len);
    solve(1,2,n);
    if(2<n-1){
		int a=2+(2>=rt);
		fl[0]=dec(1,a),fl[1]=dec(a,n+1);
	}
	else fl[0]=1;
	for(int i=0,size=f[1].size();i<size;i++) fr[i]=f[1][i];
	NTT(fl,len,0);NTT(fr,len,0);
	for(int i=0;i<len;i++)
		fl[i]=mul(fl[i],fr[i]);
	NTT(fl,len,1);
	int ans=add(m<n-1?fl[m]:0,s[1][m]);
	printf("%d\n",(n&1)?ans:dec(0,ans));
	return 0;
}
