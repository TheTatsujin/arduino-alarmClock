// -----Funciones------
extern void updateVars();

//Inicializa todos los pines necesarios
extern void pin_init();

// Inicializa todas las interrupciones necesarias
extern void interrupt_init();

// Display 7 Seg y Keyboard
extern bool checkStatus();

// Terminal I/O
extern String readLine();
extern void clearTerminal();


// Menu RTC DS3232

  /*
   RTC Menu: Maquina de Estado Finito
      state 0: Read menu option
      state 1: Read Chip Select
      state 2: Read Mem Address
      state 3: Write Operation
      state 4: Read Operation
      state 5: Final [Press enter to continue]
  
  
  */
  
extern void rtcMenu();


// Menu Chip de Memoria 24LC64 

  /*
   Memory Menu: Maquina de Estado Finito
      state 0: Read menu option
      state 1: Read Chip Select
      state 2: Read Mem Address
      state 3: Write Operation
      state 4: Read Operation
      state 5: Final [Press enter to continue]
  
  
  */
extern void memoryMenu();


// LCD Display
extern void updateLCD();


// ALARM
extern void alarmActivated();
extern void alarmBeep();
