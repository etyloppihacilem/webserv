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
CFLAGS			= -MMD -Wall -Werror -Wextra -O3 -std=c++98
CTESTFLAGS		= -MMD -Wall -Werror -Wextra -std=c++20 -g3 -pthread
CTESTFLAGS		+= -DTESTING -Igoogletest/googletest/include -Igoogletest/googlemock/include -Itest \
				   -DWORKDIR=\"${shell realpath .}\"
DEBUG_FLAG		= -g3 -DDEBUG
SANITIZE_FLAG	= -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer

HEADER_DIR		= header src
SRCS_DIR		= src
OBJS_DIR		= obj
TEST_DIR		= ${OBJS_DIR}/test
DEBUG_DIR		= ${OBJS_DIR}/debug
SANITIZE_DIR	= ${OBJS_DIR}/sanitize

########################
##  AUTO-EDIT ON VAR  ##
########################

HEADERS		= $(addprefix -I,$(HEADER_DIR))
SRCS		= $(shell find src -type f -name "*.cpp" | grep -v ".*_test.cpp" | grep -v ".*Test.cpp")

TESTS		= $(shell find src -type f -name "*_test.cpp")
TESTS		+= $(shell find src -type f -name "*Test.cpp")

TESTS_FILES	= $(shell find test -type f -name "*.cpp")

OBJS			= $(patsubst ${SRCS_DIR}%.cpp,${OBJS_DIR}%.o,${SRCS})
TEST_OBJ		= $(patsubst ${SRCS_DIR}%.cpp,${TEST_DIR}%.o,${TESTS})
TEST_OBJ		+= $(patsubst ${SRCS_DIR}%.cpp,${TEST_DIR}%.o,$(filter-out ${SRCS_DIR}/main.cpp, ${SRCS}))
TEST_OBJ		+= $(patsubst test%.cpp,${TEST_DIR}%.o,${TESTS_FILES})
DEBUG_OBJ		= $(patsubst ${SRCS_DIR}%.cpp,${DEBUG_DIR}%.o,${SRCS})
SANITIZE_OBJ	= $(patsubst ${SRCS_DIR}%.cpp,${SANITIZE_DIR}%.o,${SRCS})

ALL_FILES		= $(shell find src -type f -regex ".*\.[ch]pp")
ALL_FILES		+= $(shell find test -type f -regex ".*\.[ch]pp")
ALL_FILES		+= $(shell find header -type f -regex ".*\.[ch]pp")

DEPS		= $(patsubst ${SRCS_DIR}%.cpp,${OBJS_DIR}%.d,${SRCS})
DEPS		+= $(patsubst %.o,%.d,${TEST_OBJ})
DEPS		+= $(patsubst %.o,%.d,${DEBUG_OBJ})
DEPS		+= $(patsubst %.o,%.d,${SANITIZE_OBJ})

#######################
##  USUAL FUNCTIONS  ##
#######################

RM		= rm -f
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
	@${RM} ${OBJS} ${DEPS} ${TEST_OBJ} ${DEBUG_OBJ} ${SANITIZE_OBJ}
	@${RMDIR} ${TEST_DIR} ${DEBUG_DIR} ${SANITIZE_DIR} ${OBJS_DIR}
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

eval_tester: # setup ubuntu_tester
	@chmod 755 ubuntu_tester ubuntu_cgi_tester
	@mkdir YoupiBanane
	@touch YoupiBanane/youpi.bad_extension
	@touch YoupiBanane/youpi.bla
	@mkdir YoupiBanane/nop
	@touch YoupiBanane/nop/youpi.bad_extension
	@touch YoupiBanane/nop/other.pouic
	@mkdir YoupiBanane/Yeah
	@touch YoupiBanane/Yeah/not_happy.bad_extension

file: # Print list of source and object files
	@printf "${MAGENTA}.cpp:		${GRAY}${SRCS}${RESET}\n"
	@printf "${MAGENTA}.o:		${GRAY}${SRCS}${RESET}\n"
	@printf "${BLUE}Test files:\n"
	@printf "  ${MAGENTA}.cpp:		${GRAY}${TESTS_FILES}${RESET}\n"

help:
	@egrep -h '\s#\s' ${MAKEFILE_LIST} | sort | awk 'BEGIN {FS = ":.*?# "}; {printf "${MAGENTA}%-15s${GRAY} %s${RESET}\
		\n", $$1, $$2}'

###################
## TARGETS RULES ##
###################

