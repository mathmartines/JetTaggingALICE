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


void runCSVWriter (string filename) {
    // cout << "analysing file " << filename << endl;

    // reading the HepMC3 file
    string hepmc3_filename = "/sampa/archive/caducka/jetsml/" + filename + ".hepmc";
    // my test
    // string hepmc3_filename = "/Users/martines/Desktop/Physics/pythia8312/examples/ccbar_production_pt_10_35_GeV.hepmc";
    ReaderAscii hepmc_file (hepmc3_filename);
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
    CSVWriter csvfile ("/sampa/archive/caducka/jetsml/" + filename + ".csv", 50);
    // CSVWriter csvfile ("/Users/martines/Desktop/Physics/pythia8312/examples/ccbar_production_pt_10_35_GeV.csv", 50);

    // looping over all the events in the file
    int evt_number = 0; // simple counter to keep track on the number of evts
    while (!hepmc_file.failed()) {
        // current event
        hepmc_file.read_event(hepmc_event);
        
        // If reading failed - exit loop
        if(hepmc_file.failed()) break;

        // selecting the particles
        event_analyzer.analyseEvent(hepmc_event);

        // final state particles 
        const vector<ConstGenParticlePtr>& final_state_particles = event_analyzer.getParticles(ParticleType::FinalParticles);
        // initial state particles
        const vector<ConstGenParticlePtr>& initial_particles = event_analyzer.getParticles(ParticleType::InitialParticles);
        
        // get the energy and initial particle pid
        double q2 = event_analyzer.evaluateObservable("invariantMass", ParticleType::OutgoingHardProcessParticles);
        int initial_particle_pid = initial_particles.at(0)->abs_pid();

        if (evt_number % 1000 == 0)
            cout << "Reached " << evt_number << " events" << endl;
        
        // writing event in the file
        csvfile.writeEvent(q2, initial_particle_pid, final_state_particles);

        evt_number++;   
    }
}

int main () {

    vector<string> filenames = {
        "bbbar_prod_40_60", "bbbar_prod_90_110", 
        "ccbar_prod_20_30", "ccbar_prod_40_60", "ccbar_prod_90_110", 
        "light_prod_20_30", "light_prod_40_60", "light_prod_90_110",
        "soft_prod_20_30", "soft_prod_40_60", "soft_prod_90_110"   
    };

    for (string filename: filenames)
        runCSVWriter(filename);        
    
    return 0;
}