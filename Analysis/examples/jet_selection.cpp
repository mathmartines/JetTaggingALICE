#include <iostream>
#include <vector>
#include <string>
#include "Analysis/ParticleSelector.h"
#include "Analysis/JetClustering.h"
#include "Analysis/EventAnalyzer.h"
#include "fastjet/ClusterSequence.hh"
#include "HepMC3/Reader.h"
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/GenEvent.h"

using namespace std;
using namespace fastjet;
using namespace HepMC3;


int main () {
    // reading the HepMC3 file
    string filename = "/Users/martines/Desktop/Physics/pythia8312/examples/ccbar_production_pt_10_35_GeV.hepmc";
    ReaderAscii hepmc_file (filename);
    
    // stores the current event 
    GenEvent hepmc_event(HepMC3::Units::GEV, HepMC3::Units::MM);

    // defining which particles must be selected in each event 
    const FinalStateSelector final_state_selection;

    // creating the EventAnalyzer object to select the particles
    EventAnalyzer event_analyzer;
    event_analyzer.addParticleSelector(ParticleType::FinalParticles, &final_state_selection);

    // vector to store the final state particles and the jets
    vector<ConstGenParticlePtr> final_state_particles; 
    vector<PseudoJet> jets;

    // defining how to cluster the jets
    JetClustering jet_cluster(0.4, 10, fastjet::antikt_algorithm);

    // looping over all the events in the file
    int evt_number = 0; // simple counter to keep track on the number of evts
    while (!hepmc_file.failed()) {
        evt_number++;   
        // current event
        hepmc_file.read_event(hepmc_event);
        // selecting the particles
        event_analyzer.analyseEvent(hepmc_event);
        // getting the final state particles 
        final_state_particles = event_analyzer.getParticles(ParticleType::FinalParticles);
        // reconstructing the jets
        jets = jet_cluster.clusterJets(final_state_particles);

        cout << "Event number " << evt_number << endl;
        cout << "Number of final state particles in the event: " << final_state_particles.size() << endl;
        cout << "Number of jets in the event: " << jets.size() << endl;
        cout << "Jets properties:" << endl;
        for(auto jet: jets) {
            cout << "Jet pT = " << jet.pt() << ", eta = " << jet.eta() << endl;
            cout << "Jet constituents:" << endl;
            for (auto constituent: jet.constituents())
                cout << "-- PID: " << constituent.user_info<HepMC3Info>().pid() << " pT: " << constituent.pt() << endl;
            cout << "--------------------------------------" << endl;
        }
        cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
    }

    return 0;
}