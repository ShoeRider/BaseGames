# -*- MakeFile -*-

OPENCV_INCLUDEPATH=/usr/include



all: UTTT_Tests
	#nvcc -o ./MCTS ./SRC/MCTS.cu
	#//./MCTS
#make -B



#referenced:https://stackoverflow.com/questions/27204423/error-when-trying-link-jsoncpp-and-include-it-in-a-cuda-project-undefined-refer
#Compile and run tests within ./Tests Folder
TTT_Tests:
	@echo "Compiling(TTT_Tests) ..."

	nvcc -o TTT_Tests ./Tests/TTT_Tests.cu -ljsoncpp
	#gcc -std=c++11 ./Tests/TTT_Tests.cpp ./../ExternalLibraries/simdjson/singleheader/simdjson.cpp -lstdc++ -o ./TTT_Tests
	@echo "Running(Tests) ..."
	valgrind --leak-check=full --show-leak-kinds=all ./TTT_Tests
	#./MCTS_Tests
#gcc -std=c++11 ./Tests/TTT_Tests.cpp -lstdc++ -o ./TTT_Tests

#Compile and run tests within ./Tests Folder
UTTT_Tests:
	@echo "Compiling(UTTT_Tests) ..."

	nvcc -o UTTT_Tests ./Tests/UTTT_Tests.cu -ljsoncpp
	#gcc -std=c++11 ./Tests/TTT_Tests.cpp ./../ExternalLibraries/simdjson/singleheader/simdjson.cpp -lstdc++ -o ./TTT_Tests
	@echo "Running(Tests) ..."
	valgrind --leak-check=yes ./UTTT_Tests
	#./MCTS_Tests
#gcc -std=c++11 ./Tests/TTT_Tests.cpp -lstdc++ -o ./TTT_Tests




Test_UTTT:
	@echo "Compiling(Tests) ..."
	nvcc ./Tests/UTTT_Tests.cu -o ./UTTT_Tests
	#gcc -std=c++11 ./Tests/UTTT_Tests.cpp -lstdc++ -o ./UTTT_Tests
	@echo "Running(Tests) ..."
	valgrind --leak-check=yes ./UTTT_Tests

.PHONY: clean $(SUBDIRS)
clean :
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
	rm -f *.o *.gch
