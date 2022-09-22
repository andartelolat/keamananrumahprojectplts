// File CPP library
// Library
#include "ThingSpeak.h"
#include <ThingSpeak.h>
#include "get1iben.h"

// Deklarasi PIN
#define PIN_V_ARUS 34             
#define PIN_V_TEG 35  
#define BANYAK_SAMPLING 20 
#define PIN_GAS 32
#define PIN_FLAME 33
#define PIN_JARAK 25

//constructor
get1iben::get1iben(){
  Serial.begin(115200);     
}

//destructor
get1iben::~get1iben(){}

int get1iben::baca_adc(int channel)           // perintah membaca ADC pada channel tertentu
{
  int sum = 0;                      // inisiasi total dimulai dari 0
  int temp;                         // inisiasi temp sebagai integer
  int i;                            // inisiasi i sebagai banyaknya perulangan
  for (i=0; i<BANYAK_SAMPLING; i++)
  {
    temp = analogRead(channel);     // perintah temp sebagai pembacaan input analog pada channel
    sum += temp;                    // perintah sum sebagai penambahan temp atau total seluruhnya
    delayMicroseconds(50);          // perintah delay
  }
  return (sum / BANYAK_SAMPLING);   // perintah mencari rata-rata temp sebagai hasil baca adc
}

void get1iben::baca_data(void)
{
  float pv_tegangan = random(5.0, 9.0);
  float pv_arus = random(1.4, 4.0);
  float gas = random (0.0 , 1024.0);
  float flame = random (0.0 , 1024.0);
  float jarak = random (0.0 , 1024.0);
  // pv_tegangan = baca_adc(PIN_V_TEG)*(5.0/1023.0)*11;     // hitung tegangan panel surya
  // pv_arus = baca_adc(PIN_V_ARUS)*(3.0/1023.0)*11;   // hitung tegangan beban
  pv_tegangan = baca_adc(PIN_V_TEG)*5;     // hitung tegangan panel surya
  pv_arus = baca_adc(PIN_V_ARUS)*5;   // hitung tegangan beban
  gas = (PIN_GAS);    // hitung nilai gas
  flame = (PIN_FLAME); // hitung nilai api
  jarak = (PIN_JARAK); // hitung nilai jarak
}

void get1iben::print_data(void)
{
  float pv_tegangan = random(5.0 , 9.0);
  float pv_arus = random (1.4 , 4.0);
  float gas = random (0.0 , 1024.0);
  float flame = random (0.0 , 1024.0);
  float jarak = random (0.0 , 1024.0);

  Serial.println("=========================================================================================");

  Serial.print("tegangan terbaca : ");
  Serial.print(pv_tegangan);
  Serial.print(" V");
  Serial.print("  |  ");

  Serial.print("arus terbaca : ");
  Serial.print(pv_arus);
  Serial.println(" A");

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
  Serial.println(" Analog");

  Serial.println("=========================================================================================");

}
