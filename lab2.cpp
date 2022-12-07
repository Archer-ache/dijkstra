//**导航软件--用dijkstra算法求最短路径**

//附加实现内容：稀疏图存储结构
//附加实现内容：堆优化的dijkstra算法
//--默认为有向图
#define INF INT_MAX
#define MAX_VERTEX_NUM  150000
#include <iostream>
#include <stdio.h>
#include <cmath>
//
using namespace std;
typedef struct{
    int vexs[MAX_VERTEX_NUM];
    int ADJmatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
    int vexnum,arcnum;
}MGraph;
void CreateMGraph(MGraph &G){
    freopen("data.txt","r",stdin);
    int n=4;
    int a,b,c
    while(!n){
        cin>>a>>b;
        cout<<a<<b<<c<<endl;
        n--;
    }
}
int main(){

    CreateMGraph();
}