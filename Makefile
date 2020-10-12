CXX = clang++
CXXFLAGS = 

.PHONY: clean

%.o: %.cc
	$(CXX) -c $(CXXFLAGS) $< -o $@

tsk: tsk.o
	$(CXX) *.o -o $@

run: tsk
	./tsk

clean:
	rm -f *.o tsk
