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

#include "World.h"

// This is the main function for the NATIVE version of this project.
EMP_BUILD_CONFIG(MyConfigType,
    VALUE(SEED, int, 10, "What value should the random seed be?"), 
    VALUE(START_PROB, double, 0.5, "What cooperation probability value should the starting organism have?"),
    VALUE(FILE_NAME, std::string, "_data.dat", "Root output file name")
)

int main(int argc, char* argv[])
{
  MyConfigType config;
  config.Read("MySettings.cfg");
  bool success = config.Read("MySettings.cfg");
  if(!success) config.Write("MySettings.cfg");
  
  emp::vector<std::string> args = emp::cl::args_to_strings(argc, argv);

  emp::Random random(2);
  OrgWorld world(random);

  world.SetupOrgFile("Org_Vals_1.dat");
  

  emp::Ptr<Organism> new_org = new Organism(&random, 0.5);
  world.Inject(*new_org);
  world.Resize(100,100);
  
  for(int i=0; i<1000; i++) {
    std::cout<< "Update: " << i << std::endl;
    std::cout << "Population: " << world.GetNumOrgs() << std::endl;
    world.Update();
  }


}
