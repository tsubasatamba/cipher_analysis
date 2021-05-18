#include <TCanvas.h>
#include <TStyle.h>
#include "GenerateImage.hh"

void generateImage(TH1D* spectrum, const std::string& filename) {
  TCanvas* canv = new TCanvas();
  canv->cd();
  canv->SetLogy(1);
  spectrum->Draw();
  gStyle->SetOptStat(0);
  canv->SaveAs(filename.c_str());
  std::cout << "generated " << filename << std::endl;
}
