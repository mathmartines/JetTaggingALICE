/**
 * @headerfile - definition of the EventAnalyzer class.
 *               It's responsible to select the particles for the user given the selection requirements.
 */

#ifndef EVENT_ANALYZER_H
#define EVENT_ANALYZER_H

#include <iostream>
#include <vector>
#include <map>
#include "Analysis/ParticleSelector.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"

enum ParticleType {FinalParticles, IncommingParticles, OutgoingHardProcess};
using Particles = std::map<ParticleType, std::vector<HepMC3::ConstGenParticlePtr>>;
using ParticleSelection = std::map<ParticleType, const ParticleSelector*>;

class EventAnalyzer {

    public:
        /// @brief - adds a new selector to be used 
        /// @param particle_type - correspond type of the particle that must be selected
        /// @param part_selector - correspondent criteria that the particles must satisfy to be selected
        void addParticleSelector (ParticleType particle_type, const ParticleSelector *part_selector);

        /// @brief - performs the analysis on the event - select the particles
        void analyseEvent (const HepMC3::GenEvent& hepmc3_event);

        /// @brief - returns the particles from a given selection type
        const std::vector<HepMC3::ConstGenParticlePtr>& getParticles (ParticleType particle_type) const;

    private:
        /// @brief - map to store the particles that must be selected on the event
        Particles selected_particles;
        /// @brief - stores the selection criterias
        ParticleSelection selection_criterias;
        
        /// @brief - clears all the vectors - this has to be done at each event 
        void resetVectors ();
};


#endif