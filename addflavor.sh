#!/bin/sh
# This file is part of OpenPanel - The Open Source Control Panel
# OpenPanel is free software: you can redistribute it and/or modify it 
# under the terms of the GNU General Public License as published by the Free 
# Software Foundation, using version 3 of the License.
#
# Please note that use of the OpenPanel trademark may be subject to additional 
# restrictions. For more information, please visit the Legal Information 
# section of the OpenPanel website on http://www.openpanel.com/

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
