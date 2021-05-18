#include <TFile.h>
#include <TTree.h>
#include "GenerateSpectrum.hh"

void generateSpectrum(const std::string& filename, TH1D* spectrum) {
  TFile* f = new TFile(filename.c_str());
  TTree* tree = (TTree*)(f->Get("xetree"));
  const int num_entries = tree->GetEntries();
  double sumPH = 0.0;
  tree->SetBranchAddress("sumPH", &sumPH);
  for (int i=0; i<num_entries; i++) {
    tree->GetEntry(i);
    spectrum->Fill(sumPH, 1.0);
  }
}