${NAME}: ${OBJS_DIR} ${OBJS} # Compile ${NAME} program
	@printf "${YELLOW}Building...${RESET} ${NAME}\n"
	@${CC} ${CFLAGS} ${HEADERS} -o ${NAME} ${OBJS} ${LIBRARIES}

${NAME_TEST}: ${TEST_OBJ}
	@printf "${YELLOW}Building...${RESET} %s\n" "${NAME_TEST}"
	@${CC} $(filter-out -MMD, ${CTESTFLAGS}) ${HEADERS} -o ${NAME_TEST} ${TEST_OBJ} ${LIBRARIES} \
		./googletest/build/lib/libgtest.a ./googletest/build/lib/libgmock.a

${NAME_DEBUG}: ${DEBUG_OBJ}
	@printf "${YELLOW}Building...${RESET} ${NAME_DEBUG}\n"
	@${CC} ${CFLAGS} ${DEBUG_FLAG} ${HEADERS} -o ${NAME_DEBUG} ${DEBUG_OBJ} ${LIBRARIES}

${NAME_SANITIZE}: ${SANITIZE_OBJ}
	@printf "${YELLOW}Building...${RESET} ${NAME_SANITIZE}\n"
	@${CC} ${CFLAGS} ${SANITIZE_FLAG} ${HEADERS} -o ${NAME_SANITIZE} ${SANITIZE_OBJ} ${LIBRARIES}

#######################
## COMPILATION RULES ##
#######################

${OBJS_DIR}/%.o: ${SRCS_DIR}/%.cpp
	@printf "${BLUE}Compiling..${RESET} (${YELLOW}release${RESET})%s %s\n" "" $<
	@${CC} ${CFLAGS} ${HEADERS} -c $< -o $@
	@printf "${GREEN}.......Done${RESET} (${YELLOW}release${RESET})%s %s\n" "" $<

${TEST_DIR}/%.o: ${SRCS_DIR}/%.cpp | ./googletest/build/lib/libgtest.a ${TEST_DIR}
	@printf "${BLUE}Compiling..${RESET} (${CYAN}test${RESET})%s %s\n" "" $<
	@${CC} ${CTESTFLAGS} ${HEADERS} -c $< -o $@
	@printf "${GREEN}.......Done${RESET} (${CYAN}test${RESET})%s %s\n" "" $<

${TEST_DIR}/%.o: test/%.cpp | ./googletest/build/lib/libgtest.a ${TEST_DIR}
	@printf "${BLUE}Compiling..${RESET} (${CYAN}test${RESET})%s %s\n" "" $<
	@${CC} ${CTESTFLAGS} ${HEADERS} -c $< -o $@
	@printf "${GREEN}.......Done${RESET} (${CYAN}test${RESET})%s %s\n" "" $<

${DEBUG_DIR}/%.o: ${SRCS_DIR}/%.cpp | ${DEBUG_DIR}
	@printf "${BLUE}Compiling..${RESET} (${MAGENTA}debug${RESET})%s %s\n" "" $<
	@${CC} ${CFLAGS} ${DEBUG_FLAG} ${HEADERS} -c $< -o $@
	@printf "${GREEN}.......Done${RESET} (${MAGENTA}debug${RESET})%s %s\n" "" $<

${SANITIZE_DIR}/%.o: ${SRCS_DIR}/%.cpp | ${SANITIZE_DIR}
	@printf "${BLUE}Compiling..${RESET} (${MAGENTA}sanitize${RESET})%s %s\n" "" $<
	@${CC} ${CFLAGS} ${SANITIZE_FLAG} ${HEADERS} -c $< -o $@
	@printf "${GREEN}.......Done${RESET} (${MAGENTA}sanitize${RESET})%s %s\n" "" $<

./header:
	mkdir header

#####################
## DIRECTORY RULES ##
#####################

${OBJS_DIR}:
	@printf "${YELLOW}...Creating${RESET} ${OBJS_DIR}\n"
	@${MKDIR} ${OBJS_DIR}

${TEST_DIR}:
	@printf "${YELLOW}...Creating${RESET} ${TEST_DIR}\n"
	@${MKDIR} -p ${TEST_DIR}

${DEBUG_DIR}:
	@printf "${YELLOW}...Creating${RESET} ${DEBUG_DIR}\n"
	@${MKDIR} -p ${DEBUG_DIR}

${SANITIZE_DIR}:
	@printf "${YELLOW}...Creating${RESET} ${SANITIZE_DIR}\n"
	@${MKDIR} -p ${SANITIZE_DIR}

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
