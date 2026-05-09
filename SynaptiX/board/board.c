#include "board.h"
#include <assert.h>
#include "cqueue.h"
#include "cJSON.h"
#include "logger.h"
#include "dts.h"
#include <stdio.h>
#include "rng.h"
#include "rtc.h"
#include "w25q_mem.h"

static const char *TAG = "BSP";

typedef struct BSP_UART
{
    int port;
    CQueue_t rx_queue;
    SemaphoreHandle_t mutex;
    size_t rx_count;
    uint8_t *rx_buffer;
} BSP_UART_t;

UART_HandleTypeDef *hal_uart[BSP_UART_PORT_NUM] = {&huart1, &huart7, &huart3, &huart10, &huart9}; // 0 : LOG, 1 : RS485-1, 2 : RS485-2, 3 : RS485-3, 4 : RF

BSP_UART_t bsp_uarts[BSP_UART_PORT_NUM] = {
    {.port = LOG_PORT, .mutex = NULL, .rx_count = 0, .rx_buffer = NULL},     // Debug
    {.port = RS485_0_PORT, .mutex = NULL, .rx_count = 0, .rx_buffer = NULL}, // RS485-1
    {.port = RS485_1_PORT, .mutex = NULL, .rx_count = 0, .rx_buffer = NULL}, // RS485-2
    {.port = RS485_2_PORT, .mutex = NULL, .rx_count = 0, .rx_buffer = NULL}, // RS485-3
    {.port = RF_PORT, .mutex = NULL, .rx_count = 0, .rx_buffer = NULL}       // RF
};

uint8_t uart_data[BSP_UART_PORT_NUM];
#if USB_ENABLE
#define USB_BUFF_SIZE 512
CQueue_t usb_queue[USB_MAX_CHANNEL];
uint8_t usb_buff[USB_MAX_CHANNEL][USB_BUFF_SIZE];
#endif

volatile cJSON_Hooks jsonHook;

void *bsp_malloc(size_t size)
{
    return pvPortMalloc(size);
}
void bsp_free(void *p)
{
    vPortFree(p);
}
void *bsp_calloc(size_t num, size_t size)
{
    return pvPortCalloc(num, size);
}

void board_init(void)
{
    HAL_DTS_Start(&hdts);
    W25Q_Init();
    jsonHook.free_fn = bsp_free;
    jsonHook.malloc_fn = bsp_malloc;
    cJSON_InitHooks((cJSON_Hooks *)&jsonHook);
    for (int i = 0; i < BSP_UART_PORT_NUM; i++)
    {
        uart_init(i, 115200, 4096);
        HAL_UART_Receive_IT(hal_uart[i], &uart_data[i], 1);
    }

#if USB_ENABLE
    for (int i = 0; i < USB_MAX_CHANNEL; i++)
    {
        cqueue_init_static(&usb_queue[i], usb_buff[i], USB_BUFF_SIZE, sizeof(uint8_t));
    }
#endif
}

void board_deinit(void)
{
}

void uart_init(int port, int baudrate, uint32_t rx_length)
{
    if (port < 0 || port > BSP_UART_PORT_NUM)
        return;
    bsp_uarts[port].rx_buffer = malloc(rx_length);
    assert(bsp_uarts[port].rx_buffer != NULL);
    bsp_uarts[port].rx_count = 0;
    cqueue_init_static(&bsp_uarts[port].rx_queue, bsp_uarts[port].rx_buffer, rx_length, sizeof(uint8_t));
    bsp_uarts[port].mutex = xSemaphoreCreateMutex();
    assert(bsp_uarts[port].mutex != NULL);
}

