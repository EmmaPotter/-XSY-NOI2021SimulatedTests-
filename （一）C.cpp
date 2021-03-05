#include<iostream>
#include<cstdio>
#include<vector>
using namespace std;
int read(){
	int x=0,f=1;
	char ch=getchar();
	while(ch<'0'||ch>'9'){if(ch=='-')f=-1;ch=getchar();}
	while(ch>='0'&&ch<='9'){x=x*10+ch-'0';ch=getchar();}
	return x*f;
}
const int N=270005,mod=998244353,inv2=(mod+1)>>1;
int n,m,len,a[N],A[N],B[N],C[N],D[N],f[N],rev[N],inv[N],fac[N],ifac[N],w[20][N],iw[20][N];
//fac[i]=1*2*...*i;
//ifac[i]=1/(1*2*...*i);
//inv[i]=1/i;
int add(int a,int b){
	a=a+b;
	if(a>mod) return a-mod;
    return a;
    //return (a+b)%mod;(会WA) 
}
int sub(int a,int b){
	a=a-b;
	if(a<0) return a+mod;
	return a;
	//return (a-b+mod)%mod;(会WA)
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
        inv[i]=mul(inv[mod%i],sub(mod,mod/i));
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
                a[k+i]=sub(x,y);
            }
        }
    }
    if(op){
        for(int i=0;i<len;i++)
            a[i]=mul(a[i],inv[len]);
    } 
}
void getinv(int a[],int ans[],int len){
    static int b[N],c[N],d[N];
    for(int i=0;i<(len<<1);i++)
        b[i]=c[i]=d[i]=0;
    b[0]=power(a[0],mod-2);
    for(int i=2;i<=len;i<<=1){
        for(int j=0;j<i;j++){
            c[j]=b[j];
            d[j]=a[j];
        }
        NTT(c,i<<1,0);
        NTT(d,i<<1,0);
        for(int j=0;j<(i<<1);j++)
            c[j]=mul(c[j],sub(2,mul(c[j],d[j])));
        NTT(c,i<<1,1);
        for(int j=0;j<i;j++) b[j]=c[j];
    }
    for(int i=0;i<len;i++) ans[i]=b[i];
}
void derivative(int a[],int ans[],int len){
    static int b[N];
    for(int i=0;i<len-1;i++)
        b[i]=mul(i+1,a[i+1]);
    b[len-1]=0;
    for(int i=0;i<len;i++) ans[i]=b[i];
}
void integral(int a[],int ans[],int len){
    static int b[N];
    b[0]=0;
    for(int i=1;i<len;i++)
        b[i]=mul(a[i-1],inv[i]);
    for(int i=0;i<len;i++) ans[i]=b[i];
}
void getln(int a[],int ans[],int len){
    static int b[N],c[N];
    for(int i=0;i<(len<<1);i++)
        b[i]=c[i]=i<len?a[i]:0;
    derivative(b,b,len);
    getinv(c,c,len);
    NTT(b,len<<1,0);
    NTT(c,len<<1,0);
    for(int i=0;i<(len<<1);i++)
        b[i]=mul(b[i],c[i]);
    NTT(b,len<<1,1);
    integral(b,ans,len);
}
void getexp(int a[],int ans[],int len){
    static int b[N],c[N],d[N];
    for(int i=0;i<(len<<1);i++)
        b[i]=c[i]=d[i]=0;
    b[0]=1;
    for(int i=2;i<=len;i<<=1){
        for(int j=0;j<i;j++)
            c[j]=d[j]=b[j];
        getln(c,c,i);
        for(int j=0;j<i;j++)
            c[j]=sub(a[j]+(j==0),c[j]);
        NTT(c,i<<1,0);
        NTT(d,i<<1,0);
        for(int j=0;j<(i<<1);j++)
            c[j]=mul(c[j],d[j]);
        NTT(c,i<<1,1);
        for(int j=0;j<i;j++) b[j]=c[j];
    }
    for(int i=0;i<len;i++) ans[i]=b[i];
}
vector<int> h1[N*2],h2[N*2];
void solve(int u,int l,int r){
    if(l==r){
        h1[u].push_back(1);
        h2[u].push_back(1);
        h2[u].push_back(mod-a[l]);
        return;
    }
    int mid=(l+r)/2,ls=u*2,rs=u*2+1;
    solve(ls,l,mid);
    solve(rs,mid+1,r);
    int len; 
    for(len=1;len<=r-l+5;len<<=1);
    for(int i=0;i<len;i++)
        A[i]=B[i]=C[i]=D[i]=0;
    for(int i=0;i<h1[ls].size();i++)
        A[i]=h1[ls][i];
    for(int i=0;i<h2[ls].size();i++)
        B[i]=h2[ls][i];
    for(int i=0;i<h1[rs].size();i++)
        C[i]=h1[rs][i];
    for(int i=0;i<h2[rs].size();i++)
        D[i]=h2[rs][i];
    NTT(A,len,0);
    NTT(B,len,0);
    NTT(C,len,0);
    NTT(D,len,0);
    for(int i=0;i<len;i++){
        A[i]=add(mul(A[i],D[i]),mul(B[i],C[i]));
        B[i]=mul(B[i],D[i]);
    }
    NTT(A,len,1);
    NTT(B,len,1);
    h1[u].resize(r-l+1);
    for(int i=0;i<h1[u].size();i++)
        h1[u][i]=A[i];
    h2[u].resize(r-l+2);
    for(int i=0;i<h2[u].size();i++)
        h2[u][i]=B[i];
}
int main(){
    n=read();m=read();
    for(int i=1;i<=n;i++) a[i]=read();
    fac[0]=1;
    for(int i=1;i<=m+1;i++)
        fac[i]=mul(fac[i-1],i);
    ifac[m+1]=power(fac[m+1],mod-2);
    for(int i=m+1;i>=1;i--)
        ifac[i-1]=mul(ifac[i],i);
    for(len=1;len<=max(n,m);len<<=1);
    init(len<<1);
     
    for(int i=0;i<=m;i++)
        f[i]=ifac[i+1];
    getln(f,f,len);
     
    solve(1,1,n);
    for(int i=0;i<(len<<1);i++)
        A[i]=B[i]=0;
    for(int i=0;i<h1[1].size();i++)
        A[i]=h1[1][i];
    for(int i=0;i<h2[1].size();i++)
        B[i]=h2[1][i];
    getinv(B,B,len);
    NTT(A,len<<1,0);
    NTT(B,len<<1,0); 
    for(int i=0;i<(len<<1);i++)
        A[i]=mul(A[i],B[i]);
    NTT(A,len<<1,1);
     
    for(int i=0;i<=m;i++)
        f[i]=mul(f[i],A[i]);
     
    getexp(f,f,len);
    for(int i=1;i<=m;i++)
        printf("%d ",mul(f[i],fac[i]));
    return 0;
}
