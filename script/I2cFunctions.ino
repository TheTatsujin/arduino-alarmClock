#include "I2cVars.h"
//
bool alarm1ON = false;
bool alarm2ON = false;




byte decodeTime(byte data){
    byte time = (data & 0x70);
    time = time >>4;
    time = time *10;
    time += (data & 0x0F);
    return time;
}

byte encodeTime(byte time){
     byte data = ((time / 10) << 4) | (time % 10);
    return data;
}

int decode2C(byte data){
  // Check the sign bit (most significant bit)
  if (data & 0x80) {
    data = ~data + 1;
    // Negate the result to get the original signed integer
    return -data;
  } else {
    // If the sign bit is not set (positive value), return the value as is
    return data;
  }
 }
  
void i2c_alarmInit(){
      byte wordAddress = 0x0A;
      byte setAlarm = 0x80;
      cli();
      alarm1InitBegin:
      i2c_start();
      
      // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232);
      
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto alarm1InitBegin;
      }
      
      //Word Address
      i2c_SendByte(wordAddress);
   
      // Ack
      if (i2c_Rbit() != 0){
	 goto alarm1InitBegin;
      }
      

	 i2c_SendByte(setAlarm);

       
       // Ack
      if (i2c_Rbit() != 0){
	 goto alarm1InitBegin;
      }
      
      i2c_stop();
      
      delayMicroseconds(10);
      
      wordAddress = 0x0D;

      alarm2InitBegin:
      i2c_start();
      
      // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232);
      
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto alarm2InitBegin;
      }
      
      //Word Address
      i2c_SendByte(wordAddress);
   
      // Ack
      if (i2c_Rbit() != 0){
	 goto alarm2InitBegin;
      }
      

	i2c_SendByte(setAlarm);

       
       // Ack
      if (i2c_Rbit() != 0){
	 goto alarm2InitBegin;
      }
      
      i2c_stop();
      sei();

}

void i2c_init(){
  digitalWrite(ESC_SCL, HIGH);
  digitalWrite(ESC_SDA, HIGH); 	
  pinMode(LEE_SDA, INPUT);
  pinMode(LEE_SCL, INPUT);
  pinMode(ESC_SDA, OUTPUT);
  pinMode(ESC_SCL, OUTPUT);
}


// Este algoritmo no es optimo => es muy lento; .... Pero funciona!

void i2c_start() {    // funcion naive para start
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, HIGH); 
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, HIGH); 
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, LOW);  // Flanco de bajada =>Esto causa el start
  digitalWrite(ESC_SCL, LOW );  digitalWrite(ESC_SDA, LOW);  // Se pone para asegurarse de deja CLK a 0
}

void i2c_stop() {    // funcion naive para stop 
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, LOW); 
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, LOW); 
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, HIGH);  // Flanco de subida =>Esto causa el stop
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, HIGH);   // Si se quita sigue funcionando!
}

void i2c_Ebit1() {    // funcion naive enviar un 1
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, HIGH); 
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, HIGH); 
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, HIGH);  // Si se quita sigue funcionando!
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, HIGH);   
}

void i2c_Ebit0() {    // funcion naive enviar un 0
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, LOW);  
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, LOW);  
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, LOW);   // Si se quita sigue funcionando!
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, LOW);    
}

void i2c_Ebit(bool val) {    // funcion naive enviar un bit
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, val); 
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, val); 
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, val);  // Si se quita sigue funcionando!
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, val);   
}

int i2c_Rbit() {        // funcion naive leer un bit
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, HIGH); 
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, HIGH); 
  digitalWrite(ESC_SCL, HIGH);  int val = digitalRead(LEE_SDA);  // Aqui se produce la lectura y se guarda en val!
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, HIGH);   
  return val;                                                      // Aqui se devuelve val!
}

void i2c_SendByte(byte data){
	for(int i=0; i<8; i++) {
		if ((data&128) != 0){
			i2c_Ebit1();
		} else {
			i2c_Ebit0();
		}
		data = data << 1;
	}
}

byte i2c_ReadByte(){
   byte data = 0;
	for(int i = 0; i<8; i++){
		data= (data<<1) | (i2c_Rbit() & 1); 
	}
   return data;
}

