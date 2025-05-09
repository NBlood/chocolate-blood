// scriplib.h

#ifndef _scriplib_public
#define _scriplib_public
#ifdef __cplusplus
extern "C" {
#endif

/*
==============
=
= SCRIPT_Init
=
==============
*/
int32 SCRIPT_Init( const char * name );


/*
==============
=
= SCRIPT_Free
=
==============
*/
void SCRIPT_Free( int32 scripthandle );

/*
==============
=
= SCRIPT_Parse
=
==============
*/

int32 SCRIPT_Parse ( char *data, int32 length, char * name );


/*
==============
=
= SCRIPT_Load
=
==============
*/

int32 SCRIPT_Load ( const char * filename );

/*
==============
=
= SCRIPT_Save
=
==============
*/
void SCRIPT_Save (int32 scripthandle, const char * filename);


/*
==============
=
= SCRIPT_NumberSections
=
==============
*/

int32 SCRIPT_NumberSections( int32 scripthandle );

/*
==============
=
= SCRIPT_Section
=
==============
*/

char * SCRIPT_Section( int32 scripthandle, int32 which );

/*
==============
=
= SCRIPT_NumberEntries
=
==============
*/

int32 SCRIPT_NumberEntries( int32 scripthandle, const char * sectionname );

/*
==============
=
= SCRIPT_Entry
=
==============
*/

const char * SCRIPT_Entry( int32 scripthandle, const char * sectionname, int32 which );


/*
==============
=
= SCRIPT_GetRaw
=
==============
*/
const char * SCRIPT_GetRaw(int32 scripthandle, const char * sectionname, const char * entryname);

/*
==============
=
= SCRIPT_GetString
=
==============
*/
void SCRIPT_GetString
   (
   int32 scripthandle,
   const char * sectionname,
   const char * entryname,
   char * dest
   );

/*
==============
=
= SCRIPT_GetDoubleString
=
==============
*/
void SCRIPT_GetDoubleString
   (
   int32 scripthandle,
   const char * sectionname,
   const char * entryname,
   char * dest1,
   char * dest2
   );

/*
==============
=
= SCRIPT_GetNumber
=
==============
*/
boolean SCRIPT_GetNumber
   (
   int32 scripthandle,
   const char * sectionname,
   const char * entryname,
   int32 * number
   );

/*
==============
=
= SCRIPT_GetBoolean
=
==============
*/
void SCRIPT_GetBoolean
   (
   int32 scripthandle,
   const char * sectionname,
   const char * entryname,
   boolean * _bool
   );

/*
==============
=
= SCRIPT_GetDouble
=
==============
*/

void SCRIPT_GetDouble
   (
   int32 scripthandle,
   const char * sectionname,
   const char * entryname,
   double * number
   );



/*
==============
=
= SCRIPT_PutComment
=
==============
*/
void SCRIPT_PutComment( int32 scripthandle, const char * sectionname, const char * comment );

/*
==============
=
= SCRIPT_PutEOL
=
==============
*/
void SCRIPT_PutEOL( int32 scripthandle, const char * sectionname );

/*
==============
=
= SCRIPT_PutMultiComment
=
==============
*/
void SCRIPT_PutMultiComment
   (
   int32 scripthandle,
   const char * sectionname,
   const char * comment,
   ...
   );

/*
==============
=
= SCRIPT_PutSection
=
==============
*/
void SCRIPT_PutSection( int32 scripthandle, const char * sectionname );

/*
==============
=
= SCRIPT_PutRaw
=
==============
*/
void SCRIPT_PutRaw
   (
   int32 scripthandle,
   const char * sectionname,
   const char * entryname,
   const char * raw
   );

/*
==============
=
= SCRIPT_PutString
=
==============
*/
void SCRIPT_PutString
   (
   int32 scripthandle,
   const char * sectionname,
   const char * entryname,
   const char * string
   );

/*
==============
=
= SCRIPT_PutDoubleString
=
==============
*/
void SCRIPT_PutDoubleString
   (
   int32 scripthandle,
   const char * sectionname,
   const char * entryname,
   const char * string1,
   const char * string2
   );

/*
==============
=
= SCRIPT_PutNumber
=
==============
*/
void SCRIPT_PutNumber
   (
   int32 scripthandle,
   const char * sectionname,
   const char * entryname,
   int32 number,
   boolean hexadecimal,
   boolean defaultvalue
   );

/*
==============
=
= SCRIPT_PutBoolean
=
==============
*/
void SCRIPT_PutBoolean
   (
   int32 scripthandle,
   const char * sectionname,
   const char * entryname,
   boolean _bool
   );

/*
==============
=
= SCRIPT_PutDouble
=
==============
*/

void SCRIPT_PutDouble
   (
   int32 scripthandle,
   const char * sectionname,
   const char * entryname,
   double number,
   boolean defaultvalue
   );


#ifdef __cplusplus
};
#endif
#endif
