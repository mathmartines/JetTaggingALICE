/**
 * @headerfile - observable interface.      
 *               defines what type ofobservables can be computed for a vector of particles
 **/

#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <iostream>
#include <vector>
#include "HepMC3/GenParticle.h"
#include "HepMC3/FourVector.h"


class Observable {
    public:
        /// @brief evaluates the observable for a set of hepmc3 particles
        /// @param particles - vector with the particles
        /// @return - the value of the observable
        virtual double evaluateObservable(const std::vector<HepMC3::ConstGenParticlePtr>& particles) const = 0;
};

/// @brief - computes the invarian mass of a vector of particles
class InvariantMass: public Observable {
    public:
        double evaluateObservable(const std::vector<HepMC3::ConstGenParticlePtr>& particles) const;
};


#endif