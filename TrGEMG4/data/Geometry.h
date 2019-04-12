#include <string>
#include <vector>

namespace Geometry {
  std::vector<std::string> layerName= 
  {
    "FakeBottom",                                   //Fake
    "DriftCopper1","DriftBoard","DriftCopper2",     //Drift Board
    "GasGap1",                                      //Drift Gap
    "Gem1Copper1","Gem1","Gem1Copper2",             //gem1
    "GasGap2",                                      //Transfer I Gap
    "Gem2Copper1","Gem2","Gem2Copper2",             //gem2
    "GasGap3",                                      //Transfer II Gap
    "Gem3Copper1","Gem3","Gem3Copper2",             //gem3
    "GasGap4",                                      //Induction Gap
    "ReadCopper1","ReadoutBoard","ReadCopper2",     //Readout Board
    //"GEBCoppor", "GEB",                             //GEB Board
    //"CoolingPads",                                  //CollingPads
    //"Cover",                                        //Cover
    "FakeTop"                                       //Fake
  };
}
