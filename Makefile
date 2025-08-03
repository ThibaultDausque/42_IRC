CC = c++
FLAGS = -Wall -Wextra -std=c++98 -g

SRCS_MAIN =  $(addprefix srcs/, main.cpp User.cpp Server.cpp Channel.cpp)
SRCS_COMMANDS = $(addprefix srcs/commands/, command_utils.cpp join.cpp topic.cpp names.cpp invite.cpp kick.cpp nick.cpp part.cpp privmsg.cpp user.cpp ping.cpp who.cpp pass.cpp mode.cpp quit.cpp)
SRCS = $(SRCS_MAIN) $(SRCS_COMMANDS)

RM = rm -f
OBJS_DIR = obj
INCLUDES = -Iincludes -Isrcs
OBJS = $(SRCS:%.cpp=$(OBJS_DIR)/%.o)

NAME = ircserv

all:$(NAME)

$(NAME): $(OBJS)
	@$(CC) $(INCLUDES) $(FLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(INCLUDES) $(FLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJS_DIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re
