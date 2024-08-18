CXX = g++
ROOT = /Users/martines/Desktop/Physics
CPPFLAGS = -I$(ROOT)/fastjet-install/include -I$(ROOT)/hepmc3-install/include -Iinclude
CXXFLAGS = -Wall -O2 -std=c++17
LDFLAGS = -L$(ROOT)/fastjet-install/lib -L$(ROOT)/hepmc3-install/lib -Wl,-rpath,$(ROOT)/hepmc3-install/lib -Wl,-rpath,$(ROOT)/fastjet-install/lib
LIBS = -lHepMC3 -lfastjet -lfastjettools

IDIR = include/Analysis
ODIR = lib

_DEPS = ParticleSelector JetClustering EventAnalyzer
DEPS = $(patsubst %, $(IDIR)/%.h, $(_DEPS)) 
OBJ = $(patsubst %, $(ODIR)/%.o, $(_DEPS))

# Ensure that the output directory exists
$(ODIR):
	mkdir -p $(ODIR)

# Compile object files from the source files
$(ODIR)/%.o: src/%.cpp $(IDIR)/%.h | $(ODIR)
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

# Main target to build
jet_selection: jet_selection.o $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

# Compile the main file
jet_selection.o: examples/jet_selection.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

# Clean up the compiled files
clean:
	rm -rf $(ODIR)/*.o 
	rm -rf jet_selection
	rm -rf jet_selection.o

# Phony targets
.PHONY: clean
