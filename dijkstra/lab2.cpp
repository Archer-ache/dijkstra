//**导航软件--用dijkstra算法求最短路径**
//附加实现内容：稀疏图存储结构
//附加实现内容：堆优化的dijkstra算法
//--*默认为有向图*--
#define INF INT_MAX
#define MAX_VERTEX_NUM  10000
#define FILENAME "test.txt"
#include <iostream>
#include <stdio.h>
#include <queue>
//
using namespace std;
typedef struct{
    int vexs[MAX_VERTEX_NUM];
    int** ADJmatrix;
    int vexnum,arcnum;
}MGraph;//邻接矩阵定义
typedef struct ArcNode{
    int adjvex;
    int weight;
    struct ArcNode* nextarc;
}ArcNode;
typedef struct{
    ArcNode* firstarc;
}VexNode,*ADJList;
typedef struct{
    int vexs[MAX_VERTEX_NUM];
    ADJList vertices;
    int vexnum,arcnum;
}ALGraph;//邻接表定义
void CreateMGraph(MGraph &G){
    FILE* fp;
    G.arcnum=0;
    G.vexnum=0;
    int src,dst,dis;
    int i,j;
    for(i=0;i<MAX_VERTEX_NUM;i++)G.vexs[i]=0;//助教救的
    if(!(fp=fopen(FILENAME,"r")))
    cout<<"cannot open file"<<endl;
    else{
        while(fscanf(fp,"%d %d %d",&src,&dst,&dis)!=EOF){
            if(G.vexs[src]!=1){
                G.vexs[src]=1;
                G.vexnum++;
            }
            if(G.vexs[dst]!=1){
                G.vexs[dst]=1;
                G.vexnum++;
            }
            G.arcnum++;
        }
    }//读取第一次,获取顶点与边的信息
    fclose(fp);
    G.ADJmatrix=(int**)malloc(G.vexnum*sizeof(int*));
    for(i=0;i<G.vexnum;i++){
        G.ADJmatrix[i]=(int*)malloc(G.vexnum*sizeof(int));
        for(j=0;j<G.vexnum;j++){
            if(i==j)G.ADJmatrix[i][j]=0;//认为自身可达自身并且距离为0
            else{
                G.ADJmatrix[i][j]=INF;
            }
        }
    }//初始化邻接矩阵
    if(!(fp=fopen(FILENAME,"r")))
    cout<<"cannot open file"<<endl;
    else{
        while(fscanf(fp,"%d %d %d",&src,&dst,&dis)!=EOF){
            G.ADJmatrix[src][dst]=dis;
        }
    }
    fclose(fp);
}
void CreateALGraph(ALGraph &G){
    FILE* fp;
    G.arcnum=0;
    G.vexnum=0;
    int src,dst,dis;
    int i,j;
    if(!(fp=fopen(FILENAME,"r")))
    cout<<"cannot open file"<<endl;
    else{
        while(fscanf(fp,"%d %d %d",&src,&dst,&dis)!=EOF){
            if(G.vexs[src]!=1){
                G.vexs[src]=1;
                G.vexnum++;
            }
            if(G.vexs[dst]!=1){
                G.vexs[dst]=1;
                G.vexnum++;
            }
            G.arcnum++;
        }
    }
    fclose(fp);
    G.vertices=(ADJList)malloc(G.vexnum*sizeof(VexNode));
    for(i=0;i<G.vexnum;i++){
        G.vertices[i].firstarc=NULL;
    }
    if(!(fp=fopen(FILENAME,"r")))
    cout<<"cannot open file"<<endl;
    else{
        while(fscanf(fp,"%d %d %d",&src,&dst,&dis)!=EOF){
            if(G.vertices[src].firstarc==NULL){
                G.vertices[src].firstarc=(ArcNode*)malloc(sizeof(ArcNode));
                G.vertices[src].firstarc->adjvex=dst;
                G.vertices[src].firstarc->weight=dis;
                G.vertices[src].firstarc->nextarc=NULL;
            }
            else{
                ArcNode* p=(ArcNode*)malloc(sizeof(ArcNode));
                p->adjvex=dst;
                p->weight=dis;
                p->nextarc=G.vertices[src].firstarc;
                G.vertices[src].firstarc=p;
            }
        }
    }
    fclose(fp);
}
void PrintMGraph(MGraph G){
    int i,j;
    cout<<"##邻接矩阵有向图##"<<endl;
    cout<<"顶点数: "<<G.vexnum<<endl;
    cout<<"边数: "<<G.arcnum<<endl;
    cout<<"邻接矩阵: "<<endl;
    for(i=0;i<=G.vexnum;i++){
        for(j=0;j<=G.vexnum;j++){
            if(!i){
                if(!j){
                    cout.width(6);
                    cout<<"vex";
                }
                else{
                    cout.width(5);
                    cout<<"v"<<j-1;
                }
            }
            else if(!j){
                cout.width(5);
                cout<<"v"<<i-1;
            }
                
            else{
                if(G.ADJmatrix[i-1][j-1]==INF){
                    cout.width(8);
                    cout<<"∞";
                }
                else {
                    cout.width(6);
                    cout<<G.ADJmatrix[i-1][j-1];
                }
            }
        }
        cout<<endl;
    }
}//打印函数
void PrintALGraph(ALGraph G){
    int i;
    ArcNode* p;
    cout<<"##邻接表有向图##"<<endl;
    cout<<"顶点数: "<<G.vexnum<<endl;
    cout<<"边数: "<<G.arcnum<<endl;
    cout<<"邻接表: "<<endl;
    cout<<"eg:-->arc:{|weight|adjvex|}"<<endl;
    for(i=0;i<G.vexnum;i++){
        cout<<"v"<<i<<": ";
        for(p=G.vertices[i].firstarc;p!=NULL;p=p->nextarc){
            cout<<"-->|"<<p->weight<<"|"<<p->adjvex<<"|";
        }
        cout<<endl;
    }
}
typedef struct Node{
    int data;
    struct Node* next;
}LNode,*List;
void DijkstraMGraph(MGraph G,int src,int dst){
    if(G.vexs[src]==0||G.vexs[dst]==0){
        cout<<"不合法的操作：顶点不存在。"<<endl;
    }
    else
    {
    int* dis;//dis[i]--目前src到顶点i的最短距离
    int* T;//T[i+1]--顶点i是否已寻得最短路径 | T[0]--已寻得最短路径的顶点数
    List* path;//path[i]--src抵达顶点i路径链表
    int i;
    dis=(int*)malloc((G.vexnum)*sizeof(int));
    for(i=0;i<G.vexnum;i++){
        dis[i]=G.ADJmatrix[src][i];
    }//初始化distance
    path=(List*)malloc((G.vexnum)*sizeof(List));
    for(i=0;i<G.vexnum;i++){
        path[i]=(LNode*)malloc(sizeof(LNode));//重点:否则野指针->会导致段错误
        path[i]->data=src;
        path[i]->next=NULL;
    }//初始化path
    T=(int*)malloc((G.vexnum+1)*sizeof(int));
    for(i=0;i<G.vexnum+1;i++){
        T[i]=0;
    }
    T[src+1]=1;//初始化T
    for(T[0]=1;T[0]!=G.vexnum;T[0]++){
        int MinDis=INF;//dis中的最短距离
        int MinVex=dst;//dis最短距离对应顶点
        if(T[dst+1]){
            cout<<"src: v"<<src<<", dst: v"<<dst<<"间存在最短路径"<<endl;
            cout<<"路径长度: "<<dis[dst]<<endl;
            cout<<"path: ";
            for(List q=path[dst];q!=NULL;q=q->next){
                if(q->next==NULL){
                    cout<<"v"<<q->data;
                }
                else{
                    cout<<"v"<<q->data<<"<--";
                }
            }
            cout<<endl;
            return;
        }//output path
        else{
            for(i=0;i<G.vexnum;i++){
                if(T[i+1]!=1){
                    if(MinDis>dis[i]){
                        MinDis=dis[i];
                        MinVex=i;
                    }
                }
            }//记录src当前可达的顶点的最短路径和对应顶点
            //得到src到MinVex的最短路径
            if(MinDis==INF){
                cout<<"src: v"<<src<<", dst: v"<<dst<<"间不存在最短路径"<<endl;
                return;//output no path
            }//src无处可达
            else{
                T[MinVex+1]=1;
                //Minvex含入T中
                List p=(List)malloc(sizeof(LNode));
                p->data=MinVex;
                p->next=path[MinVex];
                path[MinVex]=p;
                //free(p);
                //头插法连接终点
                if(MinVex==dst){
                    cout<<"src: v"<<src<<", dst: v"<<dst<<"间存在最短路径"<<endl;
                    cout<<"路径长度: "<<dis[dst]<<endl;
                    cout<<"path: ";
                    for(List q=path[dst];q!=NULL;q=q->next){
                        if(q->next==NULL){
                            cout<<"v"<<q->data;
                        }
                        else{
                            cout<<"v"<<q->data<<"<--";
                        }
                    }
                    cout<<endl;
                    return;
                }//output path
                else{//通常
                    for(i=0;i<G.vexnum;i++){
                        if(G.ADJmatrix[MinVex][i]!=INF){
                            if((MinDis+G.ADJmatrix[MinVex][i])<dis[i]){
                                dis[i]=MinDis+G.ADJmatrix[MinVex][i];
                                List p=(List)malloc(sizeof(LNode));
                                p->data=MinVex;
                                p->next=path[i];
                                path[i]=p;
                                //free(p);//若通过中转顶点Minvex比原路径更短，则更新path,添加中转点
                            }
                        }
                    }
                }//else3
            }//else2
        }//else1
    }//for
    }//spc
}//输出从src到dst的最短路径长度--朴素的dijkstra算法
typedef pair<int,int>PII;//pair自动按第一关键字优先来比较大小

