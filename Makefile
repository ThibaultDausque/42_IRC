CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 -g
SRC	= srcs/User.cpp \
		srcs/main.cpp \
		srcs/Server.cpp \
		srcs/Channel.cpp \

SRC_COM = srcs/commands/Command.cpp \
		srcs/commands/join.cpp \
		srcs/commands/names.cpp \
		srcs/commands/invite.cpp \
		srcs/commands/kick.cpp \
		srcs/commands/names.cpp \
		srcs/commands/nick.cpp \
		srcs/commands/part.cpp \
		srcs/commands/privmsg.cpp \
		srcs/commands/user.cpp \
		srcs/commands/ping.cpp \
		srcs/commands/who.cpp

SRCS = $(wildcard srcs/**/*.cpp) $(wildcard srcs/*.cpp)
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
