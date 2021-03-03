#ifndef ORG_H
#define ORG_H

#include "emp/math/Random.hpp"
#include "emp/tools/string_utils.hpp"

class Organism {
    private:
        double threshold_mut_rate;
        double lysogeny_mut_rate;
        emp::Ptr<emp::Random> random;
        double threshold;
        double lysogeny_prob;

    public:
    Organism(emp::Ptr<emp::Random> _random, double _lysogeny_prob=0.0, double _threshold=0.0, double _threshold_mut_rate=0.002, double _lysogeny_mut_rate=0.02) :
        random(_random), lysogeny_prob(_lysogeny_prob), threshold(_threshold), threshold_mut_rate(_threshold_mut_rate), lysogeny_mut_rate(_lysogeny_mut_rate) {;}
    Organism(const Organism &) = default;
    Organism(Organism &&) = default;

    Organism & operator=(const Organism &) = default;
    Organism & operator=(Organism &&) = default;
    bool operator==(const Organism &other) const { return (this == &other);}
    bool operator!=(const Organism &other) const {return !(*this == other);}

    double getLysogenyProb() {return lysogeny_prob;}
    double getThreshold() {return threshold;}
    double getThresholdMutRate() {return threshold_mut_rate;}
    double getLysogenyMutRate() {return lysogeny_mut_rate;}

    void setLysogenyProb(double _in) {lysogeny_prob = _in;}
    void setThreshold(double _in) {threshold = _in;}
    void setThresholdMutRate(double _in) {threshold_mut_rate = _in;}
    void setLysogenyMutRate(double _in) {lysogeny_mut_rate = _in;}
    
    emp::Ptr<Organism> Reproduce() {
        emp::Ptr<Organism> offspring;
        offspring = new Organism(*this);
        offspring->mutate();
        
        return offspring;
    }
    void mutate() {
        lysogeny_prob += random->GetRandNormal(0.0, lysogeny_mut_rate);
        if(lysogeny_prob < 0) lysogeny_prob = 0;
        else if (lysogeny_prob > 1) lysogeny_prob = 1;
        threshold += random->GetRandNormal(0.0, threshold_mut_rate);
        if(threshold < 0) threshold = 0;
        else if (threshold > 1) threshold = 1;
    }


};
#endif