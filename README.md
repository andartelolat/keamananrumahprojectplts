# keamananrumahprojectplts
project yang dimana kita memanfaatkan tenaga plts untuk keamanan rumah berbasiskan Internet ofthings

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
==================================================
//............ 
UPDATE 0.1
-------------------------

//............ UPDATE SELASA 15 NOVEMBER 2022

//............ UPDATE CODING UNTUK TELEGRAM TIAP PIN OUTPUT DIBALIK OUTPUTNYA DARI HIGH -> LOW

==================================================
//............ 
UPDATE 0.2
--------------------------

//............ UPDATE SELASA 22 NOVEMBER 2022

//............ Update ganti parameter pada sensor jarak dan api 

//............ Fix pesan teks pada telegram 

==================================================
//............ 
UPDATE 0.3
-------------------------

//............ UPDATE JUMAT 9 DESEMBER 2022

//............ Update pin untuk kendali servo pintu

//............ update pin telegram kontrol servo

===================================================
