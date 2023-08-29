@
@ Sistemas Empotrados
@ El "hola mundo" en la Redwire EconoTAG
@

@
@ Constantes
@


        @ Registro de control de dirección del GPIO0-GPIO31
        .set GPIO_DATA0,    0x80000008
        
        @ Registro de control de dirección del GPIO0-GPIO31
        .set GPIO_PAD_DIR_RESET0,    0x80000060

        @ Registro de control de dirección del GPIO0-GPIO31
        .set GPIO_PAD_DIR_SET0,    0x80000058
        
        @ Registro de control de dirección del GPIO32-GPIO63
        .set GPIO_PAD_DIR_SET1,    0x8000005C

        @ Registro de activación de bits del GPIO0-GPIO31
        .set GPIO_DATA_SET0,   0x80000048
        
        @ Registro de activación de bits del GPIO32-GPIO63
        .set GPIO_DATA_SET1,   0x8000004c

        @ Registro de limpieza de bits del GPIO32-GPIO63
        .set GPIO_DATA_RESET1, 0x80000054

        @ El led rojo está en el GPIO 44 (el bit 12 de los registros GPIO_X_1)
        .set LED_RED_MASK,     (1 << (44-32))
        
        @ El led verde está en el GPIO 44 (el bit 12 de los registros GPIO_X_1)
        .set LED_GREEN_MASK,     (1 << (45-32))
        
        @ Entrada de los botones
        .set BUTTON_S2_IN,     (1 << 26)
        
        @ Entrada de los botones
        .set BUTTON_S3_IN,     (1 << 27)
        
        @ Salida de los botones
        .set BUTTON_S2_OUT,     (1 << 22)
        
        @ Salida de los botones
        .set BUTTON_S3_OUT,     (1 << 23)

        @ Retardo para el parpadeo
        .set DELAY,            0x00080000

@
@ Punto de entrada
@

        .code 32
        .text
        .global _start
        .type   _start, %function

_start:
	bl gpio_init

	ldr r5, =LED_RED_MASK


        @ Direcciones de los registros GPIO_DATA_SET1 y GPIO_DATA_RESET1
        ldr     r6, =GPIO_DATA_SET1
        ldr     r7, =GPIO_DATA_RESET1

loop:

	bl test_buttons
	
	bl test_buttons
	
        @ Encendemos el led --> r6 es encender
        str     r5, [r6]

        @ Pausa corta
        ldr     r0, =DELAY
        bl      pause

        @ Apagamos el led
        str     r5, [r7]
        
        bl test_buttons

        @ Pausa corta
        ldr     r0, =DELAY
        bl      pause

        @ Bucle infinito
        b       loop
        
@
@ Función que produce un retardo
@ r0: iteraciones del retardo
@
        .type   pause, %function
pause:
        subs    r0, r0, #1
        bne     pause
        mov     pc, lr
        
        
gpio_init:

        @ Configuramos el GPIO44 para que sea de salida
        ldr     r4, =GPIO_PAD_DIR_SET1
        ldr     r5, =LED_RED_MASK	@mascara del led rojo
        str     r5, [r4]

        @ Configuramos el GPIO44 para que sea de salida
        ldr     r5, =LED_GREEN_MASK	@mascara del led verde
        str     r5, [r4]
        
        
        ldr     r4, =GPIO_PAD_DIR_SET0
        ldr     r5, =BUTTON_S2_OUT	@mascara de los botones 1
        str     r5, [r4]
        
        ldr     r5, =BUTTON_S3_OUT	@mascara de los botones 2
        str     r5, [r4]
        
       
       
        ldr     r4, =GPIO_PAD_DIR_RESET0
        ldr     r5, =BUTTON_S2_IN	@mascara de los botones 1
        str     r5, [r4]
        
        ldr     r5, =BUTTON_S3_IN	@mascara de los botones 2
        str     r5, [r4]
        
        
        ldr     r4, =GPIO_DATA_SET0
        str     r5, [r4]  
        
        
	mov pc, lr
	
	
test_buttons:

        @ Deteccion de si esta pulsado el S2
        ldr     r4, =GPIO_DATA0
        ldr     r4, [r4]
        tst     r4, #BUTTON_S2_IN
        ldrne   r5, =LED_RED_MASK

        
        @ Deteccion de si esta pulsado el S3
        tst     r4, #BUTTON_S3_IN
        ldrne   r5, =LED_GREEN_MASK


        @ Volvemos a la funcion principal (al loop)
        mov     pc, lr



































