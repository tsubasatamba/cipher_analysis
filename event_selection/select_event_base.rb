#!/usr/bin/env ruby

require 'comptonsoft'
require 'json'

class SelectEvent < ANL::ANLApp
  attr_accessor :inputs, :output
  attr_accessor :sumPH_range, :rank_range, :weight_range, :angle_range
  
  def setup()
    add_namespace ComptonSoft
    
    chain :XrayEventCollection
    chain :ReadXrayEventTree
    with_parameters(file_list: @inputs)
    chain :XrayEventSelection
    with_parameters() do |a|
      a.add_condition("sumPH", @sumPH_range)
      a.add_condition("rank", @rank_range)
      a.add_condition("weight", @weight_range)
      a.add_condition("angle", @angle_range)
    end
    chain :WriteXrayEventTree
    with_parameters(collection_module: "XrayEventSelection")
    chain :SaveData
    with_parameters(output: @output)
  end
end

if (ARGV.size()!=9) then
  puts "USAGE: select_event_base.rb input_file output_file sumPH_min sumPH_max rank_min rank_max weight_min weight_max eventtype"
  exit!
end

input_file = ARGV[0]
output_file = ARGV[1]
sumPH_min = ARGV[2].to_f
sumPH_max = ARGV[3].to_f
rank_min = ARGV[4].to_i
rank_max = ARGV[5].to_i
weight_min = ARGV[6].to_i
weight_max = ARGV[7].to_i
event_type = ARGV[8]

sumPH_range = [sumPH_min, sumPH_max]
rank_range = [rank_min, rank_max]
weight_range = [weight_min, weight_max]
angle_range = []
angle_array = [0.0, Math::PI*0.5]
eps = 0.05

if event_type=="single" then
  angle_range = [-100.0, 100.0]
elsif event_type=="H-type" then
  for j in -1..1 do
    angle_range << [angle_array[0]+Math::PI*j-eps, angle_array[0]+Math::PI*j+eps]
  end
elsif event_type=="V-type" then
  for j in -1..1 do
    angle_range << [angle_array[1]+Math::PI*j-eps, angle_array[1]+Math::PI*j+eps]
  end
else
  puts "event_type inappropriate"
  exit!
end

a1 = SelectEvent.new
a1.inputs = [input_file]
a1.output = output_file
a1.sumPH_range = sumPH_range
a1.rank_range = rank_range
a1.weight_range = weight_range
a1.angle_range = angle_range
a1.run(:all)
