include makeinclude

OBJ	= main.o cfgiptables.o version.o

all: module.xml iptablesmodule.exe down_firewall.png
	grace mkapp iptablesmodule 

down_firewall.png: firewall.png
	convert -modulate 50,100,100 firewall.png down_firewall.png

module.xml: module.def
	mkmodulexml < module.def > module.xml

version.cpp:
	grace mkversion version.cpp

iptablesmodule.exe: $(OBJ)
	$(LD) $(LDFLAGS) -o iptablesmodule.exe $(OBJ) $(LIBS) \
	/usr/lib/openpanel-core/libcoremodule.a

clean:
	rm -f *.o *.exe
	rm -rf iptablesmodule.app
	rm -f iptablesmodule

SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -I/usr/include/opencore -c -g $<
