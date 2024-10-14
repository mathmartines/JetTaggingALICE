#include "Analysis/JetClustering.h"


std::vector<fastjet::PseudoJet> JetClustering::convertParticlesToPseudoJets(const std::vector<HepMC3::ConstGenParticlePtr> &particles) const {
    /// vector a vector with the same number of entries as the vector o hepmc3 particles
    std::vector<fastjet::PseudoJet> final_pseudo_jets (particles.size(), fastjet::PseudoJet());

    /// transforming the hepmc3 particles to pseudo jets
    for (int i = 0; i < particles.size(); i++) {
        const HepMC3::FourVector momentum = particles[i]->momentum();
        /// updating the jet momentum
        final_pseudo_jets[i].reset_momentum(momentum.px(), momentum.py(), momentum.pz(), momentum.e());
        /// including the hepmc information as user info
        final_pseudo_jets[i].set_user_info(new HepMC3Info(particles[i]->pid()));
    }

    return final_pseudo_jets;
}

std::vector<fastjet::PseudoJet> JetClustering::clusterJets (const std::vector<HepMC3::ConstGenParticlePtr>& particles)  {
    /// defining the fastjet cluster sequence to cluster the jets - reconstructing the jets
    _cluster_seq = fastjet::ClusterSequence(convertParticlesToPseudoJets(particles), _jet_definition);
    /// returning the jets with pT greater the min pt and sorted by pt
    return fastjet::sorted_by_pt(_cluster_seq.inclusive_jets(_min_pt));
}