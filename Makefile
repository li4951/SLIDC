VPATH = ./Debug/:./Include/:./Source/

Object = ./Debug/Simulation.o ./Debug/OuterEncoder.o ./Debug/Interleaver.o ./Debug/InsertMarker.o ./Debug/Channel.o ./Debug/MAPDetector.o ./Debug/SumProductDecoder.o

SLIDC: $(Object) 
	gcc -o SLIDC -lm $(Object)

./Debug/Simulation.o: Simulation.h
./Debug/OuterEncoder.o: OuterEncoder.h
./Debug/Interleaver.o: Interleaver.h
./Debug/InsertMarker.o: InsertMarker.h
./Debug/Channel.o: Channel.h
./Debug/MAPDetector.o: MAPDetector.h
./Debug/SumProductDecoder.o: SumProductDecoder.h

clean:
	rm SLIDC $(Obejct)
