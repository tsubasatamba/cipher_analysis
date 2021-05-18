#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <TStyle.h>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH2.h>
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
  std::vector<std::string> event_types = {"good", "single", "double", "extended"};
  
  for (auto e1: js.items()) {
    const std::string experiment_name = e1.key();
    for (auto event_type: event_types) {
      json degrees = e1.value()["degrees"];
      const int nx = 10000;
      std::string name = experiment_name + "_" + event_type;
      std::string title = experiment_name + "_" + event_type;
      cipher::CountSpectrum* cs = new cipher::CountSpectrum();
      cs->setHistogram(name, title, nx, 0, nx);
      for (std::string degree: degrees) {
        const std::string root_filename = base_dir + "/" + experiment_name + "/" + degree + "/events.root";
        cs->read(root_filename, event_type);
      }
      const std::string image_filename = "spectrum_image_eventtype/" + experiment_name + "/spectrum_" + event_type + ".png";
      cs->generateImage(image_filename);
    }
  }
  
  

  return 0;
}
