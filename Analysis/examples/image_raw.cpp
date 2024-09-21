#include <iostream>
#include <vector>
#include <string>
#include "Analysis/ParticleSelector.h"
#include "Analysis/JetClustering.h"
#include "Analysis/EventAnalyzer.h"
#include "HepMC3/Reader.h"
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/GenEvent.h"

using namespace std;
using namespace HepMC3;


int main () {
    // reading the HepMC3 file
    string filename = "/home/jhoao/Jets_ML/bbbar_prod_20_30.hepmc"; //"/sampa/archive/caducka/jetsml/bbbar_prod_20_30.hepmc"; //particles: bbbar, ccbar, light, soft //energies:   20_30, 40_60, 90_110
    ReaderAscii hepmc_file (filename);
    
    // stores the current event 
    GenEvent hepmc_event(HepMC3::Units::GEV, HepMC3::Units::MM);

    // defining which particles must be selected in each event 
    const FinalStateSelector final_state_selection;
    const ChargedParticlesSelector charged_particle_selection;
    //const MultipleParticleSelectors full_selection();
    //full_selection.addSelector(charged_particle_selection);

    // creating the EventAnalyzer object to select the particles
    EventAnalyzer event_analyzer;
    event_analyzer.addParticleSelector(ParticleType::FinalParticles, &final_state_selection);
    event_analyzer.addParticleSelector(ParticleType::FinalParticles, &charged_particle_selection);

    // vector to store the final state charged particles
    vector<ConstGenParticlePtr> final_state_charged_particles; 

    // looping over all the events in the file
    int evt_number = 0; // simple counter to keep track on the number of evts
    while (!hepmc_file.failed()) {
        evt_number++;   
        // current event
        hepmc_file.read_event(hepmc_event);
        // selecting the particles
        event_analyzer.analyseEvent(hepmc_event);
        // getting the final state particles 
        final_state_charged_particles = event_analyzer.getParticles(ParticleType::FinalParticles);
        // storing final state particles properties (eta, phi, pT, PID)
        // ...

        cout << "Event number " << evt_number << endl;
        cout << "Number of final state particles in the event: " << final_state_charged_particles.size() << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
    }

    return 0;
}