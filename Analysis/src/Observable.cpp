#include "Analysis/Observable.h"


double InvariantMass::evaluateObservable(const std::vector<HepMC3::ConstGenParticlePtr>& particles) const {
    HepMC3::FourVector total_momentum;
    
    for (auto particle: particles)
        total_momentum += particle->momentum();
    
    return total_momentum.m();
}