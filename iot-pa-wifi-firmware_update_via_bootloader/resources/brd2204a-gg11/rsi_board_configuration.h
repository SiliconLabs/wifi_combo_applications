#ifndef RSI_BOARD_CONFIG_H
#define RSI_BOARD_CONFIG_H

typedef struct {
    unsigned char port;
    unsigned char pin;
} rsi_pin_t;

#define PIN(port_id, pin_id)  (rsi_pin_t){.port=gpioPort##port_id, .pin=pin_id}

#define SLEEP_CONFIRM_PIN  PIN(A, 12)
#define WAKE_INDICATOR_PIN PIN(A, 13)
#define RESET_PIN          PIN(B, 11)

#define SPI_CLOCK_PIN PIN(E, 12) // PIN(C, 2)
#define SPI_MOSI_PIN  PIN(E, 10) // PIN(C, 0)
#define SPI_MISO_PIN  PIN(E, 11) // PIN(C, 1)
#define SPI_CS_PIN    PIN(E, 13) // PIN(C, 3)

#define INTERRUPT_PIN PIN(B, 9)



#define COM_PORT_PERIPHERAL        USART4
#define COM_PORT_BAUDRATE              115200
#define COM_PORT_PARITY                usartNoParity
#define COM_PORT_STOP_BITS             usartStopbits1
#define COM_PORT_FLOW_CONTROL_TYPE     usartHwFlowControlNone
#define COM_PORT_TX_PIN           PIN(H, 4)
#define COM_PORT_RX_PIN           PIN(H, 5)
#define COM_PORT_PERIPHERAL_NO     0
//#define COM_PORT_CLOCK            CONCAT_EXPAND(cmuClock_USART, COM_PORT_PERIPHERAL_NO)
//#define COM_PORT_NVIC             CONCAT3_EXPAND( USART, COM_PORT_PERIPHERAL_NO, _RX_IRQn)






#define RX_LDMA_CHANNEL   0
#define TX_LDMA_CHANNEL   1

#define RSI_SPI_INTERFACE
#define RSI_SPI_HIGH_SPEED_ENABLE

// Declaration of MCU type
#define RSI_ARM_CM4F

#endif // RSI_BOARD_CONFIG_H
