# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: steh <steh@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/10 21:12:27 by steh              #+#    #+#              #
#    Updated: 2023/03/12 22:11:52 by steh             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME				=	webserv
SRCS_DIR			=	srcs
SRCS				=	$(wildcard $(SRCS_DIR)/*.cpp) $(wildcard $(SRCS_DIR)/*/*.cpp)

# SRCS				=	$(wildcard $(SRCS_DIR)/*.cpp) \
# 						$(wildcard $(SRCS_DIR)/server/*.cpp) \
# 						$(wildcard $(SRCS_DIR)/config/*.cpp) \

OBJS_DIR			=	objs
OBJS				=	$(patsubst $(SRCS_DIR)/%.cpp, $(OBJS_DIR)/%.o, $(SRCS))
DEPENDS_DIR			=	dependencies
DEPENDS				=	$(patsubst $(SRCS_DIR)/%.cpp, $(OBJS_DIR)/%.d, $(SRCS))
CXXFLAGS			=	-Wall -Wextra -Werror -std=c++98 -pedantic -fsanitize=address
CXX					=	c++
INCLUDES			=	-I$(SRCS_DIR) $(patsubst %, -I%, $(wildcard $(SRCS_DIR)/*/))

# INCLUDES			=	-I$(SRCS_DIR) \
# 						-I$(SRCS_DIR)/server \
# 						-I$(SRCS_DIR)/logger \
# 						-I$(SRCS_DIR)/request \
# 						-I$(SRCS_DIR)/response \
# 						-I$(SRCS_DIR)/config \

all					:	$(NAME)
						./$(NAME) ./config/nginx.conf

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
