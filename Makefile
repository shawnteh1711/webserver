# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: steh <steh@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/10 21:12:27 by steh              #+#    #+#              #
#    Updated: 2023/04/17 11:42:01 by steh             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME				=	webserv
SRCS_DIR			=	srcs
SRCS				=	$(wildcard $(SRCS_DIR)/*.cpp) $(wildcard $(SRCS_DIR)/*/*.cpp)

OBJS_DIR			=	objs
OBJS				=	$(patsubst $(SRCS_DIR)/%.cpp, $(OBJS_DIR)/%.o, $(SRCS))
DEPENDS_DIR			=	dependencies
DEPENDS				=	$(patsubst $(SRCS_DIR)/%.cpp, $(OBJS_DIR)/%.d, $(SRCS))
CXXFLAGS			+=	-Wall -Wextra -Werror -std=c++98 -pedantic -fsanitize=address
# CXXFLAGS			+=	-Wall -Wextra -Werror -std=c++98 -pedantic
INCLUDES			=	-I$(SRCS_DIR) $(patsubst %, -I%, $(wildcard $(SRCS_DIR)/*/))

all					:	$(NAME)

$(NAME)				:	$(OBJS)
						$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJS_DIR)/%.o		:	$(SRCS_DIR)/%.cpp
						mkdir -p $(dir $@)
						$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# $(DEPENDS_DIR)/%.d	:	$(SRCS_DIR)/%.cpp
# 						mkdir -p $(@D)
# 						$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -MF $@ -c $< -o /dev/null

-include $(DEPENDS)

clean			:
					rm -rf $(OBJS_DIR)
					rm -rf $(DEPENDS_DIR)

fclean			:	clean
					rm -f $(NAME)
			
re				: 	fclean all

.PHONY			:	all clean fclean re
