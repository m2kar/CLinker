//
// Created by sony on 2017/11/3.
//

#ifndef TEST1_B_H
#define TEST1_B_H

#define MAX_FILE_COUNT 100
#define MAX_FUNC_COUNT 10000
#define NA 30000

typedef enum{False=-1,True=0} Bool;

typedef struct{
    unsigned int funcId;
    unsigned int fileVex;
    unsigned int pCallLevel;
} Func;  //定义函数类型

typedef struct{
    unsigned int fileId;
    unsigned int firstFuncVex;
    unsigned int funcNum;
    Bool isLinked;
} File ;

typedef struct ArchBox{
    unsigned int tailvex;  //tail of arch
    unsigned int headvex;  //head of arch
    struct  ArchBox * tLink; // next arch with same tail
    struct ArchBox * hLink; // next arch with same head
}ArchBox; //调用关系Node

typedef struct{
    Func data;
    ArchBox * firstIn;
    ArchBox * firstOut;
}VexNode;

typedef struct{
    //TODO 研究一下怎么把它变成二叉树来实现查找 中值查找
    File fileList[MAX_FILE_COUNT];
    unsigned int fileCount;
    VexNode funcList[MAX_FUNC_COUNT];
    //todo 待会设成动态分配
    unsigned int funcCount;
//    unsigned int archCount;
} LinkerGraph;

extern LinkerGraph _linker;

int AddFile(unsigned int FileId, unsigned int FuncIdList[], unsigned int FuncNum);

int AddCallList(unsigned int CallerId, unsigned int CalleeIdList[], unsigned int CalleeNum);

int IsFileLinked(unsigned int FileId);

int IsCallRelation(unsigned int FuncId, unsigned int * pCallLevel);

void Clear(void);

#endif //TEST1_B_H
