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
#include <TF1.h>

#include "GetEntry.hh"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

int main() {
  const std::string json_filename = "/Users/tamba/work/cipher/SPring8_analysis_2021/experiments/experiments.json";
  json js;
  std::ifstream fin(json_filename);
  fin >> js;
  const std::string base_dir = "/Users/tamba/work/cipher/SPring8_analysis_2021/products/";
  json js_out;
  std::vector<std::string> event_types = {"H-type", "V-type"};
  
  for (auto e1: js.items()) {
    const std::string experiment_name = e1.key();
    json degrees = e1.value()["degrees"];
    std::vector<long long> num(2);
    for (std::string degree: degrees) {
      for (int i=0; i<2; i++) {
        const std::string root_filename = base_dir + "/" + experiment_name + "/" + degree + "/events_" + event_types[i] + ".root";
        num[i] += getEntry(root_filename);
      }    
    }
    std::vector<double> correction(2);
    for (int i=0; i<2; i++) {
      const double proportion = static_cast<double>(num[i])/(static_cast<double>(num[0])+static_cast<double>(num[1]));
      correction[i] = 0.5/proportion;
    }
    
    json js_now;
    js_now.push_back(json::object_t::value_type("H-type", correction[0]));
    js_now.push_back(json::object_t::value_type("V-type", correction[1]));
    js_out.push_back(json::object_t::value_type(experiment_name, js_now));
  }
  const std::string output_json_filename = "/Users/tamba/work/cipher/SPring8_analysis_2021/analysis/correction_factor/correction_factors.json";
  std::ofstream fout(output_json_filename);
  fout << std::setw(2) << js_out;

  return 0;
}
