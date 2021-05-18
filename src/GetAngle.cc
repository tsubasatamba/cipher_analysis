#include "GetAngle.hh"

double getAngle(std::string degree) {
  if (degree.substr(degree.size()-3, 3)=="deg") {
    for (int i=0; i<3; i++) {
      degree.pop_back();
    }
  }
  double c = 1.0;
  if (degree[0]=='m') {
    degree.erase(0, 1);
    c = -1.0;
  }
  if (degree=="67") degree = "67.5";
  if (degree=="22") degree = "22.5";
  double d = c*std::stod(degree);
  
  return d;
}
