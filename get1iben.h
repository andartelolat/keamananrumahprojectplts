// File header Library

// Fungsi Library
#ifndef get1iben_H
#define get1iben_H
#include <Arduino.h>

// membuat class
class get1iben
{
  public:
  get1iben();
  ~get1iben();
  int baca_adc(int channel);
  void baca_data(void);
  void print_data(void);
};
#endif
