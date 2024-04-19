# ##############################################################################
#
#              """          Makefile
#       -\-    _|__
#        |\___/  . \        Created on 29 Jan. 2024 at 16:27
#        \     /(((/        by hmelica
#         \___/)))/         hmelica@student.42.fr
#
# ##############################################################################
#    ____                _
#   |  _ \ ___  __ _  __| |_ __ ___   ___
#   | |_) / _ \/ _` |/ _` | '_ ` _ \ / _ \                   :)
#   |  _ <  __/ (_| | (_| | | | | | |  __/
#   |_| \_\___|\__,_|\__,_|_| |_| |_|\___|
#
# How to use :
# ----------
#   * all
#   * re
#   * clean
#   * fclean
#   * norm : like norminette but really fast to check all files
#   * debug : like all but with -g flag, can be used with bonus too
#   * sanitize : use this as a parameter to compile executable with fsanitize
#
# Stuff to edit :
# -------------
NAME		= webserv

CFLAGS	= -MMD -Wall -Werror -Wextra -std=c++98
# Other useful flags : -O3 -Wno-unused-result
#                           ^- flag de compilation sur mac (Apple Clang)
#                        ^- flag d'optimisation maximum (peut entrainer des
#                           comportements indéfinis)
DEBUG_FLAG		= -g3
SANITIZE_FLAG	= -fsanitize=address

# **************************************************************************** #
#
# Le reste :
# --------

# N'affiche pas le changement de directory lors du ${MAKE} -C
MAKEFLAGS += --no-print-directory

CC = c++

TEST_MODE = -D TEST_MODE
# comment to disable test and compile with static

HEADERS_DIR			= headers src/libft src
HEADERS_DIR_FLAG	= ${addprefix -I ,${HEADERS_DIR}}

SRCS_DIR			= src
SRCS				= ${shell find src/ -type f -name "*.cpp"}

OBJS_DIR		= obj
OBJS			= ${patsubst ${SRCS_DIR}%.cpp,${OBJS_DIR}%.o,$(SRCS)}
DEPS			= ${patsubst ${SRCS_DIR}%.cpp,${OBJS_DIR}%.d,$(SRCS)}

ECHO	= echo
RM		= rm -f
RMDIR	= rm -df
MKDIR	= mkdir

CFLAGS_NAME		= $(if $(filter $(MAKECMDGOALS), sanitize),$(SANITIZE_FLAG),)
CFLAGS			:= ${CFLAGS} $(if $(filter $(MAKECMDGOALS), debug test),$(DEBUG_FLAG),) ${TEST_MODE}
DEBUG_PROMPT	= \033[1;35mdebug mode\033[0m
OK_PROMPT		= \033[1;32mdone \033[0m$(if $(filter $(MAKECMDGOALS), debug test),$(DEBUG_PROMPT),)
OK_PROMPT		+= $(ifdef TEST_MODE, "\033[1;35munit test mode\033[0m")

DELETE = \033[2K\r
GO_UP = \033[1A\r

all: ${NAME}
	@echo "\033[1;32mSuccess\033[0m" ":)"

-include $(DEPS)

${NAME}: ${LIBFT} ${OBJS_DIR} ${OBJS}
	@printf "$(DELETE)\033[1;33m...Building\033[0m %-33s" "${NAME}"
	@${CC} ${CFLAGS} ${CFLAGS_NAME} ${HEADERS_DIR_FLAG} -o ${NAME} ${OBJS} ${LIBFT_FLAG} -lreadline
	@echo "${OK_PROMPT}"
	@#${MAKE} tags

${OBJS_DIR}:
	@printf "\033[1;34m...Creating\033[0m %-33s" "${OBJS_DIR} directory"
	@${MKDIR} ${OBJS_DIR}
	@printf "\033[1;32mdone\033[0m\n"

${OBJS_DIR}/%.o: ${SRCS_DIR}/%.cpp
	@printf "$(DELETE)\033[1;34mCompiling\033[0m %-35s" $<
	@${CC} ${CFLAGS} ${HEADERS_DIR_FLAG} -c $< -o $@
	@printf "${OK_PROMPT}\n${GO_UP}"

clean:
	@${RM} ${OBJS} $(DEPS)
	@${RMDIR} ${OBJS_DIR}
	@printf "\033[1;34m%-44s\033[0m \033[1;32m%s\033[0m\n" "Cleaning" "done"

fclean: clean
	@${RM} ${NAME}
	@printf "\033[1;34m%-44s\033[0m \033[1;32m%s\033[0m\n" "File cleaning" "done"

re: fclean all

debug: all

sanitize:
	@printf "\033[1;34m%-44s\033[0m \033[1;32m%s\033[0m\n" "Output is sanitized" "done"

# supprime les fichiers dupliqués sur mac
mac_clean:
	@find . -type f -name "* [2-9]*" -print -delete
