/*
 * Sistemas operativos empotrados
 * Driver para el GPIO del MC1322x
 */

#include "system.h"

/*****************************************************************************/

/**
 * Acceso estructurado a los registros de control del gpio del MC1322x
 */
typedef struct{
	uint32_t PAD_DIR[2];		// GPIO Pad Direction
	uint32_t DATA[2];			// GPIO Data
	uint32_t PAD_PU_EN[2];		// GPIO Pad Pull-up Enable
	uint32_t FUNC_SEL[4];		// GPIO Function Select
	uint32_t DATA_SEL[2];		// GPIO Data Select
	uint32_t PAD_PU_SEL[2];		// GPIO Pad Pull-up Select
	uint32_t PAD_HYST_EN[2];	// GPIO Pad Hysteresis Enable
	uint32_t PAD_KEEP[2];		// GPIO Pad Keeper Enable
	uint32_t DATA_SET[2];		// GPIO Data Set
	uint32_t DATA_RESET[2];		// GPIO Data Reset
	uint32_t PAD_DIR_SET[2];	// GPIO Pad Direction Set
	uint32_t PAD_DIR_RESET[2];	// GPIO Pad Direction Reset
} gpio_regs_t;

static volatile gpio_regs_t* const gpio_regs = GPIO_BASE;

/*****************************************************************************/

/**
 * Fija la dirección de los pines seleccionados en la máscara como de entrada
 *
 * @param 	port 	Puerto
 * @param 	mask 	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son correctos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_port_dir_input(gpio_port_t port, uint32_t mask){
	if(port >= gpio_port_max){
		return gpio_invalid_parameter;
	}

	gpio_regs->PAD_DIR_RESET[port] = mask;

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Fija la dirección de los pines seleccionados en la máscara como de salida
 *
 * @param	port 	Puerto
 * @param	mask 	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son correctos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_port_dir_output(gpio_port_t port, uint32_t mask){
	if(port >= gpio_port_max){
		return gpio_invalid_parameter;
	}

	gpio_regs->PAD_DIR_SET[port] = mask;

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Fija la dirección del pin indicado como de entrada
 *
 * @param	pin 	Número de pin
 * @return	gpio_no_error si los parámetros de entrada son correctos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_pin_dir_input(gpio_pin_t pin){
	if(pin >= gpio_pin_max){
		return gpio_invalid_parameter;
	}

	gpio_regs->PAD_DIR_RESET[(pin >> 5) & 1] = 1 << (pin & 0x1f);

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Fija la dirección del pin indicado como de salida
 *
 * @param	pin 	Número de pin
 * @return	gpio_no_error si los parámetros de entrada son correctos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_pin_dir_output(gpio_pin_t pin){
	if(pin >= gpio_pin_max){
		return gpio_invalid_parameter;
	}

	gpio_regs->PAD_DIR_SET[(pin >> 5) & 1] = 1 << (pin & 0x1f);

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Escribe unos en los pines seleccionados en la máscara
 *
 * @param	port 	Puerto
 * @param	mask 	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son correctos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_port(gpio_port_t port, uint32_t mask){
	if(port >= gpio_port_max){
		return gpio_invalid_parameter;
	}

	gpio_regs->DATA_SET[port] = mask;

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Escribe ceros en los pines seleccionados en la máscara
 *
 * @param	port 	Puerto
 * @param	mask 	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son correctos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_clear_port(gpio_port_t port, uint32_t mask){
	if(port >= gpio_port_max){
		return gpio_invalid_parameter;
	}

	gpio_regs->DATA_RESET[port] = mask;

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Escribe un uno en el pin indicado
 *
 * @param	pin 	Número de pin
 * @return	gpio_no_error si los parámetros de entrada son correctos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_pin(gpio_pin_t pin){
	if(pin >= gpio_pin_max){
		return gpio_invalid_parameter;
	}

	gpio_regs->DATA_SET[(pin >> 5) & 1] = 1 << (pin & 0x1f);

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Escribe un cero en el pin indicado
 *
 * @param	pin 	Número de pin
 * @return	gpio_no_error si los parámetros de entrada son correctos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_clear_pin(gpio_pin_t pin){
	if(pin >= gpio_pin_max){
		return gpio_invalid_parameter;
	}

	gpio_regs->DATA_RESET[(pin >> 5) & 1] = 1 << (pin & 0x1f);

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Lee el valor de los pines de un puerto
 *
 * @param	port	  Puerto
 * @param	port_data Valor de los pines del puerto
 * @return	gpio_no_error si los parámetros de entrada son correctos o
 *			  gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_get_port(gpio_port_t port, uint32_t *port_data){
	if(port >= gpio_port_max){
		return gpio_invalid_parameter;
	}

	*port_data = gpio_regs->DATA[port];

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Lee el valor del pin indicado
 *
 * @param	pin	  Número de pin
 * @param       pin_data  Cero si el pin está a cero, distinto de cero en otro caso
 * @return	gpio_no_error si los parámetros de entrada son correctos o
 *			  gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_get_pin(gpio_pin_t pin, uint32_t *pin_data){
	if(pin >= gpio_pin_max){
		return gpio_invalid_parameter;
	}

	*pin_data = (gpio_regs->DATA[pin >> 5]) & (1 << (pin & 0x1f));

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Asigna un modo de funcionamiento a los pines seleccionados por la máscara
 *
 * @param   port    Puerto
 * @param   func    Modo de funcionamiento
 * @param	mask	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son correctos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_port_func(gpio_port_t port, gpio_func_t func, uint32_t mask){
	uint32_t i, pin, reg, offset;

	if (port >= gpio_port_max || func >= gpio_func_max){
		return gpio_invalid_parameter;
	}

	for (i = 0; i < 32; i++){
		if (mask & (1 << i)){	/* Si hay que modificar el modo de funcionamiento de este pin */
			pin = i + (port << 5);
			reg = pin >> 4;
			offset = (pin & 0xf) << 1;
			gpio_regs->FUNC_SEL[reg] &= (~(3 << offset));	/* Hacemos ceros en los bits de máscara */
			gpio_regs->FUNC_SEL[reg] |= (func << offset);	/* Escribimos el modo de funcionamiento */
		}
	}

	return gpio_no_error;
}

/*****************************************************************************/

/**
 *  Asigna un modo de funcionamiento al pin seleccionado
 *
 * @param	pin 	Pin
 * @param	func	Modo de funcionamiento
 * @return	gpio_no_error si los parámetros de entrada son correctos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_pin_func(gpio_pin_t pin, gpio_func_t func){
	uint32_t reg, offset;

	if (pin >= gpio_pin_max || func >= gpio_func_max){
		return gpio_invalid_parameter;
	}

	reg = pin >> 4;
	offset = (pin & 0xf) << 1;
	gpio_regs->FUNC_SEL[reg] &= (~(3 << offset));		/* Hacemos ceros en los bits de máscara */
	gpio_regs->FUNC_SEL[reg] |= (func << offset);		/* Escribimos el modo de funcionamiento */

	return gpio_no_error;
}

/*****************************************************************************/