bool i2c_WriteMem(byte chipSelect, int memAddress, byte data){
      byte high_address = (memAddress >> 8) & 0x1F;
      byte low_address = memAddress & 0xFF;
      // El formato de byte de control de 24LC64 , seleccionado Write
      byte chipAddress = B10100000 |  (chipSelect << 1);
      
      cli();
      writeBegin:
      i2c_start();
      
      // Control Byte
      i2c_SendByte(chipAddress);
      
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto writeBegin;
      }
      
      i2c_SendByte(high_address);
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto writeBegin;
      }
      
      i2c_SendByte(low_address);
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto writeBegin;
      }
      
      i2c_SendByte(data);
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto writeBegin;
      }
      
      i2c_stop();
      sei();
      
      return true;
}


byte i2c_ReadMem(byte chipSelect, int memAddress){
      byte high_address = (memAddress >> 8) & 0x1F;
      byte low_address = memAddress & 0xFF;
      // El formato de byte de control de 24LC64 , seleccionado Write
      byte chipAddress = B10100000 | (chipSelect << 1);
      byte chipAddressRead = chipAddress | 0x01;
      
      cli();
      readBegin:
      i2c_start();
      // Control Byte
      i2c_SendByte(chipAddress);
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto readBegin;
      }
      
      i2c_SendByte(high_address);
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto readBegin;
      }
      
      i2c_SendByte(low_address);
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto readBegin;
      }
      
      i2c_start();
      i2c_SendByte(chipAddressRead);
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto readBegin;
      }
      
      byte data = i2c_ReadByte();
      
      //Nack
      i2c_Ebit1();
      i2c_stop();
      sei();


      return data;
}

String i2c_getTemperature(){
   int data = 0;
   //int floatValue = 0;
   byte wordAddress = 0x11;
   
    cli();
      getTempBegin:
      i2c_start();
      
      // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232);
      
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto getTempBegin;
      }
      
      //Word Address
      i2c_SendByte(wordAddress);
   
      // Ack
      if (i2c_Rbit() != 0){
	 goto getTempBegin;
      }
     
      //Reapeated start for Reading
     i2c_start();
       // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232          |  0x1);
      
            // Ack
      if (i2c_Rbit() != 0){
	 goto getTempBegin;
      }
      
      // Most Significant Bits
      data = decode2C(i2c_ReadByte() );
      /**
      // Send Ack
      i2c_Ebit0();
      
      // Less Significant Bits
      floatValue = decode2C(i2c_ReadByte() );
      */
      // Nack
      i2c_Ebit1();
      i2c_stop();
      sei();
      
      /*
      Serial.print("Float value: ");
      Serial.println(floatValue);
      
      float temperatureValue = data*1.0 + floatValue * 0.25;
      
      */
      String sign = "";
      if (data > 0) {
	 sign = "+";
      }
      String resultTemp = sign + String(data*1.0); 
   return resultTemp;
}


String i2c_getTime(){
   byte seconds;
   byte minutes;
   byte hour;
   byte wordAddress = 0x00;
     cli();
      getTimeBegin:
      i2c_start();
      
      // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232);
      
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto getTimeBegin;
      }
      
      //Word Address
      i2c_SendByte(wordAddress);
   
      // Ack
      if (i2c_Rbit() != 0){
	 goto getTimeBegin;
      }
      
      //Reapeated start for Reading
     i2c_start();
       // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232          |  0x1);
      
            // Ack
      if (i2c_Rbit() != 0){
	 goto getTimeBegin;
      }
      
      // Position 0: Seconds
      seconds = i2c_ReadByte();
      
      //Send Ack
      i2c_Ebit0();
      
       // Position 1: Minutes
      minutes = i2c_ReadByte() ;
      
      //Send Ack
      i2c_Ebit0();
      
       // Position 2: Hour
      hour = i2c_ReadByte() & 0x1F;
      
      i2c_Ebit1();
      i2c_stop();

      
      hour = decodeTime(hour);
      minutes = decodeTime(minutes);
      seconds = decodeTime(seconds);
      
       char formattedTime[9]; // HH:MM:SS + null terminator
	snprintf(formattedTime, sizeof(formattedTime), "%02d:%02d:%02d", hour, minutes, seconds);
	   sei();

   return String(formattedTime);
}



