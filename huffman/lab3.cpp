//**文件压缩软件--用huffman树对文件进行编码压缩**
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <queue>
#include <bitset>
using namespace std;
typedef struct{
    unsigned int byte;//叶子节点对应数据单位
    long long weight;//字节对应权重
    int parent,lchild,rchild;
    unsigned char *code;//节点Huffman编码
    unsigned int codelength;
}HuffNode,*HuffTree;
typedef struct{
    unsigned int byte;
    long long weight;
    int parent,achild,bchild,cchild,dchild;//四叉树的四个孩子域
    unsigned char *code;
    unsigned int codelength;
}HuffNode_Quad,*HuffTree_Quad;

typedef pair<long long,unsigned int>Node;//节点信息 | 按.first--weight升序排序的小顶堆

void Compress_Bin(char* I_FILE_NAME,char* O_FILE_NAME,short bymod){//bymod：指示字节读取模式
    short bits=8*bymod;
    unsigned int k=1<<bits;//计算叶子节点数目
    HuffTree T=(HuffTree)malloc((2*k-1)*sizeof(HuffNode));
    for(unsigned int i=0;i<2*k-1;i++){
        T[i].byte=i;
        T[i].weight=0;
        T[i].parent=-1;
        T[i].lchild=-1;
        T[i].rchild=-1;
    }//初始化huffman树
    int unitnum=0;
    int nodenum=0;
    int amplnum=0;
    FILE* fp;
    if(!(fp=fopen(I_FILE_NAME,"rb"))){
        cout<<"cannot open file"<<endl;
        return;
    }
    else{
        while(!feof(fp)){
            unsigned int reg=0;//数据单位寄存器
            int pos=0;//指示寄存器当前读取位置（最大为bymod）
            for(int i=0;i<2*bymod;i++){//以0.5字节为粒度
                if((i+1)%2==0){
                    unsigned char buf=0;//缓冲区
                    fread(&buf,1,1,fp);
                    if(feof(fp)){
                        break;
                    }
                    reg=reg<<8;
                    reg=reg+buf;
                    pos++;
                }
            }//——读取模块——
            if(feof(fp)){
                amplnum=(bymod-pos)%bymod;
                if(!amplnum)break;
            }
            T[reg].weight++;
            unitnum++;
        }//——数据单位读取与统计——
        fclose(fp);
        priority_queue<Node,vector<Node>,greater<Node>> heap;
        for(unsigned int i=0;i<k;i++){
            if(T[i].weight>0){
                heap.push({T[i].weight,i});
                nodenum++;
            }
        }//将有效节点信息放入小顶堆中
        //此时频率表已建成，可构建huffman树
        unsigned int c;
        for(c=k;heap.size()>1;c++){
            Node temp=heap.top();
            heap.pop();
            long long curL_W=temp.first;
            unsigned int curL_B=temp.second;
            temp=heap.top();
            long long curR_W=temp.first;
            unsigned int curR_B=temp.second;
            heap.pop();
            T[c].weight=curL_W+curR_W;
            T[c].lchild=curL_B;
            T[c].rchild=curR_B;
            T[curL_B].parent=c;
            T[curR_B].parent=c;
            heap.push({T[c].weight,c});//pair的byte域大于255则说明其为中间节点
        }//——建树模块——
    }
    //Print_HuffTree(T);
    char* FILE_EXTEN=strrchr(I_FILE_NAME,'.');
    for(unsigned int i=0;i<k;i++){//遍历叶子节点
        T[i].code=(unsigned char*)malloc(k*sizeof(char));
        T[i].codelength=0;
    }//初始化编码
    for(unsigned int i=0;i<k;i++){//遍历叶子节点
        if(T[i].weight){
            int j=0;
            int c=i;
            for(int p=T[i].parent;p!=-1;p=T[p].parent){
                if(T[p].lchild==c){
                    T[i].code[j]='0';
                }
                if(T[p].rchild==c){
                    T[i].code[j]='1';
                }//左0右1
                T[i].codelength++; 
                j++;
                c=p;
            }
            T[i].code[j]='\0';
        }
    }//——数据单元编码——
    FILE* fp_OS;
    if(!(fp_OS=fopen(O_FILE_NAME,"wb"))){
        cout<<"cannot open file"<<endl;
        return;
    }
    else{
        unsigned char mode=2;
        fwrite(&mode,1,1,fp_OS);
        fprintf(fp_OS,"%d,%s%d,%d,%d,%hd,",strlen(FILE_EXTEN),FILE_EXTEN,unitnum,nodenum,amplnum,bymod);
        //printf("%d,%s,%d,%d,%d,\n",strlen(FILE_EXTEN),FILE_EXTEN,unitnum,nodenum,amplnum);//检测
        for(int i=0;i<k;i++){
            if(T[i].weight){
                fprintf(fp_OS,"%u,%lld,",T[i].byte,T[i].weight);
                //cout<<"byte: "<<bitset<16>(T[i].byte)<<",weight: "<<T[i].weight<<endl;//检测
            }
        }//源文件头部信息写入
        FILE* fp_IS;
        if(!(fp_IS=fopen(I_FILE_NAME,"rb"))){
            cout<<"cannot open file"<<endl;
            return;
        }
        else{
            unsigned char buf=0;//缓冲区
            int ComBytes=0;//压缩后文件字节数
            int pos=0;
            while(!feof(fp_IS)){
                unsigned int reg=0;//数据单位寄存器
                for(int i=0;i<2*bymod;i++){
                    if((i+1)%2==0){
                        unsigned char buffer=0;//内部缓冲区
                        fread(&buffer,1,1,fp_IS);
                        if(feof(fp_IS)){
                            break;
                        }
                        reg=reg<<8;
                        reg=reg+buffer;
                    }
                }//——读取模块——
                if(feof(fp_IS)){
                    if(!amplnum)break;
                }
                for(int i=T[reg].codelength-1;i>=0;i--){
                    switch(T[reg].code[i]){
                        case '1':{
                            buf++;
                            break;
                        }
                        case '0':{
                            break;
                        }
                        default:cout<<"error:写入时发生错误"<<endl;
                    }
                    pos++;//指示缓冲区下一bit
                    if(pos==8){
                        fwrite(&buf,1,1,fp_OS);
                        pos=0;
                        buf=0;
                        ComBytes++;
                    }//缓冲区满8 bits执行一次输入 | 维护buf和pos
                    buf=buf<<1;
                }//——文件写入——
            }
            while((pos!=7)&&(pos!=0)){
                buf=buf<<1;
                pos++;
            }//不满8-bit补0后输出
            if(pos!=0)
            fwrite(&buf,1,1,fp_OS);
            ComBytes++;
            cout<<"total bytes: "<<ComBytes<<" Bytes"<<endl;
            double ratio=(double)ComBytes/(double)(bymod*unitnum);
            cout<<"compression ratio: "<<ratio*100.0<<"%"<<endl;
            cout<<"---compression completed---"<<endl;
            fclose(fp_IS);
        }
        fclose(fp_OS);
    }
}
void Compress_Quad(char* I_FILE_NAME,char* O_FILE_NAME,short bymod){
    short bits=8*bymod;
    unsigned int k=1<<bits;//计算叶子节点数目
    HuffTree_Quad T=(HuffTree_Quad)malloc(((4*k-1)/3+1)*sizeof(HuffNode_Quad));
    for(unsigned int i=0;i<(4*k-1)/3+1;i++){
        T[i].byte=i;
        T[i].weight=0;
        T[i].parent=-1;
        T[i].achild=-1;
        T[i].bchild=-1;
        T[i].cchild=-1;
        T[i].dchild=-1;
    }//初始化huffman树
    int unitnum=0;
    int nodenum=0;
    int amplnum=0;
    FILE* fp;
    if(!(fp=fopen(I_FILE_NAME,"rb"))){
        cout<<"cannot open file"<<endl;
        return;
    }
    else{
        while(!feof(fp)){
            unsigned int reg=0;//数据单位寄存器
            int pos=0;//指示寄存器当前读取位置（最大为bymod）
            for(int i=0;i<2*bymod;i++){//以0.5字节为粒度
                if((i+1)%2==0){
                    unsigned char buf=0;//缓冲区
                    fread(&buf,1,1,fp);
                    if(feof(fp)){
                        break;
                    }
                    reg=reg<<8;
                    reg=reg+buf;
                    pos++;
                }
            }
            if(feof(fp)){
                amplnum=(bymod-pos)%bymod;
                if(!amplnum)break;
            }
            T[reg].weight++;
            unitnum++;
        }
        fclose(fp);
        priority_queue<Node,vector<Node>,greater<Node>> heap;
        for(unsigned int i=0;i<k;i++){
            if(T[i].weight>0){
                heap.push({T[i].weight,i});
                nodenum++;
            }
        }//将所有节点信息放入小顶堆中
        //此时频率表已建成，可构建huffman树
        unsigned int c=k;
        int cnt=0;
        long long cur_W;
        unsigned int cur_B;
        while(!heap.empty()){
            Node temp=heap.top();
            heap.pop();
            cnt++;
            cur_W=temp.first;
            cur_B=temp.second;
            switch(cnt){
                case 1:{
                    T[c].achild=cur_B;
                    break;
                }
                case 2:{
                    T[c].bchild=cur_B;
                    break;
                }
                case 3:{
                    T[c].cchild=cur_B;
                    break;
                }
                case 4:{
                    T[c].dchild=cur_B;
                    break;
                }
                default:cout<<"建树时发生错误"<<endl;
            }
            T[cur_B].parent=c;
            if(cnt==4){
                cnt=0;
                T[c].weight=T[T[c].achild].weight+T[T[c].bchild].weight+T[T[c].cchild].weight+T[T[c].dchild].weight;
                heap.push({T[c].weight,c});
                c++;
            }
        }
        switch(cnt){
            case 1:{
                T[cur_B].parent=-1;
                unsigned int root=cur_B;
                break;
            }
            case 2:{
                T[c].weight=T[T[c].achild].weight+T[T[c].bchild].weight;
                unsigned int root=c;
                break;
            }
            case 3:{
                T[c].weight=T[T[c].achild].weight+T[T[c].bchild].weight+T[T[c].cchild].weight;
                unsigned int root=c;
            }
            default:cout<<"建树时发生错误"<<endl;
        }
    }
    char* FILE_EXTEN=strrchr(I_FILE_NAME,'.');
    for(unsigned int i=0;i<k;i++){
        T[i].code=(unsigned char*)malloc(k*sizeof(char));
        T[i].codelength=0;
    }//初始化编码
    for(unsigned int i=0;i<k;i++){
        if(T[i].weight){
            int j=0;
            int c=i;
            for(int p=T[i].parent;p!=-1;p=T[p].parent){
                if(T[p].achild==c){
                    T[i].code[j]='0';
                }
                if(T[p].bchild==c){
                    T[i].code[j]='1';
                }
                if(T[p].cchild==c){
                    T[i].code[j]='2';
                }
                if(T[p].dchild==c){
                    T[i].code[j]='3';
                }
                T[i].codelength++; 
                j++;
                c=p;
            }
            T[i].code[j]='\0';
        }
    }//至此全部数据单位编码完成
    FILE* fp_OS;
    if(!(fp_OS=fopen(O_FILE_NAME,"wb"))){
        cout<<"cannot open file"<<endl;
        return;
    }
    else{
        unsigned char mode=4;
        fwrite(&mode,1,1,fp_OS);
        fprintf(fp_OS,"%d,%s%d,%d,%d,%hd,",strlen(FILE_EXTEN),FILE_EXTEN,unitnum,nodenum,amplnum,bymod);
        //printf("%d,%s,%d,%d,%d,\n",strlen(FILE_EXTEN),FILE_EXTEN,unitnum,nodenum,amplnum);//检测
        for(int i=0;i<k;i++){
            if(T[i].weight){
                fprintf(fp_OS,"%u,%lld,",T[i].byte,T[i].weight);
                //cout<<"byte: "<<bitset<8>(T[i].byte)<<",weight: "<<T[i].weight<<endl;//检测
            }
        }
        FILE* fp_IS;
        if(!(fp_IS=fopen(I_FILE_NAME,"rb"))){
            cout<<"cannot open file"<<endl;
            return;
        }
        else{
            unsigned char buf=0;//缓冲区
            int ComBytes=0;//压缩后文件字节数
            int pos=0;
            while(!feof(fp_IS)){
                unsigned int reg=0;//数据单位寄存器
                for(int i=0;i<2*bymod;i++){//以0.5字节为粒度
                    if((i+1)%2==0){
                        unsigned char buffer=0;//内部缓冲区
                        fread(&buffer,1,1,fp_IS);
                        if(feof(fp_IS)){
                            break;
                        }
                        reg=reg<<8;
                        reg=reg+buffer;
                    }
                }
                if(feof(fp_IS)){
                    if(!amplnum)break;
                }
                for(int i=T[reg].codelength-1;i>=0;i--){
                    switch(T[reg].code[i]){
                        case '0':{
                            break;
                        }
                        case '1':{
                            buf=buf+1;
                            break;
                        }
                        case '2':{
                            buf=buf+2;
                            break;
                        }
                        case '3':{
                            buf=buf+3;
                            break;
                        }
                        default:cout<<"error:写入时发生错误"<<endl;
                    }
                    pos=pos+2;//指示下一bit
                    if(pos==8){
                        fwrite(&buf,1,1,fp_OS);
                        pos=0;
                        buf=0;
                        ComBytes++;
                    }//缓冲区满8 bits执行一次输入 | 维护buf和pos
                    buf=buf<<2;
                }
            }
            while((pos!=6)&&(pos!=0)){
                buf=buf<<2;
                pos=pos+2;
            }//不满8 bits补0后输出
            if(pos!=0)
            fwrite(&buf,1,1,fp_OS);
            ComBytes++;
            cout<<"total bytes: "<<ComBytes<<" Bytes"<<endl;
            double ratio=(double)ComBytes/(double)(bymod*unitnum);
            cout<<"compression ratio: "<<ratio*100.0<<"%"<<endl;
            fclose(fp_IS);
        }
        fclose(fp_OS);
    }
}
void Decompress_Bin(char* I_FILE_NAME,char* name){//rebuild
    FILE* fp_IS;
    if(!(fp_IS=fopen(I_FILE_NAME,"rb"))){
        cout<<"cannot open file"<<endl;
        return;
    }
    else{
        int length;
        char extend[100]={'\0'};
        int unitnum;
        int nodenum;
        int amplnum;
        short bymod;
        unsigned char mode;
        fread(&mode,1,1,fp_IS);
        fscanf(fp_IS,"%d,",&length);
        fread(&extend,length,1,fp_IS);
        fscanf(fp_IS,"%d,%d,%d,%hd,",&unitnum,&nodenum,&amplnum,&bymod);
        char* O_FILE_NAME=strcat(name,extend);
        //获取压缩文件内头部信息
        short bits=8*bymod;
        unsigned int k=1<<bits;
        HuffTree T=(HuffTree)malloc((2*k-1)*sizeof(HuffNode));
        for(unsigned int i=0;i<2*k-1;i++){
            T[i].byte=i;
            T[i].weight=0;
            T[i].parent=-1;
            T[i].lchild=-1;
            T[i].rchild=-1;
        }//初始化huffman树
        priority_queue<Node,vector<Node>,greater<Node>> heap;
        for(int i=0;i<nodenum;i++){
            unsigned int reg;
            long long w;
            fscanf(fp_IS,"%u,%lld,",&reg,&w);
            //cout<<"byte: "<<bitset<16>(reg)<<",weight: "<<w<<endl;//检测
            heap.push({w,reg});
            T[reg].weight=w;
        }//可以开始建树
        unsigned int c;
        for(c=k;heap.size()>1;c++){
            Node temp=heap.top();
            heap.pop();
            long long curL_W=temp.first;
            unsigned int curL_B=temp.second;
            temp=heap.top();
            long long curR_W=temp.first;
            unsigned int curR_B=temp.second;
            heap.pop();
            T[c].weight=curL_W+curR_W;
            T[c].lchild=curL_B;
            T[c].rchild=curR_B;
            T[curL_B].parent=c;
            T[curR_B].parent=c;
            heap.push({T[c].weight,c});
        }
        FILE* fp_OS;
        if(!(fp_OS=fopen(O_FILE_NAME,"wb"))){
            cout<<"cannot open file"<<endl;
            return;
        }
        else{
            unsigned char buf;//缓冲区
            int root=--c;//根节点位置
            int pos=0;
            unsigned char step;//掩码结果
            int cnt=unitnum;
            unsigned int p=root;//游标
            fread(&buf,1,1,fp_IS);
            while(!feof(fp_IS)){
                while(pos!=8){
                    step=buf&0x80;
                    switch(step){
                        case 0x00:{
                            p=T[p].lchild;
                            break;
                        }
                        case 0x80:{
                            p=T[p].rchild;
                            break;
                        }
                        default:cout<<"error:解压写入时发生错误"<<endl;
                    }
                    if((T[p].lchild==-1)&&(T[p].rchild==-1)){
                        cnt--;
                        if(cnt==0){
                            for(int i=0;i<bymod-amplnum;i++){
                                unsigned int ch=T[p].byte;
                                unsigned char m=ch>>(8*(bymod-amplnum-i-1));
                                fwrite(&m,1,1,fp_OS);
                            }
                            break;
                        }
                        else{
                            for(int i=0;i<bymod;i++){
                                unsigned int ch=T[p].byte;
                                unsigned char m=ch>>(8*(bymod-i-1));
                                fwrite(&m,1,1,fp_OS);
                            }
                            p=root;//维护游标
                        }
                    }
                    buf=buf<<1;
                    pos++;
                }
                if(cnt==0)break;
                fread(&buf,1,1,fp_IS);
                pos=0;
            }
            fclose(fp_OS);
        }
        fclose(fp_IS);
        cout<<"---Decompression completed---"<<endl<<endl;
    }
}
void Decompress_Quad(char* I_FILE_NAME,char* name){//rebuild
    FILE* fp_IS;
    if(!(fp_IS=fopen(I_FILE_NAME,"rb"))){
        cout<<"cannot open file"<<endl;
        return;
    }
    else{
        int length;
        char extend[100]={'\0'};
        int unitnum;
        int nodenum;
        int amplnum;
        short bymod;
        unsigned char mode;
        fread(&mode,1,1,fp_IS);
        fscanf(fp_IS,"%d,",&length);
        fread(&extend,length,1,fp_IS);
        fscanf(fp_IS,"%d,%d,%d,%hd,",&unitnum,&nodenum,&amplnum,&bymod);
        char* O_FILE_NAME=strcat(name,extend);
        //获取压缩文件内头部信息
        short bits=8*bymod;
        unsigned int k=1<<bits;
        HuffTree_Quad T=(HuffTree_Quad)malloc(((4*k-1)/3+1)*sizeof(HuffNode_Quad));
        for(unsigned int i=0;i<(4*k-1)/3+1;i++){
            T[i].byte=i;
            T[i].weight=0;
            T[i].parent=-1;
            T[i].achild=-1;
            T[i].bchild=-1;
            T[i].cchild=-1;
            T[i].dchild=-1;
        }//初始化huffman树
        priority_queue<Node,vector<Node>,greater<Node>> heap;
        for(int i=0;i<nodenum;i++){
            unsigned int reg;
            long long w;
            fscanf(fp_IS,"%u,%lld,",&reg,&w);
            //cout<<"byte: "<<bitset<16>(reg)<<",weight: "<<w<<endl;//检测
            heap.push({w,reg});
            T[reg].weight=w;
        }//可以开始建树
        unsigned int c=k;
        int cnt=0;
        long long cur_W;
        unsigned int cur_B;
        unsigned int root;
        while(!heap.empty()){
            Node temp=heap.top();
            heap.pop();
            cnt++;
            cur_W=temp.first;
            cur_B=temp.second;
            switch(cnt){
                case 1:{
                    T[c].achild=cur_B;
                    break;
                }
                case 2:{
                    T[c].bchild=cur_B;
                    break;
                }
                case 3:{
                    T[c].cchild=cur_B;
                    break;
                }
                case 4:{
                    T[c].dchild=cur_B;
                    break;
                }
                default:cout<<"建树时发生错误"<<endl;
            }
            T[cur_B].parent=c;
            if(cnt==4){
                cnt=0;
                T[c].weight=T[T[c].achild].weight+T[T[c].bchild].weight+T[T[c].cchild].weight+T[T[c].dchild].weight;
                heap.push({T[c].weight,c});
                c++;
            }
        }
        switch(cnt){
            case 1:{
                T[cur_B].parent=-1;
                root=cur_B;
                break;
            }
            case 2:{
                T[c].weight=T[T[c].achild].weight+T[T[c].bchild].weight;
                root=c;
                break;
            }
            case 3:{
                T[c].weight=T[T[c].achild].weight+T[T[c].bchild].weight+T[T[c].cchild].weight;
                root=c;
            }
            default:cout<<"建树时发生错误"<<endl;
        }

        FILE* fp_OS;
        if(!(fp_OS=fopen(O_FILE_NAME,"wb"))){
            cout<<"cannot open file"<<endl;
            return;
        }
        else{
            unsigned char buf;//缓冲区
            int pos=0;
            unsigned char step;//掩码结果
            int cnt=unitnum;
            unsigned int p=root;//游标
            fread(&buf,1,1,fp_IS);
            while(!feof(fp_IS)){
                while(pos!=8){
                    step=buf&0xc0;
                    switch(step){
                        case 0x00:{
                            p=T[p].achild;
                            break;
                        }
                        case 0x40:{
                            p=T[p].bchild;
                            break;
                        }
                        case 0x80:{
                            p=T[p].cchild;
                            break;
                        }
                        case 0xc0:{
                            p=T[p].dchild;
                            break;
                        }
                        default:cout<<"error:解压写入时发生错误"<<endl;
                    }
                    if((T[p].achild==-1)&&(T[p].bchild==-1)&&(T[p].cchild==-1)&&(T[p].dchild==-1)){
                        cnt--;
                        if(cnt==0){
                            for(int i=0;i<bymod-amplnum;i++){
                                unsigned int ch=T[p].byte;
                                unsigned char m=ch>>(8*(bymod-amplnum-i-1));
                                fwrite(&m,1,1,fp_OS);
                            }
                            break;
                        }
                        else{
                            for(int i=0;i<bymod;i++){
                                unsigned int ch=T[p].byte;
                                unsigned char m=ch>>(8*(bymod-i-1));
                                fwrite(&m,1,1,fp_OS);
                            }
                            p=root;
                        }
                    }
                    buf=buf<<2;
                    pos=pos+2;
                }
                if(cnt==0)break;
                fread(&buf,1,1,fp_IS);
                pos=0;
            }
            fclose(fp_OS);
        }
        fclose(fp_IS);
        cout<<"---Decompression completed---"<<endl<<endl;
    }
}
void Decompress(char* I_FILE_NAME,char* name){
    FILE* fp_IS;
    if(!(fp_IS=fopen(I_FILE_NAME,"rb"))){
        cout<<"cannot open file"<<endl;
        return;
    }
    else{
        unsigned char mode;
        fread(&mode,1,1,fp_IS);
        fclose(fp_IS);
        switch(mode){
            case 2:{
                Decompress_Bin(I_FILE_NAME,name);
                break;
            }
            case 4:{
                Decompress_Quad(I_FILE_NAME,name);
                break;
            }
            default:cout<<"文件不符合规格"<<endl;
        }
    }
}
int UI(){
    int op;
    std::cin>>op;
    switch(op){
        case 1:{
            cout<<"选择压缩模式："<<endl
            <<"1.BIN_Compress"<<endl
            <<"2.QUAD_Compress"<<endl;
            short mode;
            cin>>mode;
            cout<<"选择压缩数据单位："<<endl
            <<"1.1byte/unit"<<endl
            <<"2.2bytes/unit"<<endl;
            short un;
            cin>>un;
            switch(mode){
                case 1:{
                    char I_FILE_NAME[100];
                    char O_FILE_NAME[100];
                    cout<<"输入需要压缩的文件路径： "<<endl;
                    cin>>I_FILE_NAME;
                    cout<<"输入压缩后的文件输出路径： "<<endl;
                    cin>>O_FILE_NAME;
                    Compress_Bin(I_FILE_NAME,O_FILE_NAME,un);
                    break;
                }
                case 2:{
                    char I_FILE_NAME[100];
                    char O_FILE_NAME[100];
                    cout<<"输入需要压缩的文件路径： "<<endl;
                    cin>>I_FILE_NAME;
                    cout<<"输入压缩后的文件输出路径： "<<endl;
                    cin>>O_FILE_NAME;
                    Compress_Quad(I_FILE_NAME,O_FILE_NAME,un);
                    break;                    
                }
                default:cout<<"error mode"<<endl;
            }
            return 1;
        }
        case 2:{
            char I_FILE_NAME[100];
            char name[100];
            cout<<"输入需要解压缩的文件路径： "<<endl;
            cin>>I_FILE_NAME;
            cout<<"输入解压缩后的文件名称： "<<endl;//无需后缀名
            cin>>name;
            Decompress(I_FILE_NAME,name);
            return 1;
        }
        case 0:{
            return 0;
        }
        default:{
            cout<<"不合法的操作"<<endl;
            return 1;
        }
    }
}
void UIPrinter(){
    cout<<"输入要执行的操作："<<endl;
    cout<<"1.压缩文件"<<endl;
    cout<<"2.解压文件"<<endl;
    cout<<"0.结束程序"<<endl;
}
int main(){
    int in;
    do{
        UIPrinter();
        in=UI();
    }
    while(in==1);
    return 0;
}