#include<iostream>
#include<cstdio>
#include<vector>
using namespace std;
const int N=5005;
struct node{
    int sm,mx;
    //sm:维护区间和 mx:维护区间最大前缀和 
}t[N*40];
int n,sum;
int lf[N][N];
char ch[N][N],ans[N][N];
vector<int> h[N];
void build(int u,int l,int r){
    if(l==r){
    	t[u].sm=t[u].mx=1;
    	return;
	}
    int mid=(l+r)/2;
    build(u<<1,l,mid);
    build(u<<1|1,mid+1,r);
    t[u].sm=t[u<<1].sm+t[u<<1|1].sm;
    t[u].mx=max(t[u<<1].mx,t[u<<1].sm+t[u<<1|1].mx);
}
void update(int u,int l,int r,int x,int c){
    if(l==r){
    	t[u].sm+=c;
		t[u].mx+=c;
    	return; 
	}
    int mid=(l+r)/2;
    if(x<=mid) update(u<<1,l,mid,x,c);
    else update(u<<1|1,mid+1,r,x,c);
    t[u].sm=t[u<<1].sm+t[u<<1|1].sm;
    t[u].mx=max(t[u<<1].mx,t[u<<1].sm+t[u<<1|1].mx);
}
int query(int u,int l,int r,int x){
    if(l==r) return l+1;
    int mid=(l+r)/2;
    if(t[u<<1].mx>=x) return query(u<<1,l,mid,x);
    else return query(u<<1|1,mid+1,r,x-t[u<<1].sm);
}
bool getleft(int i,int j){
    for(int k=0;k<h[i-1].size();k++){
    	int x=h[i-1][k];
        if(x>j-i+1) break;
        update(1,1,n,x,-1);
        sum++;
    }
    if(sum>j-i+1) return false;
    if(sum==j-i+1) lf[j][j-i+1]=1;
    else{
        if(t[1].mx<j-i+1-sum) lf[j][j-i+1]=j-i+2;
        else lf[j][j-i+1]=query(1,1,n,j-i+1-sum);
    }
    return true;
}
int main(){
    scanf("%d",&n);
    for(int i=1;i<=n;i++){
    	scanf("%s",ch[i]+1);
    	for(int j=1;j<=i;j++){
    		if(ch[i][j]=='0'){
    			h[i-j].push_back(j);
				ans[i][j]='-';	
			}
		}
	}
    bool flag=0;
    for(int i=n;i>=2;i--){
        vector<int> a(0);
        for(int j=1;j<=i;j++) 
			if(ch[i][j]=='0') a.push_back(j);
        if(!a.size()){
			flag=1;break;
		}
        for(int j=1;j<a[0];j++) ans[i][j]='3';
        for(int j=a[a.size()-1]+1;j<=i;j++) ans[i][j]='1';
        build(1,1,n);//初始化
        sum=0;//存被删小三角形个数 
        for(int j=1;j<a[0];j++){
            if(!getleft(i-j,i-1)){
             	flag=1;break;
            }
        }
        if(flag) break;
        for(int j=1;j<a.size();j++){
            int p1=a[j-1],p2=a[j],pos=p1;
            for(int k=p1;k<p2;k++){
                if(!getleft(i-k,i-1)){
                    flag=1;break;
                }
                if(lf[i-1][k]<=p1) pos=k+1;
            }
            if(flag) break;
            if(pos==p2){
                flag=1;break;
            }
            ch[i-1][pos]='0';
            ans[i-1][pos]='2';
            update(1,1,n,pos,-1);
            sum++;
            for(int k=p1+1;k<=pos;k++) ans[i][k]='1';
            for(int k=pos+1;k<p2;k++) ans[i][k]='3';
        }
        if(flag) break;
    }
    if(flag){
        printf("Impossible!\n");
        return 0;
    }
    for(int i=1;i<=n;i++){
        for(int j=1;j<=i;j++) 
			putchar(ans[i][j]);
        puts("");
    }
    return 0;
}