String i2c_getDate(){
   byte date;
   byte month;
   byte year;
   byte wordAddress = 0x04;
     cli();
      getDateBegin:
      i2c_start();
      
      // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232);
      
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto getDateBegin;
      }
      
      //Word Address
      i2c_SendByte(wordAddress);
   
      // Ack
      if (i2c_Rbit() != 0){
	 goto getDateBegin;
      }
      
      //Reapeated start for Reading
     i2c_start();
       // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232          |  0x1);
      
            // Ack
      if (i2c_Rbit() != 0){
	 goto getDateBegin;
      }
      
      // Position 4: day of the month
      date = i2c_ReadByte();
      
      //Send Ack
      i2c_Ebit0();
      
       // Position 5: Month
      month = i2c_ReadByte() ;
      
      //Send Ack
      i2c_Ebit0();
      
       // Position 6: Year
      year = i2c_ReadByte();
      
      i2c_Ebit1();
      i2c_stop();

      
      date = decodeTime(date);
      month = decodeTime(month);
      year = decodeTime(year);
      
      
      const char* monthStr[] = {"", "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
        char formattedDate[12]; // DDMMM20YY + null terminator
      snprintf(formattedDate, 12, "%02u%s20%02u", date, monthStr[month], year);

      sei();

   return String(formattedDate);
}

String i2c_getAlarm1(){
   byte seconds;
   byte minutes;
   byte hour;
   byte wordAddress = 0x07;
     cli();
      getAlarm1Begin:
      i2c_start();
      
      // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232);
      
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto getAlarm1Begin;
      }
      
      //Word Address
      i2c_SendByte(wordAddress);
   
      // Ack
      if (i2c_Rbit() != 0){
	 goto getAlarm1Begin;
      }
      
      //Reapeated start for Reading
     i2c_start();
       // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232          |  0x1);
      
            // Ack
      if (i2c_Rbit() != 0){
	 goto getAlarm1Begin;
      }
      
      // Position 7: Seconds
      seconds = i2c_ReadByte();
      
      //Send Ack
      i2c_Ebit0();
      
       // Position 8: Minutes
      minutes = i2c_ReadByte() ;
      
      //Send Ack
      i2c_Ebit0();
      
       // Position 9: Hour
      hour = i2c_ReadByte() & 0x1F;
      
      i2c_Ebit1();
      i2c_stop();

      
      hour = decodeTime(hour);
      minutes = decodeTime(minutes);
      seconds = decodeTime(seconds);
      
       char formattedTime[9]; // HH:MM:SS + null terminator
	snprintf(formattedTime, sizeof(formattedTime), "%02d:%02d", hour, minutes);
	      sei();

   return String(formattedTime);
}


byte i2c_getAlarm1Sec(){
   byte seconds;
   byte wordAddress = 0x07;
     cli();
      getAlarm1SecBegin:
      i2c_start();
      
      // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232);
      
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto getAlarm1SecBegin;
      }
      
      //Word Address
      i2c_SendByte(wordAddress);
   
      // Ack
      if (i2c_Rbit() != 0){
	 goto getAlarm1SecBegin;
      }
      
      //Reapeated start for Reading
     i2c_start();
       // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232          |  0x1);
      
            // Ack
      if (i2c_Rbit() != 0){
	 goto getAlarm1SecBegin;
      }
      
      // Position 7: Seconds
      seconds = i2c_ReadByte();
      
      i2c_Ebit1();
      i2c_stop();

      seconds = decodeTime(seconds);
      
            sei();
   return seconds;
}




byte i2c_getAlarm1Min(){
   byte minutes;
   byte wordAddress = 0x08;
     cli();
      getAlarm1MinBegin:
      i2c_start();
      
      // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232);
      
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto getAlarm1MinBegin;
      }
      
      //Word Address
      i2c_SendByte(wordAddress);
   
      // Ack
      if (i2c_Rbit() != 0){
	 goto getAlarm1MinBegin;
      }
      
      //Reapeated start for Reading
     i2c_start();
       // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232          |  0x1);
      
            // Ack
      if (i2c_Rbit() != 0){
	 goto getAlarm1MinBegin;
      }
      
      // Position 8: Minutes
      minutes = i2c_ReadByte();
      
      i2c_Ebit1();
      i2c_stop();

      minutes = decodeTime(minutes);
      
            sei();
   return minutes;
}

