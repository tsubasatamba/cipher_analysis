#ifndef CipherCountSpectrum_H
#define CipherCountSpectrum_H

#include <iostream>
#include <string>
#include <TH1.h>
#include <TF1.h>

namespace cipher
{

class CountSpectrum{
public:
  CountSpectrum();
  ~CountSpectrum();
  
  void setHistogram(const std::string& name, const std::string& title, int nx, double xmin, double xmax);
  bool isEvent(double sumPH, int rank, int weight, const std::string& type);
  void read(const std::string& filename, const std::string& type);
  void generateImage(const std::string& filename);
  TF1* fitGauss(double xmin, double xmax);
  double getMaximumBin(double xmin, double xmax);
  
  void setSumPHRange(double v1, double v2) {
    sumPHMin = v1;
    sumPHMax = v2;
  }
  TH1* Spectrum() {
    return spectrum_;
  }
  
private:
  double sumPHMin = 0.0;
  double sumPHMax = 10000.0;
  TH1* spectrum_ = nullptr;
};
}
#endif
