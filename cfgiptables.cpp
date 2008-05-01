// --------------------------------------------------------------------------
// OpenPanel - The Open Source Control Panel
// Copyright (c) 2006-2007 PanelSix
//
// This software and its source code are subject to version 2 of the
// GNU General Public License. Please be aware that use of the OpenPanel
// and PanelSix trademarks and the IconBase.com iconset may be subject
// to additional restrictions. For more information on these restrictions
// and for a copy of version 2 of the GNU General Public License, please
// visit the Legal and Privacy Information section of the OpenPanel
// website on http://www.openpanel.com/
// --------------------------------------------------------------------------

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

		