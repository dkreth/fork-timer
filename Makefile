#
# Filename 	Makefile
# Date     	1/27/2020
# Author     	Dylan Kreth
# Email     	dylan.kreth@utdallas.edu

CXX = g++
#CXXFLAGS = -Wall -Werror
#CPPFLAGS = -I /scratch/perkins/include
#LDFLAGS = -L /scratch/perkins/lib
#LDLIBS = -lcdk -lcurses
SRCS = fork-timer.cc
PROJECTNAME = CS4348.fork-timer
EXECFILE = fork-timer

.PHONY: all clean backup deepclean

.PRECIOUS: 

#==================================================
# Below here, everything should usually be the same
#==================================================


OBJS = $(SRCS:cc=o)

all: $(EXECFILE)

clean:
	rm -f $(OBJS) *.d* *~ \#* *.o $(EXECFILE)
deepclean:
	rm -f $(OBJS) $(EXECFILE) *.d* *~ \#* *.o

Makefile: $(SRCS:.cc=.d)

# Pattern for .d files.
%.d:%.cc
	@echo Updating .d Dependency File
	@set -e; rm -f $@; \
	$(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$


# rule for linking objects 
$(EXECFILE): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)


# Backup Target
backup:	deepclean
	@mkdir -p ~/backups; chmod 700 ~/backups
	@$(eval CURDIRNAME := $(shell basename `pwd`))
	@$(eval MKBKUPNAME := ~/backups/$(PROJECTNAME)-$(shell date +'%Y.%m.%d-%H:%M:%S').tar.gz)
	@echo
	@echo Writing Backup file to: $(MKBKUPNAME)
	@echo
	@-tar zcfv $(MKBKUPNAME) ../$(CURDIRNAME)
	@chmod 600 $(MKBKUPNAME)
	@echo
	@echo Done!


# uncomment to include the dependency files
# -include $(SRCS:.cc=.d)
