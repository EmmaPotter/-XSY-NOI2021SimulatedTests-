#include<iostream>
#include<cstdio>
#include<vector>
#include<cmath>
using namespace std;
int read(){
    int x=0,f=1;
    char ch=getchar();
    while(ch<'0'||ch>'9'){if(ch=='-')f=-1;ch=getchar();}
    while(ch>='0'&&ch<='9'){x=x*10+ch-'0';ch=getchar();}
    return x*f;
}
typedef long long ll;
const int N=1e5+5;
const int bd=1e9;
int n,q,Unit,cnt,L[505],R[505],ans;
ll X1,X2,Y1,Y2,A[N],B[N],C[N];
vector<int> bl[505][505];
int main() {
    n=read();q=read();
    Unit=bd/200;
    for(int i=1;i<=bd;i+=Unit) 
        L[++cnt]=i,R[cnt]=min(bd,i+Unit-1);
    for(int i=1;i<=n;i++){
        X1=read(),Y1=read(),X2=read(),Y2=read();
        A[i]=Y2-Y1;
        B[i]=X1-X2;
        C[i]=X2*Y1-X1*Y2;
        int x=1;ll y1,y2;
        if(B[i]==0){
            x=(X1-1)/Unit+1;
            for(int j=1;j<=cnt;j++) 
                bl[x][j].push_back(i);  
        }
        else{
            for(int j=1;j<=cnt;j++){
                y1=(-C[i]-A[i]*L[j])/B[i];
                y2=(-C[i]-A[i]*R[j]-1)/B[i]+1;
                if(y1>y2) swap(y1,y2);
                y1=max(y1,1ll);y2=min(y2,1ll*bd);//
                y1=(y1-1)/Unit+1;
                y2=(y2-1)/Unit+1;
                for(int k=y1;k<=y2;k++) 
                    bl[j][k].push_back(i);  
            }
        }
    }
    for(int i=1;i<=q;i++){
        int x=read(),y=read(),nx,ny;
        ans=0;
        nx=(x-1)/Unit+1;
        ny=(y-1)/Unit+1;
        for(int j=0;j<bl[nx][ny].size();j++){
            int id=bl[nx][ny][j];
            if(A[id]*x+B[id]*y+C[id]==0) ans++;
        }
        printf("%d\n",ans);
    }
    return 0;
}
