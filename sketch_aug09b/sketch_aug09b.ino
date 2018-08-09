//    Define the PN532 communication protocol as HSU (UART)
//    It will only work with an Arduino Leonardo or Mega because of the dual Hardware Sedial
#include <PN532_HSU.h>
#include <PN532.h>
PN532_HSU pn532hsu(Serial);
PN532 nfc(pn532hsu);

void setup() { 
  Serial.begin(115200);
  while (!Serial)
  Serial.println("System init");
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();
  Serial.println("System operational");
}

void loop() {
//  init the nfc tag variables
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
//    If an nfc tag is found , get it's uid and store it in "myuid"
    String myuid = "";
    for (uint8_t i=0; i < uidLength; i++) 
    {
      int Iuid = uid[i];
      String Huid =  String(Iuid, HEX);
      Huid.toUpperCase();
      myuid = myuid + " 0x" + Huid;
    }
    Serial.println("");
    Serial.println(myuid); //print the nfc tag's uid
//    Test if the nfc tag is registered in the local storage
    String users[] = 
    {
      " 0x4 0xE5 0x80 0x42 0x13 0x43 0x80", //user1
      " 0x19 0x22 0x6A 0xE5"//user2
    };
    for(int i = 0; i<2 ; i++)
    {
      if(myuid == users[i])
      {
//    Print "Tag ok" if the nfc tag is known
      Serial.println("Tag ok");
//    You can replace this command by anything
      }
    }
  }
  
  delay(1000);
}