byte i2c_getAlarm1Hour(){
   byte hour;
   byte wordAddress = 0x09;
     cli();
      getAlarm1HourBegin:
      i2c_start();
      
      // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232);
      
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto getAlarm1HourBegin;
      }
      
      //Word Address
      i2c_SendByte(wordAddress);
   
      // Ack
      if (i2c_Rbit() != 0){
	 goto getAlarm1HourBegin;
      }
      
      //Reapeated start for Reading
     i2c_start();
       // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232          |  0x1);
      
            // Ack
      if (i2c_Rbit() != 0){
	 goto getAlarm1HourBegin;
      }
      
      // Position 7: Seconds
      hour = i2c_ReadByte();
      
      i2c_Ebit1();
      i2c_stop();

      hour = decodeTime(hour);
      
            sei();
   return hour;
}



String i2c_getAlarm2(){
   byte seconds;
   byte minutes;
   byte hour;
   byte wordAddress = 0x0B;
     cli();
      getAlarm2Begin:
      i2c_start();
      
      // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232);
      
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto getAlarm2Begin;
      }
      
      //Word Address
      i2c_SendByte(wordAddress);
   
      // Ack
      if (i2c_Rbit() != 0){
	 goto getAlarm2Begin;
      }
      
      //Reapeated start for Reading
     i2c_start();
       // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232          |  0x1);
      
            // Ack
      if (i2c_Rbit() != 0){
	 goto getAlarm2Begin;
      }
      
      // Position B: Seconds
      seconds = i2c_ReadByte();
      
      //Send Ack
      i2c_Ebit0();
      
       // Position C: Minutes
      minutes = i2c_ReadByte() ;
      
      //Send Ack
      i2c_Ebit0();
      
       // Position D: Hour
      hour = i2c_ReadByte() & 0x1F;
      
      i2c_Ebit1();
      i2c_stop();

      
      hour = decodeTime(hour);
      minutes = decodeTime(minutes);
      seconds = decodeTime(seconds);
      
       char formattedTime[9]; // HH:MM:SS + null terminator
	snprintf(formattedTime, sizeof(formattedTime), "%02d:%02d", hour, minutes);
            sei();
   return String(formattedTime);
}

bool i2c_setTime(byte seconds, byte minutes, byte hour){
   seconds = encodeTime(seconds);
   minutes = encodeTime(minutes);
   hour = encodeTime(hour);
   byte wordAddress = 0x00;
   
     cli();
      setTimeBegin:
      i2c_start();
      
      // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232);
      
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto setTimeBegin;
      }
      
      //Word Address
      i2c_SendByte(wordAddress);
   
      // Ack
      if (i2c_Rbit() != 0){
	 goto setTimeBegin;
      }
      
       i2c_SendByte(seconds);
       
       
        // Ack
      if (i2c_Rbit() != 0){
	 goto setTimeBegin;
      }
      
         i2c_SendByte(minutes);
      
      
        // Ack
      if (i2c_Rbit() != 0){
	 goto setTimeBegin;
      }
      
         i2c_SendByte(hour);
      
        // Ack
      if (i2c_Rbit() != 0){
	 goto setTimeBegin;
      }
      
      i2c_stop();
      sei();
      return true;
}



bool i2c_setDate(byte day, byte month, byte year){
   day = encodeTime(day);
   month = encodeTime(month);
   year = encodeTime(year);
   byte wordAddress = 0x04;
   
     cli();
      setDateBegin:
      i2c_start();
      
      // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232);
      
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto setDateBegin;
      }
      
      //Word Address
      i2c_SendByte(wordAddress);
   
      // Ack
      if (i2c_Rbit() != 0){
	 goto setDateBegin;
      }
      
       i2c_SendByte(day);
       
       
        // Ack
      if (i2c_Rbit() != 0){
	 goto setDateBegin;
      }
      
         i2c_SendByte(month);
      
      
        // Ack
      if (i2c_Rbit() != 0){
	 goto setDateBegin;
      }
      
         i2c_SendByte(year);
      
        // Ack
      if (i2c_Rbit() != 0){
	 goto setDateBegin;
      }
      
      i2c_stop();
      sei();
      return true;
}



