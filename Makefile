# OPT:= -O2 -nodebug
# SIMPL:=CBEGIN
# TYPE:=int
# SENTINEL:=-1
# BIMPL:=NOBUSY
# BCHECK:=BARGINGCHECK

CXX = u++					# compiler
CXXFLAGS = -g -multi -Wall -Wextra -MMD # compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS = main.o config.o printer.o parent.o bank.o truck.o nameServer.o vendingMachine.o bottlingPlant.o # object files forming 1st executable with prefix "q2"
EXEC = soda				# executable name
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"

#############################################################

${EXEC} : ${OBJECTS}				# link step 1st executable
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXEC}