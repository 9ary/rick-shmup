NAME = rick-shmup

DEBUG = FALSE

CC = gcc

CFLAGS_COMMON = -Wall -Wextra \
		$(addprefix -I,$(INCLUDE) $(INCLUDE_EXT)) \
		-D'PROJ_NAME="$(NAME)"' \
		-MMD -MP \
		-pipe

ifeq ($(DEBUG),FALSE)
	CFLAGS_COMMON += -O2 -flto
else
	CFLAGS_COMMON += -O0 -g
endif

CFLAGS = $(CFLAGS_COMMON) -std=gnu11

LDFLAGS = $(CFLAGS_COMMON) \
	-lcsfml-system \
	-lcsfml-window \
	-lcsfml-graphics

SRCS_C :=
OBJS = $(BUILT_SRCS_C:%.c=%.o) \
	$(addprefix $(OUT)/,$(SRCS_C:%.c=%.o))
INCLUDE :=
INCLUDE_EXT :=
BUILT_SRCS :=
BUILT_SRCS_C :=

OUT = out
EXE = $(OUT)/$(NAME)

CLEAN_SPEC :=

# Figure out where we are
define whereami
$(strip $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST)))))
endef

include rules.mk
