/*****************************************************************************/
/*                                                                           */
/* Sistemas Empotrados                                                       */
/*                                                                           */
/* Linker script para la Redwire EconoTAG                                    */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* Punto de entrada                                                          */
/*****************************************************************************/

ENTRY(_start)

/*
 * Mapa de memoria de la placa
 */
MEMORY
{
    ram0 :  org = 0x00400000,       len = 0x00002000        /* (8 KB) */
    ram1 :  org = 0x00402000,       len = 0x00006000        /* (24KB) */
    ram2 :  org = 0x00408000,       len = 0x00008000        /* (32KB) */
    ram3 :  org = 0x00410000,       len = 0x00008000        /* (32KB) */
    gpio :  org = 0x80000000,       len = 0x00000068
}

SECTIONS
{
	/* Código del programa */
	.text : ALIGN(4)
	{
		*(.text);
	} > ram0
/*
	.ram1 : ALIGN(4)
	{
		s1=.;
		*(.ram1);
	} > ram1

	.ram2 : ALIGN(4)
	{
		s2=.;
		*(.ram2);
	} > ram2

	.ram3 : ALIGN(4)
	{
		s3=.;
		*(.ram3);
    } > ram3
*/
	.data : {
		*(.data);
	} > ram1

	.gpio : {
		gpio_base 		 	= . ;
		gpio_data0 		 	= gpio_base + 0x08; /* Registro de datos del GPIO00-GPIO31 */
		gpio_data_set0 	 	= gpio_base + 0x48; /* Registro de activación de bits del GPIO00-GPIO31 */
		gpio_data_set1 	 	= gpio_base + 0x4C; /* Registro de activación de bits del GPIO32-GPIO63 */
		gpio_data_reset1 	= gpio_base + 0x54; /* Registro de limpieza de bits del GPIO32-GPIO63 */
		gpio_pad_dir_set0	= gpio_base + 0x58; /* Registro de control de dirección del GPIO00-GPIO31 */
		gpio_pad_dir_set1	= gpio_base + 0x5C;	/* Registro de control de dirección del GPIO32-GPIO63 */
		gpio_pad_dir_reset0 = gpio_base + 0x50; /* Registro de control de dirección del GPIO00-GPIO31 */
	} > gpio

}
