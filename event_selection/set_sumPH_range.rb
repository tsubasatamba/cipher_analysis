#!/usr/bin/env ruby

require 'comptonsoft'
require 'json'

def read_json(filename)
  hash = {}
  File.open(filename, mode="rt") do |f|
    hash = JSON.load(f)
  end
  return hash
end


### Main

parameter_file = "/Users/tamba/work/cipher/SPring8_analysis_2021/analysis/spectrum/spectrum_fitting/fitting_parameters.json"
hin = read_json(parameter_file)
hout = {}
num_sigma = 2

hin.each do |name, parameters|
  norm = parameters["norm"]
  mean = parameters["mean"]
  sigma = parameters["sigma"]
  sumPH_range = [mean-sigma*num_sigma, mean+sigma*num_sigma]
  hout[name] = sumPH_range
end

output_file = "/Users/tamba/work/cipher/SPring8_analysis_2021/analysis/selection/sumPH_ranges.json"
json_str = JSON.pretty_generate(hout)
File.open(output_file, "w") do |f|
  f.print(json_str)
end
