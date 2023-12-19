  // DeclaraciÃƒÆ’Ã†â€™Ãƒâ€šÃ‚Â³n de  identificadores de las lÃƒÆ’Ã†â€™Ãƒâ€ Ã¢â‚¬â„¢ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â­neas dedicadas a I2C (Para usar pinMode() y digitalWrite())  ) 

#define ESC_SCL   4  // puerto de salida para escribir el valor en la lÃƒÆ’Ã†â€™Ãƒâ€šÃ‚Â­ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â­nea SCL-out =>IO4 =>PG5
#define ESC_SDA  39  // puerto de salida para escribir el valor en la lÃƒÆ’Ã†â€™Ãƒâ€šÃ‚Â­ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â­nea SDA-out =>IO39=>PG2
#define LEE_SCL  40  // puerto de entrada para leer el estado de la lÃƒÆ’Ã†â€™Ãƒâ€šÃ‚Â­ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â­nea SCL       =>IO40=>PG1
#define LEE_SDA  41  // puerto de entrada para leer el estado de la lÃƒÆ’Ã†â€™Ãƒâ€šÃ‚Â­ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â­nea SDA       =>IO41=>PG0



// Direccion del modulo 24LC64
#define BASE_ADDRESS_24LC64 B1010000


// Direccion del modulo DS3232
#define BASE_ADDRESS_DS3232 B11010000




  // DeclaraciÃƒÆ’Ã†â€™Ãƒâ€šÃ‚Â³n de  identificadores de las lÃƒÆ’Ã†â€™Ãƒâ€šÃ‚Â­ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â­neas dedicadas a I2C (Para usar con DDR, PIN y PORT ) 

/*
#define SCL_OUT 5  // puerto de entrada para leer el estado de la lÃƒÆ’Ã†â€™Ãƒâ€šÃ‚Â­ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â­nea SCL_out  =>IO4 =>PG5
#define SDA_OUT 2  // puerto de entrada para leer el estado de la lÃƒÆ’Ã†â€™Ãƒâ€šÃ‚Â­ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â­nea SDA_out  =>IO39=>PG2
#define SCL_IN 1   // puerto de salida para escribir el valor en la lÃƒÆ’Ã†â€™Ãƒâ€šÃ‚Â­ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â­nea SCL_in =>IO40=>PG1
#define SDA_IN 0   // puerto de salida para escribir el valor en la lÃƒÆ’Ã†â€™Ãƒâ€šÃ‚Â­ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â­nea SDA_in =>IO41=>PG0
*/

