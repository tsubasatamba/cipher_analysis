#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <TStyle.h>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TCanvas.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include "CountSpectrum.hh"

int main() {
  const std::string json_filename = "../../experiments/experiments.json";
  json js;
  std::ifstream fin(json_filename);
  fin >> js;
  const std::string base_dir = "../../products/";
  for (auto e1: js.items()) {
    const std::string experiment_name = e1.key();
    json degrees = e1.value()["degrees"];
    for (std::string degree: degrees) {
      const std::string root_filename = base_dir + "/" + experiment_name + "/" + degree + "/events.root";
      const int nx = 10000;
      std::string name = experiment_name + "_" + degree;
      std::string title = experiment_name + "_" + degree;
      cipher::CountSpectrum* cs = new cipher::CountSpectrum();
      cs->setHistogram(name, title, nx, 0, nx);
      cs->read(root_filename, "good");
      const std::string image_filename = "spectrum_image_all/" + experiment_name + "/spectrum_" + degree + ".png";
      cs->generateImage(image_filename);
    }
  }
  
  

  return 0;
}
