#include "FitModulationCurve.hh"

TF1* fitModulationCurve(TGraphErrors* gr) {
  TF1* f = new TF1("f1", "[0]+[1]*sin((x-[2])*(TMath::Pi()/180.0))", 0.0, 360.0);
  f->SetParameters(0.5, 0.1, 0.0);
  gr->Fit("f1");
  return f;
}
