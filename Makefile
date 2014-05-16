CPPC = g++
CPPFLAGS = -c -g -std=gnu++0x -fPIC
INCLUDE = -Iinclude
SELS = -Iinclude/selections
CROSS = -Iinclude/crossovers
MUTS = -Iinclude/mutations
GENS = -Iinclude/generations
ENDS = -Iinclude/endconditions
ALLINCLUDES = $(INCLUDE) $(SELS) $(CROSS) $(MUTS) $(GENS) $(ENDS)
STATICLIB = libs/libGAFramework.a
MAJORVERSION = 1
MINORVERSION = 0
LIBNAME = libGAFramework.so
DYNAMICLIB = $(LIBNAME).$(MAJORVERSION).$(MINORVERSION)

SHAREDLIB = -lGAFramework

info:
	@echo "Usage:"
	@echo "make examples: Produces a series of example GAs and HGAs using the framework"
	@echo "make library: Rebuilds the library from scratch"
	@echo "make core: Produces the object files for the \"core\" components"
	@echo "make selections: Produces the object files for the selection strategies"
	@echo "make mutations: Produces the object files for the mutation operators"
	@echo "make crossovers: Produces the object files for the crossover operators"
	@echo ""
	@echo "Any GA/HGA needs at least core, and one component from each of selections, mutations, and crossovers in order to function."

all: library

