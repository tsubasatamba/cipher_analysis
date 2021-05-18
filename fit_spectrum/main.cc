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
#include "CountSpectrum.hh"

int main() {
  const std::string json_filename = "peak_ADU.json";
  json js;
  std::ifstream fin(json_filename);
  fin >> js;
  const std::string base_dir = "../../products/";
  json js_out;
  
  for (auto e1: js.items()) {
    const std::string experiment_name = e1.key();
    json degrees = e1.value()["degrees"];
    double peak_ADU = e1.value()["peak_ADU"];
    const int nx = 10000;
    const std::string event_type = "good";
    std::string name = experiment_name + "_" + event_type;
    std::string title = experiment_name + "_" + event_type;
    cipher::CountSpectrum* cs = new cipher::CountSpectrum();
    cs->setHistogram(name, title, nx, 0, nx);
    for (std::string degree: degrees) {
      const std::string root_filename = base_dir + "/" + experiment_name + "/" + degree + "/events.root";
      cs->read(root_filename, event_type);
    }
    double delta = 0.05;
    if (experiment_name=="2020_sensor2_32keV" || experiment_name=="2019_normal_24keV" || experiment_name=="2020_sensor2_24keV") {
      delta = 0.02;
    }
    double real_peak_ADU = cs->getMaximumBin((1.0-delta)*peak_ADU, (1.0+delta)*peak_ADU);
    TF1* f1 = cs->fitGauss((1.0-delta)*real_peak_ADU, (1.0+delta)*real_peak_ADU);
    real_peak_ADU = f1->GetParameter(1);
    TF1* f2 = cs->fitGauss((1.0-delta)*real_peak_ADU, (1.0+delta)*real_peak_ADU);
    const double norm = f2->GetParameter(0);
    const double mean = f2->GetParameter(1);
    const double sigma = f2->GetParameter(2);
    const std::string image_filename = "spectrum_fitting/" + experiment_name + "/spectrum_" + event_type + "_fit.png";
    cs->generateImage(image_filename);
    json js_now;
    js_now.push_back(json::object_t::value_type("norm", norm));
    js_now.push_back(json::object_t::value_type("mean", mean));
    js_now.push_back(json::object_t::value_type("sigma", sigma));
    js_out.push_back(json::object_t::value_type(experiment_name, js_now));
  }

  const std::string output_json_filename = "spectrum_fitting/fitting_parameters.json";
  std::ofstream fout(output_json_filename);
  fout << std::setw(2) << js_out;

  return 0;
}
