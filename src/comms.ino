/**
*  Polargraph Server for ESP32 based microcontroller boards.
*  Written by Sandy Noble
*  Released under GNU License version 3.
*  http://www.polargraph.co.uk
*  https://github.com/euphy/polargraph_server_polarshield_esp32

Comms.

This is one of the core files for the polargraph server program.
Comms can mean "communications" or "commands", either will do, since
it contains methods for reading commands from the serial port.

*/

boolean commandConfirmed = false;
boolean commandBuffered = false;

volatile int bufferPosition = 0;

//static char nextCommand[INLENGTH+1];
char nextCommand[INLENGTH+1];

volatile static boolean currentlyExecutingACommand = false;

#define READY_STR "READY_200"
#define RESEND_STR "RESEND"
#define DRAWING_STR "DRAWING"


void comms_clearParams() {
  strcpy(inCmd, "");
  strcpy(inParam1, "");
  strcpy(inParam2, "");
  strcpy(inParam3, "");
  strcpy(inParam4, "");
  inNoOfParams = 0;
}

void comms_emptyCommandBuffer(char * buf, int length)
{
  for (int i=0; i<length; i++) {
    buf[i] = 0;
  }
}

boolean comms_parseCommand(char * inS)
{
#ifdef DEBUG_COMMS
  Serial.print("parsing inS: ");
  Serial.println(inS);
#endif
  char * comp = strstr(inS, CMD_END);
  if (comp != NULL)
  {
#ifdef DEBUG_COMMS
    Serial.println(F("About to extract params"));
#endif
    comms_extractParams(inS);    
    return true;
  }
  else if (comp == NULL) {
    Serial.println(F("IT IS NULL!"));
    return false;
  }
  else {
    Serial.println(F("Could not parse command."));
    return false;
  }
}

void comms_extractParams(char* inS) {

  // get number of parameters
  // by counting commas
  int paramNumber = 0;
  char * param = strtok(inS, COMMA);

  while (param != 0) {
    #ifdef DEBUG_COMMS
          Serial.print(F("bParam "));
          Serial.print(paramNumber);
          Serial.print(": ");
          Serial.print(param);
          Serial.print(" in ascii: ");
          for (int i = 0; i<sizeof(param); i++) {
            Serial.print(" ");
            Serial.print((int)param[i]);
          }
          Serial.println(".");
    #endif
    switch(paramNumber) {
      case 0:
        strcpy(inCmd, param);
        break;
      case 1:
        strcpy(inParam1, param);
        break;
      case 2:
        strcpy(inParam2, param);
        break;
      case 3:
        strcpy(inParam3, param);
        break;
      case 4:
        strcpy(inParam4, param);
        break;
      default:
        break;
    }
    paramNumber++;
    param = strtok(NULL, COMMA);
  }
  inNoOfParams = paramNumber;
#ifdef DEBUG_COMMS
    Serial.print(F("Command:"));
    Serial.print(inCmd);
    Serial.print(F(", p1:"));
    Serial.print(inParam1);
    Serial.print(F(", p2:"));
    Serial.print(inParam2);
    Serial.print(F(", p3:"));
    Serial.print(inParam3);
    Serial.print(F(", p4:"));
    Serial.print(inParam4);
    Serial.print(F(", inNoOfParams "));
    Serial.println(inNoOfParams);
#endif
}

void comms_executeParsedCommand()
{
#ifdef DEBUG_COMMS
  Serial.print(F("currentlyExecutingACommand: "));
  Serial.println(currentlyExecutingACommand);
  Serial.print(F("Params extracted: "));
  Serial.println(paramsExtracted);
#endif
  if (paramsExtracted)
  {
    // implement the parmsExtracted: currentCommand -> lastParsedCommandRaw -> paramsExtracted -> inParam*   following C17
    impl_processCommand(inCmd, inParam1, inParam2, inParam3, inParam4, inNoOfParams);
    paramsExtracted = false;
    inNoOfParams = 0;
  }
}

long asLong(String inParam)
{
  char paramChar[inParam.length() + 1];
  inParam.toCharArray(paramChar, inParam.length() + 1);
  return atol(paramChar);
}
int asInt(String inParam)
{
  char paramChar[inParam.length() + 1];
  inParam.toCharArray(paramChar, inParam.length() + 1);
  return atoi(paramChar);
}
byte asByte(String inParam)
{
  int i = asInt(inParam);
  return (byte) i;
}
float asFloat(String inParam)
{
  char paramChar[inParam.length() + 1];
  inParam.toCharArray(paramChar, inParam.length() + 1);
  return atof(paramChar);
}

