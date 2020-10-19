# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: joppe <joppe@student.codam.nl>               +#+                      #
#                                                    +#+                       #
#    Created: 2020/08/23 17:53:14 by joppe         #+#    #+#                  #
#    Updated: 2020/10/19 15:41:30 by joppe         ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

# Makefile for a generic project

NAME      	:= wiki

CC          := gcc
CFLAGS      := -Wall -Wextra -Werror -Wuninitialized

HEADERDIR	:= includes
SRCDIR      := src
BUILDDIR    := obj
SRCEXT      := c
OBJEXT      := o
LIBS		:= -I../ft_lib/includes/ -L../ft_lib/bin/ -lft

SOURCES     := $(shell find $(SRCDIR) -type f -name '*.$(SRCEXT)')
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%, $(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

$(NAME): all

.PHONY all
all: directories $(OBJECTS)
	$(CC) $(CFLAGS) -I ${HEADERDIR}/ $(OBJECTS) -o $(NAME) ${LIBS}

.PHONY clean
clean:
	/bin/rm -rf $(BUILDDIR)/

.PHONY fclean
fclean: clean
	/bin/rm -f $(NAME)

.PHONY re
re: fclean all

.PHONY directories
directories:
	@mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	$(CC) $(CFLAGS) -I ${HEADERDIR}/ -c $< -o $@ $(LIBS)
