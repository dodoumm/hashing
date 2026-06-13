#include "LISCH.h"
    /*
    nodes.count = 15;
    nodes.datas = new int[nodes.count]{18,5,7,10,14,32,56,78,83,87,97,5,5,34,35,31};
    */


int main(){
    NODE_SET nodes;
    nodes.printinsert=true;
    nodes.count = 5;
    nodes.datas = new int[nodes.count]{5,18,12,25,31};
    LISCH hash = LISCH(13,&nodes);
    hash.printNode();
}