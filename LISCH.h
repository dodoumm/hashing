#pragma once
#include <iostream>
using namespace std;

#define EMPTY -1


typedef struct NODE{
    int data=EMPTY;
    int link=EMPTY;
} NODE;

typedef struct NODE_SET{
    int*datas;
    int count;
    bool printinsert;
} NODE_SET;

typedef struct SEARCH_RES{
    int probe = 0;
    int index = EMPTY;
    int pindex = EMPTY;
} SEARCH_RES;


class LISCH{
    private:
        NODE* DATA;
        const int size=11;
        int R;
        void log(string log);
        void findR();
        void setR(int R);
        void del_start(int s);//s = HASH KEY
        void del_middle(int r,int p);//r = DELETE KEY | p = s->PARENT
    public:
        LISCH(int size,NODE_SET *set);
        ~LISCH();
        int hash(int data);
        NODE get(int h);
        SEARCH_RES searchNode(int data);
        int insertNode_print(int data);
        int insertNode(int data);
        bool deleteNode(int data);
        void printNode();
};