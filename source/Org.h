#ifndef ORG_H
#define ORG_H

#include "emp/math/Random.hpp"
#include "emp/tools/string_utils.hpp"

class Organism {
    private:
        double coop_prob;
        double points;
        double mut_rate = 0.002;
        emp::Ptr<emp::Random> random;

    public:
    Organism(emp::Ptr<emp::Random> _random, double _coop_prob=0.0, double _points=0.0) :
        random(_random), coop_prob(_coop_prob), points(_points) {;}
    Organism(const Organism &) = default;
    Organism(Organism &&) = default;

    Organism & operator=(const Organism &) = default;
    Organism & operator=(Organism &&) = default;
    bool operator==(const Organism &other) const { return (this == &other);}
    bool operator!=(const Organism &other) const {return !(*this == other);}

    double getCoopProb() {return coop_prob;}
    double getPoints() {return points;}

    void setCoopProb(double _in) {coop_prob = _in;}
    void setPoints(double _in) {points = _in;}
    void addPoints(double _in) {points += _in;}

    void mutate() {
        coop_prob += random->GetRandNormal(0.0, mut_rate);
        if(coop_prob < 0) coop_prob = 0;
        else if (coop_prob > 1) coop_prob = 1;
    }

    emp::Ptr<Organism> checkReproduction() {
        emp::Ptr<Organism> offspring;
        if(points>=1000) {
            offspring = new Organism(*this);
            points -= 1000;
            offspring->mutate();
            offspring->setPoints(0);
        }
        return offspring;
    }

    void Process(double resources) {
        points += resources;
        
    }


};
#endif