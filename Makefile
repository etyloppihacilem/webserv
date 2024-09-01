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

.DELETE_ON_ERROR:
SHELL			:= sh
MAKEFLAGS		+= --no-builtin-rules
MAKEFLAGS		+= --no-print-directory
MAKEFLAGS		+= -j$(shell nproc || echo 4)
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
NAME_DEBUG		= ${NAME}_debug
NAME_SANITIZE	= ${NAME}_sanitize

LIBRARIES		= -lreadline

CC				= c++
# bc debug clang does not put debug string symbols
DEBUG_CC		= g++
CFLAGS			= -MMD -Wall -Werror -Wextra -std=c++98
CTESTFLAGS		= -MMD -Wall -Werror -Wextra -std=c++20 -g3 -pthread
CTESTFLAGS		+= -DTESTING -Igoogletest/googletest/include -Igoogletest/googlemock/include -Itest \
				   -DWORKDIR=\"${shell realpath .}\"
DEBUG_FLAG		= -g3 -DDEBUG
SANITIZE_FLAG	= -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer

HEADER_DIR		    = header
SRC_DIR		        = src
TEST_DIR			= test

INCLUDE_DIR         = ${HEADER_DIR} ${SRC_DIR}
OBJ_DIR				= obj
TEST_OBJ_DIR		= ${OBJ_DIR}/test
DEBUG_OBJ_DIR		= ${OBJ_DIR}/debug
SANITIZE_OBJ_DIR	= ${OBJ_DIR}/sanitize

########################
##  AUTO-EDIT ON VAR  ##
########################

SRC_FILES		= $(shell find $(SRC_DIR) -type f -name "*.cpp" | grep -v ".*_test.cpp" | grep -v ".*Test.cpp")
TEST_FILES   	= $(shell find ${TEST_DIR} -type f -name "*.cpp")

INCLUDES		= $(addprefix -I,$(INCLUDE_DIR))

OBJS			= $(patsubst ${SRC_DIR}%.cpp,${OBJ_DIR}%.o,${SRC_FILES})
TEST_OBJS		= $(patsubst ${SRC_DIR}%.cpp,${TEST_OBJ_DIR}%.o,$(filter-out ${SRC_DIR}/main.cpp, ${SRC_FILES}))
TEST_OBJS		+= $(patsubst ${TEST_DIR}%.cpp,${TEST_OBJ_DIR}%.o,${TEST_FILES})
DEBUG_OBJS		= $(patsubst ${SRC_DIR}%.cpp,${DEBUG_OBJ_DIR}%.o,${SRC_FILES})
SANITIZE_OBJS	= $(patsubst ${SRC_DIR}%.cpp,${SANITIZE_OBJ_DIR}%.o,${SRC_FILES})

ALL_FILES		= $(shell find ${SRC_DIR} -type f -regex ".*\.[ch]pp")
ALL_FILES		+= $(shell find ${TEST_DIR} -type f -regex ".*\.[ch]pp")
ALL_FILES		+= $(shell find ${HEADER_DIR} -type f -regex ".*\.[ch]pp")

DEPS		= $(patsubst ${SRC_DIR}%.cpp,${OBJ_DIR}%.d,${SRC_FILES})
DEPS		+= $(patsubst %.o,%.d,${TEST_OBJS})
DEPS		+= $(patsubst %.o,%.d,${DEBUG_OBJS})
DEPS		+= $(patsubst %.o,%.d,${SANITIZE_OBJS})

#######################
##  USUAL FUNCTIONS  ##
#######################

ADDFILE = touch
RMFILE	= rm -f
RMDIR	= rm -df
MKDIR	= mkdir

###################
## GENERAL RULES ##
###################

release: ${NAME} # Default rule
	@printf "${GREEN}Success${RESET}  :)\n"

all: ${NAME} ${NAME_TEST} ${NAME_DEBUG} ${NAME_SANITIZE}
	@printf "${GREEN}Success${RESET}  :)\n"

