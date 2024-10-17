#include <iostream>
#include <vector>
#include <string>
#include "Analysis/ParticleSelector.h"
#include "Analysis/JetClustering.h"
#include "Analysis/EventAnalyzer.h"
#include "Analysis/Observable.h"
#include "Analysis/SignalParticlesSearcher.h"
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

    // define which particles must be selected in each event 
    // define the final charged particle selector
    const FinalStateSelector final_state_selector;
    const ChargedParticlesSelector charged_particles_selector;
    const vector<const ParticleSelector*> selectors = {&final_state_selector, &charged_particles_selector};
    const MultipleParticleSelectors final_part_selector (selectors);
    // define the selector for the initial particles
    const InitialStateSelector initial_part_selector;
    // define the selector for the outgoing particles from the hard process
    const OutgoingParticlesFromHardProcess hard_process_selector;

     // define the observable we want to compute
    const InvariantMass invariant_mass;

    // creates the EventAnalyzer object to select the particles
    EventAnalyzer event_analyzer;
    event_analyzer.addParticleSelector(ParticleType::FinalParticles, &final_part_selector);
    event_analyzer.addParticleSelector(ParticleType::InitialParticles, &initial_part_selector);
    event_analyzer.addParticleSelector(ParticleType::OutgoingHardProcessParticles, &hard_process_selector);

    // adds the observables
    event_analyzer.addObservable("invariantMass", &invariant_mass);

    // to select the particles from the hard process
    SignalParticlesSearcher signal_particles_searcher (&final_part_selector);

    // vector to store the final state particles and the jets
    vector<PseudoJet> jets;

    // defining how to cluster the jets
    JetClustering jet_cluster(0.4, 20, fastjet::antikt_algorithm);

    // looping over all the events in the file
    int evt_number = 0; // simple counter to keep track on the number of evts
    while (!hepmc_file.failed()) {
        evt_number++;   
        // current event
        hepmc_file.read_event(hepmc_event);
        // selecting the particles
        event_analyzer.analyseEvent(hepmc_event);

        // get the final state particles 
        const vector<ConstGenParticlePtr>& final_state_particles = event_analyzer.getParticles(ParticleType::FinalParticles);
        const vector<ConstGenParticlePtr>& hard_proc_particles = event_analyzer.getParticles(ParticleType::OutgoingHardProcessParticles);
        const vector<ConstGenParticlePtr>& final_particles_signal = signal_particles_searcher.selectParticles(hard_proc_particles);

        // get the energy and initial particle pid
        double q2 = event_analyzer.evaluateObservable("invariantMass", ParticleType::OutgoingHardProcessParticles);
        int initial_particle_pid =  event_analyzer.getParticles(ParticleType::InitialParticles).at(0)->abs_pid();

        // reconstructing the jets
        jets = jet_cluster.clusterJets(final_particles_signal);

        cout << "Event number " << evt_number << endl;
        cout << "Number of final state particles in the event: " << final_state_particles.size() << endl;
        cout << "Number of final state particles from the hard process in the event: " << final_particles_signal.size() << endl;
        cout << "Number of jets in the event: " << jets.size() << endl;
        cout << "q^2 = " << q2 << endl;
        cout << "initial PID: " << initial_particle_pid << endl;
        // cout << "Jets properties:" << endl;
        for(auto jet: final_particles_signal) {
            cout << "Jet pT = " << jet->momentum().pt() << endl;
            // cout << "Jet constituents:" << endl;
            // for (auto constituent: jet.constituents())
                // cout << "-- PID: " << constituent.user_info<HepMC3Info>().pid() << " pT: " << constituent.pt() << endl;
            // cout << "--------------------------------------" << endl;
        }
        cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
    }
    
    return 0;
}