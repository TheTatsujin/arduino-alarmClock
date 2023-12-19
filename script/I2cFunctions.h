// Inicializa el bus i2c
extern void i2c_init();
extern void i2c_alarmInit();

/* 
   Modulo de Memoria 24LC64
   
   chipSelect tiene que ser un valor entre 0x00 y 0x01
*/
extern bool i2c_WriteMem(byte chipSelect, int memAddress, byte data);
extern byte i2c_ReadMem(byte chipSelect, int memAddress);


// Modulo de RTC DS3232
extern String i2c_getTemperature();
extern String i2c_getTime();
extern String i2c_getDate();
extern String i2c_getAlarm1();
extern byte i2c_getAlarm1Sec();
extern byte i2c_getAlarm1Min();
extern byte i2c_getAlarm1Hour();
extern String i2c_getAlarm2();


extern bool i2c_setTime(byte seconds, byte minutes, byte hour);
extern bool i2c_setDate(byte day, byte month, byte year);
extern bool i2c_setAlarm1(byte seconds, byte minutes, byte hour);
extern bool i2c_setAlarm2(byte seconds, byte minutes, byte hour);

extern bool i2c_alarm1(bool enable);
extern bool i2c_alarm2(bool enable);


