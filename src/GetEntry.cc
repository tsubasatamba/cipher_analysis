#include "GetEntry.hh"

long long getEntry(const std::string& filename) {
  TFile* f = new TFile(filename.c_str());
  TTree* tree = (TTree*)(f->Get("xetree"));
  long long res = tree->GetEntries();
  f->Close();
  return res;
}
