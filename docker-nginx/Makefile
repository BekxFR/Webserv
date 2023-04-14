
.PHONY : all clean fclean stop

BLACK = \033[1;30m
REDBG = \033[30;41m
RED = \033[0;31m
GREEN = \033[0;32m
ORANGE = \033[0;33m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
CYAN = \033[0;36m
NC = \033[0m

DOCKER_COMPOSE_FILE = ./srcs/docker-compose.yml
DATA_DIR = /home/chillion/data

SITE_DIR := $(addsuffix /site, $(CURDIR))

all : clean
	cp -R ../site ./site
	docker build -t mynginx .
	rm -rf ./site
	docker run --name mynginx -d -p 8099:80 -p 8098:81 mynginx
#	docker run --name mynginx -d -p 8099:80 -p 8098:81 -v $(SITE_DIR):/usr/share/nginx/html mynginx

test :
	docker exec -it mynginx sh

stop :
	docker container stop $(shell docker container ls -aq) 2> /dev/null || true;
	@echo "${BLUE}###${NC}Docker-Compose ${RED}stop${NC} command done.${BLUE}###${NC}\n${BLUE}###${NC}Refer lasts logs lines for status${BLUE}###${NC}"

clean : stop
	docker container rm $(shell docker container ls -aq) 2> /dev/null || true;
	@echo "${BLUE}###${NC}Docker-Compose ${RED}stop${NC} and ${RED}remove${NC} command done.${BLUE}###${NC}\n${BLUE}###${NC}Refer lasts logs lines for status${BLUE}###${NC}"

fclean : clean
	docker stop $(shell docker ps -qa); docker rm $(shell docker ps -qa); docker rmi -f $(shell docker images -qa); docker rm $(shell docker volume ls -q); docker network rm $(shell docker network ls -q) 2> /dev/null || true;
	@echo "${BLUE}###${NC}All Data from Docker are ${RED}delete${NC}.${BLUE}###${NC}\n${BLUE}###${NC}Refer lasts logs lines for status.${BLUE}###${NC}\n${BLUE}###${NC}${ORANGE}Note${NC} : message like \"docker stop requires at least 1 argument.\" can be normal.${BLUE}###${NC}"


