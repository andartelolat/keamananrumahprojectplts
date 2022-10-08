// File header Library

// Fungsi Library bungkus
  #ifndef get1iben_H
  #define get1iben_H
  #include <Arduino.h>
// tutup bungkus

// Class iben
class get1iben{
  public:
  get1iben();
  ~get1iben();
  int baca_adc(int channel);
  void baca_data(void);
  void print_data(void);
  void pesanBaru(int nmrPesan);
};
#endif
