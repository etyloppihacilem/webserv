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

# .ONESHELL:
# oneshell does not permit error checking using current reciepes syntax
.DELETE_ON_ERROR:
SHELL			:= sh
MAKEFLAGS		+= --no-builtin-rules
MAKEFLAGS		+= --no-print-directory
MAKEFLAGS		+= -j
# .RECIPEPREFIX	=
# same as default RECIPEPREFIX prefix used here

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
NAME_TEST		= ${NAME}_test

LIBRARIES		= -lreadline

CC				= c++
CFLAGS			= -MMD -Wall -Werror -Wextra -std=c++98
CTESTFLAGS		= -MMD -Wall -Werror -Wextra -std=c++20 -g3
DEBUG_FLAG		= -g3
TEST_FLAG		= -DTESTING
SANITIZE_FLAG	= -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer

HEADER_DIR		= header src ${shell realpath googletest/googletest/include} ${shell realpath googletest/googlemock/include}
SRCS_DIR		= src
OBJS_DIR		= obj
TEST_DIR		= ${OBJS_DIR}/test

########################
##  AUTO-EDIT ON VAR  ##
########################

HEADERS		= $(addprefix -I,$(HEADER_DIR))
SRCS		= $(shell find src -type f -name "*.cpp" | grep -v ".*_test.cpp" | grep -v ".*Test.cpp")

TESTS		= $(shell find src -type f -name "*_test.cpp")
TESTS		+= $(shell find src -type f -name "*Test.cpp")

TESTS_FILES	= $(shell find test -type f -name "*.cpp")

OBJS		= $(patsubst ${SRCS_DIR}%.cpp,${OBJS_DIR}%.o,${SRCS})
TEST_OBJ	= $(patsubst ${SRCS_DIR}%.cpp,${TEST_DIR}%.o,${TESTS})
TEST_OBJ	+= $(patsubst ${SRCS_DIR}%.cpp,${TEST_DIR}%.o,$(filter-out ${SRCS_DIR}/main.cpp, ${SRCS}))
TEST_OBJ	+= $(patsubst test%.cpp,${TEST_DIR}%.o,${TESTS_FILES})
DEPS		= $(patsubst ${SRCS_DIR}%.cpp,${OBJS_DIR}%.d,${SRCS})
DEPS		+= $(patsubst %.o,%.d,${TEST_OBJ})

CFLAGS			:= ${CFLAGS} $(if $(filter ${MAKECMDGOALS}, sanitize),${SANITIZE_FLAG},)
CFLAGS			:= ${CFLAGS} $(if $(filter ${MAKECMDGOALS}, debug test),${DEBUG_FLAG},)
CTESTFLAGS		+= ${TEST_FLAG}
DEBUG_PROMPT		= ${MAGENTA}debug mode${RESET}
SANITIZE_PROMPT		= ${YELLOW} sanitize mode${RESET}
OK_PROMPT			= ${GREEN}done ${RESET}
OK_PROMPT			+= $(if $(filter $(MAKECMDGOALS) none, debug), $(DEBUG_PROMPT), )
OK_PROMPT			+= $(if $(filter $(MAKECMDGOALS) none, sanitize), $(SANITIZE_PROMPT), )

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
	@${CC} ${CFLAGS} ${HEADERS} -c $< -o $@
	@printf "${OK_PROMPT}\n${GO_UP}"

${TEST_DIR}:
	@printf "${BLUE}...Creating${RESET} %-33s" "${TEST_DIR} directory"
	@${MKDIR} -p ${TEST_DIR}
	@printf "${GREEN}done${RESET}\n"

${TEST_DIR}/%.o: ${SRCS_DIR}/%.cpp
	@printf "${DELETE}${BLUE}Compiling${RESET} %-35s" $<
	@${CC} ${CTESTFLAGS} ${HEADERS} -c $< -o $@
	@printf "${OK_PROMPT}\n${GO_UP}"

${TEST_DIR}/%.o: test/%.cpp
	@printf "${DELETE}${BLUE}Compiling${RESET} %-35s" $<
	@${CC} ${CTESTFLAGS} ${HEADERS} -c $< -o $@
	@printf "${OK_PROMPT}\n${GO_UP}"

clean: # Clean object files
	@${RM} ${TEST_OBJ} ${OBJS} ${DEPS}
	@${RMDIR} ${TEST_DIR} ${OBJS_DIR}
	@printf "${BLUE}%-44s${RESET} ${GREEN}%s${RESET}\n" "Cleaning" "done"

fclean: clean # Clean executable file
	@${RM} ${NAME} ${NAME_TEST}
	@printf "${BLUE}%-44s${RESET} ${GREEN}%s${RESET}\n" "File cleaning" "done"

re: fclean all # Execute fclean & all rules

debug: fclean all

sanitize: fclean al

./header:
	mkdir header

./googletest/build:
	git submodule sync
	git submodule update
	cd googletest && mkdir build

./googletest/build/lib/libgtest.a: ./header ./googletest/build
	cd googletest/build && cmake .. && make

test: ${NAME_TEST}
	./${NAME_TEST}

${NAME_TEST}: ./header/libgtest.a ${TEST_DIR} ${TEST_OBJ}# to run tests
	@printf "${DELETE}${YELLOW}...Building${RESET} %-33s" "${NAME_TEST}"
	@${CC} $(filter-out -MMD, ${CTESTFLAGS}) ${HEADERS} -o ${NAME_TEST} ${TEST_OBJ} ${LIBRARIES} ./googletest/build/lib/libgtest.a ./googletest/build/lib/libgmock.a
	@printf "${GREEN}done${RESET}\n"

clangd: # configure clangd for tests
	bash ./script/clangd_generator.sh

# supprime les fichiers dupliqués sur mac
mac_clean:
	@find . -type f -name "* [2-9]*" -print -delete

file: # Print list of source and object files
	@printf "${MAGENTA}.cpp:		${GRAY}${SOURCES}${RESET}\n"
	@printf "${MAGENTA}.o:		${GRAY}${OBJECTS}${RESET}\n"

help:
	@egrep -h '\s#\s' ${MAKEFILE_LIST} | sort | awk 'BEGIN {FS = ":.*?# "}; {printf "${MAGENTA}%-15s${GRAY} %s${RESET}\n", $$1, $$2}'

.PHONY: all clean fclean re debug sanitize tags file help mac_clean test
.NOTPARALLEL	: re debug sanitize
