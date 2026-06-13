#include <iostream> 
using namespace std;
#include "LISCH.h"

LISCH::LISCH(int size,NODE_SET *set=NULL){
    this->size = size;
    this->DATA = new NODE[size];
    this->R = this->size-1;
    for(int i=0; i<size; i++){
        this->DATA[i].data = NONE;
        this->DATA[i].link = NONE;
    }
    if(set!=NULL){
        for(int i = 0;i<set->count;i++){
            if(!set->printinsert){
                this->insertNode(set->datas[i]);
            }else this->insertNode_print(set->datas[i]);
        }
    }
}

int LISCH::insertNode_print(int data){
    cout <<data<<" as ${"<<this->hash(data)<<"} : ";
    if(data==NONE){
        cout << "NULL"<<endl;
        return 0;
    }
    SEARCH_RES search = this->searchNode(data);
    if(search.index!=NONE){//이미 존재하는 원소
        cout << "insert denied : found in ${"<<search.index<<"}"<<endl<<endl;
        return 0;
    }
    int index = this->hash(data);
    int probe = 1;
    cout << index;
    if(this->DATA[index].data==NONE){//새 노드라면,
        this->DATA[index].data = data;
        cout<<endl<<"probe = "<<probe<<endl<<endl;
        findR();
        return probe;
    }
    while(this->DATA[index].link!=NONE){
        index = this->DATA[index].link;
        probe++;
        cout << " -> " << index;
    }
    if(this->R < 0) {
        cout << " -> insert denied : Hash table full!" << endl<<endl;
        return probe;
    }
    this->DATA[this->R].data = data;
    this->DATA[index].link = this->R;
    probe++;
    cout << " -> save as "<<this->R;
    cout << endl<<"probe = "<<probe<<endl<<endl;
    findR();
    return probe;
}

NODE LISCH::get(int h){
    return this->DATA[h];
}

int LISCH::hash(int data){
    return data%this->size;
}

void LISCH::findR(){
    while(this->R >= 0){
        if(this->DATA[this->R].data==NONE) break;
        this->R--;
    }
}

void LISCH::setR(int R){
    if(R>this->size-1 || R<0) return;
    this->R = R;
}
SEARCH_RES LISCH::searchNode(int data){
    SEARCH_RES res = SEARCH_RES();
    if(data==NONE) return res;
    res.index = this->hash(data);
    res.probe = 1;
    if(this->DATA[res.index].data == data) return res;
    while(this->DATA[res.index].link!=NONE){
        res.pindex = res.index;
        res.index = this->DATA[res.index].link;
        res.probe++;
        if(this->DATA[res.index].data == data) return res;
    }
    res.index=NONE;
    return res;//탐색 실패
}

int LISCH::insertNode(int data){
    if(data==NONE||this->R<0) return 0;
    SEARCH_RES search = this->searchNode(data);
    if(search.index!=NONE){//이미 존재하는 원소
        return 0;
    }
    int index = this->hash(data);
    int probe = 1;
    if(this->DATA[index].data==NONE){//새 노드라면,
        this->DATA[index].data = data;
        findR();
        return probe;
    }
    while(this->DATA[index].link!=NONE){
        index = this->DATA[index].link;
        probe++;
    }
    this->DATA[this->R].data = data;
    this->DATA[index].link = this->R;
    probe++;
    this->findR();
    return probe;
}

bool LISCH::del_start(int s){
    int p = NONE;//r의 부모 원소
    int r = NONE;//s index 의 체인중에 가장 마지막에 있는 HASH가 s인 원소 
    int prenode = s;
    int nownode = this->get(s).link;
    while(nownode!=NONE){
        //HASH가 s인 노드 발견시 p와 r 갱신
        if(this->hash(this->get(nownode).data) == s){
            p = prenode;
            r = nownode;
        }
        //nownode 과 prenode를 한칸씩 이동
        prenode = nownode;
        nownode = this->get(nownode).link;
    }
    if(r==NONE){//HASH가 s인 원소를 찾지 못함
        //그냥 삭제
        this->DATA[s].data = NONE;
        this->DATA[s].link = NONE;
        return true;
    }
    //r을 s로 복사하고 r을 삭제
    this->DATA[s].data = this->DATA[r].data;
    this->DATA[s].link = this->DATA[r].link;
    return this->del_middle(r,p);
}

bool LISCH::del_middle(int r,int p){
    return true;
}

void LISCH::printNode(){
    cout << "HASH | RECORD | VALUE | LINK "<<endl;
    for(int i = 0;i<this->size;i++){
        NODE node = this->DATA[i];
        printf("%-5d  ",i);
        if(node.data==NONE){
            cout << "         ";
            cout << "        ";
        }else{
            printf("%-7d  ",node.data);
            printf("%-6d  ",this->hash(node.data));
        }
        if(node.link==NONE){
            cout << "     ";
        }else{
            printf("%-5d",node.link);
        }
        cout<<endl;
    }
    cout << endl<<endl <<"파생 노드:"<<endl;
}