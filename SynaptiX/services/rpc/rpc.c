#include "rpc.h"
#include "stdint.h"
#include "string.h"


#define RPC_FOREACH_FUNC(func) \
    for(const RPC_t *func; \
        func < &rpc_funcs[rpc_funcs_nums]; \
        ++func)
        
int RPC_Call(char *name, cJSON *param)
{
    RPC_FOREACH_FUNC(func){
        if(strcmp(func->name,name) == 0){
            return func->func(func->arg,param);
        }
    }
    return -1;
}