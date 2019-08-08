# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Create: 2014/05/18 14:42:57 by mdufaud            #+#    #+#              #
#    Updated: 2018/05/20 21:36:38 by mdufaud          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

OS := $(shell uname)
ifeq ($(OS),Linux)
SHELL := /bin/bash
else
SHELL := /bin/sh
endif

ifeq ($(OS),Linux)
ECHO = echo -e
else
ECHO = echo
endif

NAME = Durex

RED_COLOR = \033[31m
LIGHT_RED_COLOR = \033[1;31m
YELLOW_COLOR = \033[1;33m
BROWN_COLOR = \033[33m
GREEN_COLOR = \033[32m
LIGHT_GREEN_COLOR = \033[32;01m
CYAN_COLOR = \033[36m
LIGHT_CYAN_COLOR = \033[1;36m
PURPLE_COLOR = \033[35m
LIGHT_PURPLE_COLOR = \033[1;35m
BLUE_COLOR = \033[34m
LIGHT_BLUE_COLOR = \033[1;34m
LIGHT_GRAY_COLOR = \033[37m
DARK_GRAY_COLOR = \033[1;30m
BLACK_COLOR = \033[30m
WHITE_COLOR = \033[1;37m
END_COLOR = \033[00m

COMPILATOR = cc

CFLAGS = $(COMPILATOR) -Wall -Wextra -Werror

IFLAG_LIBFT = -I libft/includes
IFLAG_PRINTF = -I libft/libft_printf/includes

LFLAGS = -L libft -L libft/libft_printf -L ~/.brew/lib

LFT_FLAGS = -lft -lft_printf

OFLAGS = -I includes $(IFLAG_LIBFT) $(IFLAG_PRINTF) -I ~/.brew/include

LIB = libft/libft.a\
	  libft/libft_printf/libft_printf.a

MKLIB = make -C libft
MKLIBS = make -C libft lib

PATH_SRC = srcs
OBJ_PATH = obj
SRC = main.c\
	  infect.c\
	  start_trojan.c\
	  save_trojan.c\
	  stop_trojan.c\
	  startup_trojan.c\
	  remove_trojan_files.c\
	  daemon.c\
	  my_system.c\
	  my_kill.c\
	  log.c\
	  log_utils.c\
	  server_loop.c\
	  do_read.c\
	  do_write.c\
	  accept_client.c\
	  message_client.c\
	  remove_client.c\
	  cmd_parse.c\
	  check_pwd.c\
	  sha3.c\
	  cmd_shell.c\
	  cmd_log.c\
	  cmd_trojan.c\
	  cmd_server.c\
	  keylog.c\
	  keylog_pressed.c\
	  keylog_parse.c\
	  keylog_stop.c\
	  keylog_actions.c\
	  keylog_del.c

OBJ = $(patsubst %.c, $(OBJ_PATH)/%.o, $(SRC))

INCLUDES = includes/durex.h\
	  includes/sha3.h

all : $(NAME)

$(OBJ_PATH)/%.o : $(addprefix $(PATH_SRC)/, %.c) $(INCLUDES)
	@mkdir -p $(OBJ_PATH)
	@$(CFLAGS) $(OFLAGS) -o $@ -c $<

$(LIB):
	@$(MKLIB)
	@$(MKLIBS)

$(NAME) : $(LIB) $(OBJ) $(INCLUDES)
	@$(ECHO) "$(GREEN_COLOR)[ $(NAME) objects done ]$(END_COLOR)"
	@$(CFLAGS) -o $(NAME) $(LFLAGS) $(OBJ) $(LFT_FLAGS) $(LFT_FLAGS)
	@$(ECHO) "$(GREEN_COLOR)[ $(NAME) compiled ]$(END_COLOR)"

lib :
	@$(MKLIB)
	@$(MKLIBS)
	@$(ECHO) "$(GREEN_COLOR)[ Libraries compiled ]$(END_COLOR)"

clean :
	@$(ECHO) "$(YELLOW_COLOR)== Deleting libraries objects ==$(END_COLOR)"
	@$(MKLIB) clean
	@$(ECHO) "$(YELLOW_COLOR)== Done ==$(END_COLOR)"
	@rm -f $(OBJ)
	@$(ECHO) "$(BLUE_COLOR)[ $(NAME) objects deleted ]$(END_COLOR)"

fclean : clean
	@$(ECHO) "$(YELLOW_COLOR)== Deleting libraries ==$(END_COLOR)"
	@$(MKLIB) fclean
	@$(ECHO) "$(YELLOW_COLOR)== Done ==$(END_COLOR)"
	@rm -f $(NAME)
	@rm -df $(OBJ_PATH)
	@$(ECHO) "$(PURPLE_COLOR)[ $(NAME) deleted ]$(END_COLOR)"

re : fclean all

.PHONY : fclean re clean all lib
