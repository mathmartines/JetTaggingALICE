#include "Analysis/EventAnalyzer.h"


void EventAnalyzer::addParticleSelector (ParticleType particle_type, const ParticleSelector *part_selector) {
    /// updating the selection
    selection_criterias[particle_type] = part_selector;
    /// creating the vector to store the particles
    selected_particles[particle_type] = std::vector<HepMC3::ConstGenParticlePtr>();
}

const std::vector<HepMC3::ConstGenParticlePtr>& EventAnalyzer::getParticles (ParticleType particle_type) const {
    /// checking if the element is in the map
    auto it_part_type = selected_particles.find(particle_type);
    if (it_part_type != selected_particles.end()) 
        return it_part_type->second;
    // returns an empty vector in case the particle type is not found
    const std::vector<HepMC3::ConstGenParticlePtr> empty;
    return empty;
}

double EventAnalyzer::evaluateObservable (std::string observable_name, ParticleType particle_type) const  {
    /// checking if the element is in the map
    auto it_part_type = selected_particles.find(particle_type);
    auto it_obs_name = observables.find(observable_name);
    if (it_part_type == selected_particles.end() || it_obs_name == observables.end()) 
        return -1;
    /// returns the value of the observable
    return observables.at(observable_name)->evaluateObservable(selected_particles.at(particle_type));
}


void EventAnalyzer::resetVectors () {
    /// clears all vectors
    for (auto it_part_type = selected_particles.begin(); it_part_type != selected_particles.end(); it_part_type++)
        it_part_type->second.clear();
}


bool compareParticles (HepMC3::ConstGenParticlePtr part1, HepMC3::ConstGenParticlePtr part2) {
    return part1->momentum().pt() > part2->momentum().pt();
};

void EventAnalyzer::analyseEvent (const HepMC3::GenEvent& hepmc3_event) {
    this->resetVectors();
    // loops over the particles in the event 
    for (auto particle: hepmc3_event.particles()) {
        /// checking if the particle fits one possible selector
        auto selector_it = selection_criterias.begin();
        for(; selector_it != selection_criterias.end(); selector_it++) {
            if(selector_it->second->selectParticle(particle)) {
                selected_particles[selector_it->first].push_back(particle);
                /// moving to the next particle
                break;
            }
        }
    }
    // order the vectors by pT
    auto partIterator = selected_particles.begin();
    for(; partIterator != selected_particles.end(); partIterator++)
        std::sort(partIterator->second.begin(), partIterator->second.end(), compareParticles);
}