#ifndef UartModuleH
#define UartModuleH

#define SerialMon Serial // Set serial for debug console (to the Serial Monitor, default speed 115200)
#define BANNER  "\r********************************************************************\r\n"
#define PROMPT  "\r\n> "
#define ACK     "\r\n>> "

//Serial CLI Communication
void PrintWelcomeMsg(String);
void PrintHelpMsg(void);
void ReadSerialMon(void);
void ProcessCmd(char*);

#endif