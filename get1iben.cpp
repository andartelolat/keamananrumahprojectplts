// File CPP library bungkus
  // Library
  #include "Arduino.h"
  #include "esp32-hal-gpio.h"
  #include <WiFi.h>
  #include "ThingSpeak.h"
  #include <ThingSpeak.h> 
  #include "get1iben.h"
  #include "password.h"
  #include <WiFiClientSecure.h>
  #include <UniversalTelegramBot.h>  
  #include <ArduinoJson.h>
// CPP tutup

// Library Universal Telgram bot bungkus
  #ifdef ESP8266
    X509List cert(TELEGRAM_CERTIFICATE_ROOT);
  #endif
  #ifdef ESP32
    #include <WiFi.h>
  #else
    #include <ESP8266WiFi.h>
  #endif
  #ifdef ESP8266
    X509List cert(TELEGRAM_CERTIFICATE_ROOT);
  #endif
// tutup telegram library univ

// Deklarasi PIN bungkus
  // #define PIN_V_ARUS 34             
  #define PIN_V_TEG 35  
  #define PIN_GAS 32
  #define PIN_FLAME 33
  #define PIN_JARAK_TRIG 25
  #define PIN_JARAK_ECHO 26
  #define PIN_PINTU 27
  #define BUZZER 23
  // #define suhu 
  #define kec_suara 0.034
  #define cm_ke_inch 0.393701
  #define offset 20
// Deklarasi pin tutup

