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

/* Dirección del registro de control de dirección del GPIO00-GPIO31 */
volatile uint32_t * const reg_gpio_pad_dir0    = (uint32_t *) 0x80000000;

/* Dirección del registro de control de dirección del GPIO32-GPIO63 */
volatile uint32_t * const reg_gpio_pad_dir1    = (uint32_t *) 0x80000004;

/* Dirección del registro de activación de bits del GPIO00-GPIO31 */
volatile uint32_t * const reg_gpio_data_set0   = (uint32_t *) 0x80000048;

/* Dirección del registro de activación de bits del GPIO32-GPIO63 */
volatile uint32_t * const reg_gpio_data_set1   = (uint32_t *) 0x8000004c;

/* Dirección del registro de control de botones del GPIO00-GPIO31 */
volatile uint32_t * const reg_gpio_data0 = (uint32_t *) 0x80000008;

/* Dirección del registro de limpieza de bits del GPIO32-GPIO63 */
volatile uint32_t * const reg_gpio_data_reset1 = (uint32_t *) 0x80000054;

/* El led rojo está en el GPIO 44 (el bit 12 de los registros GPIO_X_1) */
uint32_t const led_red_mask = (1 << (44-32));

/* El led verde está en el GPIO 45 */
uint32_t const led_green_mask = (1 << (45-32));

/* KBI0 está en el GPIO 22 */
uint32_t const kbi0_mask      = (1 << 22);

/* kbi1 está en el gpio 23 */
uint32_t const kbi1_mask      = (1 << 23);

/* kbi4 está en el gpio 26 */
uint32_t const kbi4_mask      = (1 << 26);

/* kbi5 está en el gpio 27 */
uint32_t const kbi5_mask      = (1 << 27);

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
	*reg_gpio_pad_dir1 = (led_red_mask | led_green_mask);

	/* Configuramos los KBI0 y KBI1 para que sean de salida */
	/* y KBI4 y KBI5 para que sean de entrada */
	*reg_gpio_pad_dir0 = kbi0_mask | kbi1_mask;

	/* Fijamos un 1 en KBI0 y KBI1 */
	*reg_gpio_data_set0 = kbi0_mask | kbi1_mask;
}

/*****************************************************************************/

/*
 * Enciende los leds indicados en la máscara
 * @param mask Máscara para seleccionar leds
 */
void leds_on (uint32_t mask){
	/* Encendemos los leds indicados en la máscara */
	*reg_gpio_data_set1 = mask;
}

/*****************************************************************************/

/*
 * Apaga los leds indicados en la máscara
 * @param mask Máscara para seleccionar leds
 */
void leds_off (uint32_t mask){
	/* Apagamos los leds indicados en la máscara */
	*reg_gpio_data_reset1 = mask;
}

/*****************************************************************************/

/*
 * Selecciona el led que se debe parpadear en función del estado de los botones
 * @param current_led  Máscara del led actualmente selecionado
 * @return             Máscara del led que se seguirá parpadeando
 */
uint32_t test_buttons(uint32_t current_led){
	uint32_t the_led, data0;

	data0 = *reg_gpio_data0;
	the_led = current_led;

	if (data0 & kbi4_mask){
		the_led = led_green_mask;
	}
	else{
		if (data0 & kbi5_mask){
			the_led = led_red_mask;
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
    *reg_gpio_data_set1 = led_green_mask;
}

/*****************************************************************************/

/*
 * Manejador de interrupciones ASM 
 */
void asm_handler(void){
	itc_unforce_interrupt(itc_src_asm);
    *reg_gpio_data_set1 = led_green_mask;
}

/*****************************************************************************/

/*
 * Programa principal
 */
int main (){
	uint32_t the_led;	// Máscara del led que se hará parpadear

	gpio_init();
	
	/* Configuramos el manejador y habilitamos la interrupción */
	itc_set_handler(itc_src_asm, asm_handler);
	itc_enable_interrupt(itc_src_asm);

	/* Forzamos la interrupción */
    itc_force_interrupt(itc_src_asm);

	the_led = led_red_mask;

	asm(".word 0x26889912\n");
	
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

