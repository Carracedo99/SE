/*
 * Sistemas operativos empotrados
 * Driver para el controlador de interrupciones del MC1322x
 */

#include "system.h"

/*****************************************************************************/

/**
 * Acceso estructurado a los registros de control del ITC del MC1322x
 */
typedef struct{
	volatile uint32_t intcntl;
	volatile uint32_t nimask;
	volatile uint32_t intennum;
	volatile uint32_t intidsnum;
	volatile uint32_t intenable;
	volatile uint32_t inttype;
	const uint32_t RESERVED[4];
	volatile uint32_t const nivector;
	volatile uint32_t const fivector;
	volatile uint32_t const intsrc;
	volatile uint32_t intfrc;
	volatile uint32_t const nipend;
	volatile uint32_t const fipend;
} itc_regs_t;

static volatile itc_regs_t* const itc_regs = ITC_BASE;

/**
 * Tabla de manejadores de interrupción.
 */
static itc_handler_t itc_handlers[itc_src_max];

/**
 *	Variable global para guardar intenable
 */
static volatile uint32_t intenable_status;

/*****************************************************************************/

/**
 * Inicializa el controlador de interrupciones.
 * Deshabilita los bits I y F de la CPU, inicializa la tabla de manejadores a NULL,
 * y habilita el arbitraje de interrupciones Normales y rápidas en el controlador
 * de interupciones.
 */
inline void itc_init(){
	uint32_t i;

	for(i = 0; i < itc_src_max; i++){
		itc_handlers[i] = (uint32_t) NULL;
	}

	// No provocar ninguna interrupción simulada al arrancar
	itc_regs->intfrc = (uint32_t) 0;

	// Deshabilitar todas las fuentes de interrupción al activar el controlador
	itc_regs->intenable = (uint32_t) 0;
	
	// Activar arbitraje de interrupciones IRQ
	itc_regs->intcntl = ~(uint32_t)(1 << 19);
	
	// Activar arbitraje de interrupciones FIQ
	itc_regs->intcntl = ~(uint32_t)(1 << 20);
}

/*****************************************************************************/

/**
 * Deshabilita el envío de peticiones de interrupción a la CPU
 * Permite implementar regiones críticas en modo USER
 */
inline void itc_disable_ints(){
	intenable_status = itc_regs->intenable;

	itc_regs->intenable = (uint32_t) 0;
}

/*****************************************************************************/

/**
 * Vuelve a habilitar el envío de peticiones de interrupción a la CPU
 * Permite implementar regiones críticas en modo USER
 */
inline void itc_restore_ints(){
	itc_regs->intenable = intenable_status;
}

/*****************************************************************************/

/**
 * Asigna un manejador de interrupción
 * @param src		Identificador de la fuente
 * @param handler	Manejador
 */
inline void itc_set_handler(itc_src_t src, itc_handler_t handler){
	itc_handlers[src] = handler;
}

/*****************************************************************************/

/**
 * Asigna una prioridad (normal o fast) a una fuente de interrupción
 * @param src		Identificador de la fuente
 * @param priority	Tipo de prioridad
 */
inline void itc_set_priority(itc_src_t src, itc_priority_t priority){
	if(priority){
		// Las interrupciones FIQ son las de alta prioridad, por lo que
		// será el único bit activo, así que se usa = en lugar de hacer
		// OR o AND
		itc_regs->inttype = (uint32_t)(1 << src);
	}
	else{
		itc_regs->inttype &= ~(uint32_t)(1 << src);
	}
}

/*****************************************************************************/

/**
 * Habilita las interrupciones de una determinda fuente
 * @param src		Identificador de la fuente
 */
inline void itc_enable_interrupt(itc_src_t src){
	// Es el bit 000...001 desplazado a la izquierda 'src' veces,
	// para habilitar las interrupciones correspondientes
	itc_regs->intenable |= (uint32_t)(1 << src);
}

/*****************************************************************************/

/**
 * Deshabilita las interrupciones de una determinda fuente
 * @param src		Identificador de la fuente
 */
inline void itc_disable_interrupt(itc_src_t src){
	// Hace el mismo desplazamiento que en la función anterior y AND para
	// poner a 0 solo el bit correspondiente
	itc_regs->intenable &= ~(uint32_t)(1 << src);
}

/*****************************************************************************/

/**
 * Fuerza una interrupción con propósitos de depuración
 * @param src		Identificador de la fuente
 */
inline void itc_force_interrupt(itc_src_t src){
	itc_regs->intfrc |= (uint32_t)(1 << src);	// Ponemos el bit indicado a 1
}

/*****************************************************************************/

/**
 * Desfuerza una interrupción con propósitos de depuración
 * @param src		Identificador de la fuente
 */
inline void itc_unforce_interrupt(itc_src_t src){
	itc_regs->intfrc &= ~(uint32_t)(1 << src);
}

/*****************************************************************************/

/**
 * Da servicio a la interrupción normal pendiente de más prioridad.
 * En el caso de usar un manejador de excepciones IRQ que permita interrupciones
 * anidadas, debe deshabilitar las IRQ de menor prioridad hasta que se haya
 * completado el servicio de la IRQ para evitar inversiones de prioridad
 */
void itc_service_normal_interrupt(){
	itc_handlers[itc_regs->nivector]();		/* Servimos la IRQ */
}

/*****************************************************************************/

/**
 * Da servicio a la interrupción rápida pendiente de más prioridad
 */
void itc_service_fast_interrupt(){
	// Obtener el indice del manejador de la fiq y llamar a la rutina
    itc_handlers[itc_regs->fivector]();
}

/*****************************************************************************/
