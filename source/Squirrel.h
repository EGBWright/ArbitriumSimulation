#ifndef WORLD_H
#define WORLD_H

#include "emp/Evolve/World.hpp"
#include "emp/data/DataFile.hpp"
#include "emp/math/random_utils.hpp"
#include "emp/math/Random.hpp"
#include "emp/config/ArgManager.hpp"
#include <math.h>
#include "Phage.h"

class OrgWorld : public emp::World<Organism> {

    double resources_per_org = 100;
    emp::Random &random;
    emp::Ptr<emp::Random> random_ptr;
    std::vector<emp::Ptr<Organism>> PHAGE_ARRAY;
    std::vector<emp::Ptr<Organism>> LYSOGEN_ARRAY;
    int BURST_SIZE;
    float INFECTION_SUCCESS_RATE;
    int CARRYING_CAPACITY;
    float ADSORPTION_RATE;
    int BACTERIA_POP;
    int PHAGE_POP;
    int LYSOGEN_POP;
    float PHAGE_DEPRECIATION;
    float INDUCTION_RATE;
    float BACTERIA_GROWTH_RATE;
    int ARBITRIUM;


    emp::Ptr<emp::DataMonitor<double, emp::data::Histogram>> data_node_orgcoop;
    emp::Ptr<emp::DataMonitor<int>> data_node_orgcount;

    public:

    OrgWorld(emp::Random &random) : emp::World<Organism>(random), random(random)  {
        random_ptr.New(random);
    }

    ~OrgWorld() {
        if(data_node_orgcoop) data_node_orgcoop.Delete();
        if(data_node_orgcount) data_node_orgcount.Delete();
    }
    void setBURST_SIZE(int _in){BURST_SIZE = _in;}
    void setINFECTION_SUCCESS_RATE(float _in){INFECTION_SUCCESS_RATE = _in;}
    void setCARRYING_CAPACITY(int _in){CARRYING_CAPACITY = _in;}
    void setADSORPTION_RATE(float _in){ADSORPTION_RATE = _in;}
    void setBACTERIA_POP(int _in){BACTERIA_POP = _in;}
    void setPHAGE_POP(int _in){PHAGE_POP = _in;}
    void setLYSOGEN_POP(int _in){LYSOGEN_POP = _in;}
    void setPHAGE_DEPRECIATION(float _in){PHAGE_DEPRECIATION = _in;}
    void setINDUCTION_RATE(float _in){INDUCTION_RATE = _in;}
    void setBACTERIA_GROWTH_RATE(float _in){BACTERIA_GROWTH_RATE = _in;}
    void initPHAGE_ARRAY(std::vector<emp::Ptr<Organism>> _in) {PHAGE_ARRAY = _in;}
    void initLYSOGEN_ARRAY(std::vector<emp::Ptr<Organism>> _in) {LYSOGEN_ARRAY = _in;}

    emp::DataMonitor<int>& GetOrgCountDataNode() {
        if(!data_node_orgcount) {
        data_node_orgcount.New();
        OnUpdate([this](size_t){
            data_node_orgcount -> Reset();
            for (size_t i = 0; i< pop.size(); i++)
            if(IsOccupied(i))
                data_node_orgcount->AddDatum(1);
        });
        }
        return *data_node_orgcount;

    }

    emp::DataMonitor<double, emp::data::Histogram>& GetOrgCoopValDataNode() {
        if (!data_node_orgcoop) {
        data_node_orgcoop.New();
        OnUpdate([this](size_t){
            data_node_orgcoop->Reset();
            for (size_t i = 0; i< pop.size(); i++)
            if (IsOccupied(i))
                data_node_orgcoop->AddDatum(pop[i]->getLysogenyProb());
        });
        }
        return *data_node_orgcoop;
    }

