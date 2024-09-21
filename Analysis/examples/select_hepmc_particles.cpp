#include <iostream>
#include <vector>
#include <string>
#include "Analysis/ParticleSelector.h"
#include "Analysis/Observable.h"
#include "Analysis/EventAnalyzer.h"
#include "Analysis/CSVWriter.h"
#include "HepMC3/Reader.h"
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/FourVector.h"

using namespace std;
using namespace HepMC3;


int main () {
    // reading the HepMC3 file
    string filename = "/sampa/archive/caducka/jetsml/bbbar_prod_20_30.hepmc";
    // my test
    // string filename = "/Users/martines/Desktop/Physics/pythia8312/examples/ccbar_production_pt_10_35_GeV.hepmc";
    ReaderAscii hepmc_file (filename);
    // stores the current event 
    GenEvent hepmc_event(HepMC3::Units::GEV, HepMC3::Units::MM);

    // defining which particles must be selected in each event 
    // selects final state particles 
    const FinalStateSelector final_state_selector;
    // selects charged particles
    const ChargedParticlesSelector charged_particle_selector;
    // selectors that a final state particle must fullfiled
    vector<const ParticleSelector*> particle_selectors = {&final_state_selector, &charged_particle_selector};
    const MultipleParticleSelectors hepmc_particle_selector(particle_selectors);
    // selects initial state particles
    const InitialStateSelector initial_particle_selector;
    // selects the particles generated from the hard process
    const OutgoingParticlesFromHardProcess final_hard_process_particles;

    // define the observable we want to compute
    const InvariantMass invariant_mass;

    // creating the EventAnalyzer object to select the particles
    EventAnalyzer event_analyzer;

    // adding the selectors we want
    event_analyzer.addParticleSelector(ParticleType::FinalParticles, &hepmc_particle_selector);
    event_analyzer.addParticleSelector(ParticleType::InitialParticles, &initial_particle_selector);
    event_analyzer.addParticleSelector(ParticleType::OutgoingHardProcessParticles, &final_hard_process_particles);

    // adding the observables
    event_analyzer.addObservable("invariantMass", &invariant_mass);

    // creating the CSV file
    CSVWriter csvfile ("test.csv", 50);

    // looping over all the events in the file
    int evt_number = 0; // simple counter to keep track on the number of evts
    while (!hepmc_file.failed()) {
        // current event
        hepmc_file.read_event(hepmc_event);
        
        // If reading failed - exit loop
        if(hepmc_file.failed() ) break;

        // selecting the particles
        event_analyzer.analyseEvent(hepmc_event);

        // final state particles 
        const vector<ConstGenParticlePtr>& final_state_particles = event_analyzer.getParticles(ParticleType::FinalParticles);
        // initial state particles
        const vector<ConstGenParticlePtr>& initial_particles = event_analyzer.getParticles(ParticleType::InitialParticles);
        
        // get the energy and initial particle pid
        double q2 = event_analyzer.evaluateObservable("invariantMass", ParticleType::OutgoingHardProcessParticles);
        int initial_particle_pid = initial_particles.at(0)->abs_pid();

        cout << "----------------------------" << endl;
        cout << "Event number " << evt_number << endl;
        cout << "Number of selected particles: " << final_state_particles.size() << endl;
        cout << "PID of the initial particle: " << initial_particle_pid << endl;
        cout << "Invariant mass of the process: " << q2 << endl;

        // writing event in the file
        csvfile.writeEvent(q2, initial_particle_pid, final_state_particles);

        evt_number++;   
    }
    
    return 0;
}