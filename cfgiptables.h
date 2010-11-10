// This file is part of OpenPanel - The Open Source Control Panel
// OpenPanel is free software: you can redistribute it and/or modify it 
// under the terms of the GNU General Public License as published by the Free 
// Software Foundation, using version 3 of the License. The library
//
// Please note that use of the OpenPanel trademark may be subject to additional 
// restrictions. For more information, please visit the Legal Information 
// section of the OpenPanel website on http://www.openpanel.com/

#ifndef	_IPTABLES_H
#define _IPTABLES_H 1


#include <grace/str.h>
#include <grace/value.h>
#include <grace/array.h>

namespace iptables
{
	enum denymethod
	{
		method_reject	= 0x01,
		method_drop		= 0x02
	};
	
	enum filter
	{
		fil_udp			= 0x01,
		fil_tcp			= 0x02,
		fil_udptcp		= 0x03
	};
	
	enum method
	{
		allow			= 0x01,
		deny			= 0x00
	};
};


class cfgiptables
{

	public:
					 //  ==========================================
					 /// Constructor
					 //  ==========================================
					 cfgiptables (const string &conffile)
					 {
					 	this->_conffile = conffile;
					 	
					 }
					 //  ==========================================
					 /// Destructor
					 //  ==========================================
					~cfgiptables (void)
					 {
					 }


					 //  ==========================================
					 /// Set method used to deny traffic on a port
					 //  ==========================================
		void		 setmethod (iptables::denymethod method)
					 {
					 	_method = method;
					 }
		
		
		
					 //  ==========================================
					 /// Build configuration from an 
					 /// opencore request
					 //  ==========================================
		void		 buildfromval (const value &v);


						
					 //  ==========================================
					 /// Write configuration file
					 //  ==========================================
		bool		 writefile (void);

					 //  ==========================================
					 /// Get last error description
					 //  ==========================================
		const 
		string 		&error (void)
					 {
					 	return _error;
					 }
					 
					 //  ==========================================
					 /// Get last error code
					 //  ==========================================
		int			 errornum (void)
					 {
					 	return _errornum;
					 }


	private:
					 //  ==========================================
					 /// Deny all traffic by default
					 //  ==========================================
		void		 setdenyall (void)
					 {
					 	_denyall = true;
					 }

					 
					 //  ==========================================
					 /// Allow all traffic by default
					 //  ==========================================
		void		 setallowall (void)
					 {
					 	// default allow all 
					 	_denyall = false;
					 }
			

			
					 //  ==========================================					
					 /// Add port configuration
					 //  ==========================================
		void		 addport (int port, iptables::filter filter, 
							  iptables::method method);



				 	 //  ==========================================
					 /// Add port to configuration
					 //  ==========================================
		void		 addport (int port, iptables::filter filter, 
							  iptables::method method,
							  const string &ip, const string &netmask);

		string		 			_error;	
		int			 			_errornum;
		string					_conffile;
		
		// Firewall flags and data
		bool		 			_denyall;
		iptables::denymethod 	_method;
		string					_cfglines;
		
};


#endif
