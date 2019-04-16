#include <SPI.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "RTClib.h"
//SoftwareSerial mySerial(8,9);
RTC_DS3231 rtc;

// Write, poll, read, reset
#define   CMD             0x00
#define   POLL            0x03
#define   READ            0x02
#define   RESET           0x01

// Commands
#define   IDN             0x01  // gives brief information about CR95HF and its revision
#define   PROTOCOLSELECT  0x02  // Selects RF communication protocol and prepares CR95HF for communication
#define   SENDRECEIVE     0x04  // Sends data to tag and receives its reply
#define   Idle            0x07  // Switches the CR95HF into low consumption mode
#define   RDREG           0x08  // Read the wakeup register
#define   WRREG           0x09  // Set analog register config address, set timer window, set autodetect filter, configure HF2RF bit 
#define   BAUDRATE        0x0A  // changes the UART baud rate
#define   ECHO            0x55  // Verifies the possibility of communication between a Host and the CR95HF

String hhh = "";
byte NfcReady = 0;
byte readError = 0;
const int _SSpin = 10;
const int _IRQpin = 3;
const int SSI_0 = 6;
const int SSI_1 = 5;
const int MOSIPin = 11;
int temp;
//float elapsedMinVal_previous;
const int MISOPin = 12;
const int SCKPin = 13;
byte _rxBuffer[16];
byte _txBuffer[16];
byte datasave[64];

String trendValue = "";
String pointer = "";

String dataTrend = "";
String Pointer = "";
String Pointer10 = "";
String Pointer11 = "";
String Pointer12 = "";
String Pointer13 = "";
String Pointer14 = "";
String Pointer15 = "";
String Pointer16 = "";
String Pointer17 = "";
String Pointer18 = "";
String Pointer19 = "";
String Pointer20 = "";
String Pointer21 = "";
String HistValue = "";
String MinTrend = "";

String tr[12];
//Sending Command
void SendCmd(byte command, byte dataLen)
{
  digitalWrite(_SSpin, LOW);
  SPI.transfer(CMD);
  SPI.transfer(command);
  if (command != ECHO)
  {
    SPI.transfer(dataLen);


    for (byte i = 0; i < dataLen; i++)
    {
      SPI.transfer(_txBuffer[i]);

    }
  }
  digitalWrite(_SSpin, HIGH);
  delay(1);
}


//-------------------------------------------------------------------Polling the IC
void PollSlave() {
  byte resp = 0;
  digitalWrite(_SSpin, LOW);
  while (resp != 8) {
    resp = SPI.transfer(POLL);
    resp = resp & 0x08;
  }
  digitalWrite(_SSpin, HIGH);
  delay(1);
}


//----------------------------------------------------------------------Read Data
String readMsgString() //done
{
  String readOut;
  byte res = 0;
  byte len = 0;

  digitalWrite(_SSpin, LOW);
  SPI.transfer(READ);
  res = SPI.transfer(0);
  len = SPI.transfer(0);
  for (byte i = 0; i < len; i++)
  {
    _rxBuffer[i] = SPI.transfer(0);
  }

  if (res != 128) {
    readError = 1;
  }
  digitalWrite(_SSpin, HIGH);
  for (byte i = 0; i < 8; i++)
  {
    datasave[i] = _rxBuffer[i + 1];

  }
  readOut = byteArraytoString(datasave);


  return readOut;
  delay(1);

}

//------------------------------------------------------------ Read Message
void ReadMsg()
{
  String readOut;
  byte res = 0;
  byte len = 0;

  digitalWrite(_SSpin, LOW);
  SPI.transfer(READ);
  delay(1000);
  res = SPI.transfer(0);


  len = SPI.transfer(0);


  for (byte i = 0; i < len; i++)
    _rxBuffer[i] = SPI.transfer(0);
  digitalWrite(_SSpin, HIGH);
  for (byte i = 0; i < 8; i++)
  {

    _rxBuffer[i] = 0;

  }
  delay(10);

}


void SerialPrint(String nm, byte item)
{
  Serial.println(nm + ": " + item);
  // Serial.println();
}


String byteArraytoString(byte inArray[]) {
  int p, q, r;
  //  char  hex[] = {"0123456789ABCDEF"};
  char  hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  String out = "";
  for (q = 0; q < 8; q++) {
    r = inArray[q] & 0xFF ;
    p = (r >> 4) & 0x0F;
    out = out + hex[p];
    p = r & 0x0F;
    out = out + hex[p];

  }
  trendValue = trendValue + out;
  hhh = out;
  return out;
}



void readMultiple() {
  _txBuffer[0] = 0x02;
  _txBuffer[1] = 0x23;
  _txBuffer[2] = 0x03;
  _txBuffer[2] = 0x05;
  SendCmd(SENDRECEIVE, 0x04);
  PollSlave();
  // ReadMsg();
  //delay(1000);
  byte nr = 0;
  byte res = 0;
  byte len = 0;

  digitalWrite(_SSpin, LOW);
  SPI.transfer(READ);
  delay(1000);
  nr = SPI.transfer(0);
  //SerialPrint("nr", nr);

  res = SPI.transfer(0);
  //SerialPrint("res", res);

  len = SPI.transfer(0);
  //SerialPrint("len", len);


  for (byte i = 0; i < len; i++)
  {
    // Serial.println(SPI.transfer(0));
    _rxBuffer[i] = SPI.transfer(0);
    // SerialPrint("_rxBuffer", _rxBuffer[i]);
    Serial.println(_rxBuffer[i], HEX); // <---------serial
  }
  digitalWrite(_SSpin, HIGH);
  for (byte i = 0; i < len; i++)
  {
    _rxBuffer[i] = 0;
  }

  //Serial.println("Read Done ");
  delay(10);
}
boolean NfcState (byte a, byte b) {
  return ((a == 0) & (b == 0));
}
void WakeupCR95HF()
{
  digitalWrite(_IRQpin, LOW);
  delayMicroseconds(100);
  digitalWrite(_IRQpin, HIGH);
  delay(10);
  digitalWrite(_IRQpin, LOW);
}

