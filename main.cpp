// This file is part of OpenPanel - The Open Source Control Panel
// OpenPanel is free software: you can redistribute it and/or modify it 
// under the terms of the GNU General Public License as published by the Free 
// Software Foundation, using version 3 of the License. The library
//
// Please note that use of the OpenPanel trademark may be subject to additional 
// restrictions. For more information, please visit the Legal Information 
// section of the OpenPanel website on http://www.openpanel.com/

#include <openpanel-core/moduleapp.h>
#include "iptablesmodule.h"

#include <grace/file.h>
#include <grace/filesystem.h>

#include "cfgiptables.h"

using namespace moderr;

APPOBJECT(iptablesmodule);

//  =========================================================================
/// Main method.
//  =========================================================================
int iptablesmodule::main (void)
{
	string conferr;

	// Add configuration watcher
	conf.addwatcher ("config", &iptablesmodule::confSystem);


   // Load will fail if watchers did not valiate.
    if (! conf.load ("openpanel.module.iptables", conferr))
    {   
        ferr.printf ("%% Error loading configuration: %s\n", conferr.str());
        return 1;
    } 		
     		
	caseselector (data["OpenCORE:Command"])
	{
		incaseof ("create") : 
			if (! checkconfig (data)) return 0;
			if (! writeconfiguration (data)) return 0;
			break;

		incaseof ("update") :
			if (! checkconfig (data)) return 0;
			if (! writeconfiguration (data)) return 0;
			break;
			
		incaseof ("getconfig") :
			if (! readconfiguration ()) return 0;
			break;


		incaseof ("delete") :
			if (! checkconfig (data)) return 0;
			if (! writeconfiguration (data)) return 0;
			break;
		 	
		incaseof ("validate") : ;
		incaseof ("commit") : 
			if (! commitconfig ()) return 0;
			break;
			
		incaseof ("revert") : 
			if (! revertconfig ()) return 0;
			break;
			
		defaultcase:
			sendresult (err_command, "Unsupported command");
			return 0;
	}

	authd.quit ();
	sendresult (moderr::ok, "");

	return 0;
}


//	=============================================
//	METHOD: iptablesmodule::commitconfig
//	=============================================
bool iptablesmodule::commitconfig (void)
{
	// // Send to authd: iptables save
	// if( authd.saveservice (conf["config"]["iptables:servicename"]))
	// {
	// 	sendresult( moderr::err_authdaemon, 
	// 				"Error saving service's configuration");
	// 	
	// 	return false;
	// }
	
	return true;
}


//	=============================================
//	METHOD: iptablesmodule::commitconfig
//	=============================================
bool iptablesmodule::revertconfig (void)
{
	// // Send to authd: iptables restore
	// if( authd.restoreservice (conf["config"]["iptables:servicename"]))
	// {
	// 	sendresult( moderr::err_authdaemon, 
	// 				"Error restoring service's configuration");
	// 	
	// 	return false;
	// }
	
	return true;
}

//	=============================================
//	METHOD: iptablesmodule::readconfiguration
//	=============================================
bool iptablesmodule::readconfiguration (void)
{
	value retval = $type("opencore.module") ->
	               $("IPTables",
	               		$attr("type", "class") ->
	               		$("firewall",
	               			$attr("type", "object") ->
	               			$("enabled", false) ->
	               			$("blockall", true) ->
	               			$("IPTables:Port",
	               				$attr("type", "class") ->
	               				$("21",
	               					$("description", "FTP") ->
	               					$("state", "permit") ->
	               					$("filter", "tcp")
	               				 ) ->
	               				$("22",
	               					$("description", "Secure Shell (SSH)") ->
	               					$("state", "permit") ->
	               					$("filter", "tcp")
	               				 ) ->
	               				$("25",
	               					$("description", "Incoming E-mail (SMTP)") ->
	               					$("state", "permit") ->
	               					$("filter", "tcp")
	               				 ) ->
	               				$("53",
	               					$("description","DNS") ->
	               					$("state", "permit") ->
	               					$("filter", "tcp-udp")
	               				 ) ->
	               				$("80",
	               					$("description","Websites (HTTP)") ->
	               					$("state", "permit") ->
	               					$("filter", "tcp")
	               				 ) ->
	               				$("110",
	               					$("description","E-mail (POP3)") ->
	               					$("state", "permit") ->
	               					$("filter", "tcp")
	               				 ) ->
	               				$("111",
	               					$("description","Sun RPC service") ->
	               					$("state", "deny") ->
	               					$("filter", "tcp-udp")
	               				 ) ->
	               				$("143",
	               					$("description","E-mail (IMAP)") ->
	               					$("state", "permit") ->
	               					$("filter", "tcp")
	               				 ) ->
	               				$("443",
	               					$("description","Websites (HTTPS)") ->
	               					$("state", "permit") ->
	               					$("filter", "tcp")
	               				 ) ->
	               				$("993",
	               					$("description", "E-mail (IMAPS)") ->
	               					$("state", "permit") ->
	               					$("filter", "tcp")
	               				 ) ->
	               				$("4089",
	               					$("description","OpenPanel Interface") ->
	               					$("state", "permit") ->
	               					$("filter", "tcp")
	               				 )
	               			 )
	               		 )
	               	  ) ->
	               	 $("OpenCORE:Result",
	               	 	$("code", moderr::ok) ->
	               	 	$("message", "OK")
	               	  );
	
	string result;
	result = retval.toxml (false);
	fout.printf ("%u\n", result.strlen());
	fout.puts (result);	
	
	return true;
}


