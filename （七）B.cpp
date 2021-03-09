#include<iostream>
#include<cstdio>
#include<cstring>
#include<vector>
using namespace std;
const int N=2e5+5;
struct SAM{
	int tot,lst,ch[N<<1][26],len[N<<1],fa[N<<1],pos[N<<1];//fa:parent tree上的fa
	int tax[N<<1],id[N<<1];
	SAM(){
		tot=lst=1;
	}
	void add(int c,int id){
		int p=lst,np=lst=++tot;
    	len[np]=len[p]+1;
		pos[np]=id;
    	for(;p&&!ch[p][c];p=fa[p])//在这里,p=fa(p)即从长到短遍历旧串的所有后缀。
        	ch[p][c]=np;//旧串后缀加c构成新串后缀 
    	if(!p) fa[np]=1;
    	else{
        	int q=ch[p][c];
        	if(len[q]==len[p]+1) fa[np]=q;//q中原有的子串同时也是新串的后缀 
        	else{
            	int nq=++tot;
            	for(int i=0;i<26;i++) ch[nq][i]=ch[q][i];
            	fa[nq]=fa[q];
				len[nq]=len[p]+1;
				pos[nq]=pos[q];
            	fa[q]=fa[np]=nq;
            	for(;p&&ch[p][c]==q;p=fa[p]) ch[p][c]=nq;
            	//把是新串后缀但不是旧串后缀的子串放到新建节点nq上 
        	}
    	}
	}
	void sort(){
		for(int i=1;i<=tot;i++) tax[len[i]]++;
    	for(int i=1;i<=tot;i++) tax[i]+=tax[i-1];
    	for(int i=tot;i>=1;i--) id[tax[len[i]]--]=i;//基数排序: len为第一关键字,for(i=tot;i>=1;i--)时保到小证fa[id[i]]在id[i]后遍历到 
	}
}pre,suf;
char s[N],t[N];
int n,m,tag[N<<1],ans; 
struct Qry{
	int length,p;
};
vector<Qry> pf[N],sf[N];
int c[N];
int lowbit(int x){
	return x&(-x);
}
void update(int x,int v){
	for(int i=x;i<=n;i+=lowbit(i)) c[i]=max(c[i],v);
}
int query(int x){
	int res=0;
	for(int i=x;i;i-=lowbit(i)) res=max(res,c[i]);
	return res;
}
int main(){
	scanf("%s%s",s+1,t+1);
	n=strlen(s+1);m=strlen(t+1);
	for(int i=1;i<=n;i++) pre.add(s[i]-'a',i);
	for(int i=n;i>=1;i--) suf.add(s[i]-'a',n-i+1);
	pre.sort();suf.sort();
	
	for(int i=1,u=1,len=0;i<=m;i++){//正序t 在正序s 的SAM上做匹配(求s,t前缀的最长公共子串) 
		int c=t[i]-'a';
		while(u&&!pre.ch[u][c]) u=pre.fa[u];
		if(!u){u=1;len=0;}
		else{
			len=min(len,pre.len[u])+1;//注意写法 
			u=pre.ch[u][c];
		}
		if(u>1&&!tag[pre.fa[u]]) tag[pre.fa[u]]=i;//节点u的祖先中s的子串,同时也都是t的长度为i的前缀的后缀。
		pf[i].push_back((Qry){len,pre.pos[u]});
	}
	for(int i=pre.tot;i>=1;i--){
		int u=pre.id[i];
		if(!tag[u]) continue;
		if(!tag[pre.fa[u]]||tag[pre.fa[u]]>tag[u]) tag[pre.fa[u]]=tag[u];
		pf[tag[u]].push_back((Qry){pre.len[u],pre.pos[u]});
	} 
	memset(tag,0,sizeof(tag));
	for(int i=m,u=1,len=0;i>=1;i--){//逆序t 在逆序s 的SAM上做匹配(求s,t后缀的最长公共子串) 
		int c=t[i]-'a';
		while(u&&!suf.ch[u][c]) u=suf.fa[u];
		if(!u){u=1;len=0;}
		else{
			len=min(len,suf.len[u])+1;
			u=suf.ch[u][c];
		}
		if(u>1&&!tag[suf.fa[u]]) tag[suf.fa[u]]=i;
		sf[i].push_back((Qry){len,suf.pos[u]});
	} 
	for(int i=suf.tot;i>=1;i--){
		int u=suf.id[i];
		if(!tag[u]) continue;
		if(!tag[suf.fa[u]]||tag[suf.fa[u]]<tag[u]) tag[suf.fa[u]]=tag[u];
		sf[tag[u]].push_back((Qry){suf.len[u],suf.pos[u]});
	}
	ans=0;
	for(int i=m;i>=1;i--){
		for(int j=0;j<pf[i].size();j++){
			Qry q=pf[i][j];
			ans=max(ans,q.length+query(n-q.p));
		}
		for(int j=0;j<sf[i].size();j++){
			Qry q=sf[i][j];
			if(q.p) update(q.p,q.length);
		}
	}
	printf("%d\n",ans);
	return 0;
}
