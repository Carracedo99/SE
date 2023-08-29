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

/* KBI0 está en el GPIO 22 */
#define KBI0 gpio_pin_22

/* kbi1 está en el gpio 23 */
#define KBI1 gpio_pin_23

/* kbi4 está en el gpio 26 */
#define KBI4 gpio_pin_26

/* kbi5 está en el gpio 27 */
#define KBI5 gpio_pin_27

/*
 * Constantes relativas a la aplicacion
 */
uint32_t const delay = 0x10000;
 
/*****************************************************************************/

/*
 * Inicialización de los pines de E/S
 */
void gpio_init(void){
	/* Configuramos los GPIO44 y GPIO45 para que sean de salida */
	gpio_set_pin_dir_output (RED_LED);
	gpio_set_pin_dir_output (GREEN_LED);

	//Configuramos los pines de los switches
	gpio_set_port_dir_output(gpio_port_0, (1 << KBI0) | (1 << KBI1));
	gpio_set_port_dir_input(gpio_port_0, (1 << KBI4) | (1 << KBI5));

	//Ponemos un 1 en KBI0 y KBI1 para leer la pulsacion de los switches
	gpio_set_port(gpio_port_0, (1 << KBI0) | (1 << KBI1));
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

/*
 * Selecciona el led que se debe parpadear en función del estado de los botones
 * @param current_led  Máscara del led actualmente selecionado
 * @return             Máscara del led que se seguirá parpadeando
 */
uint32_t test_buttons(uint32_t current_led){
	uint32_t the_led, data0;

	gpio_get_port(gpio_port_0, &data0);

	the_led = current_led;

	if (data0 & (1 << KBI4)){
		the_led = GREEN_LED;
	}
	else{
		if (data0 & (1 << KBI5)){
			the_led = RED_LED;
		}
	}

	return the_led;
}

/*****************************************************************************/

/*
 * Retardo para el parpadeo
 */
void pause(void){
	uint32_t i;
	for (i = 0; i < delay; i++);
}

/*****************************************************************************/

/*
 * Manejador de instrucciones no definidas
 */
__attribute__ ((interrupt("UNDEF")))
void undef_handler(void){
	leds_on(GREEN_LED);
}

/*****************************************************************************/

/*
 * Manejador de interrupciones ASM 
 */
void asm_handler(void){
	itc_unforce_interrupt(itc_src_asm);
	leds_on(GREEN_LED);
}

/*****************************************************************************/

/*
 * Programa principal
 */
int main (){
	uint32_t the_led;	// Máscara del led que se hará parpadear

	gpio_init();
	
	itc_set_handler(itc_src_asm, asm_handler);
	excep_set_handler(excep_undef, undef_handler);

	the_led = RED_LED;

	leds_on(the_led);
	
	while (1){
		the_led = test_buttons(the_led);
		leds_on(the_led);
		pause();

		leds_off(the_led);
		pause();
	}

	return 0;
}

/*****************************************************************************/
