#pragma once
#define NONE -1


typedef struct NODE{
    int data=NONE;
    int link=NONE;
} NODE;

typedef struct NODE_SET{
    int*datas;
    int count;
    bool printinsert;
} NODE_SET;

typedef struct SEARCH_RES{
    int probe = 0;
    int index = NONE;
    int pindex = NONE;
} SEARCH_RES;


class LISCH{
    private:
        NODE* DATA;
        int size=11;
        int R;
        void findR();
        void setR(int R);
        bool del_start(int s);//s = HASH KEY
        bool del_middle(int r,int p);//r = DELETE KEY | p = s->PARENT
    public:
        LISCH(int size,NODE_SET *set);
        int hash(int data);
        NODE get(int h);
        SEARCH_RES searchNode(int data);
        int insertNode_print(int data);
        int insertNode(int data);
        bool deleteNode(int data);
        void printNode();
};