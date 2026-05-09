#include "sx_dev.h"
#include <stdlib.h>
#include <assert.h>
#include "logger.h"

static const char *TAG = "SX-DEV";


void sx_dev_init(SXDev_t *dev, MMBCLient_t *mmb, SXDevConfig_t *config, uint8_t id)
{
    assert(config != NULL);
    assert(mmb != NULL);
    assert(id > 0);

    dev->config = config;
    if (config->holding_reg_num > 0){
        dev->holding_reg = (uint16_t *)malloc(dev->config->holding_reg_num * sizeof(uint16_t));
        memset(dev->holding_reg,0,dev->config->holding_reg_num * sizeof(uint16_t));
    }
    if (config->input_reg_num > 0){
        dev->input_reg = (uint16_t *)malloc(dev->config->input_reg_num * sizeof(uint16_t));
        memset(dev->input_reg,0,dev->config->input_reg_num * sizeof(uint16_t));
    }
    if (config->coil_reg_num > 0)
    {
        dev->coil_reg = (uint8_t *)malloc(dev->config->coil_reg_num);
        memset(dev->coil_reg,0,dev->config->coil_reg_num);
    }
    if (config->discrete_reg_num > 0)
    {
        dev->discrete_reg = (uint8_t *)malloc(dev->config->discrete_reg_num);
        memset(dev->discrete_reg,0,dev->config->discrete_reg_num);
    }
    dev->id = id;
    dev->isConnect = false;
    dev->mmb = mmb;
}

void sx_dev_poll(SXDev_t *dev)
{
    if (dev->config->holding_reg_num > 0){
        (0 == mmb_read_holding_registers(dev->mmb, dev->id, dev->config->holding_reg_start_addr, dev->config->holding_reg_num, dev->holding_reg)) ? (dev->isConnect = true) : (dev->isConnect = false);
        bsp_delay(1);
    }
    if (dev->config->input_reg_num > 0){
        (0 == mmb_read_input_registers(dev->mmb, dev->id, dev->config->input_reg_start_addr, dev->config->input_reg_num, dev->input_reg)) ? (dev->isConnect = true) : (dev->isConnect = false);
        bsp_delay(1);
    }
    if (dev->config->coil_reg_num > 0){
        (0 == mmb_read_coils(dev->mmb, dev->id, dev->config->coil_reg_start_addr, dev->config->coil_reg_num, dev->coil_reg)) ? (dev->isConnect = true) : (dev->isConnect = false);
        bsp_delay(1);
    }
    if (dev->config->discrete_reg_num > 0){
        if(0 == mmb_read_discrete_inputs(dev->mmb, dev->id, dev->config->discrete_reg_start_addr, dev->config->discrete_reg_num, &dev->discrete_reg[0])) {
            dev->isConnect = true;
            // log_debug(TAG, "Read discrete inputs success for device ID %d", dev->id);
            // log_print_hex(LOGGER_DEBUG, TAG, dev->discrete_reg, (dev->config->discrete_reg_num));
        } else {
            dev->isConnect = false;
        } 
        bsp_delay(1);
    }
}
