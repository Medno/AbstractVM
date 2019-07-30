# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/01/08 14:57:58 by pchadeni          #+#    #+#              #
#    Updated: 2019/07/30 16:48:37 by pchadeni         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#------Name of the project------#

NAME = avm

TESTS = $(addprefix tests/, \
	empty.test	\
	instr.test	\
	instr_error.test	\
	digit.test	\
	basic.test	\
	value.test	\
)

#------All sources------#

SRCS = main.cpp		\
	   Lexer.cpp	\
	   Parser.cpp

OBJ += $(addprefix ./$(OBJ_PATH)/, $(SRCS:.cpp=.o))

#------Includes------#

INCLUDES = ./inc/
INC += -I $(INCLUDES)

HEAD = $(addprefix $(INCLUDES), \
		IOperand.hpp	\
		Lexer.hpp		\
		Parser.hpp		\
		Operand.hpp		\
)

#------Path------#

SRCS_PATH = srcs
OBJ_PATH  = obj

#------Main rules------#

all: $(OBJ_PATH) $(NAME)

$(NAME): $(OBJ)
	@printf "\33[2KObjects created $(BOLD_GREEN)✓$(EOC)\n"
	@printf "Start making $(NAME)...\n"
	@$(CCPP) $(CPP_FLAGS) $(LIBCURSES) $(FLAGS) $(INC) -o $(NAME) $(OBJ)
	@printf "$(BOLD_GREEN)⤖     $(NAME)$(EOC) created $(BOLD_GREEN)✓$(EOC)\n"

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)
	@printf "$(OBJ_PATH) created\n"

$(OBJ_PATH)/%.o: $(SRCS_PATH)/%.cpp $(HEAD)
	@$(CCPP) $(FLAGS) $(CPP_FLAGS) $(INC) -o $@ -c $<
	@printf " $(COL_GREEN)[OK]$(EOC) $(COL_YELLOW)Compiling:$(EOC) $<\r"

clean:
	@rm -Rf $(OBJ_PATH)
	@echo "$(BOLD_GREEN)$(NAME)$(EOC) clean $(BOLD_GREEN)✓$(EOC)"

fclean: clean
	@rm -Rf $(NAME)
	@echo "$(BOLD_GREEN)$(NAME)$(EOC) fclean $(BOLD_GREEN)✓$(EOC)"

re: fclean all

test:
	@$(foreach var,$(TESTS),echo "-------------Tests for $(var)-------------" && ./$(NAME) $(var);)

#------Compilation's flags------#

CCPP = clang++

# flags
CPP_FLAGS = -Wall -Wextra

ifneq ($(NOERR),yes)
CPP_FLAGS += -Werror
endif

CPP_FLAGS += -std=c++11

LIBCURSES += -lncurses

ifeq ($(DEV),yes)
FLAGS += -g
endif

ifeq ($(SAN),yes)
FLAGS += -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
endif

#------Define colors------#

BOLD_GREEN = \033[01;32m
COL_GREEN = \033[0;32m
COL_YELLOW = \033[0;33m
EOC = \033[0m

.PHONY: all re fclean clean