void comms_establishContact()
{
  comms_ready();
}


boolean comms_nextCommandIsBuffering()
{
  return bufferPosition > 0;
}

boolean comms_isMachineReadyForNextCommand()
{
  if (broadcastStatusChrono.hasPassed(comms_rebroadcastStatusInterval))
  {
    broadcastStatusChrono.restart();
    //return !commandBuffered && !comms_nextCommandIsBuffering(); don't need a function call for this
    return (!commandBuffered && !bufferPosition);  // true is not buffered or buffering
  }  else {
    return false;
  }
}


void comms_ready()
{
  PGclient.println(READY_STR);
}

void comms_reportBufferState()
{
  #ifdef DEBUG_COMMS_BUFF
  Serial.print("Serial.available: ");
  Serial.println(Serial.available());
  Serial.print("bufferPosition: ");
  Serial.println(bufferPosition);
  Serial.print("currentCommand: '");
  Serial.print(currentCommand);
  Serial.print("', nextCommand: '");
  Serial.print(nextCommand);
  Serial.println("'.");

  Serial.print("commandBuffered: ");
  Serial.print(commandBuffered);
  Serial.print(", commandConfirmed: ");
  Serial.println(commandConfirmed);
  #endif
}

void comms_drawing()
{
  PGclient.println(DRAWING_STR);
}
void comms_requestResend()
{
  PGclient.println(RESEND_STR);
}
void comms_unrecognisedCommand(String inCmd, String inParam1, String inParam2, String inParam3, String inParam4, int inNoOfParams)
{
  PGclient.print(MSG_ERROR_STR);
  PGclient.print(inCmd);
  PGclient.println(F(" not recognised."));

  Serial.print(F("Sorry, command: "));
  Serial.print(inCmd);
  Serial.print(F(", p1:"));
  Serial.print(inParam1);
  Serial.print(F(", p2:"));
  Serial.print(inParam2);
  Serial.print(F(", p3:"));
  Serial.print(inParam3);
  Serial.print(F(", p4:"));
  Serial.println(inParam4);
  Serial.println(F(" isn't a command I recognise."));
}

/*-----------------------------------------------------------------*/
// COMMS Read Task, every 20 ms...
/*-----------------------------------------------------------------*/
int commsReadCore = 0;
void commsRead(void * pvParameters) {

  Serial.printf("commsRead task: Executing on core %d\n",xPortGetCoreID());

  for (;;) {
    commsReadCore = xPortGetCoreID();

    if (!commandBuffered) {
      // bufferPosition = 0;
      if (PGclient.available() > 0) {
        #ifdef DEBUG_COMMS_BUFF
              Serial.print("Bufsize: ");
              Serial.println(Serial.available());
              Serial.print("Pos:");
              Serial.println(bufferPosition);
        #endif
        char ch = PGclient.read();       // get it
        nextCommand[bufferPosition] = ch;
        #ifdef DEBUG_COMMS_BUFF
              Serial.print("Just got '");
              Serial.print(ch);
              Serial.print("', so nextCommand is ");
              Serial.print(strlen(nextCommand));
              Serial.print(" chars long, and contains: ");
              Serial.println(nextCommand);
        #endif

        if (ch == INTERMINATOR) {
          #ifdef DEBUG_COMMS_BUFF
          Serial.print("Buffer term'd at position ");
          Serial.print(bufferPosition);
          Serial.print(", command is: ");
          Serial.println(nextCommand);
          #endif
          nextCommand[bufferPosition] = 0;  // null terminate the string

          if (strlen(nextCommand) <= 0) {
            // it's zero! empty it!
            nextCommand[0] = 0;
            // comms_emptyCommandBuffer(nextCommand, INLENGTH); // hmm, don't need it
            commandBuffered = false;
            comms_requestResend();
          } else {
            // command finished!
            #ifdef DEBUG_COMMS
                      Serial.print("command buffered: ");
                      Serial.println(nextCommand);
            #endif
            commandBuffered = true;
            bufferPosition = 0;
          }
        }
        else if (ch >= 40) {
          bufferPosition++;
          if (bufferPosition > INLENGTH)
          { // if the command is too big, chuck it out!
            // currentCommand[0] = 0;       // seems wrong
            nextCommand[0] = 0;
            commandBuffered = false;
            bufferPosition = 0;
          }
        }
        lastInteractionTime = millis();
      }
      else if ((lastInteractionTime+2000 < millis()) && (bufferPosition > 0)) {
        // 2000 gives a manual inputter some time to fumble at the keyboard
        // taken a long time for the next char to arrive...
        // cancel it!
        Serial.println("Command timed out: Cancelling");
        bufferPosition = 0;
        nextCommand[0] = 0;
        // comms_emptyCommandBuffer(nextCommand, INLENGTH); // hmm, don't need it
        commandBuffered = false;
        comms_requestResend();
        lastInteractionTime = millis();
      }
    }

    /* when commsCommand is ready to process a command, it will take nextCommand,
      clear it, set commandBuffered to false.  Nothing else needed here
    
    // maybe promote the buffered command to the currentCommand
    if (!currentlyExecutingACommand && commandBuffered) {
      // not executing, but there's a command buffered
      strcpy(currentCommand, nextCommand);
      nextCommand[0] = 0;
      comms_emptyCommandBuffer(nextCommand, INLENGTH);
      commandConfirmed = true;
      commandBuffered = false;
      comms_ready(); 
    }
    */

    // delay for 20ms
    vTaskDelay(20 / portTICK_PERIOD_MS);

  }
}

