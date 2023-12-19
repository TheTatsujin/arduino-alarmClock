#include "I2cFunctions.h"
// ------Variables------

#define PRIGHT  30    // pulsador right
#define PDOWN   31    // "" down
#define PLEFT   32    // "" left
#define PENTER   33    // "" center
#define PUP     34    // "" up
#define SPEAKER 37    // speaker

#define D4    0xFE    // 1111 1110 unidades
#define D3    0xFD    // 1111 1101 decenas
#define D2    0xFB    // 1111 1011 centenas
#define D1    0xF7    // 1111 0111 millares
#define DOFF  0xFF    // 1111 1111 apagado: todos los cÃƒÆ’Ã‚Â¡tados comunes a "1"
#define DON   0xF0    // 1111 0000   todos los cÃƒÆ’Ã‚Â¡tados comunes a "0"



// Keyboard
String key_buffer;
char teclado_map[][3] = { {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};


// 7 Segment Display
int display[] = { D4, D3, D2, D1, DOFF, DON };
//Mapeado de cada caracter
int char_map[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x63, 0x39};

int digit = 0;
int counter = 0;
int display_mode = 4;
int max_count = 9999; 

String time;
String temp;
String date;
String alarm1;
String alarm2;

void updateVars(){
   time = i2c_getTime();
   date = i2c_getDate();
   temp = i2c_getTemperature();
   alarm1 = i2c_getAlarm1();
   alarm2 = i2c_getAlarm2();

}

bool alarm = false;
bool alarm1_ON = false;
bool alarm2_ON = false; 

bool alarm1DisplayON = false;
bool displayMode = true;

String alarm1Display = "";

void pin_init(){
   i2c_init();
     // PORTA: Segmentos a-f
   DDRA=0xFF;    // PORTA de salida
   PORTA=0xFF;    // activamos segmentos a-g
	
	// PORTL[7:4]: filas del teclado
   DDRL=0x0F;    // input;
   PORTL=0xFF;     // pull-up activos, cÃ¡todos/columnas teclado desactivadas 
	
	// PORTC: Pulsadores y altavoz
   DDRC=0x01;    //PC7:1 input: PC0: output-speaker
   PORTC= 0xFE;   // pull-up activos menos el speaker que es de salida
	
}

void interrupt_init(){
   cli();
   // TIMER 1 OVF INTERRUPT
   
   // INTERRUPT ENABLE + F LAG
   TIMSK1 = 0x01;
   TIFR1 = 0x01;
   
   // CONTROL Registers
   TCCR1A = 0x83;
   TCCR1B = 0x9D;
   
   // TOP
   OCR1A = 7811.5;
   
   
   // TIMER 3  OVERFLOW   INTERRUPT
   
   // INTERRUPT ENABLE + F LAG
   TIMSK3 = 0x01;
   TIFR3 = 0x01;
   
   // CONTROL Registers
   TCCR3A = 0x0B;
   TCCR3B = 0x92;
   
   // TOP
   OCR3A = 4000;
   
   
   // INT0 INTERRUPT
  EICRA  |= (1 << ISC21) ;
  EIMSK  |= (1 << INT0);
   
   sei();
   
   // Set Alarm for when Hours, Minutes and Seconds Match
   i2c_alarmInit();
}


byte hexStringToByte(String hexString) {
   		     if(displayMode){
			return 0;
		     }
  unsigned long value = strtoul(hexString.c_str(), NULL, 16);

  // Take only the lowest 8 bits to get a byte
  byte result = value & 0xFF;

  return result;
}


int hexStringToInt(String hexString) {
   		     if(displayMode){
			return 0;
		     }
  unsigned long value = strtoul(hexString.c_str(), NULL, 16);
  int result = value & 0xFFFF;

  return result;
}


String hexToString(int data){
   		     if(displayMode){
			return "";
		     }
   String hexString = String(data, HEX);
  hexString.toUpperCase();
   return hexString;
}

void clearTerminal(){
   Serial.write(12);
}

void stringToCharArray(const String& source, char* destination) {
  // Copy the content of the String to the char array
  source.toCharArray(destination, source.length() + 1);
}

String readLine() {
  String line = "";
  bool endLine = false;
   if(displayMode){
      return "";
   }

  while (!endLine) {
      if(displayMode){
	 return "";
      }
    if (Serial.available() > 0) {
      char c = Serial.read();
      Serial.print(c);
      if (c == '\n' || c == '\r') { // ENTER
        endLine = true;
      } else {
        line += c;
      }
    }
  }

  return line;
}



void updateLCD(){

  Serial3.write(0xFE); Serial3.write(0x08); // Display off
  
            // Clear Screen
   Serial3.write(0xFE);
   Serial3.write(0x01);
   
   
   // Cursor at start [Linea 1]
   Serial3.write(0xFE);
   Serial3.write(0x00);
   
   
   if (alarm1DisplayON){
      // posicionarse en linea 2
      Serial3.write(0xFE); 
      Serial3.write(0xC0); 
      // Margen 
      Serial3.write("      ");
      Serial3.write(alarm1Display.c_str());
      
         Serial3.write(0xFE); Serial3.write(0x0C); // Display on
      return;
   }
   // Margen de 5 
      Serial3.write("     ");

   
   Serial3.write(time.c_str());
   
   // posicionarse en linea 2
   Serial3.write(0xFE); 
   Serial3.write(0xC0); 
   
   Serial3.write("ALARM");
   
    // Margen de 9

      Serial3.write("        ");

   
   Serial3.write("T=");
   Serial3.write(temp.c_str());
   
  // posicionarse en linea 3
  Serial3.write(0xFE); 
  Serial3.write(0x94); 
  
  Serial3.write(alarm1.c_str());
  if (alarm1_ON){
      Serial3.write(" ON ");
  } else{
      Serial3.write(" OFF");
  }
  
  // posicionarse en linea 4
  Serial3.write(0xFE); 
  Serial3.write(0xD4); 
  
  Serial3.write(alarm2.c_str());
    if (alarm2_ON){
      Serial3.write(" ON ");
  } else{
      Serial3.write(" OFF");
  }
   
    // Margen de 2
      Serial3.write("  ");

   
   
   Serial3.write(date.c_str());
   
   // Cursor at start [Linea 1]
   Serial3.write(0xFE);
   Serial3.write(0x00);
   
   Serial3.write(0xFE); Serial3.write(0x0C); // Display on
  
   
}




void rtcMenu(){
   bool endOperation = false;
   int state = 4;
   int mode;
   if(displayMode){
      return;
   }
   while (!endOperation){
      switch (state){
            case 0:
	       Serial.read();
	       Serial.print("Option: ");
		  while(!Serial.available()){
		     if(displayMode){
			return;
		     }
		  }
		    
		  mode = Serial.read() - 48;
		  if (mode > 2 || mode < 1){
		     Serial.println("Invalid mode");
		     state = 0;
		  }
		  else{
		     Serial.println(mode);
		     state = mode;
		  }
	      
	      break;
	      case 1:
	       rtcDisplayMenu();
	       state = 3;
	      break;
	      case 2:
	       rtcConfigMenu();
	       state = 3;
	      break;
	      case 3:
	       endOperation = true;
	       state = 4;
	      break;
	      case 4:
	       Serial.println();
	       Serial.println("1. RTC Display");
	       Serial.println("2. RTC Config");
	       state = 0;
	      break;
	      default:
	       Serial.println("Not valid state");
	       state = 4;
	      break;
      }
   
   
   
   }
   

}


void rtcConfigMenu(){
   bool endOperation = false;
   int state = 13;
   int mode;
   byte seconds;
   byte minutes;
   byte hour;
   
   byte day;
   byte month;
   byte year;
   bool operationOk;
   
   if(displayMode){
      return;
   }
   while (!endOperation){
      switch (state){
            case 0:
	       Serial.read();
	       Serial.print("Entrar opcion: ");
		  while(!Serial.available()){
		      if(displayMode){
			return;
		     }
		  }
		    
		  mode = readLine().toInt();
		  if (mode > 10 || mode < 1){
		     Serial.println("opcion invalida");
		     state = 0;
		  }
		  else{
		     state = mode;
		  }
	      
	      break;
	      case 1:
	       Serial.println("--Set Time--");
	       state = 11;
	      break;
	      case 2:
	       Serial.println("--Set Date--");
	       		  Serial.println();
		  Serial.print("Day of the month: ");
		  day = readLine().toInt();
		  if(displayMode){
			return;
		     }
		  if (day > 31 || day < 0){
		     Serial.println("Invalid Day");
		     state = 2;
		  }
		  Serial.print("Month: ");
		  month = readLine().toInt();
		  if(displayMode){
			return;
		     }
		  if (month > 12 || month < 0){
		     Serial.println("Invalid Month");
		     state = 2;
		  }
		  Serial.print("Year: ");
		  year = readLine().toInt() % 100;
		  if(displayMode){
			return;
		     }

		operationOk = i2c_setDate(day, month, year);
	       if (operationOk){
		  Serial.println("Date was set correctly");
	       }else{
		  Serial.println("There was an error setting date, try again");
	       }
	       state = 12;
	      break;
	      case 3:
   	       Serial.println("--Set Alarm 1--");
	       state = 11;
	      break;
	      case 4:
	       Serial.println("--Alarm 1: ON--");
	       alarm1_ON = true;
	       i2c_alarm1(true);
	       state = 12;
	      break;	      
	      case 5:
	       Serial.println("--Alarm 1: OFF--");
	       alarm1_ON = false;
	       i2c_alarm1(false);
	       state = 12;
	      break;
	      case 6:
	       Serial.println("--Set Alarm 2--");
	       state = 11;
	      break;
	      case 7:
	       Serial.println("--Alarm 2: ON--");
	       alarm2_ON = true;
	       i2c_alarm2(true);
	       state = 12;
	      break;
	      
	      case 8:
	       Serial.println("--Alarm 2: OFF--");
	       alarm2_ON = false;
	       i2c_alarm2(false);
	       state = 12;
	      break;
	      
	      case 9:
	       Serial.println("--Alarm 1 and Alarm 2 OFF--");
	       alarm1_ON = false;
	       i2c_alarm1(false);
	       
	       alarm2_ON = false;
	       i2c_alarm2(false);
	       state = 12;
	      break;
	      case 10:
	       seconds = i2c_ReadMem(0x00, 2);
	       minutes = i2c_ReadMem(0x00, 1);
	       hour = i2c_ReadMem(0x00, 0);
	       
	       
	       char formattedTime[11]; // HH:MM:SS + null terminator
	       snprintf(formattedTime, sizeof(formattedTime), "%02d:%02d:%02d", hour, minutes, seconds);
	       alarm1Display = String(formattedTime);
	       
	       alarm1DisplayON = true;
	       Serial.println(alarm1Display);
	       state = 12;
	      break;
	      case 11:
		  Serial.println();
		  Serial.print("Seconds: ");
		  seconds = readLine().toInt();
		  if(displayMode){
			return;
		     }
		  if (seconds > 59 || seconds < 0){
		     Serial.println("Invalid Seconds");
		     state = 12;
		     break;
		  }
		  Serial.print("Minutes: ");
		  minutes = readLine().toInt();
		  if(displayMode){
			return;
		     }
		  if (minutes > 59 || minutes < 0){
		     Serial.println("Invalid Minutes");
		     state = 12;
		     break;
		  }
		  Serial.print("Hour [24h format]: ");
		  hour = readLine().toInt();
		  if(displayMode){
			return;
		     }
		  if (hour > 23 || hour < 0){
		     Serial.println("Invalid Hour");
		     state = 12;
		     break;
		  }
		  if (mode == 1){
		     operationOk = i2c_setTime(seconds, minutes, hour);
		  }
		  else if (mode == 3){
		     operationOk = i2c_setAlarm1(seconds, minutes, hour);
		  }
		  else if (mode == 6){
		     operationOk = i2c_setAlarm2(seconds, minutes, hour);
		  }
		  
		  if (operationOk){
		     Serial.println("Correctly configured");
		  } else{
		     Serial.println("There was an error, try again");
		  }
		  state = 12;
	      break;
	      case 12:
	       endOperation = true;
	       state = 13;
	      break;
	      case 13:
	       Serial.println();
	       Serial.println("*** Menu de configuracion ***");
	       Serial.println("1.-  Ajustar hora");
	       Serial.println("2.-  Ajustar fecha");
	       
	       Serial.println();
	       Serial.println("3.- Configurar Alarma 1");
	       Serial.println("4.- Alarma 1 ON");
	       Serial.println("5.- Alarma 2 OFF");
	       
	       Serial.println();
	       Serial.println("6.- Configurar Alarma 2");
	       Serial.println("7.- Alarma 2 ON");
	       Serial.println("8.- Alarma 2 OFF");
	       
	       Serial.println();
	       Serial.println("9.- Apagar sonido de alarmas 1 y 2");
	       
	       Serial.println();
	       Serial.println("10.- Mostrar configuracion Alarma 1 guardada");
	       
	       Serial.println();
	       state = 0;
	      break;
	      default:
	       Serial.println("Not valid state");
	       state = 13;
	      break;
      }
   
   
   
   }

}
void rtcDisplayMenu(){
   bool endOperation = false;
   int state = 7;
   int mode;
   String temp;
   String time;
   String date;
   String alarm1;
   String alarm2;
   if(displayMode){
      return;
   }
   while (!endOperation){
      switch(state){
	 case 0:
	    Serial.read();
	    Serial.print("Option: ");
	       while(!Serial.available()){
		  	if(displayMode){
			return;
		     }
	       }
		 
	       mode = Serial.read() - 48;
	       if (mode > 5 || mode < 1){
		  Serial.println("Invalid mode");
		  state = 0;
	       }
	       else{
		  Serial.println(mode);
		  state = mode;
	       }
	   break;
	   case 1:
	   	       Serial.println();
		       
	       Serial.println("--Temperature--");
	       temp = i2c_getTemperature();
	       Serial.print(temp);
	       state = 6;
	   break;
	   case 2:
	   	       Serial.println();
	       Serial.println("--Time--");
	       time = i2c_getTime();
	       Serial.println(time);
	       state = 6;
	   break;
	   case 3:
	   	       Serial.println();
	       date = i2c_getDate();
	       Serial.println("--Date--");
	       Serial.println(date);
	       state = 6;
	   break;
	   case 4:
	       alarm1 = i2c_getAlarm1();
	       Serial.println("--Alarm 1--");
	       Serial.println(alarm1);
	       state = 6;
	   break;
	  case 5:
	       alarm2 = i2c_getAlarm2();
	       Serial.println("--Alarm 2--");
	       Serial.println(alarm2);
	       state = 6;
	   break;
	   
	    case 6:
	       endOperation = true;
	       state = 7;
	   break;
	   case 7:    
	       Serial.println();
	       Serial.println("Select operation: ");
	       Serial.println("1. Show temperature");
	       Serial.println("2. Tell the time");
	       Serial.println("3. Show Date");
	       Serial.println("4. Show Alarm 1");
	       Serial.println("5. Show Alarm 2");
	       
	       state = 0;
	   break;
	   default:
	    Serial.println("Not valid state");
	    state = 7;
	    
	 
      
      }
   
   }

}

void memoryMenu(){
   int state = 6;
   int userInput;
   int mode;

   byte chipSelect;
   int memAddress;
   byte data;
   
   bool endOperation = false;
   
   
   		     if(displayMode){
			return;
		     }
   while (!endOperation){
      switch (state){
      
      case 0:
      Serial.read();
      Serial.print("Option: ");
	 while(!Serial.available()){
	 		     if(displayMode){
			return;
		     }
	 }
	   
	 mode = Serial.read() - 48;
	 if (mode > 2 || mode < 1){
	    Serial.println("Invalid mode");
	    state = 0;
	 }
	 else{
	    Serial.println(mode);
	    state = 1;
	 }
     
      break;
      case 1:
	    Serial.println();
	    Serial.print("Select Chip [0, 1]: ");

	 userInput = hexStringToInt(readLine());
	    	if(displayMode){
			return;
		     }
	  if (userInput > 1 || userInput < 0){
	       Serial.println("Invalid chip");
	       state = 1;
	    }
	    else{
	       chipSelect = userInput & 0x01;
	       state = 2;
	    }

      break;
      case 2:
	    Serial.println();
      	    Serial.print("Enter Memory Address [13 bits, HEX]: ");

	 userInput = hexStringToInt(readLine());
	 if(displayMode){
			return;
		     }
	  if (userInput > 0x1FFF || userInput < 0x0000){
	       Serial.println("Invalid address");
	       state = 2;
	    }
	    else if (mode == 1){ // Write
		 memAddress =  userInput & 0x1FFF;
		 state = 3;
	    }
	    else if (mode == 2){ // Read
	       memAddress = userInput & 0x1FFF;
	       state = 4;
	    }

      
      break;
      case 3:
	  Serial.println();
	  Serial.print("Enter byte to write [8 bits, HEX]: ");  	       

	  userInput = hexStringToByte(readLine());
	  if(displayMode){
			return;
		     }
	  if (userInput > 0xFF || userInput < 0x00){
	       Serial.println("Not a byte");
	       state = 3;
	    }
	    else{
	       Serial.println("");
	       data = userInput & 0xFF;
	       // Write operation
		      Serial.print("Chip Selected: ");
			Serial.println(hexToString(chipSelect));
			
			Serial.print("Memory Address: ");
			Serial.println(hexToString(memAddress));
			
			Serial.print("Data to write: ");
			Serial.println(hexToString(data));
			bool result = i2c_WriteMem(chipSelect, memAddress, data);
			if (result){
			   Serial.println("The data was written successfuly in memory");
			}else{
			   Serial.println("There was an error writing in memory");
			}
			
			state = 5;
	    }
	    

      break;
      case 4:
	 // Read operation
	 Serial.println();
	 Serial.print("Chip Selected: ");
	 Serial.println(hexToString(chipSelect));
	 
	 Serial.print("Memory Address: ");
	 Serial.println(hexToString(memAddress));
	 
	 data = i2c_ReadMem(chipSelect, memAddress);
	 
	    Serial.print("The data: ");
	    Serial.println(hexToString(data));
	 
	 state = 5;
      break;
      case 5:
	 endOperation = true;
	 state = 6;
	 
      break;
      case 6:
	 Serial.println();
	 Serial.println("Select operation: ");
	 Serial.println("1.-Write a byte to 24LC64 Memory");
	 Serial.println("2.-Read a byte from 24LC64 Memory\r");
	 state = 0;
      break;
      
      default:
	 Serial.println("Not valid state");
	 state = 0;
      break;


   delay(100);
       
  }  
   
   
   }

}


 
void keyboard(int digit) {
  int value = PINL;
  value = value >> 4;
  if (value == 15) {
    // NOT VALID
    return;
  }
  while ((PINL >> 4) != 15);
  switch (value) {
    case 0x7:
      //FILA 1
      if (digit == 2 && key_buffer == "#"){
	 Serial.println("Modo visualizacion extendida Alarma 1");
	 display_mode = 2;
	 key_buffer = "";
	 counter = i2c_getAlarm1Sec();
	 break;
      }
      

      key_buffer += teclado_map[0][digit];
      break;
    case 0x0B:
      //FILA 2
      key_buffer += teclado_map[1][digit];
      break;
    case 0x0D:
      //FILA 3
      if (digit == 2 && key_buffer == "#"){
	 byte hour = i2c_getAlarm1Hour();
	 byte minutes = i2c_getAlarm1Min();
	 byte seconds = i2c_getAlarm1Sec();
	 i2c_WriteMem(0x00, 0, hour);
	 i2c_WriteMem(0x00, 1, minutes);
	 i2c_WriteMem(0x00, 2, seconds);
	 Serial.println("Configuracion Alarma 1 guardada");
	 key_buffer = "";
	 break;
      }
      
      key_buffer += teclado_map[2][digit];
      break;
    case 0x0E:
      //FILA 4
      if (digit == 0 && key_buffer == "#" && !displayMode){
            clearTerminal();
	       Serial.println("Modo Display");
	       displayMode = true;
	       key_buffer = "";
	       	    display_mode = 4;
		    counter = 0;
		    	       alarm1DisplayON = false;
	       break;

      }
      if (digit == 2) {
        // ENTER
	Serial.println();
	 if (key_buffer == ""){
	    key_buffer = "#";
	    break;
	}
	
	if (alarm && key_buffer == ""){
	    Serial.println("Alarm Stopped");
	    alarm = false;
	      key_buffer = "";
	      break;

	}
	 if (key_buffer == "*" && displayMode){
	          clearTerminal();
		  alarm1DisplayON = false;
	    Serial.println("Modo Configuracion");
	       displayMode = false;
	            key_buffer = "";
		    display_mode = 4;
		    counter = 0;
		    break;
	}
	   
	 if (key_buffer == "#"){
	    key_buffer == "";
	    break;
	 }
	
	
	if (key_buffer != "" && key_buffer != "#") {
	       Serial.println();
	      Serial.print("Keyboard entered: ");
	      Serial.println(key_buffer);
	      key_buffer = "";
	      break;
	}
	
	 

 
      } else {
        key_buffer += teclado_map[3][digit];
      }
      break;
  }
}



bool checkStatus() {
  PORTL = DOFF;
 

  //Variables de contador
  int unidades = counter % 10;
  int decenas = (counter % 100) / 10;
  int centenas = (counter % 1000) / 100;
  int milesimas = (counter % 10000) / 1000;
  int digit_vect[] = { char_map[unidades], char_map[decenas], char_map[centenas], char_map[milesimas] };
  
  
  
 
  //Variables de temperatura
  int temp_unidades;
  int temp_decenas;
  int temperatureCelsius;
  float voltage;
  int sensorValue;
  
 
  switch (display_mode) {
    case 0:
      //TEMPERATURA EN CELSIUS
      sensorValue = analogRead(54);
      // Conversion de analogico a voltage
      voltage = (sensorValue / 1023.0) * 5.0;
      // Conversion voltage -> temperatura
      temperatureCelsius = (voltage - 0.5) * 100.0;
      temp_unidades = temperatureCelsius % 10;
      temp_decenas = (temperatureCelsius % 100) / 10;
 
      if (digit == 0){
        // Caracter "C"
        PORTA = char_map[11];
      }
      else if (digit == 1) {
        // Caracter "º"
        PORTA = char_map[10];
      } 
      else if (digit == 2) {
        PORTA = char_map[temp_unidades];
      }
       else if (digit == 3) {
        PORTA = char_map[temp_decenas];
       }
      if (digit == 2) {
        PORTL = D2;
      } else if (digit == 3) {
        PORTL = D1;
      } else {
        PORTL = display[digit];
      }
      keyboard(digit);
      digit = (digit + 1) % 4;
      break;
    case 1:
      //OFF-CENTENAS-DECENAS-UNIDADES
      max_count = 999;
      PORTA = digit_vect[digit];
      PORTL = display[digit];
      keyboard(digit);
      digit = (digit + 1) % 3;
      break;
    case 2:
      //OFF-OFF-DECENAS-UNIDADES
      if (digit == 2) {
        PORTA = 0x00;
      } else {
        PORTA = digit_vect[digit];
      if (alarm1_ON && digit == 0){
	    PORTA |= 0x80;
	}
      }

      PORTL = display[digit];
      keyboard(digit);
      digit = (digit + 1) % 3;
      max_count = 99;
      break;
    case 3:
      //DECENAS-UNIDADES-OFF-OFF
      if (digit == 1 or digit == 0) {
        PORTA = 0x00;
      } else if (digit == 3) {
        PORTA = char_map[decenas];
      } else if (digit == 2) {
        PORTA = char_map[unidades];
      }
 
      if (digit == 2) {
        PORTL = D2;
      } else if (digit == 3) {
        PORTL = D1;
      } else {
        PORTL = display[digit];
      }
 
      keyboard(digit);
      digit = (digit + 1) % 4;
      max_count = 99;
      break;
      case 4:
      //MILESIMAS-CENTENAS-DECENAS-UNIDADES
      max_count = 9999;
      PORTA = digit_vect[digit];
      PORTL = display[digit];
      keyboard(digit);
      digit = (digit + 1) % 4;
      break;
      
  }
  
  return !displayMode;
}

void alarmActivated(){
   alarm = true;
}

void alarmBeep(){
   if(alarm){
           tone(SPEAKER, 50, 300); 
	   //Serial.println("OHIOOOOO");
   }
}


