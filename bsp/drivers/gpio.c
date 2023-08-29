/*
 * Sistemas operativos empotrados
 * Driver para el GPIO del MC1322x
 */

#include "system.h"

/*****************************************************************************/

/**
 * Acceso estructurado a los registros de control del gpio del MC1322x
 */
typedef struct
{
	/* ESTA ESTRUCTURA SE DEFINIRÁ EN LA PRÁCTICA 7 */
} gpio_regs_t;

static volatile gpio_regs_t* const gpio_regs = GPIO_BASE;

/*****************************************************************************/

/**
 * Fija la dirección de los pines seleccionados en la máscara como de entrada
 *
 * @param 	port 	Puerto
 * @param 	mask 	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_port_dir_input (gpio_port_t port, uint32_t mask)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 7 */

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Fija la dirección de los pines seleccionados en la máscara como de salida
 *
 * @param	port 	Puerto
 * @param	mask 	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_port_dir_output (gpio_port_t port, uint32_t mask)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 7 */

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Fija la dirección del pin indicado como de entrada
 *
 * @param	pin 	Número de pin
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_pin_dir_input (gpio_pin_t pin)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 7 */

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Fija la dirección del pin indicado como de salida
 *
 * @param	pin 	Número de pin
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_pin_dir_output (gpio_pin_t pin)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 7 */

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Escribe unos en los pines seleccionados en la máscara
 *
 * @param	port 	Puerto
 * @param	mask 	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_port (gpio_port_t port, uint32_t mask)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 7 */

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Escribe ceros en los pines seleccionados en la máscara
 *
 * @param	port 	Puerto
 * @param	mask 	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_clear_port (gpio_port_t port, uint32_t mask)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 7 */

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Escribe un uno en el pin indicado
 *
 * @param	pin 	Número de pin
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_pin (gpio_pin_t pin)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 7 */

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Escribe un cero en el pin indicado
 *
 * @param	pin 	Número de pin
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_clear_pin (gpio_pin_t pin)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 7 */

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Lee el valor de los pines de un puerto
 *
 * @param	port	  Puerto
 * @param	port_data Valor de los pines del puerto
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			  gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_get_port (gpio_port_t port, uint32_t *port_data)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 7 */

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Lee el valor del pin indicado
 *
 * @param	pin	  Número de pin
 * @param       pin_data  Cero si el pin está a cero, distinto de cero en otro caso
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			  gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_get_pin (gpio_pin_t pin, uint32_t *pin_data)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 7 */

	return gpio_no_error;
}

/*****************************************************************************/

/**
 * Asigna un modo de funcionamiento a los pines seleccionados por la máscara
 *
 * @param   port    Puerto
 * @param   func    Modo de funcionamiento
 * @param	mask	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_port_func (gpio_port_t port, gpio_func_t func, uint32_t mask)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 7 */

	return gpio_no_error;
}

/*****************************************************************************/

/**
 *  Asigna un modo de funcionamiento al pin seleccionado
 *
 * @param	pin 	Pin
 * @param	func	Modo de funcionamiento
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_pin_func (gpio_pin_t pin, gpio_func_t func)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 7 */

	return gpio_no_error;
}

/*****************************************************************************/