install:
	sudo cp libs/$(LIBNAME) /usr/lib
	sudo chmod 0755 /usr/lib/$(LIBNAME)
	sudo mkdir /usr/include/libGAFramework
	sudo cp -r include/* /usr/include/libGAFramework
	sudo ldconfig

uninstall:
	sudo rm /usr/lib/*libGAFramework*
	sudo rm -r /usr/include/*libGAFramework*
	sudo ldconfig

examples: install 1-max hier1-max royalroad hierroyalroad hier3royalroad

library: core selections mutations crossovers generations endconditions
	g++ -shared -o libs/$(LIBNAME) obj/*/*.o
	ar -cvq $(STATICLIB) obj/*/*.o

core:
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/core/CrossoverOperation.cpp -o obj/core/CrossoverOperation.o
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/core/MutationOperation.cpp -o obj/core/MutationOperation.o
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/core/Genome.cpp -o obj/core/Genome.o
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/core/Individual.cpp -o obj/core/Individual.o
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/core/GenePool.cpp -o obj/core/GenePool.o
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/core/NonHierarchicalGenePool.cpp -o obj/core/NonHierarchicalGenePool.o
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/core/SelectionStrategy.cpp -o obj/core/SelectionStrategy.o
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/core/GenerationModel.cpp -o obj/core/GenerationModel.o
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/core/EndCondition.cpp -o obj/core/EndCondition.o
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/core/HierarchicalGenePool.cpp -o obj/core/HierarchicalGenePool.o

selections: tournamentSelection

mutations: uniformMutation boundaryMutation

crossovers: nPointCrossover uniformCrossover cutAndSpliceCrossover

generations: ga es ssga

endconditions: fitnessMatch

ga:
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/generations/GAGeneration.cpp -o obj/generations/GAGeneration.o

es:
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/generations/ESGeneration.cpp -o obj/generations/ESGeneration.o

ssga:
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/generations/SSGAGeneration.cpp -o obj/generations/SSGAGeneration.o

tournamentSelection:
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/selections/TournamentSelection.cpp -o obj/selections/TournamentSelection.o

uniformMutation:
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/mutations/UniformMutation.cpp -o obj/mutations/UniformMutation.o

boundaryMutation:
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/mutations/BoundaryMutation.cpp -o obj/mutations/BoundaryMutation.o

nPointCrossover:
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/crossovers/NPointCrossover.cpp -o obj/crossovers/NPointCrossover.o

uniformCrossover:
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/crossovers/UniformCrossover.cpp -o obj/crossovers/UniformCrossovers.o

cutAndSpliceCrossover:
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/crossovers/CutAndSpliceCrossover.cpp -o obj/crossovers/CutAndSpliceCrossover.o

fitnessMatch:
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/endconditions/FitnessMatchEnd.cpp -o obj/endconditions/FitnessMatchEnd.o

1-max:
	$(CPPC) $(CPPFLAGS) -IlibGAFramework src/examples/1max/1maxFitness.cpp -o obj/examples/1max/1maxFitness.o
	$(CPPC) $(CPPFLAGS) -Isrc/examples/1max src/examples/1max/1max.cpp -o obj/examples/1max/1max.o
	$(CPPC) -o examples/1max obj/examples/1max/*.o $(SHAREDLIB)

hier1-max:
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/examples/Hier1max/1maxFitness.cpp -o obj/examples/Hier1max/1maxFitness.o
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/examples/Hier1max/Hier1maxFitness.cpp -o obj/examples/Hier1max/Hier1maxFitness.o
	$(CPPC) $(CPPFLAGS) $(ALLINCLUDES) -Isrc/examples/Hier1max src/examples/Hier1max/Hier1max.cpp -o obj/examples/Hier1max/Hier1max.o
	$(CPPC) -o examples/Hier1max obj/examples/Hier1max/*.o $(STATICLIB)

royalroad:
	$(CPPC) $(CPPFLAGS) $(INCLUDE) src/examples/RoyalRoad/RoyalRoadFitness.cpp -o obj/examples/RoyalRoad/RoyalRoadFitness.o
	$(CPPC) $(CPPFLAGS) $(ALLINCLUDES) -Isrc/examples/RoyalRoad src/examples/RoyalRoad/RoyalRoad.cpp -o obj/examples/RoyalRoad/RoyalRoad.o
	$(CPPC) -o examples/RoyalRoad obj/examples/RoyalRoad/*.o $(STATICLIB)

hierroyalroad:
	$(CPPC) $(CPPFLAGS) $(INCLUDE) -Isrc/examples/HierRoyalRoad src/examples/HierRoyalRoad/RoyalRoadFitness.cpp -o obj/examples/HierRoyalRoad/RoyalRoadFitness.o
	$(CPPC) $(CPPFLAGS) $(INCLUDE) -Isrc/examples/HierRoyalRoad src/examples/HierRoyalRoad/HierRoyalRoadFitness.cpp -o obj/examples/HierRoyalRoad/HierRoyalRoadFitness.o
	$(CPPC) $(CPPFLAGS) $(ALLINCLUDES) -Isrc/examples/HierRoyalRoad src/examples/HierRoyalRoad/HierRoyalRoad.cpp -o obj/examples/HierRoyalRoad/HierRoyalRoad.o
	$(CPPC) -o examples/HierRoyalRoad obj/examples/HierRoyalRoad/*.o $(STATICLIB)

hier3royalroad:
	$(CPPC) $(CPPFLAGS) $(INCLUDE) -Isrc/examples/Hier3RoyalRoad src/examples/Hier3RoyalRoad/RoyalRoadFitness.cpp -o obj/examples/Hier3RoyalRoad/RoyalRoadFitness.o
	$(CPPC) $(CPPFLAGS) $(INCLUDE) -Isrc/examples/Hier3RoyalRoad src/examples/Hier3RoyalRoad/HierRoyalRoadFitness.cpp -o obj/examples/Hier3RoyalRoad/HierRoyalRoadFitness.o
	$(CPPC) $(CPPFLAGS) $(ALLINCLUDES) -Isrc/examples/Hier3RoyalRoad src/examples/Hier3RoyalRoad/HierRoyalRoad.cpp -o obj/examples/Hier3RoyalRoad/HierRoyalRoad.o
	$(CPPC) -o examples/Hier3RoyalRoad obj/examples/Hier3RoyalRoad/*.o $(STATICLIB)


clean:
	find obj -name *.o | xargs rm -f
	rm -f libs/*
	cd examples; ls | xargs rm -f
	rm -f /usr/libs/libGAFramework.so*
