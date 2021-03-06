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
    int STARTING_PHAGES;
    float ARBITIUM_DECAY_RATE; 


    emp::Ptr<emp::DataMonitor<float, emp::data::Histogram>> data_node_lysogeny_rate;
    emp::Ptr<emp::DataMonitor<float, emp::data::Histogram>> data_node_threshold;
    emp::Ptr<emp::DataMonitor<float, emp::data::Histogram>> data_node_evolvers;

    public:

    OrgWorld(emp::Random &random) : emp::World<Organism>(random), random(random)  {
        random_ptr.New(random);
    }

    ~OrgWorld() {
        if(data_node_lysogeny_rate) data_node_lysogeny_rate.Delete();
        if(data_node_threshold) data_node_threshold.Delete();
        if(data_node_evolvers) data_node_evolvers.Delete();

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
    void setSTARTING_PHAGES(int _in){STARTING_PHAGES = _in;}
    void setARBITIUM_DECAY_RATE(int _in){ARBITIUM_DECAY_RATE = _in;}
    void initPHAGE_ARRAY(std::vector<emp::Ptr<Organism>> _in) {PHAGE_ARRAY = _in;}
    void initLYSOGEN_ARRAY(std::vector<emp::Ptr<Organism>> _in) {LYSOGEN_ARRAY = _in;}

    emp::DataMonitor<float, emp::data::Histogram>& GetLysogenyRateDataNode() {
        if (!data_node_lysogeny_rate) {
        data_node_lysogeny_rate.New();
        OnUpdate([this](size_t){
            data_node_lysogeny_rate->Reset();
            for (auto phage : PHAGE_ARRAY)
              data_node_lysogeny_rate->AddDatum(phage->getLysogenyProb());
            for (auto lysogen : LYSOGEN_ARRAY)
              data_node_lysogeny_rate->AddDatum(lysogen->getLysogenyProb());
        });
        }
        return *data_node_lysogeny_rate;
    }

    emp::DataMonitor<float, emp::data::Histogram>& GetThresholdDataNode() {
        if (!data_node_threshold) {
        data_node_threshold.New();
        OnUpdate([this](size_t){
            data_node_threshold->Reset();
            for (auto phage : PHAGE_ARRAY)
              data_node_threshold->AddDatum(phage->getThreshold());
            for (auto lysogen : LYSOGEN_ARRAY)
              data_node_threshold->AddDatum(lysogen->getThreshold());
        });
        }
        return *data_node_threshold;
    }

    emp::DataMonitor<float, emp::data::Histogram>& GetEvolversDataNode() {
        if (!data_node_evolvers) {
        data_node_evolvers.New();
        OnUpdate([this](size_t){
            data_node_evolvers->Reset();
            for (auto phage : PHAGE_ARRAY)
              data_node_evolvers->AddDatum((phage->getThresholdMutRate() == 0.0)? 0.0f : 1.0f);
            for (auto lysogen : LYSOGEN_ARRAY)
              data_node_evolvers->AddDatum((lysogen->getThresholdMutRate() == 0.0)? 0.0f : 1.0f);
        });
        }
        return *data_node_evolvers;
    }

    emp::DataFile & SetupOrgFile(const std::string & filename) {
    auto & file = SetupFile(filename);
    auto & node = GetLysogenyRateDataNode();
    auto & node1 = GetThresholdDataNode();
    auto & node2 = GetEvolversDataNode();
    node.SetupBins(0.0, 1.1, 10); //Necessary because range exclusive
    file.AddVar(update, "update", "Update");
    file.AddVar(BACTERIA_POP, "bacteria_pop", "number of bacteria");
    file.AddVar(PHAGE_POP, "phage_pop", "number of phages");
    file.AddVar(LYSOGEN_POP, "lysogen_pop", "number of lysogens");
    file.AddVar(ARBITRIUM, "arbitrium", "number of arbitrium peptides");
    file.AddMean(node, "mean_lysogeny_rate", "Average organism lysogeny rate");
    file.AddMean(node1, "mean_threshold", "Average arbitrium threshold");
    file.AddMean(node2, "evolver_pct", "proportion of population whose arbitrium threshold is evolving/mutating");
    /*
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
    */


    file.PrintHeaderKeys();

    return file;
  }
  void Reset(){
    BACTERIA_POP = CARRYING_CAPACITY;
    ARBITRIUM = 0;
    for (auto lysogen : LYSOGEN_ARRAY){
      for (int i=0; i< BURST_SIZE;i++){
        PHAGE_ARRAY.push_back(lysogen->Reproduce());
      }
      PHAGE_POP = PHAGE_POP + BURST_SIZE;
      //PHAGE_ARRAY.push_back(lysogen);
      //PHAGE_POP += 1;
    }
    LYSOGEN_ARRAY.clear();
    LYSOGEN_POP=0;
    if (PHAGE_POP > STARTING_PHAGES){
      size_t max = 200;
      size_t min = 0;
      size_t nums = (size_t) STARTING_PHAGES;

      emp::vector<size_t> vals =  RandomUIntVector(random, nums, min, max);
      std::vector<emp::Ptr<Organism>> NEW_PHAGE_ARRAY;
      for (auto val : vals){
        NEW_PHAGE_ARRAY.push_back(PHAGE_ARRAY.at(val));
      }
      PHAGE_ARRAY = NEW_PHAGE_ARRAY;
      PHAGE_POP = STARTING_PHAGES;
    }
    
  }

  void ArbitriumDecay(){
    int arbitrium_lost = (int) ARBITIUM_DECAY_RATE * ARBITRIUM + 0.5f;
    ARBITRIUM -= arbitrium_lost;
  }

  void Growth() {
    int empty = CARRYING_CAPACITY - BACTERIA_POP - LYSOGEN_ARRAY.size();
    int new_bacteria = (int) ((BACTERIA_POP * empty * BACTERIA_GROWTH_RATE) / CARRYING_CAPACITY)+ 0.5;
    BACTERIA_POP = BACTERIA_POP + new_bacteria;
    size_t new_lysogens = (size_t) ((LYSOGEN_POP * empty * BACTERIA_GROWTH_RATE) / CARRYING_CAPACITY)+ 0.5;
    LYSOGEN_POP = LYSOGEN_POP + new_lysogens;
    size_t min = 0;
    size_t max = LYSOGEN_ARRAY.size();
    emp::vector<size_t> vals =  RandomUIntVector(random, new_lysogens, min,max);
    for (size_t val : vals){
      emp::Ptr<Organism> lysogen = LYSOGEN_ARRAY.at(val);
      emp::Ptr<Organism> new_lysogen = lysogen->Reproduce();
      LYSOGEN_ARRAY.push_back(new_lysogen);
    }
  }

  void Lysis(emp::Ptr<Organism> phage) {;
    for (int i=1; i< BURST_SIZE;i++){
      PHAGE_ARRAY.push_back(phage->Reproduce());
    }
    phage->mutate();
    PHAGE_POP = PHAGE_POP + BURST_SIZE - 1;
  }

  void removePhage(emp::Ptr<Organism> phage){
    std::remove(PHAGE_ARRAY.begin(),PHAGE_ARRAY.end(),phage);
    PHAGE_ARRAY.pop_back();
    PHAGE_POP = PHAGE_POP - 1;
  }

  void removeLysogen(emp::Ptr<Organism> phage){
    std::remove(LYSOGEN_ARRAY.begin(),LYSOGEN_ARRAY.end(),phage);
    LYSOGEN_ARRAY.pop_back();
    LYSOGEN_POP = LYSOGEN_POP - 1;
  }

  void LysogenAdsorption(){
    std::vector<emp::Ptr<Organism>> victims (0);
    double adsorption_prob = LYSOGEN_POP * ADSORPTION_RATE;
    //std::cout << adsorption_prob << std::endl;
    for (auto phage : PHAGE_ARRAY){
      if ((random.GetDouble(0, 1) < adsorption_prob) || ((float) random.GetDouble(0, 1) < PHAGE_DEPRECIATION)){
        victims.push_back(phage);
      }
    }
    //std::cout << victims.size() << "victims: {";
    for (auto victim : victims){
      //std::cout << victim->getLysogenyProb() << " ";
      removePhage(victim);
    }
  }
  void Induction(){
    //std::cout << "induction reached" << std::endl;
    std::vector<emp::Ptr<Organism>> inducers (0);
    for (auto lysogen : LYSOGEN_ARRAY){
      if ((float)random.GetDouble(0,1) < INDUCTION_RATE){
        inducers.push_back(lysogen);
      }
    }
    //std::cout << inducers.size() << " inducers: { " << std::endl;
    for (auto inducer : inducers){
      for (int i=0; i< BURST_SIZE;i++){
        PHAGE_ARRAY.push_back(inducer->Reproduce());
      }
      PHAGE_POP = PHAGE_POP + BURST_SIZE;
      removeLysogen(inducer);
    }
  }

  void Infections(){
    std::vector<emp::Ptr<Organism>> failures (0);
    std::vector<emp::Ptr<Organism>> infections (0);
    double adsorption_prob = BACTERIA_POP * ADSORPTION_RATE;
    //std::cout << adsorption_prob << std::endl;
    for (auto phage : PHAGE_ARRAY){
      if (random.GetDouble(0, 1) < adsorption_prob){
        if ((float)random.GetDouble(0,1) < INFECTION_SUCCESS_RATE){
          infections.push_back(phage);
        } else {
          failures.push_back(phage);
        }
      }
    }
    //std::cout << infections.size() << " infections: {";
    for (auto infection : infections){
      //std::cout << infection->getLysogenyProb() << " ";
      BACTERIA_POP -=1;
      float ARBITRIUM_RATE = (float) ARBITRIUM/ CARRYING_CAPACITY;
      if ((ARBITRIUM_RATE>= infection->getThreshold())&& (random.GetDouble(0, 1) < infection->getLysogenyProb())){
        Lysogeny(infection);
      } else{
        Lysis(infection);
      }
      ARBITRIUM += 1;
    }
    //std::cout << failures.size() << "failures: {";
    for (auto failure : failures){
      //std::cout << failure->getLysogenyProb() << " ";
      removePhage(failure);
    }
  }



  void Infection(){
    size_t StimesP = BACTERIA_POP * PHAGE_ARRAY.size();
    float failure_rate = 1 - INFECTION_SUCCESS_RATE;
    size_t num_failures = (size_t) ((StimesP * ADSORPTION_RATE)*(failure_rate)) + 0.5 ;
    //std::cout << StimesP << " " << ADSORPTION_RATE << " " << failure_rate << " " << num_failures << std::endl;
    size_t min = 0;
    size_t max = PHAGE_ARRAY.size();
    emp::vector<size_t> vals =  RandomUIntVector(random, num_failures, min,max);
    std::vector<emp::Ptr<Organism>> failures (0);
    for (auto val : vals){
      emp::Ptr<Organism> phage = PHAGE_ARRAY.at(val);
      //std::cout << val << "  " << &PHAGE_ARRAY.at(val) << std::endl;
      failures.push_back(phage);

    }
    //std::cout << "failures: {";
    for (auto failure : failures){
      //std::cout << failure->getLysogenyProb() << " ";
      removePhage(failure);
    }
    //std::cout << "}" <<std::endl;
    size_t num_infections = (size_t) ((StimesP * ADSORPTION_RATE)*(INFECTION_SUCCESS_RATE)) + 0.5 ;
    //std::cout << num_infections << std::endl;
    size_t new_max = PHAGE_ARRAY.size();
    vals.clear();
    vals =  RandomUIntVector(random, num_infections, min,new_max);
    std::vector<emp::Ptr<Organism>> infections (0);
    for (auto val : vals){
      emp::Ptr<Organism> phage = PHAGE_ARRAY.at(val);
      //std::cout << val << "  " << &PHAGE_ARRAY.at(val) << std::endl;
      infections.push_back(phage);

    }
    //std::cout << "infections: {";
    for (auto infection : infections){
      //std::cout << infection->getLysogenyProb() << " ";
      BACTERIA_POP -=1;
      float ARBITRIUM_RATE = (float) ARBITRIUM/ CARRYING_CAPACITY;
      if ((ARBITRIUM_RATE>= infection->getThreshold())&& (random.GetDouble(0, 1) < infection->getLysogenyProb())){
        Lysogeny(infection);
      } else{
        Lysis(infection);
      }
      ARBITRIUM += 1;
    }
      
  }
  
  void Lysogeny (emp::Ptr<Organism> phage) {
    LYSOGEN_ARRAY.push_back(phage);
    LYSOGEN_POP += 1;
    PHAGE_POP -= 1;
    //std::cout << " LYSOGENY BEFORE" <<PHAGE_ARRAY.size() << std::endl;
    std::remove(PHAGE_ARRAY.begin(),PHAGE_ARRAY.end(),phage);
    PHAGE_ARRAY.pop_back();
    //std::cout << " LYSOGENY AFTER" <<PHAGE_ARRAY.size() << std::endl;
    //std::cout << "phage vector after lysogeny: {";
      //for (auto phage : PHAGE_ARRAY){
        //std::cout << phage->getLysogenyProb() << " ";
      
      //}
      //std::cout << "}" <<std::endl;
  }
  void Update() {
      emp::World<Organism>::Update();

      
      //std::cout << "bacteria pop before growth: " << BACTERIA_POP <<std::endl;
      Growth();
      //std::cout << "bacteria pop after growth: " << BACTERIA_POP <<std::endl;
      Infections();
      LysogenAdsorption();
      Induction();
      /*
      std::cout << "phage vector: {";
      for (auto phage : PHAGE_ARRAY){
        std::cout << phage->getLysogenyProb() << " ";
      
      }
      std::cout << "}" <<std::endl;
      
      std::cout << "lysogeny vector: {";
      for (auto phage : LYSOGEN_ARRAY){
        std::cout << phage->getLysogenyProb() << " ";
      
      }
      std::cout << "}" <<std::endl;
      */
  }

};
#endif