MKDIR ?= $(shell which mkdir)
RM ?= $(shell which rm)

PROG = ml-numbers

SRCDIR = src
INCDIR = include
OBJDIR = obj

SRCS = $(wildcard $(SRCDIR)/*.cpp)
INCS = $(wildcard $(INCDIR)/*.hpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

CFLAGS += -I$(INCDIR)
CFLAGS += -std=c++17 -g

.PHONY: all
all: makedirs $(PROG)

.PHONY: makedirs
makedirs:
	$(MKDIR) -p $(OBJDIR)

$(OBJS): | makedirs

$(PROG): $(OBJS)
	$(CXX) $^ -o $@

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(INCS)
	$(CXX) -c $< $(CFLAGS) -o $@

.PHONY: clean
clean:
	$(RM) -rf $(OBJDIR) $(PROG)
