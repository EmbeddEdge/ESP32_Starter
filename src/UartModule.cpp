#include <Arduino.h>
#include "UartModule.h"

extern uint8_t gFlagTrig1;
extern uint8_t gFlagTrig2; 
const int g_buf_index = 30;
char    g_buf_serialmon[g_buf_index];

/* **************PrintWelcomeMsg******************************************************
Prints the default Welcome message
*/
void PrintWelcomeMsg(String version)
{
    //SerialMon.print(F("\033[0;0H"));
    //SerialMon.print(F("\033[2J"));
    SerialMon.print(F(BANNER));
    SerialMon.print(F("\tWiFi RGB Strip Device Console\n\r"));
    SerialMon.print(F("\tVersion "));
    SerialMon.print(version);
    SerialMon.print(F("\tFor list of available commands type '-h' or 'help'\r\n"));
    SerialMon.print(F(BANNER));
    SerialMon.print(F(PROMPT));    
}

/* **************PrintHelpMsg*************************************************************************
Prints the the Help screen to explain commands
*/
void PrintHelpMsg()
{
  SerialMon.print(F("Commands sent in space delimited format <command> <arg1> <arg2> etc.\r\n"));
  SerialMon.print(F("   List of available commands:\r\n"));
  SerialMon.print(F("   (Can be used as listed or all lower case)\r\n"));
  SerialMon.println();
  SerialMon.print(F("B1.           Acts as Button 1 Press\r\n"));
  SerialMon.print(F("              Usage: 'B1' or 'b1'\r\n"));
  SerialMon.print(F("B2.           Acts as Button 2 Press\r\n"));
  SerialMon.print(F("Help.         Prints this Help screen\r\n"));
  SerialMon.print(F("              Usage: 'Help'\r\n"));
  SerialMon.print(F(PROMPT)); 
}

/* **************ReadSerialMon************************************
Read the CLI Serial command, periodically check the serial 
on a timer interrupt and fills a buffer. Once the terminating
characters are found the buffer is sent for processing in 
another function and then cleared
Input:  N/A
Output: N/A 
*/
void ReadSerialMon(void)
{
  char byte,lastByte;
  int iIndex;
  static int bIndex = 0;

  while((SerialMon.available()) && (bIndex < g_buf_index))
  {
    g_buf_serialmon[bIndex] = SerialMon.read();             //Fill an array with the user input characters
    lastByte = g_buf_serialmon[bIndex-1];
    byte = g_buf_serialmon[bIndex];
    ++bIndex;
      
    if (lastByte == '\r' && byte == '\n')                   //Process the Command when user hits return
    {
      ProcessCmd(g_buf_serialmon);
      for (iIndex = 0; iIndex < bIndex; iIndex++)
      {
        g_buf_serialmon[iIndex] = 0x00;
      }
      bIndex = 0;
    }
  }
}

/* **************ProcessCmd*****************************************************************
Process the input command and calls the necessary functions
Input:  p_Command -> Command char array to be processed
Output: N/A 
*/   
void ProcessCmd(char* p_Command)
{
    char* userCMD;

    userCMD = strtok(p_Command, " ");

    if(strcmp(userCMD , "B1\r\n")==0 || strcmp(userCMD , "b1\r\n")==0)
    {
      gFlagTrig1 = 1;
      SerialMon.print(F("Simulation Button 1 Pressed\n\r"));
    }
    else if(strcmp(userCMD , "B2\r\n")==0 || strcmp(userCMD , "b2\r\n")==0)
    {
      gFlagTrig2 = 1;
      SerialMon.print(F("Simulation Button 2 Pressed\n\r"));
    }
    else if(strcmp(userCMD , "CommWithPar")==0 || strcmp(userCMD , "CommWithPar")==0)
    {
      uint8_t count = 0;
      while (userCMD != NULL)
      {
        if (count == 1) 
        {
          //pv_values.str_input_watthour = userCMD;
          SerialMon.print(F("Use the 1st Parameter value\n\r"));
        }
        else if(count == 2)
        {
          //timing_values.str_duration = userCMD;
          SerialMon.print(F("Use the 2nd Parameter value\n\r"));
        }
        else if (count == 3)
        {
          //timing_values.str_duration = userCMD;
          SerialMon.print(F("Use the 3rd Parameter value\n\r"));
        }
        userCMD = strtok(NULL, " ");  //Go to next string in command buffer
        ++count;
        #if DEBUG
        SerialMon.print("userCMD: ");
        SerialMon.println(userCMD);
        SerialMon.print("count: ");
        SerialMon.println(count);
        #endif
      }
      //g_enum_action_mode = STATE_ACTION_PUBLISH_INPUT_ENERGY;
    }
    else if(strcmp(userCMD , "Help\r\n")==0 || strcmp(userCMD , "help\r\n")==0)
    {
      PrintHelpMsg();
    }
    else
    {
      SerialMon.print(F("You typed: "));
      SerialMon.println(userCMD);
      SerialMon.println(F("\rCommand not recognised. Please Enter a Valid Command"));
      SerialMon.print(F(PROMPT));
    }

}