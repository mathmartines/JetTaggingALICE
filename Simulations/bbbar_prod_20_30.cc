#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC3.h"

using namespace Pythia8;

int main() {

    // Conversion from Pythia8::Event to HepMC event and
    // file where events are saved
    Pythia8ToHepMC toHepMC("/sampa/archive/caducka/jetsml/bbbar_prod_20_30.hepmc");

    // Generator, event configurations and pythia initialization
    Pythia pythia;
    pythia.readString("Random:setSeed = on");
    pythia.readString("Random:seed = 0");
    pythia.readString("Beams:eCM = 13000."); // CM Energy of 13 TeV
    pythia.readString("Tune:ee = 7"); // Unsing Monash 2013 Tune
    pythia.readString("HardQCD:hardbbbar = on"); // Events must produce b quarks
    pythia.readString("PDF:pSet = LHAPDF6/CT10nlo/4");
    pythia.readString("PhaseSpace:pTHatMin = 20.");
    pythia.readString("PhaseSpace:pTHatMax = 30.");
    if (!pythia.init()) return 1; //exit with error if fails to initialize

    // Number of events
    int nEvent = 500000;

    // Start event loop, generate event. Skip if error.
    for (int iEvent = 0; iEvent < nEvent; ++iEvent) {
        if (!pythia.next()) continue;

        toHepMC.writeNextEvent( pythia );
        
    }

    //Statistics
    pythia.stat();

    // Done
    return 0;
}
