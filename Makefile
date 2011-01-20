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

install:
	mkdir -p ${DESTDIR}/var/openpanel/modules/IPTables.module
	mkdir -p ${DESTDIR}/var/openpanel/conf/staging/IPTables
	cp -rf ./iptablesmodule.app    ${DESTDIR}/var/openpanel/modules/IPTables.module/
	ln -sf iptablesmodule.app/exec ${DESTDIR}/var/openpanel/modules/IPTables.module/action
	cp     module.xml          ${DESTDIR}/var/openpanel/modules/IPTables.module/module.xml
	install -m 755 verify      ${DESTDIR}/var/openpanel/modules/IPTables.module/verify
	cp *.html ${DESTDIR}/var/openpanel/modules/IPTables.module
	cp *.png ${DESTDIR}/var/openpanel/modules/IPTables.module

clean:
	rm -f *.o *.exe
	rm -rf iptablesmodule.app
	rm -f iptablesmodule

SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -I/usr/include/opencore -c -g $<
