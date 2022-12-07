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
void GetVexnum(MGraph &G){
    G.vexnum=0;
    freopen("test.txt","r",stdin);
    int src,dst,dis;
    int i=0;
    while(cin>>src>>dst>>dis){
        if(G.vexs[src]!=1){
            G.vexs[src]=1;
            G.vexnum++;
        }
        if(G.vexs[dst]!=1){
            G.vexs[dst]=1;
            G.vexnum++;
        }
    }
    fclose(stdin);
}//确定G.vexnum
void CreateMGraph(MGraph &G){
    G.arcnum=0;
    int src,dst,dis;
    int i,j;
    G.ADJmatrix=(int**)malloc(G.vexnum*sizeof(int*));
    for(i=0;i<G.vexnum;i++){
        G.ADJmatrix[i]=(int*)malloc(G.vexnum*sizeof(int));
        for(j=0;j<G.vexnum;j++)
        G.ADJmatrix[i][j]=INF;
    }//初始化邻接矩阵
    if(!freopen("test.txt","r",stdin))
    cout<<"cannot open file"<<endl;
    else{
            G.ADJmatrix[src][dst]=dis;
        while(cin>>src>>dst>>dis){
            G.arcnum++;
        }
    }
    fclose(stdin);
}
void PrintMGraph(MGraph G){
    int i, j;
    cout<<"##邻接矩阵有向图##"<<endl;
    cout<<"顶点数: "<<G.vexnum<<endl;
    cout<<"边数: "<<G.arcnum<<endl;
    cout<<"邻接矩阵: "<<endl;
    for(i=0;i<G.vexnum;i++){
        for(j=0;j<G.vexnum;j++){
            if(G.ADJmatrix[i][j]==INF)
            cout<<"∞ ";
            else cout<<G.ADJmatrix[i][j]<<" ";
        }
        cout<<endl;
    }
}//打印函数
int main(){
    MGraph G;
    GetVexnum(G);
    CreateMGraph(G);
    PrintMGraph(G);
}