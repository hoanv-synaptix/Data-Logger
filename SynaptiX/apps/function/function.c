#include "function.h"
#include "sx_485_io.h"
#include <string.h>
#include <stdlib.h>
#include "logger.h"
#include "app.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

static int set_output(void *arg,cJSON *params);
static int set_all_output(void *arg,cJSON *params);
static int reset_counter(void *arg,cJSON *params);

static const RPC_t rpc_funcs_commands[] = {
    {.name = "setOutput",.arg = NULL,.func = set_output},
    {.name = "setAllOutput",.arg = NULL,.func = set_all_output},
    {.name = "resetCouter",.arg = NULL,.func = reset_counter},
};

static const char *TAG = "Function";

// extern SX_485_IO_t sx485io[NUM_SX485IO_DEV];

SX_485_IO_t *getSX485IO_dev(uint8_t id){
   SX_485_IO_t *dev = NULL;
    for(int i = 0;i<gateway.numDevIO;i++){
        if(gateway.devio[i].dev.id == id){
            dev = &gateway.devio[i];
            break;
        }
    } 
    return dev;
}

int set_output(void *arg, cJSON *params)
{
    cJSON *id = cJSON_GetObjectItem(params,"id");
    cJSON *output = cJSON_GetObjectItem(params,"output");
    cJSON *state = cJSON_GetObjectItem(params,"state");

    if(id == NULL || output == NULL || state == NULL) return -1;

    if(cJSON_IsNumber(id) && cJSON_IsNumber(output) && cJSON_IsBool(state)){
        SX_485_IO_t *dev = getSX485IO_dev(cJSON_GetNumberValue(id));
        if(dev != NULL){
           bool st = (cJSON_IsTrue(state) ? true : false);
           return SX485IO_SetOutput(dev,cJSON_GetNumberValue(output),st);
        }
    }

    return -2;
}

int set_all_output(void *arg, cJSON *params)
{
    return 0;
}

int reset_counter(void *arg, cJSON *params)
{
    cJSON *id = cJSON_GetObjectItem(params,"id");
    cJSON *counter = cJSON_GetObjectItem(params,"counter");

    if(id == NULL || counter == NULL) return -1;

    if(cJSON_IsNumber(id) && cJSON_IsNumber(counter)){
        SX_485_IO_t *dev = getSX485IO_dev(cJSON_GetNumberValue(id));
        if(dev != NULL){
           return SX485IO_ResetCounter(dev,cJSON_GetNumberValue(counter));
        }
    }
    return -2;
}

const RPC_t  *const rpc_funcs = rpc_funcs_commands;
const size_t rpc_funcs_nums = ARRAY_SIZE(rpc_funcs_commands);