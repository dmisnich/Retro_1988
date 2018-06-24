# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    MakeFile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dmisnich <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/06/08 16:32:14 by dmisnich          #+#    #+#              #
#    Updated: 2018/06/08 16:32:16 by dmisnich         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_retro
CC	=	clang++

CFLAGS	=	-std=c++98 -Wall -Wextra -Werror -g

HEADERS = ./inc/Game.hpp ./inc/Player.hpp ./inc/Objects.hpp ./inc/Bullets.hpp ./inc/Enemy.hpp ./inc/Boss.hpp ./inc/Stars.hpp ./inc/IObjClass.hpp

SRCS = ./src/main.cpp ./src/Game.cpp ./src/Player.cpp ./src/Objects.cpp ./src/Bullets.cpp ./src/Enemy.cpp ./src/Boss.cpp ./src/Stars.cpp



OBJ = $(SRCS:.cpp=.o)


all: $(NAME) $(OBJ)

$(NAME): $(OBJ) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lncurses

%.o : %.cpp $(HEADERS)
	$(CC)  $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
