#include<iostream>
#include<cstdio>
#include<algorithm>
using namespace std;
struct Que{
    int x,id;
}que[200010];
int n,m,q,len;
char ans[200010][31],ch[31];
bool cmp(Que a,Que b){
    return a.x<b.x;
}
int main(){
    scanf("%d%d%d",&n,&m,&q);
    for(int i=1;i<=q;i++){
        scanf("%d",&que[i].x);
        que[i].id=i;
    }
    sort(que+1,que+q+1,cmp);
    ch[len=1]='a';
    int p=1;
    for(int i=1;i<=q;i++){
        while(p<que[i].x){
            for(int j=len+1;j<=n;j++) ch[j]=ch[j-len];
            len=n;
            while(ch[len]=='a'+m-1) ch[len--]=0;
            ch[len]++;
            p++;            
        }
        for(int j=1;j<=len;j++) ans[que[i].id][j]=ch[j];
    }
    for(int i=1;i<=q;i++){
        for(int j=1;ans[i][j];j++) putchar(ans[i][j]);
        puts("");
    }
    return 0;
}
