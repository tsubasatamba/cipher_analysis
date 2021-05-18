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
#include <TGraphErrors.h>

#include "FitModulationCurve.hh"
#include "GetAngle.hh"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

int main() {
  const std::string json_filename = "/Users/tamba/work/cipher/SPring8_analysis_2021/modulation_factor/modulation_factors.json";
  json js;
  std::ifstream fin(json_filename);
  fin >> js;
  json js_out;
  
  for (auto e1: js.items()) {
    const std::string experiment_name = e1.key();
    int n = 0;
    for (json e2: e1.value().items()) {
      n++;
    }
    TGraphErrors* gr = new TGraphErrors(n);
    int index = 0;
    for (auto e2: e1.value().items()) {
      const std::string degree = e2.key();
      const double x = getAngle(degree);
      const double xerror = 0.0;
      const double y = e2.value()["modulation_factor"];
      const double yerror = e2.value()["error"];
      gr->SetPoint(index, x, y);
      gr->SetPointError(index, xerror, yerror);
      index++;
    }
    TF1* f = fitModulationCurve(gr);
    std::vector<std::string> parameters = {"offset", "amplitude", "phase"};
    json js_now;
    for (int i=0; i<3; i++) {
      const double value = f->GetParameter(i);
      const double error = f->GetParError(i);
      json js_tmp;
      js_tmp.push_back(json::object_t::value_type("value", value));
      js_tmp.push_back(json::object_t::value_type("error", error));
      js_now.push_back(json::object_t::value_type(parameters[i], js_tmp));
    }
    js_out.push_back(json::object_t::value_type(experiment_name, js_now));
  }
  const std::string output_json_filename = "/Users/tamba/work/cipher/SPring8_analysis_2021/analysis/modulation_curve/modulation_curves.json";
  std::ofstream fout(output_json_filename);
  fout << std::setw(2) << js_out;

  return 0;
}
