CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98
SRCS = srcs/Command.cpp \
		srcs/User.cpp \
		srcs/mainDev.cpp \
	   	srcs/Server.cpp \
		srcs/Channel.cpp \
		srcs/join.cpp \
		srcs/names.cpp \
		srcs/nick.cpp \
		srcs/user.cpp \
		srcs/kick.cpp \
		srcs/privmsg.cpp
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
