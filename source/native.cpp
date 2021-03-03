//  This file is part of Evolutionary Algorithm
//  Copyright (C) Anya Vostinar, 2021.
//  Released under MIT license; see LICENSE
// run with make / ./evo-algo
#include <iostream>
#include "emp/config/ArgManager.hpp"
#include "emp/base/vector.hpp"
#include "emp/config/command_line.hpp"
#include "emp/math/random_utils.hpp"
#include "emp/math/Random.hpp"

#include "Squirrel.h"

// This is the main function for the NATIVE version of this project.
EMP_BUILD_CONFIG(MyConfigType,
    VALUE(SEED, int, 10, "What value should the random seed be?"), 
    VALUE(START_PROB, double, 0.5, "What cooperation probability value should the starting organism have?"),
    VALUE(FILE_NAME, std::string, "_data.dat", "Root output file name"),
    VALUE(BURST_SIZE, int, 4, "B, Number of viruses released by lysing a bacteria "),
    VALUE(INFECTION_SUCCESS_RATE, float, 0.5,"b, Fraction of attempted infections on susceptible bacteria that are successful"),
    VALUE(CARRYING_CAPACITY, int, 1000, "K, initial size of bacteria population"),
    VALUE(ADSORPTION_RATE, float, 0.001, "a, adsorption rate of free phages to lysogens and susceptible bacteria"),
    VALUE(PHAGE_DEPRECIATION, float, 0.01, "fraction of free phages who die at each period"),
    VALUE(THRESHOLD_MEAN, float, 0.0, "mean threshold of arbitrium density for lysogeny to occur" ),
    VALUE(INDUCTION_RATE, float, 0.0001, "alpha, Chance for a given lysogen to lyse in a certain time period" ),
    VALUE(THRESHOLD_DEVIATION, float, 0.0, "standard deviation of arbitrium threshold"),
    VALUE(LYSOGENY_RATE_MEAN, float, 0.5, "mean fraction of infections that enter lysogenic cycle, if arbitrium above threshold"),
    VALUE(LYSOGENY_RATE_DEVIATION, float, 0.2,"what it says"),
    VALUE(BACTERIA_GROWTH_RATE, float, 0.1, "logistic growth rate for bacteria and lysogens"),
    VALUE(PERIODS, int, 1000, "number of periods per reset"),
    VALUE(RESETS, int, 1, "number of resets"),
    VALUE(STARTING_PHAGES, int, 10, "initial number of phages in the"),
    VALUE(ARBITIUM_DECAY_RATE, float, 0.01, "percent of Arbitrium lost each period"),
    VALUE(THRESHOLD_EVOLVERS, float, 1.0, "initial percent of phages whose threshold values mutate"),
    VALUE(THRESHOLD_MUT_RATE, double , 0.002, "the standard deviation of mutation for threshold evolvers"),
    VALUE(LYSOGENY_MUT_RATE, double , 0.002, "the standard deviation of mutation for Lysogeny rate"),
    VALUE(TIMING_REPEAT, int , 1 , "how often data are recorded in the data file")

)

int main(int argc, char* argv[])
{
  MyConfigType config;
  config.Read("Settings.cfg");
  bool success = config.Read("Settings.cfg");
  if(!success) config.Write("Settings.cfg");
  
  emp::vector<std::string> args = emp::cl::args_to_strings(argc, argv);

  emp::Random random(config.SEED());
  OrgWorld world(random);

  world.SetupOrgFile(config.FILE_NAME()).SetTimingRepeat(1);
  
  emp::Random * sploink = &random;
  double newRate;
  double newThreshold;
  double newThresholdMutationRate; 
  std::vector<emp::Ptr<Organism>> phage_array;
  int evolvers = (int) config.THRESHOLD_EVOLVERS() * config.STARTING_PHAGES() + 0.5f;
  //emp::Ptr<Organism> new_org;

  for (int i=0; i < config.STARTING_PHAGES(); i++){
    newRate = sploink->GetRandNormal(config.LYSOGENY_RATE_MEAN(),config.LYSOGENY_RATE_DEVIATION());
    newRate = std::min(newRate,1.0 );
    newRate = std::max(newRate,0.0);
    newThreshold = sploink->GetRandNormal(config.THRESHOLD_MEAN(),config.THRESHOLD_DEVIATION());
    newThreshold = std::min(newThreshold,1.0);
    newThreshold = std::max(newThreshold,0.0);
    if (i < evolvers){
      newThresholdMutationRate = config.THRESHOLD_MUT_RATE();
    }
    else {
      newThresholdMutationRate = (double) 0.0;
    }
    phage_array.push_back(new Organism(&random, newRate, newThreshold, newThresholdMutationRate, config.LYSOGENY_MUT_RATE()));

  }
  
  world.Resize(1000,1000);
  world.setCARRYING_CAPACITY(config.CARRYING_CAPACITY() );
  world.setBACTERIA_POP(config.CARRYING_CAPACITY() );
  world.setLYSOGEN_POP(0);
  world.setBACTERIA_GROWTH_RATE(config.BACTERIA_GROWTH_RATE() );
  world.setINFECTION_SUCCESS_RATE(config.INFECTION_SUCCESS_RATE());
  world.setBURST_SIZE(config.BURST_SIZE());
  world.setADSORPTION_RATE(config.ADSORPTION_RATE());
  world.setPHAGE_POP(phage_array.size());
  world.initPHAGE_ARRAY(phage_array);
  world.setINDUCTION_RATE(config.INDUCTION_RATE());
  world.setPHAGE_DEPRECIATION(config.PHAGE_DEPRECIATION());
  world.setSTARTING_PHAGES(config.STARTING_PHAGES());
  
  for (int i=0; i<config.RESETS(); i++){
    world.Reset();
    for(int i=0; i< config.PERIODS(); i++) {
    world.Update();
    }
  }
  std::cout << "aaaahh " << "Data File:   " << config.FILE_NAME() << std::endl;
}