void usart_de_on(int port)
{

    switch (port)
    {
    case RS485_0_PORT:
        /* code */
        HAL_GPIO_WritePin(RS485_0_DE_GPIO_Port, RS485_0_DE_Pin, GPIO_PIN_SET);
        break;
    case RS485_1_PORT:
        /* code */
        HAL_GPIO_WritePin(RS485_1_DE_GPIO_Port, RS485_1_DE_Pin, GPIO_PIN_SET);
        break;
    case RS485_2_PORT:
        /* code */
        HAL_GPIO_WritePin(RS485_2_DE_GPIO_Port, RS485_2_DE_Pin, GPIO_PIN_SET);
        break;
    default:
        break;
    }
}

void usart_de_off(int port)
{

    switch (port)
    {
    case RS485_0_PORT:
        /* code */
        HAL_GPIO_WritePin(RS485_0_DE_GPIO_Port, RS485_0_DE_Pin, GPIO_PIN_RESET);
        break;
    case RS485_1_PORT:
        /* code */
        HAL_GPIO_WritePin(RS485_1_DE_GPIO_Port, RS485_1_DE_Pin, GPIO_PIN_RESET);
        break;
    case RS485_2_PORT:
        /* code */
        HAL_GPIO_WritePin(RS485_2_DE_GPIO_Port, RS485_2_DE_Pin, GPIO_PIN_RESET);
        break;
    default:
        break;
    }
}

void uart_write_bytes(int port, const uint8_t *data, uint32_t length)
{
    if (port < 0 || port > BSP_UART_PORT_NUM)
        return;
    xSemaphoreTake(bsp_uarts[port].mutex, portMAX_DELAY);
    usart_de_on(port);
    HAL_UART_Transmit(hal_uart[port], (uint8_t *)data, length, HAL_MAX_DELAY);
    usart_de_off(port);
    xSemaphoreGive(bsp_uarts[port].mutex);
}

int uart_read_bytes(int port, uint8_t *data, uint32_t length,
                    uint32_t ticks_to_wait)
{
    if (port < 0 || port > BSP_UART_PORT_NUM)
        return 0;
    uint32_t timeout = bsp_get_tick() + length;
    size_t count = 0;
    while (count < length)
    {
        if(bsp_get_tick() > timeout) break;
        if (cqueue_receive(&bsp_uarts[port].rx_queue, &data[count]) == true)
        {
            count++;
        }
        else
        {
            bsp_delay(1);
        }
    }
    return count;
}

int uart_get_buffered_data_len(int port, size_t *length)
{
    if (port < 0 || port > BSP_UART_PORT_NUM)
        return -1;

    *length = bsp_uarts[port].rx_queue.count;
    return 0;
}

void uart_flush(int port)
{
    if (port < 0 || port > BSP_UART_PORT_NUM)
        return;

    bsp_uarts[port].rx_queue.count = 0;
    bsp_uarts[port].rx_queue.head = 0;
    bsp_uarts[port].rx_queue.tail = 0;
    bsp_uarts[port].rx_count = 0;
}

float bsp_get_mcu_temperature()
{
    ADC_ChannelConfTypeDef sConfig = {0};
    uint32_t adc_value = 0;
    float temperature = 0.0f;

    // Configure ADC channel to internal temperature sensor
    sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_24CYCLES_5; // ensure long enough sampling time
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;

    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    // Start ADC conversion
    HAL_ADC_Start(&hadc1);

    // Wait for conversion
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

    // Read ADC value
    adc_value = HAL_ADC_GetValue(&hadc1);

    HAL_ADC_Stop(&hadc1);

    // --- Convert ADC value to temperature ---
    // Get calibration values from system memory
    uint32_t *TS_CAL1 = (uint32_t *)0x08FFF810; // 30°C calibration
    uint32_t *TS_CAL2 = (uint32_t *)0x08FFF814; // 130°C calibration
    float temp30 = (float)(*TS_CAL1);
    float temp130 = (float)(*TS_CAL2);

    // Convert to Celsius
    temperature = ((float)(adc_value - temp30) * (130.0f - 30.0f) / (temp130 - temp30)) + 30.0f;

    return temperature;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    for (int i = 0; i < BSP_UART_PORT_NUM; i++)
    {
        if (huart->Instance == hal_uart[i]->Instance)
        {
            cqueue_send(&bsp_uarts[i].rx_queue, &uart_data[i]);
            bsp_uarts[i].rx_count++;
            HAL_UART_Receive_IT(hal_uart[i], &uart_data[i], 1); // RS485
            return;
        }
    }
}
#if USB_ENABLE

