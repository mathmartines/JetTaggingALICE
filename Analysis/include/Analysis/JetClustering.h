/**
 * @headerfile - definition of the jet clustering class.
 *               This class is responsible to transform HepMC3::GenParticles
 *               to fastjet::PseudoJets and cluster the final state jets.
 **/

#ifndef JET_CLUSTERING_H
#define JET_CLUSTERING_H

#include <vector>
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "HepMC3/GenParticle.h"
#include "fastjet/ClusterSequenceArea.hh"
#include "HepMC3/FourVector.h"


/**
 * @class - Subclassing UserInfo class to store the particle
 *          information from the HepMC3::GenParticle class.  
 **/
class HepMC3Info: public fastjet::PseudoJet::UserInfoBase {

    public:
        /// @brief - we can store the pid number and the respective eletric charge from the particle 
        HepMC3Info (const int pid): _pid(pid){};
        
        /// access to the pid number and eletric charge
        const int pid() const {return _pid;};

    private:
        const int _pid;
};


/**
 * @class - reconstruct the jets out of the HepMC3::GenParticles
 **/
class JetClustering {
    
    public:
        JetClustering(double jet_radius, double min_pt, fastjet::JetAlgorithm jet_algorithm):  _min_pt(min_pt), _jet_definition(jet_algorithm, jet_radius) {};

        /// @brief - performs the jet reconstruction out of the HepMC3 particles
        /// @param particles - vector with the particles that must be used for the jet reconstruction
        std::vector<fastjet::PseudoJet> clusterJets (const std::vector<HepMC3::ConstGenParticlePtr>& particles);


    private:
        /// @brief - the minimum jet pt
        double _min_pt;

        /// @brief - the jet definition will be the same for every jet
        fastjet::JetDefinition _jet_definition;
        fastjet::ClusterSequence _cluster_seq;

        /// @brief - Converts the HepMC3_Particles to a vector of PseudoJet objects
        std::vector<fastjet::PseudoJet> convertParticlesToPseudoJets(const std::vector<HepMC3::ConstGenParticlePtr> &particles) const;
};

#endif