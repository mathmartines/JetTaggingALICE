#include "Analysis/SignalParticlesSearcher.h"

const std::vector<HepMC3::ConstGenParticlePtr>& SignalParticlesSearcher::selectParticles(const std::vector<HepMC3::ConstGenParticlePtr>& hard_particles){
    /// clear all the vector
    _vertices.clear();
    _final_particles.clear();
    /// performing the search for each particle
    for (HepMC3::ConstGenParticlePtr particle: hard_particles)
        this->searchParticles(particle);
    return _final_particles;
}

void SignalParticlesSearcher::searchParticles(HepMC3::ConstGenParticlePtr incomming_particle) {
    // check if the particle is a final state particle
    if (_selector->selectParticle(incomming_particle)) {
        /// check if the particle is not already in the vector 
        std::vector<HepMC3::ConstGenParticlePtr>::iterator it = std::find(_final_particles.begin(), _final_particles.end(), incomming_particle);
        if (it == _final_particles.end())
            _final_particles.push_back(incomming_particle);
        return;
    }
    // only go to the vertex if we did not look at it before
    if (_vertices.find(incomming_particle->end_vertex()) == _vertices.end()) {
        _vertices.insert(incomming_particle->end_vertex());
        // look at every outgoing particle from the vertex
        for (HepMC3::ConstGenParticlePtr outgoing_particle: incomming_particle->end_vertex()->particles_out())
            this->searchParticles(outgoing_particle);
    }
}