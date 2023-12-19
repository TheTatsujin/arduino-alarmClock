/* PrÃ¡ctica Lab4: Reloj despertador
 * 
 * DescripciÃ³n: Proyecto base con esquema, definiciones y programa demo
 * Programa demo: uso bÃ¡sico pantalla LCD
 *
 * Fichero: 	23-24_plab4_reloj_lmp.pdsprj
 * Creado: 		14 noviembre 2023
 * Autor:			--------
*/


#include "mainVars.h"
#include "i2cFunctions.h"
#include "Utils.h"

void mainMenu(){
   if(!configON){

      return;
   }
   switch (mainState) {
      case 0:
	 Serial.read();
	 Serial.print("Option: ");
	    while(!Serial.available()){
		     if(!configON){
			mainState = 5;
			return;
		     }
	    }
	      
	    moduleSelect = Serial.read() - 48;
	    if (moduleSelect > 3 || moduleSelect < 1){
	       Serial.println("Invalid module");
	       mainState = 0;
	    }
	    else{
	       Serial.println(moduleSelect);
	       mainState = moduleSelect;
	    }
	
      break;
      case 1:
	 memoryMenu();
	 mainState = 4;
      break;
      case 2:
	 rtcMenu();
	 mainState = 4;
      break;
      case 3:
	 // Counter Menu
	 mainState = 4;
      break;
      case 4:
      		 if(configON){
			  Serial.println("");
	 Serial.println("Press any key to continue");
		  }
	 
	 while(!Serial.available()){
		 if(!configON){
		     mainState = 5;
		     return;
		  }
	 
	 }
	 if (configON){
	    Serial.read();
	 }
	 
	 clearTerminal();
	 
	 mainState = 5;
      break;
      case 5:
	 Serial.println("Select module: ");
	 Serial.println("1, Memory chip 24LC64");
	 Serial.println("2. RTC DS3232");
	 Serial.println("3. Counter Menu");
	 mainState = 0;
      break;
      default:
	 Serial.println("Not valid state");
	 mainState = 5;
      break;
   }
}

ISR(TIMER1_OVF_vect){
   updateVars();
   updateLCD();
}


ISR(TIMER3_OVF_vect){
   configON = checkStatus();
   alarmBeep();
}

ISR(INT0_vect){
   Serial.println("Alarm is Beeping");
   Serial.println("Press # on the keyboard to turn off");
   alarmActivated();
}

void setup() {


	// habilitar canal TX0/RX0, canal de comunicaciones serie con el virtual terminal.
  Serial.begin(9600);
	// Prueba del la pantalla LCD
	// habilitar canal TX3/RX3, canal de comunicaciones serie con la pantalla LCD (MILFORD 4x20 BKP)
  Serial3.begin(9600); //canal 3, 9600 baudios,
                      // 8 bits, no parity, 1 stop bit
	delay(1000);
	//Estado inicial del Menu principal
	
	   pin_init();
   interrupt_init();
   
  mainState = 5;
   Serial.println("Modo Display");
}


void loop() {    
   mainMenu();
}

