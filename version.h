// version.h
//		Copyright (C) 2023 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2023/03/17  JeffyTS  	New edit.
//

#define VER_MAJOR		0
#define VER_MINOR		9
#define VER_BUILD_HI	4
#define VER_BUILD_LO	10
#define VER_FLAGS		VS_FF_PRERELEASE


// The Binary form of the version numbers
#define VER_FILE		VER_MAJOR, VER_MINOR, VER_BUILD_HI, VER_BUILD_LO
#define VER_PRODUCT		VER_MAJOR, VER_MINOR, VER_BUILD_HI, VER_BUILD_LO

#define VER_STR(arg)	#arg

// The String form of the version numbers
#define VER_FILE_STRING VALUE		"FileVersion",		"0.9.4.10"
#define VER_PRODUCT_STRING VALUE	"ProductVersion",	"0.9.4.10"
