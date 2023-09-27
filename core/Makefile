NAME			= lib$(notdir $(CURDIR)).a

INC_DIR			= lib
SRC_DIR			= src
OBJ_DIR			= obj

CC				= g++
CPPFLAGS			= -std=c++11 -pthread
#-std=c++98

SRC				= $(wildcard $(SRC_DIR)/*.cpp)
OBJ				= $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all				: $(NAME)

$(NAME)			: $(OBJ)
	ar -rcs $(NAME) $(OBJ)

$(OBJ_DIR)/%.o 	: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) -I $(INC_DIR) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)		:
	mkdir -p $(OBJ_DIR)

clean			:
	rm -rf $(OBJ_DIR)

fclean			: clean
	rm -f $(NAME)

re				: fclean all

.PHONY			: $(NAME)