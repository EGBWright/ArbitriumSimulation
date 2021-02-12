//  This file is part of Evolutionary Algorithm
//  Copyright (C) Anya Vostinar, 2021.
//  Released under MIT license; see LICENSE

#include <iostream>

#include "emp/base/vector.hpp"
#include "emp/config/command_line.hpp"
#include "emp/math/random_utils.hpp"
#include "emp/math/Random.hpp"

#include "World.h"

// This is the main function for the NATIVE version of this project.

int main(int argc, char* argv[])
{
  emp::vector<std::string> args = emp::cl::args_to_strings(argc, argv);

  emp::Random random(2);
  OrgWorld world(random);

  //world.SetupOrgFile("Org_Vals_1.dat");
  

  emp::Ptr<Organism> new_org = new Organism(&random, 0.5);
  world.Inject(*new_org);
  world.Resize(100,100);
  
  for(int i=0; i<1000; i++) {
    std::cout<< "Update: " << i << std::endl;
    std::cout << "Population: " << world.GetNumOrgs() << std::endl;
    world.Update();
  }


}
