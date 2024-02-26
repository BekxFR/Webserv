# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/01 12:07:22 by chillion          #+#    #+#              #
#    Updated: 2023/05/10 18:08:35 by mgruson          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY : all clean fclean re test

CXX := c++
CXXFLAGS := -std=c++98 -Wall -Wextra -Werror -MMD -MP -Isources/
# CXXFLAGS += -fsanitize=address
CXXFLAGS += -g3
SRC_DIR := sources/
OBJ_DIR := objects/
RM := rm
VAL := valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
VAL += --track-fds=yes

BLACK = \033[1;30m
REDBG = \033[30;41m
RED = \033[0;31m
GREEN = \033[0;32m
ORANGE = \033[0;33m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
CYAN = \033[0;36m
NC = \033[0m

SRCS =	main.cpp server_configuration.cpp server_request.cpp server_response.cpp server_location_configuration.cpp ErrorCorresp.cpp cgi.cpp utils.cpp initServ.cpp

SOFT_NAME := webserv
OBJS = $(SRCS:%.cpp=%.o)
SRC = $(addprefix $(SRC_DIR),$(SRCS))
OBJ = $(addprefix $(OBJ_DIR),$(OBJS))
DEPS = $(OBJ:%.o=%.d)
OBJF := ${OBJ_DIR}.cache_exists

all : ${SOFT_NAME}

${OBJ_DIR}%.o : $(SRC_DIR)%.cpp $(OBJF) Makefile
	@echo "${BLUE}###${NC}Creation du fichier ${@:%.cpp=%.o}${BLUE}###${ORANGE}"
	${CXX} ${CXXFLAGS} -c $< -o $@
	@echo "${NC}"

$(OBJF) :
	@mkdir -p ${OBJ_DIR}
	@touch ${OBJF}

${SOFT_NAME} : ${OBJ}
	@echo "${BLUE}###${NC}Creation du fichier ${SOFT_NAME}${BLUE}###${ORANGE}"
	${CXX} ${OBJ} ${CXXFLAGS} -o ${SOFT_NAME}
	@echo "${NC}"

-include ${DEPS}

test : all
	$(VAL) ./${SOFT_NAME} conf/server.conf3

clean : 
	@echo "${RED}###${NC}Nettoyage des fichiers .o${RED}###"
	${RM} -rf ${OBJ_DIR} ${OBJF}
	@echo "${GREEN}###${NC}Nettoyage OK${GREEN}###${NC}\n"

fclean : clean
	@echo "${RED}###${NC}Nettoyage d'archives et de Softs${RED}###"
	${RM} -f ${SOFT_NAME} ${SOFT_NAME2}
	@echo "${GREEN}###${NC}Nettoyage OK${GREEN}###${NC}\n"

re : fclean all