bool EchoResponse()
{
  SendCmd(ECHO, 0);
  //PollSlave();
  //ReadMsg();

  byte res = 0;
  byte len = 0;

  digitalWrite(_SSpin, LOW);
  SPI.transfer(0x02);
  res = SPI.transfer(8);
  delay(100);
  //SerialPrint("res", res);
  digitalWrite(_SSpin, HIGH);
  delay(100);
  if (res == ECHO)
  {
    return true;
  }
  return false;
}

float glucoseRead(unsigned int val) {
  int bitmask = 0x0FFF;
  return (((val & bitmask) / 9.5));
}

void SetProtocol()
{
  _txBuffer[0] = 0x01;
  _txBuffer[1] = 0x0D;
  SendCmd(PROTOCOLSELECT, 0x02);
  PollSlave();
  digitalWrite(_SSpin, LOW);
  SPI.transfer(0x02);
  _rxBuffer[0] = SPI.transfer(0);
  _rxBuffer[1] = SPI.transfer(0);
  digitalWrite(_SSpin, HIGH);

  if (NfcState(_rxBuffer[0], _rxBuffer[1])) {
    NfcReady = 1;
    Serial.println("NFC Protocol Successful");
  }
  else {
    Serial.println("NFC Not Set");
    NfcReady = 0;
  }
}

void indexMod() {
  _txBuffer[0] = 0x68;
  _txBuffer[1] = 0x01;
  _txBuffer[2] = 0x01;
  _txBuffer[3] = 0x50;
  SendCmd(WRREG, 0x04);
  PollSlave();
  ReadMsg();
}

void read_per(int x) {
  _txBuffer[0] = 0x02;
  _txBuffer[1] = 0x20;
  _txBuffer[2] = x;
  SendCmd(SENDRECEIVE, 0x03);
  PollSlave();//
  pointer = readMsgString();
}


void readBlock9() {
  _txBuffer[0] = 0x02;
  _txBuffer[1] = 0x20;
  _txBuffer[2] = 0x09;
  SendCmd(SENDRECEIVE, 0x03); //10
  PollSlave();//
  Pointer = readMsgString();//
  delay(10);
}


void setup()
{
  digitalWrite(7, HIGH);

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif
  Serial.begin(9600);
  // SSI_0 and 1 needed?
  //mySerial.begin(9600);

  if (! rtc.begin()) {
    //  Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    //Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  delay(100);
  pinMode(7, OUTPUT);

  pinMode(_SSpin, OUTPUT);
  pinMode(_IRQpin, OUTPUT);
  pinMode(MOSIPin, OUTPUT);
  pinMode(SCKPin, OUTPUT);
  pinMode(MISOPin, INPUT);

  pinMode(SSI_0, OUTPUT);
  digitalWrite(SSI_0, HIGH);

  pinMode(SSI_1, OUTPUT);
  digitalWrite(SSI_1, LOW);

  digitalWrite(_IRQpin, HIGH); // wakeup
  delay(10);
  digitalWrite(_SSpin, HIGH);  // slave select, high for not listening
  delay(10);

  WakeupCR95HF();
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV32);
  delay(10);

  EchoResponse();
  while (!EchoResponse())
  {
    WakeupCR95HF();
  }
  Serial.println("Echo Received");

}
void loop() {
  digitalWrite(7, HIGH);
  Serial.println("Entering loop and reading block9");
  if (NfcReady == 0)
  {
    SetProtocol(); // ISO 15693 settings
    delay(100);
  }
  else {
    readBlock9();
    int elapsedMinVal = strtoul((Pointer.substring(6, 8) + Pointer.substring(4, 6)).c_str(), NULL, 16);
    int GlucTrendVal = strtoul((Pointer.substring(10, 12) + Pointer.substring(8, 10)).c_str(), NULL, 16);
    SerialPrint("temp", temp);
    String elapsedMin = "Time Elapsed before switch case:" + String(elapsedMinVal) ;
    Serial.println(elapsedMin);






    while ((GlucTrendVal == temp) || (elapsedMinVal == 0)) {
      //wait and read the block 9()

      delay(1000);
      digitalWrite(7, LOW);

    }
    String GlucTrend = "Buffer Position:" + String(GlucTrendVal);
    Serial.println(GlucTrend);
    //mySerial.println(GlucTrend);

    //DateTime now = rtc.now();

    //-------------------------------------------------------------------------------------------------------------------------------------------------------------

    for (int i = 22; i < 245; i++) {
      read_per(i);
   
      Serial.print(i);
      Serial.print("-memory block-");
      Serial.println(pointer);
      dataTrend = dataTrend + "-" + pointer;


    }
    Serial.println(dataTrend);

    //-----*********************************************************************************----------------------------****************----------------------
    // End of switch case

    float CurrentGlucVal = glucoseRead( strtoul(MinTrend.c_str(), NULL, 16));
    if (CurrentGlucVal == 0)
      digitalWrite(7, LOW);




    delay(5000);
  }
}
