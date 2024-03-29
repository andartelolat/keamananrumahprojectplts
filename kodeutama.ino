//............ UPDATE 0.1
//............ UPDATE SELASA 15 NOVEMBER 2022
//............ UPDATE CODING UNTUK TELEGRAM TIAP PIN OUTPUT DIBALIK OUTPUTNYA DARI HIGH -> LOW
//............ UPDATE 0.2
//............ UPDATE SELASA 22 NOVEMBER 2022
//............ Update ganti parameter pada sensor jarak dan api 
//............ Fix pesan teks pada telegram 
//............ UPDATE 0.3
//............ UPDATE JUMAT 9 DESEMBER 2022
//............ Update pin untuk kendali servo pintu
//............ update pin telegram kontrol servo
//............ UPDATE 0.4
//............ Update MINGGU 8 Januari 2023
//............ update pembaharuan coding nonaktifkan serial print dan non aktifkan beberapa buzzer
//............ UPDATE 0.5
//............ UPDATE SELASA 10 Januari 2023
//............ update verifikasi ulang untuk mengaktifkan pompa dan kipas jika terdeteksi gas atau api.
//............ UPDATE 0.6
//............ UPDATE KAMIS 12 Januari 2023
//............ UPDATE penambahan parameter lampu thingspeak
//............ UPDATE 0.7
//............ UPDATE Rabu 25 Januari 2023
//............ UPDATE nonaktifin beberapa parameter tegangan dan arus dan revisi coding untuk sensor gas dan minimalisir penggunaan memori

/*Catatan 
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

    Web Peneliti : ( https://iot-iben.my.id ) dengan menggunakan layanan cloud dari cloudkilat.com peneliti menyewa untuk menggunakan layanan hosting
                   dan domain serta database. layanan yang digunakan Cloud VM S. 
                
    Copyright 2022, Ibnul Sahgianto, iben.
*/

// Library bungkus
  #include <WiFi.h>
  #include "ThingSpeak.h"
  #include <ThingSpeak.h> 
  #include "get1iben.h"
  #include "password.h"
  #include <WiFiClientSecure.h>
  #include <UniversalTelegramBot.h>  
  #include <ArduinoJson.h>
  #include <ESP32_Servo.h>
// tutup library

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

// Filler Akses point
  const char* ssid = ibena;   
  const char* password = pass_ibena;  
  const char * BOTtoken = token_bot;
  const char *  CHAT_ID = cet_ID;
// tutup filler

// Telegram pin
  Servo pintu;
  const int lampu = 5;
  const int pervo = 19;
  const int kipas = 18;
  const int pompa = 21;
  bool kampu = HIGH;
  bool cipas = HIGH;
  bool kompa = HIGH;
// tutup telegram pin

// membuat object dari library get1iben.h
  get1iben ciben;
  WiFiClient client;
  WiFiClientSecure klient;
  UniversalTelegramBot bot(BOTtoken, klient);
// tutup library iben

// jeda waktu bungkus telegram
  int botRequestDelay = 1000;
  unsigned long lastTimeBotRan;
// tutup jeda waktu

// setup
void setup() {
  Serial.begin(115200);
  //sertifikat web masing2 mikrokontroller bungkus
    #ifdef ESP32
      klient.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
    #endif
    #ifdef ESP8266
      configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
      klient.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
    #endif
  // sertifikat tutup
  // pin telegram bungkus
    pintu.attach(pervo);
    pinMode(lampu, OUTPUT);
    pinMode(kipas, OUTPUT);
    pinMode(pompa, OUTPUT);
  // pin telegram tutup 
  // Wifi bungkus
    WiFi.mode(WIFI_STA);
    ThingSpeak.begin(client);
  
    if(WiFi.status() != WL_CONNECTED){
        Serial.print("Menghubungkan......");
        while(WiFi.status() != WL_CONNECTED){
          // bot.sendMessage(CHAT_ID, "Terhubung!", "");
          WiFi.begin(ssid, password); 
          delay(5000);     
        } 
        Serial.println("\nTerhubung.");
      }
    Serial.println(WiFi.localIP());
  // Wifi Tutup
}

// loop
void loop() {
  // memanggil fungsi ciben
  ciben.baca_data();
    // set telegram pesan respon bungkus
      // telegram
      if (millis() > lastTimeBotRan + botRequestDelay)  {
        int nmrPesan = bot.getUpdates(bot.last_message_received + 1);

        while(nmrPesan) {
          Serial.println("Dapet respon : ");
          pesanBaru(nmrPesan);
          nmrPesan = bot.getUpdates(bot.last_message_received + 1);
        }
        lastTimeBotRan = millis();
      }
    // tutup telegram respon pesan
}

// Telegram pesan baru
void pesanBaru(int nmrPesan) {
  Serial.println("Pesan yang masuk");
  Serial.println(String(nmrPesan));
  for (int i=0; i<nmrPesan; i++) {
    // id chat request
    String chat_id = String(bot.messages[i].chat_id);
    // if (chat_id != CHAT_ID){
    //   bot.sendMessage(chat_id, "Bzzzt", "");
    //   continue;
    // }
    
    // cetak pesan yang masuk
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Selamat datang ! -=[ " + from_name + " ]=-.\n";
      welcome += "Silahkan masukkan perintah sesuai dengan perintah di bawah.\n\n";
      welcome += "/L1 untuk mematikan lampu \n";
      welcome += "/L0 untuk menghidupkan lampu \n";
      welcome += "/F1 untuk mematikan kipas \n";
      welcome += "/F0 untuk menghidupkan kipas \n";
      welcome += "/P1 untuk mematikan pompa/support \n";
      welcome += "/P0 untuk menghidupkan pompa/support \n";
      welcome += "/K0 untuk membuka kunci pintu \n";
      welcome += "/K1 untuk menutup kunci pintu \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    // LAMPU
    if (text == "/L0") {
      bot.sendMessage(chat_id, "Lampu Hidup", "");
      float temperatur = 0;
      ThingSpeak.setField(1, temperatur);
      kampu = LOW;
      digitalWrite(lampu, kampu);
      Serial.print(kampu);
    }
    if (text == "/L1") {
      bot.sendMessage(chat_id, "Lampu Mati", "");
      float temperatur = 1;
      ThingSpeak.setField(1, temperatur);
      kampu = HIGH;
      digitalWrite(lampu, kampu);
      Serial.print(kampu);
    }
    
    // FAN
    if (text == "/F0") {
      bot.sendMessage(chat_id, "Kipas Hidup", "");
      cipas = LOW;
      digitalWrite(kipas, cipas);
      Serial.print(cipas);
    }
    if (text == "/F1") {
      bot.sendMessage(chat_id, "Kipas Mati", "");
      cipas = HIGH;
      digitalWrite(kipas, cipas);
      Serial.print(cipas);
    }
    
    // Pompa
    if (text == "/P0") {
      bot.sendMessage(chat_id, "Pompa Hidup", "");
      kompa = LOW;
      digitalWrite(pompa, kompa);
      Serial.print(kompa);
    }
    if (text == "/P1") {
      bot.sendMessage(chat_id, "Pompa Mati", "");
      kompa = HIGH;
      digitalWrite(pompa, kompa);
      Serial.print(kompa);
    }

    // Pintu
    if (text == "/K0") {
      bot.sendMessage(chat_id, "Kunci Pintu terbuka", "");
      pintu.write(90);
    }
    if (text == "/K1") {
      bot.sendMessage(chat_id, "Kunci pintu tertutup", "");
      pintu.write(0);
    }
  }
}
