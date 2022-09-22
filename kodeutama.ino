/*Catatan Peneliti
  Program utama Project keamanan rumah dengan PLTS
  IBNUL SAHGIANTO 1901021076 UNIVERSITAS BUMIGORA MATARAM
  1901021076@universitasbumigora.ac.id
  
  Deskripsi:  mendemonstrasikan bagaimana perangkat iot dapat terhubung dan saling bertukar
              data terhadap layanan cloud yang disediakan oleh Thingspeak API
              Mikrokontroller yang digunakan dalam project ini adalah ESP32 WROOM 32
              nantinya data akan ditampilkan di dashboard thingspeak kemudian akan disisipkan 
              dalam website peneliti yaitu iot-iben.my.id dengan memanfaatkan embedded link dari thingspeak.

    Hardware : ESP32 WROOM 32 Development Board
               Sensor pembagi tegangan
               Sensor Gas
               Sensor Flame / api
               Sensor jarak
               Motor Servo sebagai doorlock
               Pompa DC Mini

    !!! Note : beberapa kode dipisah menggunakan library guna untuk menghemat memori
  
    Thingspeak : ( https://www.thingspeak.com ) adalah layanan platform integrasi untuk internet of things 
                 yang dimana dapat kita manfaatkan untuk mengaggregat, visualisasikan dan menaganalisa data secara langsung melalui cloud
                 kunjungi https://www.thingspeak.com untuk daftar dan membuat channel.
                
    Copyright 2022, Ibnul Sahgianto, iben.
*/

// Library
#include <WiFi.h>
#include "ThingSpeak.h"
#include <ThingSpeak.h> 
#include "get1iben.h"
#include "password.h"

// Deklarasi PIN
#define PIN_V_ARUS 34             
#define PIN_V_TEG 35                 
#define BANYAK_SAMPLING 20
#define PIN_GAS 32
#define PIN_FLAME 33
#define PIN_JARAK 25  

// Filler Akses point
const char* ssid = ibena;   
const char* password = pass_ibena;  
unsigned long myChannelNumber = ch_id_1;
const char * myWriteAPIKey = api_ch_1;

// Variable sensor mencoba dengan nilai random sebelum implementasi ke sensor
// PV
float kondisi = random(0.0, 5.0);
float pv_tegangan = random(5.0, 9.0);
float pv_arus = random(1.4, 4.0);
float pv_daya = random(1.4, 4.0);

// HS
float gas = random(0.0, 1024.0);
float flame = random(0.0, 1024.0);
float jarak = random(0.0, 1024.0);

// membuat object
get1iben ciben;
WiFiClient  client;

// jeda waktu
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

// STATUS
String myStatus = "";
String my2Status= "";

// setup
void setup() {
  Serial.begin(115200);  
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  
}

// loop
void loop() {
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Menghubungkan......");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(5000);     
      } 
      Serial.println("\nTerhubung.");
    }
  // memanggil fungsi ciben
  ciben.baca_data();
  ciben.print_data();

  // set nilai ke setiap field
  // solar panel
  ThingSpeak.setField(1, kondisi);
  ThingSpeak.setField(2, pv_tegangan);
  ThingSpeak.setField(3, pv_arus);
  ThingSpeak.setField(4, pv_daya);

  // sensor rumah
  ThingSpeak.setField(5, gas);
  ThingSpeak.setField(6, flame);
  ThingSpeak.setField(7, jarak);

  // pesan status PV
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
}
