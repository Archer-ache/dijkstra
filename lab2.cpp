//**导航软件--用dijkstra算法求最短路径**
//附加实现内容：稀疏图存储结构
//附加实现内容：堆优化的dijkstra算法
//--默认为有向图
#define INF INT_MAX
#define MAX_VERTEX_NUM  150000
#include <iostream>
#include <stdio.h>
//
using namespace std;
typedef struct{
    int vexs[MAX_VERTEX_NUM];
    int** ADJmatrix;
    int vexnum,arcnum;
}MGraph;
void CreateMGraph(MGraph &G){
    G.arcnum=0;
    G.vexnum=0;
    int src,dst,dis;
    int i,j;
    if(!freopen("test.txt","r",stdin))
    cout<<"cannot open file"<<endl;
    else{
        while(cin>>src>>dst>>dis){
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
    cin.clear();//一定要加！！
    G.ADJmatrix=(int**)malloc(G.vexnum*sizeof(int*));
    for(i=0;i<G.vexnum;i++){
        G.ADJmatrix[i]=(int*)malloc(G.vexnum*sizeof(int));
        for(j=0;j<G.vexnum;j++){
            if(i==j)G.ADJmatrix[i][j]=0;//认为自身可达自身并且距离为0
            G.ADJmatrix[i][j]=INF;
        }
    }//初始化邻接矩阵
    if(!(freopen("test.txt","r",stdin)))
    cout<<"cannot open file"<<endl;
    else{
        while(cin>>src>>dst>>dis){
            G.ADJmatrix[src][dst]=dis;
            G.arcnum++;
        }
    }
}
void PrintMGraph(MGraph G){
    int i, j;
    cout<<"##邻接矩阵有向图##"<<endl;
    cout<<"顶点数: "<<G.vexnum<<endl;
    cout<<"边数: "<<G.arcnum<<endl;
    cout<<"邻接矩阵: "<<endl;
    for(i=0;i<=G.vexnum;i++){
        for(j=0;j<=G.vexnum;j++){
            if(!i){
                if(!j)
                cout<<"vex ";
                else
                cout<<j-1<<" ";
            }
            else if(!j)
                cout<<i-1<<"   ";
            else{
                if(G.ADJmatrix[i-1][j-1]==INF)
                cout<<"∞ ";
                else cout<<G.ADJmatrix[i-1][j-1]<<" ";
            }
        }
        cout<<endl;
    }
}//打印函数
typedef struct Node{
    int data;
    Node* next;
}*list;
typedef struct{
    int dst;
    list path;
}Path;
int DijkstraMGraph(MGraph G,int src,int dst){
    int dis[G.vexnum];//记录从src到各顶点的距离
    int
    Path T[G.vexnum];
    int i;
    for(i=0;i<G.vexnum;i++)
        dis[i]=G.ADJmatrix[src][i];
    
}//输出从src到dst的最短路径长度，返回最短路径长度
int main(){
    MGraph G;
    CreateMGraph(G);
    PrintMGraph(G);
}