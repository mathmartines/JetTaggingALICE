#include "Analysis/CSVWriter.h"

void CSVWriter::writeEvent (double event_q2, int initial_part_pid, std::vector<HepMC3::ConstGenParticlePtr> final_particles) {
    if (output_file.is_open()) {
        // adding the information about the energy of the process and pid of the incomming particles
        output_file << event_q2 << "," << initial_part_pid;
        // adding the information about the particles
        int part_counter = 0;
        // checks if we have at least 50 particles and
        // checks if there's particle left in the vector
        while (part_counter < max_number_particles && part_counter < final_particles.size()) {
            // writing the info 
            const HepMC3::FourVector& momentum = final_particles.at(part_counter)->momentum();
            // write particle info in the csv file
            output_file << "," << momentum.pt()  << "," << momentum.eta() << "," << momentum.phi() << "," << final_particles.at(part_counter)->pid();
            part_counter++;
        }
        if (part_counter < max_number_particles)
            this->addZeroPaddedParticles(max_number_particles - part_counter);

        // break line
        output_file << "\n";
    }
    else
        std::cout << "File not open" << std::endl;
}

void CSVWriter::addZeroPaddedParticles(int numberZeroPaddedPart) {
    for(int i = 0; i < numberZeroPaddedPart * 4; i++) 
        output_file << ",0";
}
