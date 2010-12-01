// This file is part of OpenPanel - The Open Source Control Panel
// OpenPanel is free software: you can redistribute it and/or modify it 
// under the terms of the GNU General Public License as published by the Free 
// Software Foundation, using version 3 of the License. The library
//
// Please note that use of the OpenPanel trademark may be subject to additional 
// restrictions. For more information, please visit the Legal Information 
// section of the OpenPanel website on http://www.openpanel.com/

#include "cfgiptables.h"

#include <grace/file.h>
#include <grace/filesystem.h>

using namespace iptables;



//	==========================================================================
/// METHOD: cfgiptables::buildfromval
//	==========================================================================
void cfgiptables::buildfromval (const value &v)
{
	string &out = _cfglines;
	
	out.crop();
	out += "#!/bin/sh\n#\n# Openpanel chains\n#\n";
	
    out += "################################################################################\n";
    out += "## WARNING: AUTOMATICALLY GENERATED                                           ##\n";
    out += "## This file was automatically generated by OpenPanel. Manual changes to this ##\n";
    out += "## file will be lost the next time this file is generated.                    ##\n";
    out += "################################################################################\n";
    out += "\n";
	
	out += "IPTABLES=\"/sbin/iptables\"\n";
	out += "\n# Flush and Delete existing chaines\n";

	out += "$IPTABLES -F openpanel || /bin/true\n";	
	out += "$IPTABLES -F openpanel-allow || /bin/true\n";
	out += "$IPTABLES -F openpanel-deny || /bin/true\n";
	out += "$IPTABLES -F INPUT\n";
	out += "$IPTABLES -X openpanel || /bin/true\n";
	out += "$IPTABLES -X openpanel-allow || /bin/true\n";
	out += "$IPTABLES -X openpanel-deny || /bin/true\n\n\n";
	
	if (v["enabled"] == "false")
	{
		out += "$IPTABLES -P INPUT ACCEPT";
		return;
	}

	// Ad global rules:
	// * Flush existing chains
	// * Add global chains
	out += "\n# Create new chains\n";
	out += "$IPTABLES -N openpanel\n";
	out += "$IPTABLES -N openpanel-allow\n";
	out += "$IPTABLES -N openpanel-deny\n\n\n\n";
	
	
	// Configure accept / deny chains
	out += "# Configure openpanel accept/deny behaviour\n";
	out += "$IPTABLES -A openpanel-allow -j ACCEPT\n";
	
	// Define deny policy
	if (_method == method_reject)
		out += "$IPTABLES -A openpanel-deny -j REJECT\n\n\n";
	else
		out += "$IPTABLES -A openpanel-deny -j DROP\n\n\n";

	out += "# Sysadmin defined policies through openpanel\n";

	foreach (port, v["IPTables:Port"])
	{
		int dport;
		
		dport = port.id().sval().toint();
		
		foreach (rule, port["IPTables:Port:Rule"])
		{
			string target;
			string srcip;
			string nmask;
			
			srcip = rule["ip"].sval().filter ("0123456789.");
			nmask = rule["subnet"].sval().filter ("0123456789.");
			
			if (! nmask) nmask = "255.255.255.255";
			
			caseselector (rule["state"])
			{
				incaseof ("permit") : target = "openpanel-allow"; break;
				incaseof ("deny") : target = "openpanel-deny"; break;
				defaultcase : target = "openpanel-deny"; break;
			}
			
            if (port["filter"] == "tcp" || 
                port["filter"] == "tcp-udp")
            {
                out += "$IPTABLES -A openpanel -p tcp -s %s/%s --dport %i "
                       "-j %s\n" %format (srcip, nmask, dport, target);
            }
            if (port["filter"] == "udp" || 
                port["filter"] == "tcp-udp")
            {
                out += "$IPTABLES -A openpanel -p udp -s %s/%s --dport %i "
                       "-j %s\n" %format (srcip, nmask, dport, target);
            }
		}
		
		string ptarget;
		caseselector (port["state"])
		{
			incaseof ("permit") : ptarget = "openpanel-allow"; break;
			incaseof ("deny") : ptarget = "openpanel-deny"; break;
			defaultcase : ptarget = "openpanel-allow"; break;
		}
	
		// Add global port rule to the 
		// ip table script
		
		if (port["filter"] == "tcp" || 
			port["filter"] == "tcp-udp")
		{
			out += "$IPTABLES -A openpanel -p tcp -s 0.0.0.0/0 --dport %i "
				   "-j %s\n" %format (dport, ptarget);
		}
		if (port["filter"] == "udp" || 
				 port["filter"] == "tcp-udp")
		{
			out += "$IPTABLES -A openpanel -p udp -s 0.0.0.0/0 --dport %i "
				   "-j %s\n" %format (dport, ptarget);
		}

	}

	// * Add global policy	
	out += "$IPTABLES -I INPUT -m state --state RELATED,ESTABLISHED "
		   "-j ACCEPT\n";
	out += "$IPTABLES -A INPUT -i lo -j openpanel-allow\n";
	out += "$IPTABLES -A INPUT -j openpanel\n";
	
	if (v["blockall"] == "true")
	{
		out += "$IPTABLES -P INPUT DROP";
	}
	else
	{
		out += "$IPTABLES -P INPUT ACCEPT";
	}
}



//	==========================================================================
/// METHOD: cfgiptables::writefile
//	==========================================================================
bool cfgiptables::writefile (void)
{
	fs.save (_conffile, _cfglines);
	return true;
}

		
