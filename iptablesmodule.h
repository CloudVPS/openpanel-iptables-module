// This file is part of OpenPanel - The Open Source Control Panel
// OpenPanel is free software: you can redistribute it and/or modify it 
// under the terms of the GNU General Public License as published by the Free 
// Software Foundation, using version 3 of the License. The library
//
// Please note that use of the OpenPanel trademark may be subject to additional 
// restrictions. For more information, please visit the Legal Information 
// section of the OpenPanel website on http://www.openpanel.com/

#ifndef _iptablesmodule_H
#define _iptablesmodule_H 1

#include <moduleapp.h>
#include <grace/system.h>
#include <grace/configdb.h>


typedef configdb<class iptablesmodule> appconfig;

//  -------------------------------------------------------------------------
/// Main application class.
//  -------------------------------------------------------------------------
class iptablesmodule : public moduleapp
{
public:
			 // ===============================================
			 /// Constructor
			 // ===============================================
		 	 iptablesmodule (void) :
				moduleapp ("openpanel.module.iptables"),
				conf (this)
			 {
			 }
			 
			 // ===============================================
			 /// Destructor
			 // ===============================================
			~iptablesmodule (void)
			 {
			 }

	int		 main (void);
	
			 // ===============================================
			 // Sendresult event handler
			 // ===============================================
	void	 onsendresult (int code)
			 {
			 	// Rollback all changes if there 
			 	// was an authd error
			 	if ((code == moderr::err_authdaemon))
			 		authd.rollback ();
			 }
	
	
protected:

	appconfig		 conf;			///< Modules configuration data
	value			 networkconf;	///< Zone configuration

	
			 //	 =============================================
			 /// Configuration handler 
			 //	 =============================================
	bool     confSystem (config::action act, keypath &path, 
					  	 const value &nval, const value &oval);	
	
			 //	=============================================
			 /// validate the given configuration
			 /// \return true on ok / false on failure
			 //	=============================================
	bool	 checkconfig (value &v);	
	
			 //	=============================================
			 /// Read the current loaded network module
			 /// and sends the network configuration to the 
			 /// standard output or gives an error.
			 /// \return true on ok / false on failure
			 //	=============================================
	bool	 readconfiguration (void);


			 //	=============================================
			 /// Delete configuration
			 /// \return true on ok / false on failure
			 //	=============================================			 
	bool	 deleteconfiguration (const value &v);
	
	
			 //	=============================================
			 /// Install configuration using authd
			 /// \param v postdata
			 /// \return true on ok / false on failure
			 //	=============================================			 			 
	bool	 installconfig (const value &v);
	
	
			 //	=============================================
			 /// Reload services using authd
			 /// \return true on ok / false on failure
			 //	=============================================			 
	bool	 reloadservices (void);
	
	
			 //	=============================================
			 /// Commit iptables configuration
			 /// \return true on ok / false on failure
			 //	=============================================			 
	bool	 commitconfig (void);

	
			 //	=============================================
			 /// Revert iptables configuration, this cannot
			 /// be done after a configuration has already
			 /// been committed
			 /// \return true on ok / false on failure
			 //	=============================================			 
	bool	 revertconfig (void);

	
			 //	=============================================
			 /// Writes a network configuration
			 /// \param v given post data
			 /// \return true on ok / false on failure
			 //	=============================================
	bool 	 writeconfiguration (const value &v);
				
};

#endif