#include "usbd_cdc_acm_if.h"

void bsp_restart()
{
    for (int i = 0; i < BSP_UART_PORT_NUM; i++)
    {
        HAL_UART_DeInit(hal_uart[i]);
    }
    HAL_ADC_DeInit(&hadc1);
    HAL_RNG_DeInit(&hrng);
    HAL_RTC_DeInit(&hrtc);
    HAL_DTS_DeInit(&hdts);
    HAL_RCC_DeInit();
    HAL_DeInit();
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;
    SCB->VTOR = FLASH_BASE;
    __set_MSP(*(volatile uint32_t *)FLASH_BASE);
    void (*app_reset_handler)(void) = (void *)(*((volatile uint32_t *)(FLASH_BASE + 4U)));
    NVIC_SystemReset();
}

int bsp_usb_cdc_transmit(int ch, uint8_t *buff, size_t len)
{
    if (0 > ch && ch >= USB_MAX_CHANNEL)
        return -1;
    return CDC_Transmit(ch, buff, len);
}

int bsp_usb_cdc_read(int ch, uint8_t *buff, size_t len)
{
    if (0 > ch && ch >= USB_MAX_CHANNEL)
        return -1;
    size_t i = 0;
    for (i = 0; i < len; i++)
        if (!cqueue_receive(&usb_queue[ch], &buff[i]))
            break;
    return i;
}

int bsp_usb_cdc_rx_callback(int ch, uint8_t *buff, size_t len)
{
    if (0 > ch && ch >= USB_MAX_CHANNEL)
        return -1;
    // log_debug(TAG,"USB CH %d Receive %lu bytes",ch,len);
    size_t i = 0;
    for (i = 0; i < len; i++)
        if (!cqueue_send(&usb_queue[ch], &buff[i]))
            break;
    return (int)i;
}
uint32_t bsp_usb_cdc_available(int ch)
{
    if (0 > ch && ch >= USB_MAX_CHANNEL)
        return 0;
    return usb_queue[ch].count;
}
void bsp_usb_cdc_flush(int ch)
{
    if (0 > ch && ch >= USB_MAX_CHANNEL)
        return;
    cqueue_init_static(&usb_queue[ch], usb_buff[ch], USB_BUFF_SIZE, sizeof(uint8_t));
}
#endif

#include "rtc.h"

struct tm sTime;

struct tm *bsp_get_time()
{
    RTC_TimeTypeDef gTime;
    RTC_DateTypeDef gDate;

    HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);

    sTime.tm_hour = gTime.Hours;
    sTime.tm_min = gTime.Minutes;
    sTime.tm_sec = gTime.Seconds;
    sTime.tm_wday = gDate.Date;
    sTime.tm_mon = gDate.Month;
    sTime.tm_year = gDate.Year + 100;
    sTime.tm_yday = gDate.Date;
    return &sTime;
}
void bsp_set_time(struct tm *time)
{
    RTC_TimeTypeDef gTime = {.Hours = time->tm_hour, .Minutes = time->tm_min, .Seconds = time->tm_sec};
    RTC_DateTypeDef gDate = {.Year = time->tm_year - 100, .Month = time->tm_mon, .Date = time->tm_mday};
    HAL_RTC_SetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &gDate, RTC_FORMAT_BIN);
}

int bsp_get_temp()
{
    int32_t Temperature;
    HAL_DTS_GetTemperature(&hdts, &Temperature);
    return Temperature;
}

void assert_failed(uint8_t *file, uint32_t line)
{
    printf("assert failed file : %s , line : %lu \r\n", file, line);
}