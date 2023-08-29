/*****************************************************************************/
/*                                                                           */
/* Sistemas Empotrados                                                       */
/* El "hola mundo" en la Redwire EconoTAG en C                               */
/*                                                                           */
/*****************************************************************************/

#include <stdint.h>
#include "system.h"

/*
 * Constantes relativas a la plataforma
 */

/* El led rojo está en el GPIO 44 (el bit 12 de los registros GPIO_X_1) */
#define RED_LED gpio_pin_44

/* El led verde está en el GPIO 45 */
#define GREEN_LED gpio_pin_45

/*****************************************************************************/

/*
 * Inicialización de los pines de E/S
 */
void gpio_init(void){
	/* Configuramos los GPIO44 y GPIO45 para que sean de salida */
	gpio_set_pin_dir_output (RED_LED);
	gpio_set_pin_dir_output (GREEN_LED);
}

/*****************************************************************************/

/*
 * Enciende el led indicado en el pin
 * @param pin Máscara para seleccionar led
 */
void leds_on(uint32_t pin){
	gpio_set_pin(pin);
}

/*****************************************************************************/

/*
 * Apaga el led indicado en el pin
 * @param pin Máscara para seleccionar led
 */
void leds_off (uint32_t pin){
	gpio_clear_pin(pin);
}

/*****************************************************************************/

/**
 * Imprime una cadena de caracteres por la UART1
 * @param str La cadena
 */
void print_str(char * str){
	while (*str){
		uart_send_byte(UART1_ID, *str++);
	}
}

/*****************************************************************************/

/*
 * Programa principal
 */
int main (){
	uint32_t red_led, green_led;	// Máscara del led que se hará parpadear
	uint32_t veces_mensaje = 0;

	gpio_init();

	red_led = 0;
	green_led = 0;

	while (1){
		char c;

		c = uart_receive_byte(uart_1);

		switch(c){
			case 'r':
				red_led = !red_led;
				veces_mensaje = 0;	// El programa está contento y te perdona (por ahora)

				if(red_led == 0){
					leds_on(RED_LED);
				}
				else{
					leds_off(RED_LED);
				}

				break;
			case 'g':
				green_led = !green_led;
				veces_mensaje = 0;

				if(green_led == 0){
					leds_on(GREEN_LED);
				}
				else{
					leds_off(GREEN_LED);
				}

				break;
			default:
				switch(veces_mensaje){
					case 0:
						print_str("Solo se pueden usar las teclas g y r\r\n");

						veces_mensaje++;

						break;
					case 1:
						print_str("De verdad, solo las teclas g y r\r\n");

						veces_mensaje++;

						break;
					case 2:
						print_str("Lo hemos hablado muchas veces, solo se pueden usar las teclas g y r\r\n");
						
						veces_mensaje++;

						break;
					case 30:
						print_str("Me rindo, ahi te quedas... *sonido de puerta cerrando*\r\n");

						return 0;	// ¿Cómo te atreves? Has enfadado al programa y se ha ido :(

						break;
					default:
						print_str("g y r...\r\n");

						veces_mensaje++;

						break;
				}
		}
	}

	return 0;
}

/*****************************************************************************/
