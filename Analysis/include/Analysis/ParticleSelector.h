/**
 * @headerfile - Declaration of the ParticleSelector interface and its concrete implementations.
 *               The ParticleSelector has only one method called selectParticle which returns 
 *               true if the particle must be selected and false otherwise.
 **/

#ifndef PARTICLE_SELECTOR_H
#define PARTICLE_SELECTOR_H

#include <iostream>
#include <vector>
#include <set>
#include "HepMC3/GenParticle.h"


/** 
* @class - Declaration of the ParticleSelector interface.
*          Its only has one method which receives a pointer to a HepMC3::GenParticle.
*          The method returns true if the particle must be selected and false otherwise.
**/
class ParticleSelector {
    public:
        /// @brief - Indicates wether the particle must be selected or not.
        /// @param particle - pointer to the HepMC3::GenParticle object.
        /// @return - True if the particle must be selected, false otherwise.
        virtual bool selectParticle(const HepMC3::GenParticlePtr particle) const = 0;
};


/**
 * @class - Class to select only final state particles.
 **/
class FinalStateSelector: public ParticleSelector { 
    public:
        bool selectParticle(const HepMC3::GenParticlePtr particle) const {
            // status 1 represents a final state particle that is not further decayed by the generator
            return particle->status() == 1;
        };
};


/** 
 * @class - Class to select only the charged particles.
 * **/
class ChargedParticlesSelector: public ParticleSelector {
    public:
        /// we do not have information about the charge, but since we have the PID
        /// we can only accept particles with PIDs corresponding to charged particles
        bool selectParticle(const HepMC3::GenParticlePtr particle) const;

        /// @brief - adds a new pid to the list
        void addPID(int pid) {_charged_part_pids.insert(pid);};

    private:
        /// stores the absolute value of the PIDs of charged particles
        std::set<int> _charged_part_pids = {11, 13, 211, 321, 2212};
};

/**
 * @class - selects a particle only if it passes a set of particle selectors
 **/
class MultipleParticleSelectors: public ParticleSelector {
    public:
        MultipleParticleSelectors(const std::vector<const ParticleSelector*>& particleSelectors): _part_selectors(particleSelectors) {};

        /// @brief - adds a new selector
        /// @param partSelector - pointer to the particle selector that must be added
        void addSelector(const ParticleSelector* partSelector) {_part_selectors.push_back(partSelector);};

        /// selects the particle only if the particle is select by all the selectors
        bool selectParticle(const HepMC3::GenParticlePtr particle) const;

    private:
        std::vector<const ParticleSelector*> _part_selectors;
};

#endif