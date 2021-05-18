#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TROOT.h>

#include "CountSpectrum.hh"

namespace cipher
{

CountSpectrum::CountSpectrum()
{
}

void CountSpectrum::setHistogram(const std::string& name, const std::string& title, int nx, double xmin, double xmax)
{
  spectrum_ = new TH1D(name.c_str(), title.c_str(), nx, xmin, xmax);
}

CountSpectrum::~CountSpectrum() = default;

bool CountSpectrum::isEvent(double sumPH, int rank, int weight, const std::string& type) {
  if (sumPH<sumPHMin || sumPH>sumPHMax) {
    return false;
  }
  if (type=="good") {
    return rank<=1;
  }
  if (type=="single") {
    return rank==0;
  }
  if (type=="double") {
    return rank==1 && weight==2;
  }
  if (type=="extended") {
    return rank==1 && weight>=3;
  }
  return false;
}

void CountSpectrum::read(const std::string& filename, const std::string& type) {
  TFile* f = new TFile(filename.c_str());
  TTree* tree = (TTree*)(f->Get("xetree"));
  const int num_entries = tree->GetEntries();
  double sumPH = 0.0;
  int rank = 0;
  int weight = 0;
  tree->SetBranchAddress("sumPH", &sumPH);
  tree->SetBranchAddress("rank", &rank);
  tree->SetBranchAddress("weight", &weight);
  for (int i=0; i<num_entries; i++) {
    tree->GetEntry(i);
    if (isEvent(sumPH, rank, weight, type)) {
      spectrum_->Fill(sumPH, 1.0);
    }
  }
  f->Close();
}

void CountSpectrum::generateImage(const std::string& filename) {
  TCanvas* canv = new TCanvas();
  canv->cd();
  canv->SetLogy(1);
  spectrum_->Draw();
  gStyle->SetOptStat(0);
  canv->SaveAs(filename.c_str());
  std::cout << "generated " << filename << std::endl;
}

TF1* CountSpectrum::fitGauss(double xmin, double xmax)
{
  TF1 *f1 = new TF1("f1", "[0]*TMath::Gaus(x, [1], [2])");
  double mean = 0.5*(xmin+xmax);
  double norm = spectrum_->GetBinContent(spectrum_->FindBin(mean));
  double sigma = mean*0.01;
  f1->SetParameters(norm, mean, sigma);
  spectrum_->Fit("f1", "", "", xmin, xmax);
  return f1;
}

double CountSpectrum::getMaximumBin(double xmin, double xmax)
{
  double res = xmin;
  double v = 0.0;
  for (int i=static_cast<int>(xmin); i<=static_cast<int>(xmax); i++) {
    const double now = spectrum_->GetBinContent(spectrum_->FindBin(i));
    if (now>v) {
      res = i;
      v = now;
    }
  }
  return res;
}

}
