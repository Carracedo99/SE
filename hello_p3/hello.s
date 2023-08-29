@
@ Sistemas Empotrados
@ El "hola mundo" en la Redwire EconoTAG
@

@
@ Variables Globales
@

.data
	@ El led rojo está en el GPIO 44 (el bit 12 de los registros GPIO_X_1)
        led_red_mask:   .word	(1 << (44-32))

	@ El led verde está en el GPIO 45 (el bit 13 de los registros GPIO_X_1)
        led_green_mask: .word	(1 << (45-32))

	@ La entrada de S2 está en el GPIO 26 (el bit 26 de los registros GPIO_X_0)
	pin_s2_in:      .word	(1 << 26)

	@ La entrada de S3 está en el GPIO 27 (el bit 27 de los registros GPIO_X_0)
        pin_s3_in:      .word	(1 << 27)
	
        @ La salida de S2 está en el GPIO 22 (el bit 22 de los registros GPIO_X_0)
        pin_s2_out:     .word	(1 << 22)
	
        @ La salida de S3 está en el GPIO 23 (el bit 23 de los registros GPIO_X_0)
        pin_s3_out:     .word	(1 << 23)

	@---------------------------------------------------------@
        
        @ Retardo para el parpadeo
	delay:	.word	0x80000

        @---------------------------------------------------------@

@
@ Punto de entrada
@
        .code 32
        .text
        .global _start
        .type   _start, %function

_start:
        @ Inicializamos los pines de E/S
        bl gpio_init

        @ Usaremos r5 para mantener la máscara del led que debe
        @ parpadear. Por defecto escogemos el rojo
        ldr r3, =led_red_mask
        ldr r5, [r3]

        @ Usamos r6 y r7 para mantener las direcciones de los registros
        @ GPIO_DATA_SET1 y GPIO_DATA_RESET1
        ldr r6, =gpio_data_set1
        ldr r7, =gpio_data_reset1

loop:
        @ Comprobamos los botones y actualizamos r5 con la
        @ máscaras correspondiente si se detecta alguna pulsación
        bl test_buttons
        
        @ Encendemos el led
        str r5, [r6]

        @ Pausa corta
        ldr r0, =delay
        ldr r0, [r0]
        bl pause

        @ Apagamos el led
        str r5, [r7]

        @ Comprobamos los botones y actualizamos r5 con la
        @ máscaras correspondiente si se detecta alguna pulsación

        bl test_buttons

        @ Pausa corta
        ldr r0, =delay
        ldr r0, [r0]
        bl pause

        @ Bucle infinito
        b loop

gpio_init:
        @ Configuramos el GPIO26 y el GPIO27 para que sean de entrada
        ldr     r4, =gpio_pad_dir_reset0
        ldr     r0, =pin_s2_in
        ldr     r0, [r0]
        ldr     r5, =pin_s3_in
        ldr     r5, [r5]
        orr     r5, r0, r5 
        str     r5, [r4]
        
        @---------------------------------------------------------@

        @ Configuramos el GPIO22 y el GPIO23 para que sean de salida
        ldr     r4, =gpio_pad_dir_set0
        ldr     r0, =pin_s2_out
        ldr     r0, [r0]
        ldr     r5, =pin_s3_out
        ldr     r5, [r5]
        orr     r5, r0, r5 
        str     r5, [r4]

        @---------------------------------------------------------@

        @ Configuramos el GPIO22 y el GPIO23 para que sean 1
        ldr     r4, =gpio_data_set0
        str     r5, [r4]
       
        @---------------------------------------------------------@

        @ Configuramos el GPIO44 y el GPIO45 para que sean de salida
        ldr     r4, =gpio_pad_dir_set1
        ldr     r0, =led_red_mask
        ldr     r0, [r0]
        ldr     r5, =led_green_mask
        ldr     r5, [r5]
        orr     r5, r0, r5
        str     r5, [r4]

        @---------------------------------------------------------@

        @ Volvemos a la funcion principal
        mov     pc, lr

test_buttons:
        @ Deteccion de si esta pulsado el S2        
        ldr     r4, =gpio_data0
        ldr     r4, [r4]
        ldr     r0, =pin_s2_in
        ldr     r0, [r0]
        tst     r4, r0        
        ldrne   r5, =led_red_mask
        ldrne   r5, [r5]

        @---------------------------------------------------------@
        
        @ Deteccion de si esta pulsado el S3
        ldr     r0, =pin_s3_in
        ldr     r0, [r0]
        tst     r4, r0
        ldrne   r5, =led_green_mask
        ldrne   r5, [r5]

        @ Deteccion de si esta pulsado el S3
        @tst     r4, #(pin_s2_in + pin_s3_in)
        @ldrne   r5, =(LED_RED_MASK + LED_GREEN_MASK)

        @---------------------------------------------------------@

        @ Volvemos a la funcion principal
        mov     pc, lr

        
@
@ Función que produce un retardo
@ r0: iteraciones del retardo
@
        .type   pause, %function
pause:
        subs    r0, r0, #1
        bne     pause
        mov     pc, lr

