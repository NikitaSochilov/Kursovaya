client:
	g++ main.cpp interface.cpp connection.cpp crypto.cpp -o main -lboost_program_options -lcryptopp
	./main -i input.bin -r result.txt -d data.txt -t 33333 -a 127.0.0.1
test:
	g++ UnitTest.cpp interface.cpp connection.cpp crypto.cpp -o UnitTest -lUnitTest++ -lboost_program_options -lcryptopp
	