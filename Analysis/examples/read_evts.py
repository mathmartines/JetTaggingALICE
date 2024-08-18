import pyHepMC3.pyHepMC3.HepMC3 as hepmc3

if __name__ == "__main__":
    # path to the file
    filename = "/Users/martines/Desktop/Physics/pythia8312/examples/ccbar_production_pt_10_35_GeV.hepmc"

    events = hepmc3.ReaderAscii(filename)
    event_reader = hepmc3.GenEvent(hepmc3.Units.GEV)
    nevts = 0
    while not events.failed():
        events.read_event(event_reader)
        print("------------")
        for particle in event_reader.particles():
            if particle.status() == 1:
                print(particle.isFinal())

        nevts += 1
    print(nevts)