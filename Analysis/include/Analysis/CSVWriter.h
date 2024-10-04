/**
 * @headerfile - writes the event information into a CSV file.
 **/

#ifndef CSVWriter_H
#define CSVWriter_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "HepMC3/GenParticle.h"
#include "HepMC3/FourVector.h"

class CSVWriter {

    public:
        CSVWriter(std::string filename, int number_particles): filename_(filename), max_number_particles(number_particles), output_file(filename){};
        ~CSVWriter() {output_file.close();};

        /// @brief - writes the event into the CSV file
        /// @param event_q2 - invariant mass of the event
        /// @param initial_part_pid - pid of the initial particle
        /// @param final_particles -  vector with the final particles in the event (assumed that it's already ordered by pT)
        void writeEvent(double event_q2, int initial_part_pid, std::vector<HepMC3::ConstGenParticlePtr> final_particles);

    private:
        /// @brief - name to give to the file
        std::string filename_;
        /// @brief - maximum number of particles to store in each event
        int max_number_particles;
        /// @brief - file to store the particles
        std::ofstream output_file;

        /// @brief - adds zero padded particles to the file
        /// @param numberZeroPaddedPart - number of zero padded particles to add
        void addZeroPaddedParticles(int numberZeroPaddedPart);
};


#endif 