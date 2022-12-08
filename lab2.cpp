//**导航软件--用dijkstra算法求最短路径**
//附加实现内容：稀疏图存储结构
//附加实现内容：堆优化的dijkstra算法
//--*默认为有向图*--
#define INF INT_MAX
#define MAX_VERTEX_NUM  10000
#define FILENAME "data.txt"
#include <iostream>
#include <stdio.h>
//
using namespace std;
typedef struct{
    int vexs[MAX_VERTEX_NUM];
    int** ADJmatrix;
    int vexnum,arcnum;
}MGraph;//邻接矩阵定义
typedef struct ArcNode{
    int adjvex;
    struct ArcNode* nextarc;
}ArcNode;
typedef struct VexNode{
    int data;
    ArcNode* firstarc;
}VexNode,*ADJList;
typedef struct{
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
    G.arcnum=0;
    G.vexnum=0;
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
                    cout<<"v"<<j;
                }
            }
            else if(!j){
                cout.width(5);
                cout<<"v"<<i;
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
        path[i]=(LNode*)malloc(sizeof(LNode));//重点
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
            cout<<"src: v"<<src+1<<", dst: v"<<dst+1<<"间存在最短路径"<<endl;
            cout<<"路径长度: "<<dis[dst]<<endl;
            cout<<"path: ";
            for(List q=path[dst];q!=NULL;q=q->next){
                if(q->next==NULL){
                    cout<<"v"<<q->data+1;
                }
                else{
                    cout<<"v"<<q->data+1<<"<--";
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
                cout<<"src: v"<<src+1<<", dst: v"<<dst+1<<"间不存在最短路径"<<endl;
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
                    cout<<"src: v"<<src+1<<", dst: v"<<dst+1<<"间存在最短路径"<<endl;
                    cout<<"路径长度: "<<dis[dst]<<endl;
                    cout<<"path: ";
                    for(List q=path[dst];q!=NULL;q=q->next){
                        if(q->next==NULL){
                            cout<<"v"<<q->data+1;
                        }
                        else{
                            cout<<"v"<<q->data+1<<"<--";
                        }
                    }
                    cout<<endl;
                    return;
                }//output path
                else{//通常
                    for(i=0;i<G.vexnum;i++){
                        if(G.ADJmatrix[MinVex][i]!=INF){
                            if(T[i+1]!=1){
                                if((MinDis+G.ADJmatrix[MinVex][i])<dis[i]){
                                    dis[i]=MinDis+G.ADJmatrix[MinVex][i];
                                    List p=(List)malloc(sizeof(LNode));     //
                                    p->data=MinVex;
                                    p->next=path[i];
                                    path[i]=p;
                                    //free(p);//若通过中转顶点Minvex比原路径更短，则更新path,添加中转点
                                }
                            }
                        }
                    }//for2
                }//else3
            }//else2
        }//else1
    }//for1
    }//spc
}//输出从src到dst的最短路径长度，返回最短路径长度
//朴素的dijkstra算法

int UI(MGraph &G){
    int op;
    cin>>op;
    switch(op){
        case 1:{
            PrintMGraph(G);
            return 1;
        }
        case 2:{
            int src,dst;
            cout<<"输入src,dst"<<endl;
            cin>>src>>dst;
            DijkstraMGraph(G,src,dst);
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
    cout<<"1.打印图"<<endl;
    cout<<"2.求最短路径"<<endl;
    cout<<"0.结束程序"<<endl;
    int in;
    MGraph G;
    CreateMGraph(G);
    do{
        in=UI(G);
    }
    while(in==1);
    return 0;
}