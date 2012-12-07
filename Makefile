VPATH = ./Debug/:./Include/:./Source/

Object = ./Debug/Simulation.o ./Debug/OuterEncoder.o ./Debug/Interleaver.o ./Debug/InsertMarker.o ./Debug/Channel.o ./Debug/MAPDetector.o ./Debug/SumProductDecoder.o

SLIDC: $(Object) 
	gcc -o SLIDC -lm $(Object)

./Debug/Simulation.o: ./Source/Simulation.c ./Include/BasicDef.h ./Include/OuterEncoder.h
	gcc -c ./Source/Simulation.c -o ./Debug/Simulation.o
./Debug/OuterEncoder.o: ./Source/OuterEncoder.c ./Include/BasicDef.h ./Include/OuterEncoder.h
	gcc -c ./Source/OuterEncoder.c -o ./Debug/OuterEncoder.o
./Debug/Interleaver.o: ./Source/Interleaver.c ./Include/Interleaver.h
	gcc -c ./Source/Interleaver.c -o ./Debug/Interleaver.o
./Debug/InsertMarker.o: ./Source/InsertMarker.c ./Include/InsertMarker.h
	gcc -c ./Source/InsertMarker.c -o ./Debug/InsertMarker.o
./Debug/Channel.o: ./Source/Channel.c ./Include/Channel.h
	gcc -c ./Source/Channel.c -o ./Debug/Channel.o
./Debug/MAPDetector.o: ./Source/MAPDetector.c ./Include/MAPDetector.h
	gcc -c ./Source/MAPDetector.c -o ./Debug/MAPDetector.o
./Debug/SumProductDecoder.o: ./Source/SumProductDecoder.c ./Include/SumProductDecoder.h
	gcc -c ./Source/SumProductDecoder.c -o ./Debug/SumProductDecoder.o
clean:
	rm SLIDC $(Obejct)
