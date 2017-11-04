#include <stdio.h>
#include <stdlib.h>

#include "linker.h"

LinkerGraph _linker={{},0,{},0};

int findFile(unsigned int FileId){
    int i;
    for(i=_linker.fileCount-1;i>=0;i--){
        if(_linker.fileList[i].fileId==FileId){
            return i;
        }
    }
    return -1; //not found
}

int findFunc(unsigned int FuncId){
    int i;
    for(i=_linker.funcCount-1;i>=0;i--){
        if(_linker.funcList[i].data.funcId==FuncId) return i;
    }
    return -1; //not found
}

void updateCallLevel(unsigned int tailVex, unsigned int nLevel){
    VexNode * pTV=&_linker.funcList[tailVex];
    if(nLevel<NA && nLevel<pTV->data.pCallLevel){
        pTV->data.pCallLevel=nLevel;
        _linker.fileList[pTV->data.fileVex].isLinked=True;
        for (ArchBox*next=pTV->firstOut;next!=NULL;next=next->tLink){
            updateCallLevel(next->headvex,nLevel+1);
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////
//下面是要求的接口
int AddFile(unsigned int FileId, unsigned int FuncIdList[], unsigned int FuncNum){
    if (findFile(FileId)!=-1) return -1;
//    _linker.fileList[0]={11,50,3,False};
    _linker.fileList[_linker.fileCount]=(File){FileId,_linker.funcCount,FuncNum,False};
    int d,s;
    for(d=_linker.funcCount,s=0;s<FuncNum;s++,d++){
        if (findFunc(FuncIdList[s])!=-1) return -1;
        _linker.funcList[d]=(VexNode){
                {
                        FuncIdList[s],
                        _linker.fileCount,
                        FuncIdList[s]==11?0:NA
                },
                NULL,
                NULL
        };
    }
    _linker.funcCount+=FuncNum;
    _linker.fileCount++;
    return 0;
}

int AddCallList(unsigned int CallerId, unsigned int CalleeIdList[], unsigned int CalleeNum){
    int tV=findFunc(CallerId); //tail vex: callerList
    if(tV==-1) return -1;
    int * hVL=(int*)malloc(sizeof(int)*CalleeNum); // head vex list:calleeList
    int i;
    for(i=0;i<CalleeNum;i++){
        if (-1==(hVL[i]=findFunc(CalleeIdList[i]))) {
            free(hVL);
            return -1;
        }
    }
    ArchBox * pArc;
    VexNode * pHV,* pTV;
    for(i=0;i<CalleeNum;i++){
        pHV=&_linker.funcList[hVL[i]];
        pTV=&_linker.funcList[tV];
        pArc=(ArchBox*)malloc(sizeof(ArchBox));
        pArc->tailvex=tV;
        pArc->headvex=hVL[i];
        pArc->tLink=pTV->firstOut; //insert to tail VexNode
        pTV->firstOut=pArc;
        pArc->hLink=pHV->firstIn;//insert to head VexNode
        pHV->firstIn=pArc;
        unsigned int tailLevel=pTV->data.pCallLevel;
        updateCallLevel(hVL[i],tailLevel+1);
    }
    free(hVL);
    return 0;
}
int IsFileLinked(unsigned int FileId){
    int fileVex;
    if ((fileVex=findFile(FileId))==-1) return -1;
    else return (int) _linker.fileList[fileVex].isLinked;
}

int IsCallRelation(unsigned int FuncId, unsigned int * pCallLevel){
    int funcVex;
    if ((funcVex=findFunc(FuncId))==-1) return -1;
    unsigned int l=_linker.funcList[funcVex].data.pCallLevel;
    if (l>=NA) return -1;
    else{
        *pCallLevel=l;
        return 0;
    }
}

void Clear(){
    int i;
    for(i=0;i<_linker.funcCount;i++){
        ArchBox*p1=_linker.funcList[i].firstOut,*p2;
        while(p1!=NULL){
            p2=p1->tLink;
            free(p1);
            p1=p2;
        }
    }
    _linker.funcCount=0;
    _linker.fileCount=0;
    return;
}
