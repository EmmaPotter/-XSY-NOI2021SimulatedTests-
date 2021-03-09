#include<iostream>
#include<cstdio>
#include<cstring>
#include<vector>
using namespace std;
const int N=2e5+5;
struct SAM{
	int tot,lst,ch[N<<1][26],len[N<<1],fa[N<<1],pos[N<<1];//fa:parent tree�ϵ�fa
	int tax[N<<1],id[N<<1];
	SAM(){
		tot=lst=1;
	}
	void add(int c,int id){
		int p=lst,np=lst=++tot;
    	len[np]=len[p]+1;
		pos[np]=id;
    	for(;p&&!ch[p][c];p=fa[p])//������,p=fa(p)���ӳ����̱����ɴ������к�׺��
        	ch[p][c]=np;//�ɴ���׺��c�����´���׺ 
    	if(!p) fa[np]=1;
    	else{
        	int q=ch[p][c];
        	if(len[q]==len[p]+1) fa[np]=q;//q��ԭ�е��Ӵ�ͬʱҲ���´��ĺ�׺ 
        	else{
            	int nq=++tot;
            	for(int i=0;i<26;i++) ch[nq][i]=ch[q][i];
            	fa[nq]=fa[q];
				len[nq]=len[p]+1;
				pos[nq]=pos[q];
            	fa[q]=fa[np]=nq;
            	for(;p&&ch[p][c]==q;p=fa[p]) ch[p][c]=nq;
            	//�����´���׺�����Ǿɴ���׺���Ӵ��ŵ��½��ڵ�nq�� 
        	}
    	}
	}
	void sort(){
		for(int i=1;i<=tot;i++) tax[len[i]]++;
    	for(int i=1;i<=tot;i++) tax[i]+=tax[i-1];
    	for(int i=tot;i>=1;i--) id[tax[len[i]]--]=i;//��������: lenΪ��һ�ؼ���,for(i=tot;i>=1;i--)ʱ����С֤fa[id[i]]��id[i]������� 
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
	
	for(int i=1,u=1,len=0;i<=m;i++){//����t ������s ��SAM����ƥ��(��s,tǰ׺��������Ӵ�) 
		int c=t[i]-'a';
		while(u&&!pre.ch[u][c]) u=pre.fa[u];
		if(!u){u=1;len=0;}
		else{
			len=min(len,pre.len[u])+1;//ע��д�� 
			u=pre.ch[u][c];
		}
		if(u>1&&!tag[pre.fa[u]]) tag[pre.fa[u]]=i;//�ڵ�u��������s���Ӵ�,ͬʱҲ����t�ĳ���Ϊi��ǰ׺�ĺ�׺��
		pf[i].push_back((Qry){len,pre.pos[u]});
	}
	for(int i=pre.tot;i>=1;i--){
		int u=pre.id[i];
		if(!tag[u]) continue;
		if(!tag[pre.fa[u]]||tag[pre.fa[u]]>tag[u]) tag[pre.fa[u]]=tag[u];
		pf[tag[u]].push_back((Qry){pre.len[u],pre.pos[u]});
	} 
	memset(tag,0,sizeof(tag));
	for(int i=m,u=1,len=0;i>=1;i--){//����t ������s ��SAM����ƥ��(��s,t��׺��������Ӵ�) 
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
