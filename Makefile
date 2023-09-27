NAME			= 42api

INC_DIR			= .
SRC_DIR			= .

CC				= g++
CPPFLAGS			= -std=c++11 -pthread
#-std=c++98

SRC				= $(wildcard $(SRC_DIR)/*.cpp)

all				: $(NAME)

$(NAME)			:
	make -C core
	$(CC) -I $(INC_DIR) $(CPPFLAGS) $(SRC) -l core -L core -o $(NAME)

clean			:
	rm -f $(NAME)

fclean			: clean
	make fclean -C core

re				: fclean all

.PHONY			: $(NAME)