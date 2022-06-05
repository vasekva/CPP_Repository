NAME = run

INCLUDES = -I ./includes

CXX = clang++
CXXFLAGS = -g -Wall -Wextra -Werror -std=c++14 $(INCLUDES)
#CXXFLAGS = -g -std=c++14

SRCS = main.cpp Serializator.cpp

F_NONE		= \033[37m
F_BOLD		= \033[1m
F_RED		= \033[31m
F_ORANGE	= \033[38m
F_YELLOW	= \033[0;33m
F_GREEN		= \033[32m
F_CYAN		= \033[36m
F_BLUE		= \033[34m

all: $(SRCS)
	@($(CXX) $(CXXFLAGS) $(SRCS) -o $(NAME))

re: clean all

clean:
	rm -rf run.dSYM
	rm -rf $(NAME)