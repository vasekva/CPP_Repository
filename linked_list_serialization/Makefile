NAME = run

INCLUDES = -I ./includes/

CXX = clang++
#CXXFLAGS = -g -Wall -Wextra -Werror -std=c++14 $(INCLUDES)
CXXFLAGS = -g -std=c++14 $(INCLUDES)

SRCS = main.cpp Serializator.cpp Deserializator.cpp

all: $(SRCS)
	@($(CXX) $(CXXFLAGS) $(SRCS) -o $(NAME))

re: clean all

clean:
	rm -rf run.dSYM
	rm -rf $(NAME)
	rm -rf some_file.txt