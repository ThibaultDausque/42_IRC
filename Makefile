CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98
SRCS = srcs/main.cpp srcs/Command.cpp
RM = rm -f
OBJS_DIR = obj
OBJS = $(SRCS:%.cpp=$(OBJS_DIR)/%.o)

NAME = ircserv

all:$(NAME)

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o:%.cpp
	@mkdir -p $(@D)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJS_DIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re
