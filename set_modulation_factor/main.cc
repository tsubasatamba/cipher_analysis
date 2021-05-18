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

#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include "GetEntry.hh"

int main() {
  const std::string json_filename = "/Users/tamba/work/cipher/SPring8_analysis_2021/experiments/experiments.json";
  const std::string correction_factor_filename = "/Users/tamba/work/cipher/SPring8_analysis_2021/analysis/correction_factor/correction_factors.json";
  json js;
  std::ifstream fin(json_filename);
  fin >> js;
  json js_corr;
  std::ifstream fin2(correction_factor_filename);
  fin >> js;
  const std::string base_dir = "/Users/tamba/work/cipher/SPring8_analysis_2021/products/";
  json js_out;
  std::vector<std::string> event_types = {"H-type", "V-type"};
  
  for (auto e1: js.items()) {
    const std::string experiment_name = e1.key();
    json degrees = e1.value()["degrees"];
    json correction_factor = js_corr[experiment_name];
    json js_now;
    for (std::string degree: degrees) {
      std::vector<double> num(2);
      for (int i=0; i<2; i++) {
        const std::string root_filename = base_dir + "/" + experiment_name + "/" + degree + "/events_" + event_types[i] + ".root";
        const double corr = correction_factor[event_types[i]];
        num[i] = static_cast<double>(getEntry(root_filename))*corr;
      }
      const double modulation_factor = num[0]/(num[0]+num[1]);
      const double error = modulation_factor*(1.0/std::sqrt(num[0]+num[1]));
      json js_tmp;
      js_tmp.push_back(json::object_t::value_type("modulation_factor", modulation_factor));
      js_tmp.push_back(json::object_t::value_type("error", error));
      js_now.push_back(json::object_t::value_type(degree, js_tmp));
    }
    js_out.push_back(json::object_t::value_type(experiment_name, js_now));
  }
  const std::string output_json_filename = "/Users/tamba/work/cipher/SPring8_analysis_2021/analysis/modulation_factor/modulation_factors.json";
  std::ofstream fout(output_json_filename);
  fout << std::setw(2) << js_out;

  return 0;
}
