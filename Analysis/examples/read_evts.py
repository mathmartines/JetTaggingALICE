import pyHepMC3.pyHepMC3.HepMC3 as hepmc3


class SignalParticlesFinder:
    """Selects the final state particles originated from the hard process using recursion."""

    def __init__(self):
        # set to store the final state particles
        self._final_particles = set()
        # stores particles from the hard process
        self._hard_process_particles = []
        # stores the vertices that have already been taken into acount
        self._vertices = set()

    def get_final_particles(self, event):
        """Returns all the final state particles"""
        # clear all vectors and sets
        self._final_particles.clear()
        self._vertices.clear()
        self._hard_process_particles.clear()

        # fiding the particles from the hard process
        for particle in event.particles():
            if particle.status() == 23:
                self._hard_process_particles.append(particle)

        # searching for the final state particles from the two particles out of the hard process
        for hard_proc_particles in self._hard_process_particles:
            self.search_final_state_particles(hard_proc_particles)

        return self._final_particles

    def search_final_state_particles(self, incomming_particle):
        """Recursively searches if the final state particles"""
        # bae case: particle is a final state particle
        if incomming_particle.status() == 1:
            self._final_particles.add(incomming_particle)
        # check if the final vertex of the incomming particle have been taken into account
        elif incomming_particle.end_vertex() not in self._vertices:
            # add the vertex in the list of vertices we already looked up
            self._vertices.add(incomming_particle.end_vertex())
            # look at every outgoing particle from the end vertex
            for outgoing_particle in incomming_particle.end_vertex().particles_out():
                self.search_final_state_particles(outgoing_particle)


if __name__ == "__main__":
    # path to the file
    filename = "/Users/martines/Desktop/Physics/pythia8312/examples/ccbar_production_pt_10_35_GeV.hepmc"

    # look at only one event
    events = hepmc3.ReaderAscii(filename)
    event_reader = hepmc3.GenEvent(hepmc3.Units.GEV)
    events.read_event(event_reader)

    # find all the final state particles from the hard process
    final_particles_finder = SignalParticlesFinder()
    final_particles_from_hard_proc = final_particles_finder.get_final_particles(event_reader)

    # counting the number of final state particles
    total_final_st_particles = sum([p.status() == 1 for p in event_reader.particles()])

    print(f"Total number of final state particles: {total_final_st_particles}")
    print(f"Number of final state particles from hard process: {len(final_particles_from_hard_proc)}")
