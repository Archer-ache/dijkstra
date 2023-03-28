#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;
typedef struct LinkedlistNode{
    double coef;
    int expn;
    struct LinkedlistNode *next;
}LNode,*Linkedlist;
Linkedlist LA=NULL,LB=NULL;
int elemnumA,elemnumB;
void CreatePolyn(Linkedlist &head,int elemnum){
    head=(Linkedlist)malloc(sizeof(LNode));
    head->expn=elemnum;//头节点指数域存放多项式项数
    head->next=NULL;
    for(int i=0;i<elemnum;i++){
        Linkedlist p=(Linkedlist)malloc(sizeof(LNode));
        cin>>p->coef;
        cin>>p->expn;
        p->next=head->next;
        head->next=p; 
    }
}
void PrintPolyn(Linkedlist head){
    Linkedlist top=head;
    for(Linkedlist p=head;p->next!=NULL;p=p->next){
        if(p==head)cout<<p->expn<<"-items Polynomial: ";
        if(head->next->next==NULL&&head->next->coef==0)cout<<0;
        else if(p==top){
            if(p->next->coef==0)top=top->next;
            else if(p->next->coef==1&&p->next->expn==1)cout<<"x";
            else if(p->next->expn==1)cout<<p->next->coef<<"x";
            else if(p->next->expn==0)cout<<p->next->coef;
            else if(p->next->coef==1)cout<<"x^"<<p->next->expn;
            else cout<<p->next->coef<<"x^"<<p->next->expn;
        }
        else{
            if(p->next->coef>0){
                if(p->next->coef==1&&p->next->expn==1)cout<<"+x";
                else if(p->next->expn==1)cout<<"+"<<p->next->coef<<"x";
                else if(p->next->expn==0)cout<<"+"<<p->next->coef;
                else if(p->next->coef==1)cout<<"+x^"<<p->next->expn;
                else cout<<"+"<<p->next->coef<<"x^"<<p->next->expn;
            }
            else if(p->next->coef<0){
                if(p->next->coef==-1&&p->next->expn==1)cout<<"-x";
                else if(p->next->expn==1)cout<<p->next->coef<<"x";
                else if(p->next->expn==0)cout<<p->next->coef;
                else if(p->next->coef==-1)cout<<"-x^"<<p->next->expn;
                else cout<<p->next->coef<<"x^"<<p->next->expn;
            }
        }
    }
    cout<<endl;
}
void RedefPolyn(Linkedlist &head){
    int count=0;
    for(Linkedlist p=head->next;p!=NULL;p=p->next)count++;
    head->expn=count;
    if(head->next==NULL){
        head->expn=1;
        head->next=(Linkedlist)malloc(sizeof(LNode));
        head->next->expn=head->next->coef=0;
        head->next->next=NULL;
    }
}
void AddPolyn(Linkedlist &La,Linkedlist &Lb){
    Linkedlist temp,temp1,temp2;
    Linkedlist Pa,Pb;
    for(Pa=La,Pb=Lb;Pa->next!=NULL&&Pb->next!=NULL;){
        if(Pa->next->expn>Pb->next->expn)Pa=Pa->next;
        else if(Pa->next->expn<Pb->next->expn){
            temp=Pb->next;
            Pb->next=temp->next;
            temp->next=Pa->next;
            Pa->next=temp;
            Pa=Pa->next;
        }
        else if(Pa->next->expn==Pb->next->expn){
             double texp=Pb->next->coef+Pa->next->coef;
            if(texp==0){
                temp1=Pa->next->next;
                free(Pa->next);
                Pa->next=temp1;
                temp2=Pb->next->next;
                free(Pb->next);
                Pb->next=temp2;                
            }
            else{
                Pa->next->coef=texp;
                temp2=Pb->next->next;
                free(Pb->next);
                Pb->next=temp2;
            }
        }
    }
    if(Pa->next==NULL){
        Pa->next=Pb->next;
        free(Lb);  
        Lb=NULL;
    }
    else{
        free(Lb);  
        Lb=NULL;
    }
    Linkedlist tp;
    for(Linkedlist p=La;p->next!=NULL&&p->next->next!=NULL;p=p->next){
        if(p->next->expn==p->next->next->expn){
            tp=p->next->next->next;
            p->next->coef=p->next->coef+p->next->next->coef;
            free(p->next->next);
            p->next->next=tp;
        }
    }
    RedefPolyn(La);
}
void NegPolyn(Linkedlist &head){
    for(Linkedlist p=head->next;p!=NULL;p=p->next)
        p->coef=-(p->coef);
}
void DestroyPolyn(Linkedlist &head){
    Linkedlist temp;
    for(;head->next!=NULL;free(temp)){
        temp=head->next;
        head->next=temp->next;
    }
    free(head);
    head=NULL;
}
void DiffPolyn(Linkedlist &head){
    Linkedlist temp;
    for(Linkedlist p=head;p!=NULL&&p->next!=NULL;p=p->next){
        if(p->next->expn==0){
            temp=p->next->next;
            free(p->next);
            p->next=temp;
        }
        else{
            p->next->coef=p->next->coef*p->next->expn;
            p->next->expn=p->next->expn-1;
        }
    }
    RedefPolyn(head);
}
int EvalPolyn(Linkedlist head,double x){
    double sum=0;
    for(Linkedlist p=head->next;p!=NULL;p=p->next){
        sum=sum+p->coef*pow(x,p->expn);
    }
    return sum;
}
int UI(){
    int OPTR;
    cin>>OPTR;
    switch(OPTR){
        case 1:{
            if(LA==NULL){
                cout<<"initialize the polynomial A:"<<endl;
                cout<<"enter the itemnum:"<<endl;
                cin>>elemnumA;
                cout<<"enter the coefficients and the exponents:"<<endl;
                CreatePolyn(LA,elemnumA);
                if(LA!=NULL)cout<<"succeeded."<<endl;
                else cout<<"error."<<endl;
            }
            else if(LA!=NULL&&LB==NULL){
                cout<<"initialize the polynomial B:"<<endl;
                cout<<"enter the itemnum:"<<endl;
                cin>>elemnumB;
                cout<<"enter the coefficients and the exponents:"<<endl;
                CreatePolyn(LB,elemnumB);
                if(LB!=NULL)cout<<"succeeded."<<endl;
                else cout<<"error."<<endl;
            }
            else cout<<"you already have two polynomials."<<endl;
            return 1;
        }
        case 2:{
            cout<<"choose the polynomial you want to check:"<<endl;
            cout<<"1.--LA"<<endl<<"2.--LB"<<endl;
            int optr;
            cin>>optr;
            switch(optr){
                case 1:{
                    if(LA!=NULL)PrintPolyn(LA);
                    else cout<<"you do not have LA"<<endl;
                    break;
                }
                case 2:{
                    if(LB!=NULL)PrintPolyn(LB);
                    else cout<<"you do not have LB"<<endl;  
                    break;                  
                }
                default:cout<<"error"<<endl;
            }
            return 1;
        }
        case 3:{
            if(LA==NULL||LB==NULL)cout<<"you do not have enough polynomials for the operation."<<endl;
            else{
                AddPolyn(LA,LB);
                cout<<"the result is: "<<endl;
                PrintPolyn(LA);
            }
            return 1;
        }
        case 4:{
            if(LA==NULL||LB==NULL)cout<<"you do not have enough polynomials for the operation."<<endl;
            else{
                cout<<"choose a way to operate:"<<endl;
                cout<<"1.LA-LB"<<endl<<"2.LB-LA"<<endl;            
                int optr;
                cin>>optr;
                switch(optr){
                case 1:{
                    NegPolyn(LB);
                    AddPolyn(LA,LB);
                    cout<<"the result is: "<<endl;
                    PrintPolyn(LA);
                    break;
                }
                case 2:{          
                    NegPolyn(LA);
                    AddPolyn(LA,LB);
                    cout<<"the result is: "<<endl;
                    PrintPolyn(LA);      
                    break;                     
                }
                default:cout<<"error"<<endl;
                }
            }     
            return 1;   
        }
        case 5:{
            cout<<"choose the polynomial you want to differentiate:"<<endl;
            cout<<"1.--LA"<<endl<<"2.--LB"<<endl;
            int optr;
            cin>>optr;
            switch(optr){
                case 1:{
                    if(LA!=NULL){
                        DiffPolyn(LA);
                        cout<<"succeeded."<<endl;
                    }
                    else cout<<"you do not have LA"<<endl;
                    break;
                }
                case 2:{
                    if(LB!=NULL){
                        DiffPolyn(LB);
                        cout<<"succeeded."<<endl;
                    }
                    else cout<<"you do not have LB"<<endl;  
                    break;                  
                }
                default:cout<<"error"<<endl;
            }
            return 1;
        }
        case 6:{
           cout<<"choose the polynomial you want to evaluate:"<<endl;
            cout<<"1.--LA"<<endl<<"2.--LB"<<endl;
            int optr;
            cin>>optr;
            switch(optr){
                case 1:{
                    if(LA!=NULL){
                        double x,eval;
                        cout<<"set a value for x:"<<endl;
                        cin>>x;
                        eval=EvalPolyn(LA,x);
                        cout<<"the value here is: "<<eval<<endl;
                    }
                    else cout<<"you do not have LA"<<endl;
                    break;
                }
                case 2:{
                    if(LB!=NULL){
                        double x,eval;
                        cout<<"set a value for x:"<<endl;
                        cin>>x;
                        eval=EvalPolyn(LB,x);
                        cout<<"the value here is: "<<eval<<endl;
                    }
                    else cout<<"you do not have LB"<<endl;  
                    break;                  
                }
                default:cout<<"error"<<endl;
            }
            return 1; 
        }
        case 7:{
            cout<<"choose the polynomial you want to reset:"<<endl;
            cout<<"1.--LA"<<endl<<"2.--LB"<<endl;
            int optr;
            cin>>optr;
            switch(optr){
                case 1:{
                    if(LA!=NULL){
                        DestroyPolyn(LA);
                        cout<<"reset the polynomial A:"<<endl;
                        cout<<"enter the itemnum:"<<endl;
                        cin>>elemnumA;
                        cout<<"enter the coefficients and the exponents:"<<endl;
                        CreatePolyn(LA,elemnumA);
                        if(LA!=NULL)cout<<"succeeded."<<endl;
                        else cout<<"error."<<endl;                       
                    }
                    else cout<<"you haven't initialized LA"<<endl;
                    break;
                }
                case 2:{
                    if(LB!=NULL){
                        DestroyPolyn(LB);
                        cout<<"reset the polynomial B:"<<endl;
                        cout<<"enter the itemnum:"<<endl;
                        cin>>elemnumB;
                        cout<<"enter the coefficients and the exponents:"<<endl;
                        CreatePolyn(LB,elemnumB);
                        if(LA!=NULL)cout<<"succeeded."<<endl;
                        else cout<<"error."<<endl;
                    }
                    else cout<<"you haven't initialized LB"<<endl;  
                    break;                  
                }
                default:cout<<"error"<<endl;
            }
            return 1;         
        }
        case 8:return 0;
        default: {
            cout<<"error"<<endl;
            return 1;
        }
    }
}
void UIPrinter(){
    cout<<"##Sparse polynomial calculator##"<<endl<<endl;
    cout<<"enter an OPTR to select an operation: "<<endl;
    cout<<"1.create a polynomial."<<endl;
    cout<<"2.print your polynomial."<<endl;
    cout<<"3.add two polynomials(LA + LB)."<<endl;
    cout<<"4.subtract two polynomials(LA - LB/LB - LA)."<<endl;
    cout<<"5.differentiate a polynomial."<<endl;
    cout<<"6.evaluate the polynomial at x."<<endl;
    cout<<"7.reset a polynomial."<<endl;
    cout<<"8.terminate the process."<<endl;
    cout<<"(//warning:to make it easier for continuous operations,op3 and op4 will reset LA to the result.//)"<<endl;
}
int main(){
    int interface;
    UIPrinter();
    do{
        interface=UI();
    }
    while(interface==1);
    return 0;
}