clean: # Clean object files
	@${RM} ${OBJS} ${DEPS} ${TEST_OBJS} ${DEBUG_OBJS} ${SANITIZE_OBJS}
	@${RMDIR} ${TEST_OBJ_DIR} ${DEBUG_OBJ_DIR} ${SANITIZE_OBJ_DIR} ${OBJ_DIR}
	@printf "${BLUE}%-44s${RESET}\n" "Cleaning"

fclean: clean # Clean executable file
	@${RM} ${NAME} ${NAME_TEST} ${NAME_DEBUG} ${NAME_SANITIZE}
	@printf "${BLUE}%-44s${RESET}\n" "File cleaning"

re: fclean # Execute fclean & release rules
	@${MAKE}

test: ${NAME_TEST} # Compile tests.
	@printf "${CYAN}Test build ${GREEN}success${RESET}  :)\n"

debug: ${NAME_DEBUG} # Compile tests.
	@printf "${MAGENTA}Debug build ${GREEN}Success${RESET}  :)\n"

sanitize: ${NAME_SANITIZE} # Compile tests.
	@printf "${MAGENTA}Sanitize build ${GREEN}Success${RESET}  :)\n"

run_tests: test
	./${NAME_TEST}

format:
	@which clang-format; if [ $$? -ne 0 ]; then echo \
		'clang-format is not in path. If installed with mason, please run\n\n    echo "export PATH=\$$PATH:$$(echo' \
		'~/.local/share/nvim/mason/bin)" >> ~/.zshrc\n\nand re-open your terminal or run\n\n    export' \
		'PATH=$$PATH:$$(echo ~/.local/share/nvim/mason/bin)'; else clang-format -i --verbose ${ALL_FILES}; fi

clangd: # configure clangd for tests
	bash ./script/clangd_generator.sh

mac_clean: # supprime les fichiers dupliqués sur mac
	@find . -type f -name "* [2-9]*" -print -delete

# TODO: add cleaning rule
eval_tester: # setup ubuntu_tester
	@chmod 755 ubuntu_tester ubuntu_cgi_tester

file: # Print list of source and object files
	@printf "${MAGENTA}.cpp:		${GRAY}${SRC_FILES}${RESET}\n"
	@printf "${MAGENTA}.o:		${GRAY}${OBJS}${RESET}\n"
	@printf "${BLUE}Test files:\n"
	@printf "\t${MAGENTA}.cpp:		${GRAY}${TEST_FILES}${RESET}\n"

help:
	@egrep -h '\s#\s' ${MAKEFILE_LIST} | sort | awk 'BEGIN {FS = ":.*?# "}; {printf "${MAGENTA}%-15s${GRAY} %s${RESET}\
		\n", $$1, $$2}'

###################
## TARGETS RULES ##
###################

${NAME}: ${OBJ_DIR} ${OBJS} # Compile ${NAME} program
	@printf "${YELLOW}Building...${RESET} ${NAME}\n"
	@${CC} ${CFLAGS} ${INCLUDES} -o ${NAME} ${OBJS} ${LIBRARIES}

${NAME_TEST}: ${TEST_OBJS} # Compile ${NAME_TEST} program
	@printf "${YELLOW}Building...${RESET} %s\n" "${NAME_TEST}"
	@${CC} $(filter-out -MMD, ${CTESTFLAGS}) ${INCLUDES} -o ${NAME_TEST} ${TEST_OBJS} ${LIBRARIES} \
		./googletest/build/lib/libgtest.a ./googletest/build/lib/libgmock.a

${NAME_DEBUG}: ${DEBUG_OBJS} # Compile ${NAME_DEBUG} program
	@printf "${YELLOW}Building...${RESET} ${NAME_DEBUG}\n"
	@${DEBUG_CC} ${CFLAGS} ${DEBUG_FLAG} ${INCLUDES} -o ${NAME_DEBUG} ${DEBUG_OBJS} ${LIBRARIES}

${NAME_SANITIZE}: ${SANITIZE_OBJS} # Compile ${NAME_SANITIZE} program
	@printf "${YELLOW}Building...${RESET} ${NAME_SANITIZE}\n"
	@${CC} ${CFLAGS} ${SANITIZE_FLAG} ${INCLUDES} -o ${NAME_SANITIZE} ${SANITIZE_OBJS} ${LIBRARIES}

