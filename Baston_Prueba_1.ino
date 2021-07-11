#include <SPI.h>
#include <MFRC522.h>
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <string.h>

#define RST_PIN 9 //vea la disposición típica del perno arriba
#define SS_PIN 10 //Configurable, vea la disposición típica del perno arriba
 
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance. Crear instancia de MFRC522. 
MFRC522::MIFARE_Key key;

/**
* Initialize.
*/
SoftwareSerial mySoftwareSerial(3, 2); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
void setupMP3()
{
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
}
/**
* Ayudante para volcar una matriz de bytes como valores hexadecimales a Serial.
*/
void dump_byte_array(byte *buffer, byte bufferSize)
{
  for (byte i = 0; i < bufferSize; i++) {
    // Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    // Serial.print(buffer[i], HEX);
    Serial.write(buffer[i]);
  }
  //Serial.println();
  //Serial.write("hola");
  //Serial.println();
  Serial.write((char*)&buffer[16]);
}
void setup()
{
//  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  //Serial.begin(9600); // Inicializar comunicaciones serie con el PC
  while (!Serial)
    ; // No hacer nada si no se abre ningún puerto serie (añadido para Arduinos basado en ATMEGA32U4)
  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card

/*
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(10);  //Set volume value. From 0 to 30
  myDFPlayer.play(2);  */
}

 
  #if 0
  // utilizando FFFFFFFFFFFFh que es el valor predeterminado en la entrega de chips desde la fábrica
  for (byte i = 0; i < 6; i++)
    key.keyByte[i] = 0xFF;
  Serial.println(F("Ingrese Tarjeta"));
  #endif
//}
#if 0
void beep(unsigned char pausa)
        {
            analogWrite(3, 20);
            delay(pausa);                 // Espera
            analogWrite(3, 0);            // Apaga
            delay(pausa);                 // Espera  
        }
#endif
void (*myReset) (void)=NULL;  
void loop()
{
  // Busque nuevas tarjetas
  if (! mfrc522.PICC_IsNewCardPresent())
   return;
  // Seleccione una de las tarjetas
  if(! mfrc522.PICC_ReadCardSerial())
    return;
      // utilizando FFFFFFFFFFFFh que es el valor predeterminado en la entrega de chips desde la fábrica
  for (byte i = 0; i < 6; i++)
    key.keyByte[i] = 0xFF;
  Serial.println(F("Ingrese Tarjeta"));
  byte sector = 1;
  byte blockAddr = 5;//Se especifica que bloque se leera _____------______-----______

  //byte blockAddr:
  for (blockAddr = 5; blockAddr < 6; blockAddr++){//Bucle nuevo, experimental
  }
  //byte blockAddr1 = 5;//B5

  byte dataBlock[] = {
    0x01, 0x02, 0x03, 0x04, // 1, 2, 3, 4,
    0x05, 0x06, 0x07, 0x08, // 5, 6, 7, 8,
    0x08, 0x09, 0xff, 0x0b, // 9, 10, 255, 12,
    0x0c, 0x0d, 0x0e, 0x0f // 13, 14, 15, 16
  };
  byte trailerBlock = 7;
  MFRC522::StatusCode status;
  byte buffer[18];
  byte size = sizeof(buffer);
  //Serial.println(F("Datos actuales en el sector:"));
  mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
  //Serial.println();
  // Leer los datos del bloque 6
  for (blockAddr = 5; blockAddr < 6; blockAddr++){// lectura experimental
    //Serial.print(F("Cargando dato del bloque ")); Serial.print(blockAddr );
    //Serial.println(F(" ..."));
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer , &size);
  }
  if(status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  }

  for (blockAddr = 5; blockAddr < 6; blockAddr++){
    //Serial.print(F("Dato en el bloque ")); Serial.print(blockAddr); Serial.println(F(":"));
    dump_byte_array(buffer, 16); Serial.println();
    Serial.println();
  }
/*  if(strncmp((const char*)(buffer+16), "mm", 2)==0) {
    myDFPlayer.volume(20);  //Set volume value. From 0 to 30
    myDFPlayer.play(1);  //Play the first mp3
    //myDFPlayer.playFolder(10, 001);  
  }
  else if(strncmp((const char*)(buffer+16), "guido", 5)==0) {
    myDFPlayer.volume(20);  //Set volume value. From 0 to 30
    myDFPlayer.play(3);  //Play the first mp3
   // myDFPlayer.playFolder(10, 002); 
*/

Serial.println("pasando por aca");
    if(buffer[0]=='m' && buffer[1]=='m') {
    Serial.println("tocando mm");
    myDFPlayer.volume(20);
   myDFPlayer.play(0001);  //Play the first mp3
    delay(5000);
    }


    if(buffer[0]=='m' && buffer[1]=='a') {
    Serial.println("suena ma");
    myDFPlayer.volume(20);
    myDFPlayer.play(0002);  //Play the first mp3
    delay(5000);
    }

    if(buffer[0]=='m' && buffer[1]=='b' && buffer[2]=='1' ) {
    Serial.println("en vivo mb1");
  myDFPlayer.volume(20);
   myDFPlayer.play(0003);  //Play the first mp3
    delay(5000);
    
  }
 
  delay(3000);
  myReset();
  
}
