#include<bits/stdc++.h>
using namespace std;
int n,a[1050],b[1050];
char str[1050];
bitset<1024> A,B;
int getstr(int x,int y,bool op){//op=1:��ǰҪɾ1	op=0:��ǰҪɾ0 
    if(!y){
        for(int i=0;i<x;i++) a[i]=op;
        return x;
    }
    int t=x/y;
    int d=getstr(y,x-y*t,op^1);
    int cnt=0;
    for(int i=0;i<x+y-y*t;i++){
        if(a[i]!=op){//a[i]����Ҫɾ�ģ�˵�����ǵ�ǰ�����μ�ķֽ� 
            for(int j=1;j<=t;j++) b[cnt++]=op;
        }
        b[cnt++]=a[i];
    }
    for(int i=0;i<cnt;i++) a[i]=b[i];
    return d;//����ѭ���ڸ��� 
}
int solve(int d){//d=ѭ���ڳ��� 
    bitset<1024> cur0,cur1;
    for(int i=0;i<n;i++){
        if(!a[i]) cur0[i]=1;
        else cur1[i]=1;
    }
    int s=0;
    for(int i=0;i<d;i++){
        if((A&cur1).none()&&(B&cur0).none()) s++;
        cur0>>=1;cur1>>=1;
        if(!a[i]) cur0[n-1]=1; 
        else cur1[n-1]=1;//ѭ����λ 
    }
    return s;
}
int main(){
    scanf("%d%s",&n,str);
    for(int i=0;i<n;i++){
        if(str[i]=='0') A[i]=1;
        else if(str[i]=='1') B[i]=1;
    }
    int ans=0;
    for(int i=0;i<=n;i++){//ö�ٳ�ʼ����0�ĸ��� 
        int d;
        if(i<n-i) d=getstr(n-i,i,1);
        else d=getstr(i,n-i,0);
        ans+=solve(n/d);
    }
    printf("%d\n",ans);
    return 0;
}