bool i2c_setAlarm1(byte seconds, byte minutes, byte hour){
   seconds = encodeTime(seconds);
   minutes = encodeTime(minutes);
   hour = encodeTime(hour);
   byte wordAddress = 0x07;
   
     cli();
      setAlarm1Begin:
      i2c_start();
      
      // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232);
      
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto setAlarm1Begin;
      }
      
      //Word Address
      i2c_SendByte(wordAddress);
   
      // Ack
      if (i2c_Rbit() != 0){
	 goto setAlarm1Begin;
      }
      
       i2c_SendByte(seconds);
       
       
        // Ack
      if (i2c_Rbit() != 0){
	 goto setAlarm1Begin;
      }
      
         i2c_SendByte(minutes);
      
      
        // Ack
      if (i2c_Rbit() != 0){
	 goto setAlarm1Begin;
      }
      
         i2c_SendByte(hour);
      
        // Ack
      if (i2c_Rbit() != 0){
	 goto setAlarm1Begin;
      }
      
      i2c_stop();
      sei();
      return true;
}

bool i2c_setAlarm2(byte seconds, byte minutes, byte hour){
   seconds = encodeTime(seconds);
   minutes = encodeTime(minutes);
   hour = encodeTime(hour);
   byte wordAddress = 0x0B;
   
     cli();
      setAlarm2Begin:
      i2c_start();
      
      // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232);
      
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto setAlarm2Begin;
      }
      
      //Word Address
      i2c_SendByte(wordAddress);
   
      // Ack
      if (i2c_Rbit() != 0){
	 goto setAlarm2Begin;
      }
      
       i2c_SendByte(seconds);
       
       
        // Ack
      if (i2c_Rbit() != 0){
	 goto setAlarm2Begin;
      }
      
         i2c_SendByte(minutes);
      
      
        // Ack
      if (i2c_Rbit() != 0){
	 goto setAlarm2Begin;
      }
      
         i2c_SendByte(hour);
      
        // Ack
      if (i2c_Rbit() != 0){
	 goto setAlarm2Begin;
      }
      
      i2c_stop();
      sei();
      return true;
}


bool i2c_alarm1(bool enable){
      byte wordAddress = 0x0E;
      byte enableInt = 0x01 | 0x04;
      if (alarm2ON){
	 enableInt |= 0x2;
      }
      cli();
      toggleAlarm1Begin:
      i2c_start();
      
      // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232);
      
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto toggleAlarm1Begin;
      }
      
      //Word Address
      i2c_SendByte(wordAddress);
   
      // Ack
      if (i2c_Rbit() != 0){
	 goto toggleAlarm1Begin;
      }
      
      if (enable){
	 i2c_SendByte(enableInt);
      }
      else {
	 i2c_SendByte(0x04);
      }
       
       // Ack
      if (i2c_Rbit() != 0){
	 goto toggleAlarm1Begin;
      }
      
      i2c_SendByte(0x04);
      
             // Ack
      if (i2c_Rbit() != 0){
	 goto toggleAlarm1Begin;
      }
      
      
      
      
      i2c_stop();
      sei();
       
       alarm1ON = enable;
   return true;
}

bool i2c_alarm2(bool enable){
      byte wordAddress = 0x0E;
      byte enableInt = 0x02 | 0x04;
      
      if (alarm1ON){
	 enableInt |= 0x1;
      }
      cli();
      toggleAlarm2Begin:
      i2c_start();
      
      // Control Byte
      i2c_SendByte(BASE_ADDRESS_DS3232);
      
      
      // Ack
      if (i2c_Rbit() != 0){
	 goto toggleAlarm2Begin;
      }
      
      //Word Address
      i2c_SendByte(wordAddress);
   
      // Ack
      if (i2c_Rbit() != 0){
	 goto toggleAlarm2Begin;
      }
      
      if (enable){
	 i2c_SendByte(enableInt);
      }
      else {
	 i2c_SendByte(0x40);
      }
       
       // Ack
      if (i2c_Rbit() != 0){
	 goto toggleAlarm2Begin;
      }
      
            
      i2c_SendByte(0x04);
      
             // Ack
      if (i2c_Rbit() != 0){
	 goto toggleAlarm2Begin;
      }
      
      
      i2c_stop();
      sei();
       alarm2ON = enable;
       
   return true;
}

