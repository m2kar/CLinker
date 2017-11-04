#include <stdio.h>
#include "linker.h"

int main() {
    unsigned int file_1_func_list[]={11,15};
    AddFile(1,file_1_func_list,2);
    return 0;
}