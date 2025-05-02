#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

MFRC522 rfid(SS_PIN, RST_PIN);

// Kayıtlı UID'ler ve isimler
const byte adminUIDs[][4] = {
  {},    // Admin
  {},    // Kullanici_1
  {}      // Kullanici_2
};

const char* adminNames[][2] = {
  {"Admin", "Admin"},
  {"Kullanici_1", "1"},
  {"Kullanici_2", "2"}
};

const int adminCount = sizeof(adminUIDs) / sizeof(adminUIDs[0]);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("Hazir, kart okutun.");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  bool yetkili = false;
  const char* ad = "Tanimsiz";
  const char* soyad = "Tanimsiz";

  for (int i = 0; i < adminCount; i++) {
    bool eslesme = true;
    for (byte j = 0; j < 4; j++) {
      if (rfid.uid.uidByte[j] != adminUIDs[i][j]) {
        eslesme = false;
        break;
      }
    }
    if (eslesme) {
      yetkili = true;
      ad = adminNames[i][0];
      soyad = adminNames[i][1];
      break;
    }
  }

  if (yetkili) {
    Serial.print("GIRIS,");
  } else {
    Serial.print("RED,");
  }
  Serial.print(ad);
  Serial.print(",");
  Serial.println(soyad);

  rfid.PICC_HaltA();
  delay(2000);
}
