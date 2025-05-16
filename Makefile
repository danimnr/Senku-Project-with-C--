# Compiler to use
CXX = g++

# Compiler flags (you can customize these)
CXXFLAGS = -Wall -Wextra -std=c++17 # Example: Enable all warnings and use C++17 standard

SRC = mainGSenku.cpp GSenku.cpp GSenku.hpp

# Name of the executable
EXECUTABLE = GSenku

main: mainGSenku.cpp GSenku.o
	$(CXX) mainGSenku.cpp GSenku.o -o main $(CXXFLAGS)

GSenku.o: GSenku.cpp GSenku.hpp
	$(CXX) GSenku.cpp -c $(CXXFLAGS) 



