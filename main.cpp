#include "LISCH.h"
#include <iostream>
#include <cstdlib>
using namespace std;
    /*
    nodes.count = 15;
    nodes.datas = new int[nodes.count]{18,5,7,10,14,32,56,78,83,87,97,5,5,34,35,31};
    */


int main(){
    NODE_SET nodes;
    nodes.printinsert=true;
    nodes.count = 7;
    nodes.datas = new int[nodes.count]{5,18,12,25,31,9,8};
    LISCH hash = LISCH(13,&nodes);
    hash.printNode();
    //play
    int command;
    int data;
    cout << "0 : reload\n1 <int> : 삽입\n2 <int> : 삭제\n3 : LISCH 출력\n\n$ : ";
    while(true){
        cin >> command;
        if(command>3) continue;
        switch (command){
            case 0:
                system("cls");
                return 0;
            case 1:
                cin >> data;
                hash.insertNode_print(data);
                break;
            case 2:
                cin >> data;
                hash.deleteNode(data);
                break;
            case 3:
                hash.printNode();
                break;
            default:
                break;
        }
        cout <<endl;
    }
}