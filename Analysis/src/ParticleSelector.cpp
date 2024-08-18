#include "Analysis/ParticleSelector.h"


bool MultipleParticleSelectors::selectParticle(HepMC3::ConstGenParticlePtr particle) const {
    /// only selects the particle if it passes all selectors
    for (const ParticleSelector* selector: _part_selectors) {
        if (!selector->selectParticle(particle))
            return false;
    }
    return true;
}

bool ChargedParticlesSelector::selectParticle(HepMC3::ConstGenParticlePtr particle) const {
    /// checks if the particle pid is in the list of allowed charged particles
    return _charged_part_pids.find(particle->abs_pid()) != _charged_part_pids.end();
}