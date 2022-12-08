//**导航软件--用dijkstra算法求最短路径**
//附加实现内容：稀疏图存储结构
//附加实现内容：堆优化的dijkstra算法
//--默认为有向图
#define INF INT_MAX
#define MAX_VERTEX_NUM  150000
#define FILENAME "test.txt"
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
    if(!freopen(FILENAME,"r",stdin))
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
            else{
                G.ADJmatrix[i][j]=INF;
            }
        }
    }//初始化邻接矩阵
    if(!(freopen(FILENAME,"r",stdin)))
    cout<<"cannot open file"<<endl;
    else{
        while(cin>>src>>dst>>dis){
            G.ADJmatrix[src][dst]=dis;
        }
    }
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
    int* dis;//dis[i]--目前src到顶点i的最短距离
    int* T;//T[i+1]--顶点i是否已寻得最短路径 | T[0]--已寻得最短路径的顶点数
    List* path;//path[i]--顶点i路径链表
    int MinDis=INF;//dis中的最短距离
    int MinVex;//dis最短距离对应顶点
    int i;
    dis=(int*)malloc((G.vexnum)*sizeof(int));
    for(i=0;i<G.vexnum;i++){
        dis[i]=G.ADJmatrix[src][i];
    }//初始化distance
    path=(List*)malloc((G.vexnum)*sizeof(List));
    for(i=0;i<G.vexnum;i++){
        path[i]->data=src;
    }//初始化path
    T=(int*)malloc((G.vexnum+1)*sizeof(int));
    T[src+1]=1;//自身已可达

    for(T[0]=1;T[0]!=G.vexnum;T[0]++){
        for(i=0;i<G.vexnum;i++){
            if(T[i+1]!=1){
                if(MinDis>dis[i]){
                    MinDis=dis[i];
                    MinVex=i;
                }
            }
        }//记录src可直达的顶点的最短路径和对应顶点
        //得到src到MinVex的最短路径
        T[MinVex+1]=1;
        //Minvex含入T中
        List p=(List)malloc(sizeof(LNode));
        p->data=MinVex;
        p->next=path[MinVex];
        path[MinVex]=p;
        free(p);
        //头插法连接终点
        if(MinVex==dst){
            //output path
        }
        else{//通常
            for(i=0;i<G.vexnum;i++){
                if((MinDis+G.ADJmatrix[MinVex][i])<dis[i]){
                    dis[i]=MinDis+G.ADJmatrix[MinVex][i];
                    List p=(List)malloc(sizeof(LNode));
                    p->data=MinVex;
                    p->next=path[i];
                    path[i]=p;
                    free(p);//若通过中转顶点Minvex比原路径更短，则更新path
                }
            }
        }
    }

}//输出从src到dst的最短路径长度，返回最短路径长度
//朴素的dijkstra算法

int main(){
    MGraph G;
    CreateMGraph(G);
    PrintMGraph(G);
    //DijkstraMGraph(G,1,0);
}