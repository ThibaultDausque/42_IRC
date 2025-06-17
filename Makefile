CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98
SRCS = srcs/main.cpp \
		srcs/Command.cpp \
		srcs/Client.cpp \
		srcs/main.cpp \
	   	srcs/Server.cpp 
RM = rm -f
OBJS_DIR = obj
INCLUDES = -Iincludes -Isrcs
OBJS = $(SRCS:%.cpp=$(OBJS_DIR)/%.o)

NAME = ircserv

all:$(NAME)

$(NAME): $(OBJS)
	@$(CC) $(INCLUDES) $(FLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o:%.cpp
	@mkdir -p $(@D)
	$(CC) $(INCLUDES) $(FLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJS_DIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re