#######################
## COMPILATION RULES ##
#######################

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@printf "${BLUE}Compiling..${RESET} (${YELLOW}release${RESET})%s %s\n" "" $<
	@${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@
	@printf "${GREEN}.......Done${RESET} (${YELLOW}release${RESET})%s %s\n" "" $<

${TEST_OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp | ./googletest/build/lib/libgtest.a ${TEST_OBJ_DIR}
	@printf "${BLUE}Compiling..${RESET} (${CYAN}test${RESET})%s %s\n" "" $<
	@${CC} ${CTESTFLAGS} ${INCLUDES} -c $< -o $@
	@printf "${GREEN}.......Done${RESET} (${CYAN}test${RESET})%s %s\n" "" $<

${TEST_OBJ_DIR}/%.o: ${TEST_DIR}/%.cpp | ./googletest/build/lib/libgtest.a ${TEST_OBJ_DIR}
	@printf "${BLUE}Compiling..${RESET} (${CYAN}test${RESET})%s %s\n" "" $<
	@${CC} ${CTESTFLAGS} ${INCLUDES} -c $< -o $@
	@printf "${GREEN}.......Done${RESET} (${CYAN}test${RESET})%s %s\n" "" $<

${DEBUG_OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp | ${DEBUG_OBJ_DIR}
	@printf "${BLUE}Compiling..${RESET} (${MAGENTA}debug${RESET})%s %s\n" "" $<
	@${DEBUG_CC} ${CFLAGS} ${DEBUG_FLAG} ${INCLUDES} -c $< -o $@
	@printf "${GREEN}.......Done${RESET} (${MAGENTA}debug${RESET})%s %s\n" "" $<

${SANITIZE_OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp | ${SANITIZE_OBJ_DIR}
	@printf "${BLUE}Compiling..${RESET} (${MAGENTA}sanitize${RESET})%s %s\n" "" $<
	@${CC} ${CFLAGS} ${SANITIZE_FLAG} ${INCLUDES} -c $< -o $@
	@printf "${GREEN}.......Done${RESET} (${MAGENTA}sanitize${RESET})%s %s\n" "" $<

#####################
## DIRECTORY RULES ##
#####################

${HEADER_DIR}:
	@printf "${YELLOW}...Creating${RESET} ${HEADER_DIR}\n"
	@${MKDIR} ${HEADER_DIR}

${OBJ_DIR}:
	@printf "${YELLOW}...Creating${RESET} ${OBJ_DIR}\n"
	@${MKDIR} ${OBJ_DIR}

${TEST_OBJ_DIR}:
	@printf "${YELLOW}...Creating${RESET} ${TEST_OBJ_DIR}\n"
	@${MKDIR} -p ${TEST_OBJ_DIR}

${DEBUG_OBJ_DIR}:
	@printf "${YELLOW}...Creating${RESET} ${DEBUG_OBJ_DIR}\n"
	@${MKDIR} -p ${DEBUG_OBJ_DIR}

${SANITIZE_OBJ_DIR}:
	@printf "${YELLOW}...Creating${RESET} ${SANITIZE_OBJ_DIR}\n"
	@${MKDIR} -p ${SANITIZE_OBJ_DIR}

${TESTER_DIR}:
	@mkdir YoupiBanane
	@touch YoupiBanane/youpi.bad_extension
	@touch YoupiBanane/youpi.bla
	@mkdir YoupiBanane/nop
	@touch YoupiBanane/nop/youpi.bad_extension
	@touch YoupiBanane/nop/other.pouic
	@mkdir YoupiBanane/Yeah
	@touch YoupiBanane/Yeah/not_happy.bad_extension

###########
## OTHER ##
###########

-include ${DEPS}

./googletest/build:
	git submodule init
	git submodule sync
	git submodule update
	cd googletest && mkdir build

./googletest/build/lib/libgtest.a: ./googletest/build
	cd googletest/build && cmake .. && ${MAKE}

.PHONY: all clean fclean re debug sanitize tags file help mac_clean test release format
