
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

// Deklarasi PIN bungkus
  #define PIN_V_ARUS 34             
  #define PIN_V_TEG 35  
  #define PIN_GAS 32
  #define PIN_FLAME 33
  #define PIN_JARAK_TRIG 25
  #define PIN_JARAK_ECHO 26
  #define PIN_PINTU 19
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
  }

  //destructor object
  get1iben::~get1iben(){}
// cd tutup

void get1iben::baca_data(void){
  // Channel dan API key
  unsigned long myChannelNumber = ch_id_1;
  const char * myWriteAPIKey = api_ch_1;

  // jeda waktu
    unsigned long lastTime = 0;
    unsigned long timerDelay = 30000;
  // tutup jeda waktu

  // bungkus pengukuran tegangan dan arus
    // Floats for ADC voltage & Input voltage
    float adc_voltage = 0.0;
    float pv_tegangan = 0.0;
    float teg_pv = 0.0;
    float pv_arus = 0.0;
    float pv_daya = 0.0;

    // resep pembanding
    float R1 = 30000.0;
    float R2 = 7500.0;
    const double faktor_pembanding = 0.006; // range sensor 30A 

    // variable pengaturan pengukuran teg dan arus
    float ref_voltage = 3.3;
    float vref_arus = 3.3;
    float resESP = 4095.0;
    float res_adc = vref_arus/resESP;
    float zeroPoint = ref_voltage/2;

    teg_pv = (teg_pv + (res_adc*analogRead(PIN_V_ARUS)));
    teg_pv = teg_pv / 1000;

    // nilai adc untuk sensor teg
    int adc_value = 0;
  
    // baca adc tegangan
    adc_value = analogRead(PIN_V_TEG);
    
    // adc tegangan
    adc_voltage  = (adc_value * ref_voltage) / 4095.0; 
    
    // hitung variable final teg, arus dan daya
    pv_tegangan = adc_voltage / (R2/(R1+R2)) + 0.59 ;
    pv_arus = (teg_pv - zeroPoint) / faktor_pembanding;
    pv_daya = pv_arus*pv_tegangan; 
  // tutup bungkus tegangan

  // bungkus pengukuran HS (gas, flame, jarak, pintu)
    //gas
    float gas =0.0;
    float batas_gas = 400.0;
    gas = analogRead(PIN_GAS);
    //api
    float flame = 0.0;
    flame = digitalRead(PIN_GAS);
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
    float temperatur = random(1, 20);
  // tutup variable sementara
  
  // cetak nilai sensor ke thingspeak
  ThingSpeak.setField(1, temperatur);
  ThingSpeak.setField(2, pv_tegangan);
  ThingSpeak.setField(3, pv_arus);
  ThingSpeak.setField(4, pv_daya);
  ThingSpeak.setField(5, gas);
  ThingSpeak.setField(6, flame);
  ThingSpeak.setField(7, jarak);

  // bungkus status
      // pesan status PV
      // STATUS
      String myStatus = "";
      String my2Status= "";

      if(pv_tegangan > 9.0)
      {
        myStatus = String("[ Tegangan overshoot ]"); 
      }
      else if(pv_tegangan < 5.0)
      {
        myStatus = String("[ Tegangan masuk rendah ]");
      }
      else
      {
        myStatus = String("[ Tegangan stabil ]");
      }

      // pesan status HS 
      // GAS
      if(gas > 900)
      {
        myStatus = String("[ Gas terdeteksi HIGH 2 ]");
      }
      else if(gas > 500)
      {
        myStatus = String("[ Gas terdeteksi HIGH 1 ]");
      }

      // FLAME
      if(flame > 900)
      {
        myStatus = String("[ api terdeteksi HIGH 2 ]");
      }
      else if(flame > 500)
      {
        myStatus = String("[ api terdeteksi HIGH 1 ]");
      }
      
      // JARAK
      if(jarak < 300 && jarak > 200)
      {
        myStatus = String("[ objek terdeteksi mendekat 1 ]");
      }
      else if(jarak < 100)
      {
        myStatus = String("[ Gas terdeteksi mendekat 2 ]");
      }
      // Pintu
      if (kond_pintu == HIGH){
        myStatus = String("Pintu terbuka");
      }
      else {
        myStatus = String("Pintu tertutup");
      }
    
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
      delay(20000);
  // tutup bungkus status
  // bungkus print data
    Serial.println("=========================================================================================");

    Serial.print("tegangan terbaca : ");
    Serial.print(pv_tegangan, 2);
    Serial.print(" Volt");
    Serial.print("  |  ");

    Serial.print("arus terbaca : ");
    Serial.print(pv_arus, 2);
    Serial.print(" Ampere");
    Serial.print("  |  ");

    Serial.print("Daya Terbawa : ");
    Serial.print(pv_daya, 2);
    Serial.println(" Watt");

    Serial.println("=========================================================================================");
    
    Serial.print("Nilai gas : ");
    Serial.print(gas);
    Serial.print(" Analog");
    Serial.print("  |  ");

    Serial.print("Nilai flame : ");
    Serial.print(flame);
    Serial.print(" Analog");
    Serial.print("  |  ");

    Serial.print("Nilai jarak : ");
    Serial.print(jarak);
    Serial.println(" cm");

    Serial.print("Nilai jarak : ");
    Serial.print(jar_inch);
    Serial.println(" inch");

    Serial.println("=========================================================================================");
  // tutup bungkus print data
  // ganti nilai perhitungan sementara
      // temperatur++;
      // if(temperatur > 70){
      //   temperatur = 0;
      // }
      // gas = random(0,1024);
      // flame = random(0,1024);
      // jarak = random(0,1024);
      delay(20000);
  // tutup ganti nilai
}
