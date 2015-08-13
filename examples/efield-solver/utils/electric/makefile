BINDIR = bin
SRCDIR = src
PSRCDIR = ../../src
TSRCDIR = test
OBJDIR = obj
export OBJDIR

MPIBASE=/usr
CC=$(MPIBASE)/bin/mpicc
CXX=$(MPIBASE)/bin/mpic++

GTEST_DIR = /home/andrew/code/gtest

MKDIR_P = mkdir -p
export MKDIR_P

CFLAGS = -g -Wall -O0 -I$(GTEST_DIR)/include -I../../src -DC_CLIENT
LDFLAGS =
CXXFLAGS = -g -Wall -pthread -Wextra -I$(GTEST_DIR)/include

CFLAGS   += -I$(PETSC_DIR)/include -I$(PETSC_DIR)/$(PETSC_ARCH)/include
include $(PETSC_DIR)/$(PETSC_ARCH)/lib/petsc/conf/petscvariables
LDFLAGS     += -L$(PETSC_DIR)/$(PETSC_ARCH) $(PETSC_KSP_LIB)

TEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h


GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

CSRC = map.c grid.c problem.c state.c boundary.c solver.c option.c
PCSRC = ef_solver.c ef_interface.c ef_bc.c ef_metric.c ef_grid.c ef_operator.c ef_stencil.c ef_fd.c ef_callback.c ef_dmap.c ef_neumann.c ef_dirichlet.c ef_boundary.c ef_io.c
TCSRC = check_models.c

PSRC = $(addprefix $(PSRCDIR)/, $(PCSRC))
TSRC = $(addprefix $(TSRCDIR)/, $(TCSRC))
SRC = $(addprefix $(SRCDIR)/, $(CSRC))
SRC += $(PSRC)
SRC += $(TSRC)
#$(info $$var is [${SRC}])

COBJ = $(CSRC:.c=.o)
COBJ += $(PCSRC:.c=.o)
#$(info $$var is [${COBJ}])
OBJ = $(addprefix $(OBJDIR)/, $(COBJ))

all: $(OBJDIR) $(OBJ) $(BINDIR)/ef-test

test: $(BINDIR)/test-models

gtest-all.o : $(GTEST_SRCS_)
		    $(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc


gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^


$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(PSRCDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(TSRCDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(BINDIR)/ef-test: $(OBJ) obj/driver.o | $(BINDIR)
	$(CC) $(OBJ) obj/driver.o $(LDFLAGS) -o $(BINDIR)/ef-test

obj/test_models.o: $(TSRCDIR)/test_models.cc $(GTEST_HEADERS)
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $< -o $@

$(BINDIR)/test-models: $(OBJ) obj/test_models.o gtest.a | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $(BINDIR)/test-models -lpthread

$(BINDIR):
	$(MKDIR_P) $(BINDIR)

$(OBJDIR):
	$(MKDIR_P) $(OBJDIR)

clean:
	@rm -f $(OBJDIR)/*.o $(BINDIR)/ef-test gtest.a $(BINDIR)/check-models $(BINDIR)/test-models a.dat a.dat.info b.txt x.txt ef-*.vts ef_sol.vts gtest-all.o
	rmdir $(OBJDIR)
	rmdir $(BINDIR)
