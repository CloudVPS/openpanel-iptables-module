include makeinclude

OBJ	= main.o cfgiptables.o version.o

all: module.xml iptablesmodule.exe
	mkapp iptablesmodule 

module.xml: module.def
	mkmodulexml < module.def > module.xml

version.cpp:
	mkversion version.cpp

iptablesmodule.exe: $(OBJ)
	$(LD) $(LDFLAGS) -o iptablesmodule.exe $(OBJ) $(LIBS) \
	../opencore/api/c++/lib/libcoremodule.a -lz -lssl

clean:
	rm -f *.o *.exe
	rm -rf iptablesmodule.app
	rm -f iptablesmodule

SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -I../opencore/api/c++/include -c -g $<
