
 
/**
 * @file    MKL02Z32xxx4_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL02Z4.h"
#include "fsl_debug_console.h"
#include "sdk_hal_gpio1.h"
#include "sdk_hal_uart0.h"
#include "sdk_hal_i2c0.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

#define MMA851_I2C_DEVICE_ADDRESS	0x1D

#define MMA8451_WHO_AM_I_MEMORY_ADDRESS		0x0D
//define para las posiciones en el acelerometro
#define MMA8451_XMSB_MEMORY_ADDRESS		0x01
#define MMA8451_XLSB_MEMORY_ADDRESS		0x02
#define MMA8451_YMSB_MEMORY_ADDRESS		0x03
#define MMA8451_YLSB_MEMORY_ADDRESS		0x04
#define MMA8451_ZMSB_MEMORY_ADDRESS		0x05
#define MMA8451_ZLSB_MEMORY_ADDRESS		0x06


/*
 * @brief   Application entry point.
 */
int main(void) {

status_t status;
status_t status_i2c;
uint8_t nuevo_byte_uart;
uint8_t	nuevo_dato_i2c;
uint16_t nuevo_dato_i2c_1;
uint16_t nuevo_dato_i2c_2;
uint16_t dato_acelerometro;

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    (void)uart0Inicializar(115200);	//115200bps
    (void)i2c0MasterInit(100000);	//100kbps
    status_i2c=i2c0MasterWriteByte(MMA851_I2C_DEVICE_ADDRESS,0X2A, 1);

    PRINTF("Hello World\n");

    PRINTF("Usar teclado para controlar LEDs\r\n");
       PRINTF("r-R led ROJO\r\n");
       PRINTF("v-V led VERDE\r\n");
       PRINTF("a-A led AZUL\r\n");
       PRINTF("M buscar acelerometro\r\n");



    while(1) {
        	if(uart0CuantosDatosHayEnBuffer()>0){
        		status=uart0LeerByteDesdeBuffer(&nuevo_byte_uart);
        		if(status==kStatus_Success){
        			printf("dato:%c\r\n",nuevo_byte_uart);
        			switch (nuevo_byte_uart) {
    				case 'a':
    				case 'A':
    					gpioPutToggle(KPTB10);
    					break;

    				case 'v':
    					gpioPutHigh(KPTB7);
    					break;
    				case 'V':
    					gpioPutLow(KPTB7);
    					break;

    				case 'r':
    					gpioPutValue(KPTB6,1);
    					break;
    				case 'R':
    					gpioPutValue(KPTB6,0);
    					break;
    				case 'M':
    					i2c0MasterReadByte(&nuevo_dato_i2c, MMA851_I2C_DEVICE_ADDRESS, MMA8451_WHO_AM_I_MEMORY_ADDRESS);

    					if(nuevo_dato_i2c==0x1A)
    					    printf("MMA8451 encontrado!!\r\n");
    					else
    						printf("MMA8451 error\r\n");

    					break;

    				case 'X':
    				case 'x':
    			     i2c0MasterReadByte(&nuevo_dato_i2c_1, MMA851_I2C_DEVICE_ADDRESS, MMA8451_XMSB_MEMORY_ADDRESS);
    				 i2c0MasterReadByte(&nuevo_dato_i2c_2, MMA851_I2C_DEVICE_ADDRESS, MMA8451_XLSB_MEMORY_ADDRESS);
    				 nuevo_dato_i2c_1<<=8;
    				   dato_acelerometro=nuevo_dato_i2c_1|nuevo_dato_i2c_2;
    				   dato_acelerometro>>=2;

    				 printf("el valor en X:%d\r\n",dato_acelerometro);
    					break;

    				case 'Y':
    				case 'y':
    				     i2c0MasterReadByte(&nuevo_dato_i2c_1, MMA851_I2C_DEVICE_ADDRESS, MMA8451_YMSB_MEMORY_ADDRESS);
    				     i2c0MasterReadByte(&nuevo_dato_i2c_2, MMA851_I2C_DEVICE_ADDRESS, MMA8451_YLSB_MEMORY_ADDRESS);
    				    	nuevo_dato_i2c_1<<=8;
    				     dato_acelerometro=nuevo_dato_i2c_1|nuevo_dato_i2c_2;
    				     dato_acelerometro>>=2;

    				    				 printf("el valor en Y:",dato_acelerometro);
    				 break;


    				case 'Z':
    				case 'z':
    			         i2c0MasterReadByte(&nuevo_dato_i2c_1, MMA851_I2C_DEVICE_ADDRESS, MMA8451_ZMSB_MEMORY_ADDRESS);
    				     i2c0MasterReadByte(&nuevo_dato_i2c_2, MMA851_I2C_DEVICE_ADDRESS, MMA8451_ZLSB_MEMORY_ADDRESS);
    				        nuevo_dato_i2c_1<<=8;
    				     dato_acelerometro=nuevo_dato_i2c_1|nuevo_dato_i2c_2;
    				     dato_acelerometro>>=2;

    				    				   printf("el valor en Y:",dato_acelerometro);
    				  break;

        			}
        	    		}else{
        	    			printf("error\r\n");
        	    		}
        	    	}
        	    }

    return 0 ;
}