/*-----------------------------------------------------------------*/
// COMMS Command Task... execute the completed command
/*-----------------------------------------------------------------*/
int commsCommandCore = 0;
void commsCommand(void * pvParameters) {

  Serial.printf("commsCommand task: Executing on core %d\n",xPortGetCoreID());

  for (;;) {
    commsCommandCore = xPortGetCoreID();

    if (!PGclient) {
      //need to connect a client
      PGclient = PGserver.available();
      delay(1000);
      if (PGclient) comms_ready();

      continue;
    }

    if (commandBuffered) {      // there's a command to process
    //if (commandConfirmed && !currentlyExecutingACommand) {
      //currentlyExecutingACommand = true;
      //#ifdef DEBUG_COMMS
      //Serial.print(F("Command Confirmed: "));
      //Serial.println(currentCommand);
      //#endif

      //strcpy(lastParsedCommandRaw, currentCommand);
      strcpy( currentCommand, nextCommand );
      nextCommand[0] = 0;
      commandBuffered = false;
      comms_ready();              // output the READY_200 message so it will start buffering next command

      paramsExtracted = comms_parseCommand(currentCommand);
      if (paramsExtracted) {
        //#ifdef DEBUG_COMMS
        //Serial.println(F("Params extracted."));
        //#endif
        //strcpy(currentCommand, "");
        //commandConfirmed = false;
  // execute parsed command:  currentCommand -> (lastParsedCommandRaw?) -> paramsExtracted  following C17
        comms_executeParsedCommand();   // beginning of a long chain of calls to actually move the motors
        /*
        comms_executeParsedCommand > 
        impl_processCommand > 
        impl_executeCommand > 
        exec_executeBasicCommand >
        exec_changeLengthDirect (C17) >
        exec_drawBetweenPoints >
        changeLength(float,float) > changeLength(long,long) >
        motorX.moveTo...

        comms_executeParsedCommand() returns when the move is complete
        */
        comms_clearParams();
      } else {
        Serial.println(F("Command not parsed."));
        //strcpy(currentCommand, "");
        comms_clearParams();
        //commandConfirmed = false;
      }
      //strcpy(lastParsedCommandRaw, "");
      //currentlyExecutingACommand = false;
    }

  // combined into comms task
  //void comms_broadcastStatus(){
    if (comms_isMachineReadyForNextCommand()) {   // timer expired (4 seconds), and no command buffered or buffering
      reportPosition();           // output the SYNC message
      reportStepRate();           // null, doesn't do anything
      comms_reportBufferState();  // null, doesn't do anything
      comms_ready();              // output the READY_200 message
    }

    vTaskDelay(20 / portTICK_PERIOD_MS);


  }
}

/*-----------------------------------------------------------------*/
// COMMS Tasks setup
/*-----------------------------------------------------------------*/
TaskHandle_t commsReadHandle = NULL;
void commsReadTaskCreate() {
  Serial.println("commsRead started...");

  // commsRead seems to need elevated priority in order to work properly
  xTaskCreate( commsRead, "COMMS Read", 5000, NULL, 1, &commsReadHandle );
}

TaskHandle_t commsCommandHandle = NULL;
void commsCommandTaskCreate() {
  Serial.println("commsCommand started...");

  xTaskCreate( commsCommand, "COMMS Command", 5000, NULL, 1, &commsCommandHandle );
}