// contruct dan destructor bungkus
  //constructor object
  get1iben::get1iben(){
    Serial.begin(115200); 
    pinMode(PIN_JARAK_TRIG, OUTPUT);
    pinMode(PIN_JARAK_ECHO, INPUT);
    pinMode(PIN_FLAME, INPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(PIN_PINTU, INPUT_PULLUP);
  }

  //destructor object
    get1iben::~get1iben(){}
  // cd tutup
// Construct tutup

void get1iben::baca_data(void) {
  // Filler Akses point
      const char* ssid = ibena;   
      const char* password = pass_ibena;  
      const char * BOTtoken = token_bot;
      const char *  CHAT_ID = cet_ID;
  // tutup filler
  // membuat object dari library get1iben.h
    get1iben ciben;
    WiFiClient client;
    WiFiClientSecure klient;
    UniversalTelegramBot bot(BOTtoken, klient);
  // tutup library iben
  
  // sertifikat
    #ifdef ESP32
      klient.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
    #endif
    #ifdef ESP8266
      configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
      klient.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
    #endif
  // sertifikat tutup
  // Channel dan API key
  unsigned long myChannelNumber = ch_id_1;
  const char * myWriteAPIKey = api_ch_1;

  // bungkus pengukuran tegangan dan arus
    // Floats for ADC voltage & Input voltage
    float adc_voltage = 0.0;
    float pv_tegangan = 0.0;
    float teg_pv = 0.0;
    // float pv_arus = 0.0;
    // float pv_daya = 0.0;

    // resep pembanding
    float R1 = 30000.0;
    float R2 = 7500.0;
    int sensitivitas = 185; // range sensor 30A 

    // variable pengaturan pengukuran teg dan arus
    int nilaiadc= 00;
    int teganganoffset = 10;
    double ceg = 00;
    double nilaiarus = 00;
    float ref_voltage = 3.3;

    // hitung adc arus
    // nilaiadc = analogRead(PIN_V_ARUS);
    // ceg = (nilaiadc / 2047.5) * 3.3;
    

    // nilai adc untuk sensor teg
    int adc_value = 0;
  
    // baca adc tegangan
    adc_value = analogRead(PIN_V_TEG);
    
    // adc tegangan
    adc_voltage  = (adc_value * ref_voltage) / 4095.0; 
    
    // hitung variable final teg, arus dan daya
    pv_tegangan = adc_voltage / (R2/(R1+R2)) + 0.59 ;
    // pv_arus = (teg_pv - ofcet) / faktor_pembanding;
    // nilaiarus = ((ceg - teganganoffset) / sensitivitas);
    // pv_daya = (nilaiarus+1.52)*pv_tegangan; 
  // tutup bungkus tegangan

  // bungkus pengukuran HS (gas, flame, jarak, pintu)
    //gas
    float gas;
    // float batas_gas;
    gas = analogRead(PIN_GAS);
    float batas_gas=gas;
    // batas_gas = gas*(4095/3.3);
    //api
    bool flame;
    flame = digitalRead(PIN_FLAME);
    //jarak
    long durasi;
    float jarak;
    float jar_inch;
    digitalWrite(PIN_JARAK_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_JARAK_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_JARAK_TRIG, LOW);
    durasi = pulseIn(PIN_JARAK_ECHO, HIGH);
    jarak = durasi*kec_suara/2;
    jar_inch = jarak* cm_ke_inch;
    // pintu
    bool kond_pintu;
    kond_pintu = digitalRead(PIN_PINTU);
  // tutup pengukuran hs

  // isian variable sementara bungkus
    // float temperatur = random(1, 20);
  // tutup variable sementara
  
  // cetak nilai sensor ke thingspeak
    // ThingSpeak.setField(1, temperatur);
    ThingSpeak.setField(2, pv_tegangan);
    // ThingSpeak.setField(3, pv_arus);
    // ThingSpeak.setField(4, pv_daya);
    ThingSpeak.setField(5, batas_gas);
    // ThingSpeak.setField(6, flame);
    ThingSpeak.setField(7, jarak);
  // tutup cetak

  // bungkus status
      // pesan status PV
      // STATUS
      String myStatus = "";
      String my2Status= "";

      // if(pv_tegangan > 9.0)
      // {
      //   myStatus = String("[ Tegangan overshoot ]"); 

      // }
      // else if(pv_tegangan < 5.0)
      // {
      //   myStatus = String("[ Tegangan masuk rendah ]");
      // }
      // else
      // {
      //   myStatus = String("[ Tegangan stabil ]");
      // }

      // pesan status HS 
      // GAS
      if(batas_gas > 200)
      {
        myStatus = String("[ Gas terdeteksi  ]");
        bot.sendMessage(CHAT_ID, "Ada gas! apakah mau mengaktifkan kipas? ketuk [ /F0 ] jika mau mengaktifkan kipas", "");
        digitalWrite(BUZZER, HIGH);
        delay(500);
        digitalWrite(BUZZER, LOW);
      }
      // else if(batas_gas > 1.4 && batas_gas < 1.5)
      // {
      //   myStatus = String("[ Gas terdeteksi HIGH 1 ]");
      // }

      // FLAME
      if(flame == 0)
      {
        myStatus = String("[ api terdeteksi ]");
        bot.sendMessage(CHAT_ID, "ada api! apakah mau mengaktifkan pompa? ketuk [ /P0 ] jika mau mengaktifkan pompa", "");
        digitalWrite(BUZZER, HIGH);
        delay(300);
        digitalWrite(BUZZER, LOW);
      }
      // else if(flame == 1)
      // {
      //   myStatus = String("[ tidak ada api ] ]");
      // }
      
      // JARAK
      if(jarak > 7 && jarak < 10)
      {
        myStatus = String("[ objek terdeteksi mendekat 1 ]");
        bot.sendMessage(CHAT_ID, "ada objek mendekat ruangan simpan", "");
        digitalWrite(BUZZER, HIGH);
        delay(500);
        digitalWrite(BUZZER, LOW);
      }
      else if(jarak < 6 && jarak > 2)
      {
        myStatus = String("[ objek terdeteksi lebih mendekat 2 ]");
        bot.sendMessage(CHAT_ID, "ada objek semakin mendekati ruangan simpan!" , "");
        Serial.println("[ OBJEK TERDETEKSI ]");
        digitalWrite(BUZZER, HIGH);
        delay(500);
        digitalWrite(BUZZER, LOW);
        // delay(500);
      }

      // Pintu
      if (kond_pintu == HIGH){
        myStatus = String("Pintu terbuka");
        bot.sendMessage(CHAT_ID, "pintu terbuka", "");
        digitalWrite(BUZZER, HIGH);
        delay(500);
        digitalWrite(BUZZER, LOW);
        // delay(2000);
        // digitalWrite(BUZZER, HIGH);
        // delay(1000);
        // digitalWrite(BUZZER, LOW); 
      }
      // else {
      //   myStatus = String("Pintu tertutup");
      // }
    
      // set status pesan
      // pesan status PV
      ThingSpeak.setStatus(myStatus);
        int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
        if(x == 200){
          Serial.println("Channel telah diperbaharui.");
        }
        else{
          Serial.println("Gagal memperbaharui . HTTP error code " + String(x));
        }
      delay(1000);
  // tutup bungkus status
  // bungkus print data
    // Serial.println("=========================================================================================");

    // Serial.print("tegangan terbaca : ");
    // Serial.print(pv_tegangan, 2);
    // Serial.print(" Volt");
    // Serial.print("  |  ");

    // Serial.print("arus terbaca : ");
    // Serial.print(nilaiarus, 3);
    // Serial.print(" Ampere");
    // Serial.print("  |  ");

    // Serial.print("Daya Terbaca : ");
    // Serial.print(pv_daya, 3);
    // Serial.println(" Watt");

    // Serial.println("=========================================================================================");
    
    // Serial.print("Nilai gas : ");
    // Serial.print(batas_gas);
    // Serial.print(" Analog");
    // Serial.print("  |  ");

    // Serial.print("Detektor api : ");
    // if (flame==0) {
    //   Serial.print("Ada api");
    // }
    // else if (flame ==1) {
    //   Serial.print("Api tidak terdeteksi");
    // }
    // Serial.print("  |  ");

    // Serial.print("Nilai jarak : ");
    // Serial.print(jarak);
    // Serial.print(" cm ");
    // Serial.print("  |  ");

    // Serial.print("kondisi pintu : ");
    // if (kond_pintu==1) {
    //   Serial.print("pintu terbuka");
    // }
    // else if (kond_pintu ==0) {
    //   Serial.print("pintu tertutup");
    // }
    // Serial.println(" ");

    // Serial.print("Nilai jarak : ");
    // Serial.print(jar_inch);
    // Serial.println(" inch");

    // Serial.println("=========================================================================================");
  // tutup bungkus print data
  // ganti nilai perhitungan sementara
      // temperatur++;
      // if(temperatur > 70){
      //   temperatur = 0;
      // }
      // gas = random(0,1024);
      // flame = random(0,1024);
      // jarak = random(0,1024);
      delay(1000);
  // tutup ganti nilai
}
