/*
 * Mini typewriter using the JP-QR701-TTL thermal printer and the
 * TI TM4C1294NCPDT ARM based microcontroller Launchpad. The thermal printer
 * is available from Adafruit or AliExpress
 *
 * The connections are a virtual serial connection over USB from a terminal
 * emulator like PuTTY to UART0 of the TM41294 over port A0 to receive and
 * port A1 to transmit, and a TTL serial connection from UART3 of the TM1294
 * to the thermal printer over port A4 to receive and port A5 to transmit
 *
 * Add the TivaWare peripheral driver library root directory to the #include
 * search path and add a link to the TivaWare file driverlib.lib to the project
 *
 * Written by Jonathan Kho
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

uint32_t ui32SysClkFreq;

int main(void)
{
	ui32SysClkFreq = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
			SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
			SYSCTL_CFG_VCO_480), 120000000);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); // Input from terminal emulator
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3); // Output to thermal printer
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinConfigure(GPIO_PA4_U3RX);
	GPIOPinConfigure(GPIO_PA5_U3TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1 |
			GPIO_PIN_4 | GPIO_PIN_5);

	UARTConfigSetExpClk(UART0_BASE, ui32SysClkFreq, 115200, (UART_CONFIG_WLEN_8 |
			UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	UARTConfigSetExpClk(UART3_BASE, ui32SysClkFreq, 9600, (UART_CONFIG_WLEN_8 |
			UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	while(1)
	{
		if (UARTCharsAvail(UART0_BASE))
		{
			UARTCharPut(UART3_BASE, UARTCharGet(UART0_BASE));
		}
	}

}
