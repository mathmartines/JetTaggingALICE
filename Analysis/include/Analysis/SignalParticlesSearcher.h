#ifndef SIGNAL_PARTICLES_SEARCHER_H
#define SIGNAL_PARTICLES_SEARCHER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "Analysis/ParticleSelector.h"
#include "Analysis/EventAnalyzer.h"

/// @brief - searches for the final state particles originated from the hard process
class SignalParticlesSearcher {

    public: 
        SignalParticlesSearcher(const ParticleSelector* selector): _selector(selector) {};

        /// @brief - searches for the final state particles
        /// @param hard_particles - particles originated from the hard process
        /// @return final stable particles from the hard process
        const std::vector<HepMC3::ConstGenParticlePtr>& selectParticles(const std::vector<HepMC3::ConstGenParticlePtr>& hard_particles);

    private:
        /// @brief - fills the final_particles vector by recursively looking to the decay product of the particle
        /// @param incomming_particle 
        void searchParticles (HepMC3::ConstGenParticlePtr incomming_particle);

        /// @brief - specifies how the criterias the end particles must fullfilled
        const ParticleSelector* _selector;  

        /// @brief - stores the vertices to avoid lookup repetition
        std::set<HepMC3::ConstGenVertexPtr> _vertices; 

        /// @brief - stores the selected particles
        std::vector<HepMC3::ConstGenParticlePtr> _final_particles;
};

#endif