#!/usr/bin/env ruby

require 'json'

def read_json(filename)
  hash = {}
  File.open(filename, mode="rt") do |f|
    hash = JSON.load(f)
  end
  return hash
end


### Main
event_types = ["single", "H-type", "V-type"]
rank_ranges = [[0, 0], [1, 1], [1, 1]]
weight_ranges = [[1, 1], [2, 2], [2, 2]]

experiment_file = "/Users/tamba/work/cipher/SPring8_analysis_2021/experiments/experiments.json"
sumPH_range_file = "/Users/tamba/work/cipher/SPring8_analysis_2021/analysis/selection/sumPH_ranges.json"
experiments = read_json(experiment_file)
sumPH_ranges = read_json(sumPH_range_file)

experiments.each do |name, experiment|
  degrees = experiment["degrees"]
  degrees.each do |degree|
    base_directory = "/Users/tamba/work/cipher/SPring8_analysis_2021/products/#{name}/#{degree}/"
    input_file = base_directory + "/events.root"
    event_types.each_with_index do |event_type, i|
      output_file = base_directory + "/events_#{event_type}.root"
      sumPH_range = sumPH_ranges[name]
      rank_range = rank_ranges[i]
      weight_range = weight_ranges[i]
      system("select_event_base.rb #{input_file} #{output_file} #{sumPH_range[0]} #{sumPH_range[1]} #{rank_range[0]} #{rank_range[1]} #{weight_range[0]} #{weight_range[1]} #{event_type}")
    end
  end
end