    emp::DataFile & SetupOrgFile(const std::string & filename) {
    auto & file = SetupFile(filename);
    auto & node1 = GetOrgCountDataNode();
    auto & node = GetOrgCoopValDataNode();
    node.SetupBins(0.0, 1.1, 10); //Necessary because range exclusive
    file.AddVar(update, "update", "Update");
    file.AddMean(node, "mean_coopval", "Average organism cooperation value");
    file.AddTotal(node1, "count", "Total number of organisms");
    file.AddHistBin(node, 0, "Hist_0.0", "Count for histogram bin 0.0 to <0.1");
    file.AddHistBin(node, 1, "Hist_0.1", "Count for histogram bin 0.1 to <0.2");
    file.AddHistBin(node, 2, "Hist_0.2", "Count for histogram bin 0.2 to <0.3");
    file.AddHistBin(node, 3, "Hist_0.3", "Count for histogram bin 0.3 to <0.4");
    file.AddHistBin(node, 4, "Hist_0.4", "Count for histogram bin 0.4 to <0.5");
    file.AddHistBin(node, 5, "Hist_0.5", "Count for histogram bin 0.5 to <0.6");
    file.AddHistBin(node, 6, "Hist_0.6", "Count for histogram bin 0.6 to <0.7");
    file.AddHistBin(node, 7, "Hist_0.7", "Count for histogram bin 0.7 to <0.8");
    file.AddHistBin(node, 8, "Hist_0.8", "Count for histogram bin 0.8 to <0.9");
    file.AddHistBin(node, 9, "Hist_0.9", "Count for histogram bin 0.9 to 1.0");


    file.PrintHeaderKeys();

    return file;
  }
  void Growth() {
    int empty = CARRYING_CAPACITY - BACTERIA_POP - LYSOGEN_POP;
    int new_bacteria = (int) ((BACTERIA_POP * empty * BACTERIA_GROWTH_RATE) / CARRYING_CAPACITY)+ 0.5;
    BACTERIA_POP = BACTERIA_POP + new_bacteria;
  }
  void Lysis(emp::Ptr<Organism> phage) {;
    for (int i=1; i< BURST_SIZE;i++){
      PHAGE_ARRAY.push_back(phage->Reproduce());
    }
    phage->mutate();
    PHAGE_POP = PHAGE_POP + BURST_SIZE - 1;
    BACTERIA_POP -= 1;
  }

  void removePhage(emp::Ptr<Organism> phage){
    std::remove(PHAGE_ARRAY.begin(),PHAGE_ARRAY.end(),phage);
    PHAGE_ARRAY.pop_back();
  }

  void Adsorption(){
    size_t LtimesP = PHAGE_ARRAY.size() * LYSOGEN_ARRAY.size();
    size_t adsorptions = (size_t) (LtimesP * ADSORPTION_RATE) + 0.5;
    
  }

  void Lysogeny (emp::Ptr<Organism> phage) {
    LYSOGEN_ARRAY.push_back(phage);
    LYSOGEN_POP += 1;
    PHAGE_POP -= 1;
    BACTERIA_POP -= 1;
    std::cout << " LYSOGENY BEFORE" <<PHAGE_ARRAY.size() << std::endl;
    std::remove(PHAGE_ARRAY.begin(),PHAGE_ARRAY.end(),phage);
    PHAGE_ARRAY.pop_back();
    std::cout << " LYSOGENY AFTER" <<PHAGE_ARRAY.size() << std::endl;
    std::cout << "phage vector after lysogeny: {";
      for (auto phage : PHAGE_ARRAY){
        std::cout << phage->getLysogenyProb() << " ";
      
      }
      std::cout << "}" <<std::endl;
  }
  void Update() {
      emp::World<Organism>::Update();

      emp::vector<size_t> schedule = emp::GetPermutation(random, GetSize());
      double total_coop = 0;
      BACTERIA_POP -= 100;
      if (BACTERIA_POP < 0) {
        BACTERIA_POP=0;
      }
      std::cout << "bacteria pop before growth: " << BACTERIA_POP <<std::endl;
      Growth();
      for (size_t i: schedule) {
          if (IsOccupied(i) == false) continue; 

          emp::Ptr<Organism> offspring = pop[i]->Reproduce();
          if(offspring) {
              DoBirth(*offspring, i);
          }
      
          total_coop += pop[i]->getLysogenyProb();

      }
      std::cout << "Average cooperation probability: " << total_coop/GetNumOrgs() <<std::endl;
    
      emp::Ptr<Organism> back_ptr = PHAGE_ARRAY.back();
      Lysis(back_ptr);
      std::cout << "phage vector: {";
      for (auto phage : PHAGE_ARRAY){
        std::cout << phage->getLysogenyProb() << " ";
      
      }
      std::cout << "}" <<std::endl;
      
      emp::Ptr<Organism> front_ptr = PHAGE_ARRAY.front();
      Lysogeny(front_ptr);
      std::cout << "lysogeny vector: {";
      for (auto phage : LYSOGEN_ARRAY){
        std::cout << phage->getLysogenyProb() << " ";
      
      }
      std::cout << "}" <<std::endl;

  }

};
#endif