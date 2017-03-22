
CPP := g++
CPPFLAGS := -g -std=c++11 # -Wall

#################################################################################
########### Ensure these paths are set correctly for your environment ###########
#################################################################################
TTHBBLEPTONICDIR := /users/jwebster/tth/2.3.39.2/TTHbbLeptonic/TTHbbLeptonic
DELPHESDIR := /users/jwebster/Delphes-3.3.0
MGDIR := /users/jwebster/MG5/MG5_aMC_v2_3_3
#################################################################################

MYINCS := -I$(PWD) -I$(PWD)/$(notdir $(shell pwd)) -I$(TTHBBLEPTONICDIR)
TTHBBLEPTONICEXTRAS := TopConfiguration/ConfigurationSettings.cxx $(TTHBBLEPTONICDIR)/Root/PairedSystem.cxx $(TTHBBLEPTONICDIR)/Root/MVAUtils.cxx $(TTHBBLEPTONICDIR)/Root/MVAVariables.cxx
TTHBBLEPTONICOBJ := bin/ConfigurationSettings.o bin/PairedSystem.o bin/MVAUtils.o bin/MVAVariables.o

ROOTLIBS := `root-config --libs` -lRooFitCore -lRooFit -lMinuit -lRooStats -lCore -lHistFactory
ROOTLIBFLAGS := `root-config --ldflags`
ROOTINCS := `root-config --cflags` 

DELPHESLIBS := -L$(DELPHESDIR) -lDelphes
DELPHESINCS := -I$(DELPHESDIR) -I$(MGDIR)/Template/NLO/MCatNLO/include

BUILD_OBJ := $(CPP) $(CPPFLAGS) $(MYINCS) $(ROOTINCS) $(DELPHESINCS)
LINK_OBJ := $(CPP) -g $(ROOTLIBS) $(DELPHESLIBS) $(MYINCS) $(ROOTINCS) $(DELPHESINCS)

SOURCES := $(wildcard src/*.cpp)

# Don't automatically delete the object files after compiling
# .PRECIOUS : bin/%.o

all : $(patsubst src/%.cpp, %, $(SOURCES))

# Linking
% : bin/%.o
	@rm -f $@
	$(LINK_OBJ) $^ $(TTHBBLEPTONICOBJ) -o run/$@
	@rm -f bin/*.o

# Objects
bin/%.o : src/%.cpp extras
	@rm -f $@
	$(BUILD_OBJ) -c $< -o $@

extras : $(TTHBBLEPTONICEXTRAS)
	$(BUILD_OBJ) -c TopConfiguration/ConfigurationSettings.cxx -o bin/ConfigurationSettings.o
	$(BUILD_OBJ) -c $(TTHBBLEPTONICDIR)/Root/PairedSystem.cxx -o bin/PairedSystem.o
	$(BUILD_OBJ) -c $(TTHBBLEPTONICDIR)/Root/MVAUtils.cxx -o bin/MVAUtils.o
	$(BUILD_OBJ) -c $(TTHBBLEPTONICDIR)/Root/MVAVariables.cxx -o bin/MVAVariables.o

clean : 
	rm -f bin/* run/* *~ */*~ *.o */*.gch log*



