# ##############################################################################
#
#              """          Makefile
#       -\-    _|__
#        |\___/  . \        Created on 29 Jan. 2024 at 16:27
#        \     /(((/        by hmelica
#         \___/)))/         hmelica@student.42.fr
#
# ##############################################################################

###################
##  MAKE CONFIG  ##
###################

.ONESHELL:
.DELETE_ON_ERROR:
SHELL			:= sh
MAKEFLAGS		+= --no-builtin-rules
MAKEFLAGS		+= --no-print-directory
MAKEFLAGS		+= -j4
.RECIPEPREFIX	=

###################
##  PRINT COLOR  ##
###################

RESET		= \033[0m
BLACK		= \033[0;30m
RED			= \033[0;31m
GREEN		= \033[0;32m
YELLOW		= \033[0;33m
BLUE		= \033[0;34m
MAGENTA		= \033[0;35m
CYAN		= \033[0;36m
GRAY		= \033[0;37m
DELETE		= \033[2K\r
GO_UP		= \033[1A\r

###################
##  PROJECT VAR  ##
###################

NAME			= webserv

LIBRARIES		= -lreadline

CC				= c++
CFLAGS			= -MMD -Wall -Werror -Wextra -std=c++98
DEBUG_FLAG		= -g3
SANITIZE_FLAG	= -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer

HEADER_DIR		= header src
SRCS_DIR		= src
OBJS_DIR		= obj

########################
##  AUTO-EDIT ON VAR  ##
########################

HEADER_DIR	= ${addprefix -I ,${HEADERS_DIR}}
SRCS		= ${shell find src/ -type f -name "*.cpp"}
OBJS		= ${patsubst ${SRCS_DIR}%.cpp,${OBJS_DIR}%.o,${SRCS}}
DEPS		= ${patsubst ${SRCS_DIR}%.cpp,${OBJS_DIR}%.d,${SRCS}}

CFLAGS			:= ${CFLAGS} $(if $(filter ${MAKECMDGOALS}, sanitize),${SANITIZE_FLAG},)
CFLAGS			:= ${CFLAGS} $(if $(filter ${MAKECMDGOALS}, debug test),${DEBUG_FLAG},) ${TEST_MODE}
DEBUG_PROMPT		= ${MAGENTA}debug mode${RESET}
SANITIZE_PROMPT		= ${YELLOW} sanitize mode${RESET}
OK_PROMPT			= ${GREEN}done ${RESET}
OK_PROMPT			+= ${if ${filter ${MAKECMDGOALS}, debug}, ${DEBUG_PROMPT}, }
OK_PROMPT			+= ${if ${filter ${MAKECMDGOALS}, sanitize}, ${SANITIZE_PROMPT}, }

#######################
##  USUAL FUNCTIONS  ##
#######################

RM		= rm -f
RMDIR	= rm -df
MKDIR	= mkdir


all: ${NAME} # Default rule
	@printf "${GREEN}Success${RESET}  :)\n"

-include ${DEPS}

${NAME}: ${OBJS_DIR} ${OBJS} # Compile ${NAME} program
	@printf "${DELETE}${YELLOW}...Building${RESET} %-33s" "${NAME}"
	@${CC} ${CFLAGS} ${HEADERS} -o ${NAME} ${OBJS} ${LIBRARIES}
	@printf "${OK_PROMPT}\n"

${OBJS_DIR}:
	@printf "${BLUE}...Creating${RESET} %-33s" "${OBJS_DIR} directory"
	@${MKDIR} ${OBJS_DIR}
	@printf "${GREEN}done${RESET}\n"

${OBJS_DIR}/%.o: ${SRCS_DIR}/%.cpp
	@printf "${DELETE}${BLUE}Compiling${RESET} %-35s" $<
	@${CC} ${CFLAGS} ${HEADERS_DIR_FLAG} -c $< -o $@
	@printf "${OK_PROMPT}\n${GO_UP}"

clean: # Clean object files
	@${RM} ${OBJS} ${DEPS}
	@${RMDIR} ${OBJS_DIR}
	@printf "${BLUE}%-44s${RESET} ${GREEN}%s${RESET}\n" "Cleaning" "done"

fclean: clean # Clean executable file
	@${RM} ${NAME}
	@printf "${BLUE}%-44s${RESET} ${GREEN}%s${RESET}\n" "File cleaning" "done"

re: fclean all # Execute fclean & all rules

debug: fclean all

sanitize: fclean all

./header:
	mkdir header

./googletest/build:
	git submodule sync
	git submodule update
	cd googletest && mkdir build && cd build && cmake .. && make

./header/libgtest.a: ./googletest/build ./header
	ln -f ./googletest/build/lib/* header/

test: ./header/libgtest.a
	@echo "Tests not implemented yet..."

# supprime les fichiers dupliqués sur mac
mac_clean:
	@find . -type f -name "* [2-9]*" -print -delete

file: # Print list of source and object files
	@printf "${MAGENTA}.cpp:		${GRAY}${SOURCES}${RESET}\n"
	@printf "${MAGENTA}.o:		${GRAY}${OBJECTS}${RESET}\n"

help:
	@egrep -h '\s#\s' ${MAKEFILE_LIST} | sort | awk 'BEGIN {FS = ":.*?# "}; {printf "${MAGENTA}%-15s${GRAY} %s${RESET}\n", $$1, $$2}'

.PHONY: all clean fclean re debug sanitize tags file help mac_clean test
