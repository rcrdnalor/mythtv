#ifndef MARKUPCODES_H
#define MARKUPCODES_H
/*
	markupcodes.h

	(c) 2003 Thor Sigvaldason and Isaac Richards
	Part of the mythTV project
	
	Headers for mdcap markup codes

*/

#define MDCAP_PROTOCOL_VERSION_MAJOR 1
#define MDCAP_PROTOCOL_VERSION_MINOR 0

class MarkupCodes
{

  public:

    static MarkupCodes& theCodes(){static MarkupCodes mc; return mc;}

    //
    //  These are just a bunch of static const's that map from a human
    //  remember'able name into a short symbol
    //


    //
    //  Codes for containers that hold groups of items
    //

    static const char server_info_group     = 1;
    static const char login_group           = 2;
    static const char update_group          = 3;
    static const char collection_group      = 4;
    
    //
    //  Smaller items that implicitly imply groups (by virtue of being of
    //  variable length)
    //
    
    static const char name                  = 16;
    
    //
    //  Simple types of fixed lengths
    //

    static const char status_code           = 32;
    static const char protocol_version      = 33;
    static const char session_id            = 34;
    static const char collection_count      = 35;
    static const char collection_id         = 36;
    static const char collection_generation = 37;
    
    

  private:

    MarkupCodes();
    MarkupCodes(const MarkupCodes& rhs);

};

#endif
