#include<iostream>
#include<cstdio>
#include<cstring>
#define mod 998244353
using namespace std;
typedef pair<int,int> pr;
const int N=30001;
const int K=5;
const int S=256;
int n,m,ans,dp[N][K][S],f[31],g[31];
pr p[K][S][S<<1];
char ch[N];
bool flag[31];
int add(int a,int b){
	a=a+b;
	if(a>mod) a-=mod;
	return a;
} 
int mul(int a,int b){
	return 1ll*a*b%mod;
}
void dfs(int a,int b,int c){
	if(dp[a][b][c]==0) return;
	int l=max(a-m,0),r=min(a+m,n-1),sz=26;
	for(int i=l;i<=r;i++){
		if(!flag[ch[i+1]-'A']){
			flag[ch[i+1]-'A']=1;
			sz--;
		}
		f[ch[i+1]-'A']|=(1<<(i-a+m));//统计每种字符的匹配情况 
	}
	for(int i=l;i<=r;i++){
        if(flag[ch[i+1]-'A']){
            int x=p[b][c][f[ch[i+1]-'A']].first;
			int y=p[b][c][f[ch[i+1]-'A']].second;
            if(x>=0) dp[a+1][x][y]=add(dp[a+1][x][y],dp[a][b][c]);
            flag[ch[i+1]-'A']=0;
            f[ch[i+1]-'A']=0;
        }
    }
    int x=p[b][c][0].first,y=p[b][c][0].second;
    if(x>=0) dp[a+1][x][y]=add(dp[a+1][x][y],mul(dp[a][b][c],sz));
}
int main(){
	scanf("%s%d",ch+1,&m);
	n=strlen(ch+1);
	//省略一维l，表示考虑到第l位 
	for(int i=0;i<=m;i++){//lcs[l][l]-l+m==i 
		for(int j=0;j<(1<<(2*m));j++){//lcs[l-m][l]~lcs[l+m][l]的差分数组状压 
			for(int k=0;k<(1<<(2*m+1));k++){//T[l+1]与S[l+1-m]~S[l+1+m]匹配情况状压 
				//f:(lcs[l-m][l]-l+m)~(lcs[l+m][l]-l+m) 
				//g:(lcs[l+1-m][l+1]-l+m)~(lcs[l+1+m][l+1]-l+m)
				f[m]=i;
				for(int a=m-1;a>=0;a--) f[a]=f[a+1]-(j>>a&1);
				for(int a=m;a<2*m;a++) f[a+1]=f[a]+(j>>a&1);
				for(int a=0;a<=2*m;a++){
                    if((k>>a)&1) g[a+1]=max(f[a]+1,f[a+1]);//匹配
                    else g[a+1]=max(f[a],f[a+1]); //不匹配 
                    g[a+1]=max(g[a],g[a+1]);
                }
                int x=g[m+1]-1;//x=lcs[l+1][l+1]-(l+1)+m
                int y=0;
                if(x>=0){
                    for(int a=m-1;a>=0;a--) 
						if(g[a+1]^g[a+2]) y|=1<<a;
                    for(int a=m;a<2*m;a++) 
						if(g[a+1]^g[a+2]) y|=1<<a;
                }
                p[i][j][k]=make_pair(x,y);
			}
		}
	}//预处理 
	for(int i=0;i<=30;i++) f[i]=0;
	dp[0][m][0]=1;
	for(int i=0;i<n;i++){//考虑到第i位 
        for(int j=0;j<=m;j++){//lcs[i][i]-i+m==j 
            for(int k=0;k<(1<<(2*m));k++){//差分数组状压 
                dfs(i,j,k);
            }
        }
    }//dp
    for(int i=0;i<=m;i++){
        for(int j=0;j<(1<<(2*m));j++){
            ans=add(ans,dp[n][i][j]);
        }
    }
    printf("%d\n",ans);
	return 0;
}
