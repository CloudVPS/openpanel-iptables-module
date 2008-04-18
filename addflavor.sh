#!/bin/sh
inflav=`cat makeinclude | grep ^CXXFLAGS | sed -e "s/.*__FLAVOR_//;s/ .*//"`
if [ "$inflav" = "LINUX_REDHAT" ]; then
 cp rsrc/openpanel.module.awstats.conf.xml.redhat rsrc/openpanel.module.awstats.conf.xml
 cp module.xml.redhat module.xml
elif [ "$inflav" = "LINUX_DEBIAN" ]; then
 cp rsrc/openpanel.module.awstats.conf.xml.debian  rsrc/openpanel.module.awstats.conf.xml
 cp module.xml.debian module.xml
else
 echo "No compatible environment found."
 exit 1
fi