void DijkstraALGraph(ALGraph G, int src,int dst){
    if(G.vexs[src]==0||G.vexs[dst]==0){
        cout<<"不合法的操作：顶点不存在。"<<endl;
    }
    else
    {
    priority_queue<PII,vector<PII>,greater<PII>> heap;
    int* dis;//同朴素dijkstra
    int* T;//T[i]--顶点i是否已寻得最短路径
    List* path;//同朴素dijkstra
    ArcNode* p;
    int i;
    dis=(int*)malloc((G.vexnum)*sizeof(int));
    for(i=0;i<G.vexnum;i++){
        if(i==src)dis[i]=0;
        else dis[i]=INF;
    }//初始化distance
    path=(List*)malloc((G.vexnum)*sizeof(List));
    for(i=0;i<G.vexnum;i++){
        path[i]=(LNode*)malloc(sizeof(LNode));
        path[i]->data=src;
        path[i]->next=NULL;
    }//初始化path
        T=(int*)malloc((G.vexnum)*sizeof(int));
    for(i=0;i<G.vexnum;i++){
        T[i]=0;
    }//初始化T
    heap.push({0,src});
    while(!heap.empty()){
        PII temp=heap.top();
        heap.pop();
        int MinDis=temp.first;
        int MinVex=temp.second;
        if(!T[MinVex]){
            T[MinVex]=1;
            LNode* q=(LNode*)malloc(sizeof(LNode));
            q->data=MinVex;
            q->next=path[MinVex];
            path[MinVex]=q;//在path中增加可达节点MinVex
            for(p=G.vertices[MinVex].firstarc;p!=NULL;p=p->nextarc){
                if(dis[p->adjvex]>dis[MinVex]+p->weight){
                    dis[p->adjvex]=dis[MinVex]+p->weight;
                    heap.push({dis[p->adjvex],p->adjvex});
                    if(MinVex!=src){
                    LNode* q=(LNode*)malloc(sizeof(LNode));
                    q->data=MinVex;
                    q->next=path[p->adjvex];
                    path[p->adjvex]=q;
                    }
                }
            }//在path中增加中转节点MinVex
            if(MinVex==dst){
                cout<<"src: v"<<src<<", dst: v"<<dst<<"间存在最短路径"<<endl;
                cout<<"路径长度: "<<dis[dst]<<endl;
                cout<<"path: ";
                for(List q=path[dst];q!=NULL;q=q->next){
                    if(q->next==NULL){
                        cout<<"v"<<q->data;
                    }
                    else{
                        cout<<"v"<<q->data<<"<--";
                    }
                }
                cout<<endl;
                return;
            }//output path
        }
    }
    if(dis[dst]==INF){
        cout<<"src: v"<<src<<", dst: v"<<dst<<"间不存在最短路径"<<endl;
        return;
    }
    }
}//基于稀疏图/邻接表的堆优化dijkstra算法
int UI(MGraph &MG,ALGraph &ALG){
    int op;
    cin>>op;
    switch(op){
        case 1:{
            PrintMGraph(MG);
            return 1;
        }
        case 2:{
            PrintALGraph(ALG);
            return 1;
        }
        case 3:{
            int src,dst;
            cout<<"输入src,dst"<<endl;
            cin>>src>>dst;
            DijkstraMGraph(MG,src,dst);
            return 1;
        }
        case 4:{
            int src,dst;
            cout<<"输入src,dst"<<endl;
            cin>>src>>dst;
            DijkstraALGraph(ALG,src,dst);
            return 1;
        }
        case 0:return 0;
        default:{
            cout<<"error"<<endl;
            return 1;
        }
    }
}
int main(){
    cout<<"执行操作："<<endl;
    cout<<"1.打印邻接矩阵图"<<endl;
    cout<<"2.打印邻接表图"<<endl;
    cout<<"3.邻接矩阵朴素算法求最短路径"<<endl;
    cout<<"4.邻接表堆优化算法求最短路径"<<endl;
    cout<<"0.结束程序"<<endl;
    int in;
    MGraph MG;
    ALGraph ALG;
    CreateMGraph(MG);
    CreateALGraph(ALG);
    do{
        in=UI(MG,ALG);
    }
    while(in==1);
    return 0;
}