//	=============================================
//	METHOD: iptablesmodule::readconfiguration
//	=============================================
bool iptablesmodule::deleteconfiguration (const value &v)
{
	// 
	// No configuration can be deleted, this module
	// only uses updates
	//

	return true;
}



//	==============================================
//	METHOD: iptablesmodule::writeconfiguration
//	==============================================
bool iptablesmodule::writeconfiguration (const value &v)
{
	string filename;
	filename.printf ("%s/%s", conf["config"]["local:staging"].cval(),
							  conf["config"]["iptables:rcfilename"].cval());
	cfgiptables cfgip (filename);
	
	cfgip.buildfromval (v["IPTables"]);
	cfgip.writefile ();


	return installconfig (v);
}



//  =========================================================================
/// domainModule::installconfig
//  =========================================================================
bool iptablesmodule::installconfig (const value &v)
{

	// Install shell script using authd
	if (authd.installfile (conf["config"]["iptables:rcfilename"], 
						   conf["config"]["iptables:destdir"]))
	{
		sendresult( moderr::err_authdaemon, 
					"Error installing iptables configuration script");
		
		return false;
	}
	
	// todo; replace true by real command
	if (authd.runscript (conf["config"]["authd:scriptname"], ""))
	{
		sendresult( moderr::err_authdaemon, 
					"Error executing iptables configuration script");
		
		return false;
	}

	return true;
}



//  =========================================================================
/// domainModule::reloadservices
//  =========================================================================
bool iptablesmodule::reloadservices (void)
{
	// Unused
	// Reload of iptables is not necessary.
	
	return true;
}



//  =========================================================================
/// domainModule::checkconfig
//  =========================================================================
bool iptablesmodule::checkconfig (value &vv)
{
	value &v = vv["IPTables"];
	// TODO:
	
	// We will get the full iptables configuration from opencore,
	// the whole tree will be nested and we have to check if the
	// three does fit all our requirements to function well
	
	// Check IPTables's class content
	//
	// .. TODO ..
	// The folowing fields need to exist:
	// * enabled: true / false
	// * blockall: true / false
	
	if (! v.exists ("enabled"))
	{
		sendresult (moderr::err_value, 
					"No item 'enabled' found in class IPTables");
		return false;
	}
	
	if (! v.exists ("blockall"))
	{
		sendresult (moderr::err_value, 
					"No item 'blockall' found in class IPTables");	
		return false;
	}
	
	foreach (port, v["IPTables:Port"])
	{
	
		// .. TODO ..
		// The folowing fields need to exist:
		// * description (not required by the module)
		// * state (openpanel-allow / openpanel-deny)
		// The id needs to be > 0 and 
		
		if (! port.exists ("state"))
		{
			sendresult (moderr::err_value, 
						"No item 'state' found in class IPTables:Port:Rule");	
			return false;
		}
		else
		{
			// Check value on openpanel-(deny/allow)
			if ((port["state"] != "permit") &&
				(port["state"] != "deny"))
			{
				sendresult (moderr::err_value, 
							"Invalid value in item 'state' in class IPTables:Port");	
				return false;
			}	
		}
		
		if ((port.id().sval().toint() < 0)||(port.id().sval().toint() > 65535))
		{
			sendresult (moderr::err_value, 
						"Illegal portnum specified in class IPTables:Port");	
			return false;
		}
		
		foreach (rule, port["IPTables:Port:Rule"])
		{
			// .. TODO ..
			// The folowing fields need to exist
			// * ip: xxx.xxx.xxx.xxx/x(x)
			// * state (openpanel-allow / openpanel-deny)
			
			if (! rule.exists ("ip"))
			{
				sendresult (moderr::err_value, 
							"No item 'ip' found in class IPTables:Port");	
				return false;
			}
			else
			{
				// Check format..
				// TODO:
			}
			
			if (! rule.exists ("state"))
			{
				sendresult (moderr::err_value, 
							"No item 'state' found in class IPTables:Port:Rule");	
				return false;
			}
			else
			{
				// Check value on openpanel-(deny/allow)
				if (rule["state"] != "permit" &&
					rule["state"] != "deny")
				{
					sendresult (moderr::err_value, 
						"Invalid value in item 'state' in class IPTables:Port:Rule");	
					return false;
				}	
			}
		}
	}
	
	return true;
}



//  =========================================================================
/// Configuration watcher for the event log.
//  =========================================================================
bool iptablesmodule::confSystem (config::action act, keypath &kp,
                const value &nval, const value &oval)
{
	switch (act)
	{
		case config::isvalid:
			return true;

		case config::create:
			return true;		
	}

	return false;
}
