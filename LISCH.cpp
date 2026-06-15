#include "LISCH.h"

LISCH::~LISCH(){
    if(this->DATA != NULL) delete[] this->DATA;
}

void LISCH::log(string log){
    cout << ":: "<<log<<endl;
    return;
}

LISCH::LISCH(int count,NODE_SET *set=NULL):size(count){
    this->DATA = new NODE[this->size];
    this->R = this->size-1;
    for(int i=0; i<count; i++){
        this->DATA[i].data = EMPTY;
        this->DATA[i].link = EMPTY;
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
    if(data==EMPTY){
        cout << "NULL"<<endl;
        return 0;
    }
    SEARCH_RES search = this->searchNode(data);
    if(search.index!=EMPTY){//이미 존재하는 원소
        cout << "insert denied : found in ${"<<search.index<<"}"<<endl<<endl;
        return 0;
    }
    int index = this->hash(data);
    int probe = 1;
    cout << index;
    if(this->DATA[index].data==EMPTY){//새 노드라면,
        this->DATA[index].data = data;
        cout<<endl<<"probe = "<<probe<<endl<<endl;
        findR();
        return probe;
    }
    while(this->DATA[index].link!=EMPTY){
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
        if(this->DATA[this->R].data==EMPTY) break;
        this->R--;
    }
}

void LISCH::setR(int R){
    if(R>this->size-1 || R<0) return;
    this->R = R;
}
SEARCH_RES LISCH::searchNode(int data){
    SEARCH_RES res = SEARCH_RES();
    if(data==EMPTY) return res;
    res.index = this->hash(data);
    res.probe = 1;
    if(this->DATA[res.index].data == data) return res;
    while(this->DATA[res.index].link!=EMPTY){
        res.pindex = res.index;
        res.index = this->DATA[res.index].link;
        res.probe++;
        if(this->DATA[res.index].data == data) return res;
    }
    res.index=EMPTY;
    return res;//탐색 실패
}

int LISCH::insertNode(int data){
    if(data==EMPTY||this->R<0) return 0;
    SEARCH_RES search = this->searchNode(data);
    if(search.index!=EMPTY){//이미 존재하는 원소
        return 0;
    }
    int index = this->hash(data);
    int probe = 1;
    if(this->DATA[index].data==EMPTY){//새 노드라면,
        this->DATA[index].data = data;
        findR();
        return probe;
    }
    while(this->DATA[index].link!=EMPTY){
        index = this->DATA[index].link;
        probe++;
    }
    this->DATA[this->R].data = data;
    this->DATA[index].link = this->R;
    probe++;
    this->findR();
    return probe;
}

void LISCH::del_start(int s){
    this->log("del_start "+to_string(s));
    int p = EMPTY;//r의 부모 원소
    int r = EMPTY;//s index 의 체인중에 가장 마지막에 있는 HASH가 s인 원소 
    int prev = s;
    int curr = this->get(s).link;
    while(curr!=EMPTY){
        //HASH가 s인 노드 발견시 p와 r 갱신
        if(this->hash(this->get(curr).data) == s){
            p = prev;
            r = curr;
        }
        //curr 과 prev를 한칸씩 이동
        prev = curr;
        curr = this->get(curr).link;
    }
    if(r==EMPTY){//HASH가 s인 원소를 찾지 못함
        //그냥 삭제
        this->log("deleted as case [1][start]\n");
        //this->log("=> deleted "+to_string(s)+"\n");
        this->DATA[s].data = EMPTY;
        this->DATA[s].link = EMPTY;
        return;
    }
    //r을 s로 복사하고 r을 삭제
    this->log("deleted as case [2][start]\n");
    //this->log("=> "+to_string(s)+" link = EMPTY and del_middle("+to_string(r)+","+to_string(p)+")\n");
    this->DATA[s].data = this->DATA[r].data;
    this->DATA[s].link = this->DATA[r].link;
    this->del_middle(r,p);
    return;
}

void LISCH::del_middle(int r,int p){
    this->log("del_middle "+to_string(r)+" (p="+to_string(p)+")");
    // 1: 중간에 있는 원소만 빼고 앞 뒤 이어주기
    // -> 삭제할 원소의 [index]를 hash(data)로 갖는 레코드가 뒤에 없을때:
    // => [index]를 hash로 가지는 오버플로우된(파생된) 원소가 없기 때문에 뒤는 신경쓰지 않고 잘라내도 된다. 

    // 2: 뒤에서 레코드를 가져오고, 중간 사슬을 끊어서 두개의 분리된 체인으로 만들기
    // -> 삭제할 노드 뒤의 노드들이 모두 삭제할 노드 뒤에서부터 파생된 경우:
    // => 삭제할 노드 뒤의 모든 노드들의 hash(data)가 삭제할 노드 뒤의 노드들의 [index] 집합에 포함되는경우,
    // 삭제할 노드 앞에서부터 오버플로우된(파생된) 노드가 없으므로 연결을 끊어도 됨

    // 3: 뒤에서 레코드를 가져오고, 이어진 체인으로 두기
    // -> 삭제할 노드 뒤의 노드들중 앞에서 파생된 노드가 있을때(2가 아닐때):
    // =>

    // 1이 아니라면 2, 2가 아니라면 3 수행
    int curr = this->get(r).link;
    while(curr!=EMPTY){
        //HASH(data)가 r인 노드 발견시 (2,3) 수행
        if(this->hash(this->get(curr).data) == r){
            this->log("found hash["+to_string(r)+"] in "+to_string(curr)+"(data="+to_string(this->get(curr).data)+")");
            curr = r;//처음부터 다시 순회
            bool *D = new bool[this->size]();// 뒤의 index 배열
            string dstring = "";//출력용 텍스트
            while(curr!=EMPTY){
                dstring += to_string(curr)+", ";
                D[curr] = true;
                //curr를 한칸씩 이동
                curr = this->get(curr).link;
            }
            this->log("created D as ["+dstring+"\b\b]");
            int prev = r;
            curr = this->get(r).link;//처음부터 다시 순회
            while(curr!=EMPTY){
                //hash(data)가 D에 포함되지 않는 노드 발견됨. (3) 수행
                if(!D[this->hash(this->get(curr).data)]){
                    this->log("found unincluded hash(data["+to_string(this->get(curr).data)+"])["+to_string(this->hash(this->get(curr).data))+"] in D");
                    this->log("deleted as case [3]\n");
                    //발견한 노드를 r위치로 앞당기고 발견한 노드를 지운다.
                    NODE foundednode = this->get(curr);
                    this->DATA[r].data = foundednode.data;
                    delete[] D;
                    this->del_middle(curr,prev);
                    return;
                }
                //curr 과 prev를 한칸씩 이동
                prev = curr;
                curr = this->get(curr).link;
            }
            // hash(data)가 D에 포함되지 않는 노드가 발견되지 않았으므로 (2) 수행.
            this->log("deleted as case [2]\n");
            this->DATA[p].link = EMPTY;
            this->del_start(r);//노드의 맨 앞 삭제 판정
            delete[] D;
            return;
        }
        //curr를 한칸씩 이동
        curr = this->get(curr).link;
    }
    this->log("deleted as case [1]\n");
    //HASH(data)가 r인 노드를 못찾았으므로 (1)실행
    this->DATA[p].link = this->get(r).link;
    this->DATA[r].data = EMPTY;
    this->DATA[r].link = EMPTY;
    return;
}

bool LISCH::deleteNode(int data){
    this->log("deletenode "+to_string(data));
    if(data < 0) return false;
    SEARCH_RES search = this->searchNode(data);
    if(search.index==EMPTY) return false;
    if(data == search.index){
        this->log("data["+to_string(data)+"] = hash(data["+to_string(data)+"])["+to_string(search.index)+"] -> del_start\n");
        this->del_start(search.index);
    }else{
        this->log("data["+to_string(data)+"] ≠ hash(data["+to_string(data)+"])["+to_string(search.index)+"] -> del_middle\n");
        this->del_middle(search.index,search.pindex);
    }
    return true;
}

void LISCH::printNode(){
    cout << "HASH | RECORD | VALUE | LINK "<<endl;
    for(int i = 0;i<this->size;i++){
        NODE node = this->DATA[i];
        printf("%-5d  ",i);
        if(node.data==EMPTY){
            cout << "         ";
            cout << "        ";
        }else{
            printf("%-7d  ",node.data);
            printf("%-6d  ",this->hash(node.data));
        }
        if(node.link==EMPTY){
            cout << "     ";
        }else{
            printf("%-5d",node.link);
        }
        cout<<endl;
    }
    cout << endl;
    //cout << endl<<endl <<"파생 노드:"<<endl;
}