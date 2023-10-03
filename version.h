//
// version.h
//		Copyright (C) 2023 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2023/03/17  JeffyTS  	New edit.
//

#define MAJOR_VERSION		1
#define MINOR_VERSION		2
#define BUILD_VERSION		0
#define REVISION_VERSION	11
#define	VERSION				"1.2.0.11"

#define FILE_VERSION_STRING		VALUE	"FileVersion",		VERSION
#define PRODUCT_VERSION_STRING	VALUE	"ProductVersion",	VERSION

#define FILE_VERSION		MAJOR_VERSION, MINOR_VERSION, BUILD_VERSION, REVISION_VERSION
#define PRODUCT_VERSION		MAJOR_VERSION, MINOR_VERSION, BUILD_VERSION, REVISION_VERSION

/* = EOF = */