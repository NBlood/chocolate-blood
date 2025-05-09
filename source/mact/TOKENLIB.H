// tokenlib.h

#ifndef _token_public
#define _token_public
#ifdef __cplusplus
extern "C" {
#endif

#define        MAXTOKEN    128
extern	char	token[MAXTOKEN];


//====================================================================
//
// TokenLib
//
//====================================================================
// TokenLib is a primitive file parser. The basic operation consists of
// the following:
// TOKEN_Parse or TOKEN_Load -- pre-loaded or not
// while (TOKEN_TokenAvailable())  -- while a token is available
//     TOKEN_Get
//     TOKEN_GetDouble
//     TOKEN_GetInteger
//     TOKEN_GetSpecific
// TOKEN_Free()
//
/*
==============
=
= TOKEN_GetLineNumber
=
= Get current line number in token file
=
==============
*/

int32 TOKEN_GetLineNumber( int32 tokenhandle );

/*
==============
=
= TOKEN_Reset
=
= Reset token file to the beginning
=
==============
*/

void TOKEN_Reset ( int32 tokenhandle );

/*
==============
=
= TOKEN_TokenAvailable
=
= Is there a token available?
= crossline determines whether or not to look past the current line
=
==============
*/

boolean TOKEN_TokenAvailable (int32 tokenhandle, boolean crossline);

/*
==============
=
= TOKEN_CommentAvailable
=
= Is there a token or a comment available
= crossline determines whether or not to look past the current line
=
==============
*/

boolean TOKEN_CommentAvailable (int32 tokenhandle, boolean crossline);

/*
==============
=
= TOKEN_UnGet
=
= Signals that the current token was not used, and should be reported
= for the next GetToken.  Note that
=
= TOKEN_Get (true);
= TOKEN_UnGet ();
= TOKEN_Get (false);
=
= could cross a line boundary.
=
==============
*/

void TOKEN_UnGet (int32 tokenhandle);

/*
==============
=
= TOKEN_Get
=
= Get the next token
= crossline determines whether or not to look past the current line
=
==============
*/

void TOKEN_Get
   (
   int32 tokenhandle,
   boolean crossline
   );

/*
==============
=
= TOKEN_GetLine
=
= Get the next token all the way to the end of the line
= crossline determines whether or not to look past the current line
=
==============
*/

void TOKEN_GetLine
   (
   int32 tokenhandle,
   boolean crossline
   );

/*
==============
=
= TOKEN_GetRaw
=
= Grab the next line raw
==============
*/

void TOKEN_GetRaw (int32 tokenhandle);

/*
==============
=
= TOKEN_GetSpecific
=
= Get a specific token.  Skips over the file until the right one is found
= returns false if it runs out of tokens.
= otherwise it returns true
=
==============
*/

boolean TOKEN_GetSpecific(int32 tokenhandle, char *string);

/*
==============
=
= TOKEN_GetInteger
=
= Get the next token and interpret it as a integer
= crossline determines whether or not to look past the current line
=
==============
*/

void TOKEN_GetInteger(int32 tokenhandle, int32 * number, boolean crossline);

/*
==============
=
= TOKEN_GetDouble
=
= Get the next token and interpret it as a double
= crossline determines whether or not to look past the current line
=
==============
*/

void TOKEN_GetDouble(int32 tokenhandle, double * number, boolean crossline);

/*
===================
=
= TOKEN_LinesInFile
=
= returns the number of lines in the token file
=
===================
*/
int32 TOKEN_LinesInFile( int32 tokenhandle );

/*
==============
=
= TOKEN_Free
=
= Free up a token file
=
==============
*/
void TOKEN_Free( int32 tokenhandle );

/*
==============
=
= TOKEN_Parse
=
= Parse the buffer and return a token file handle
= name refers to the name associated with this token file
=
==============
*/

int32 TOKEN_Parse ( const char *data, int32 length, const char * name );

/*
==============
=
= TOKEN_Load
=
= Load the file, parse it and return a token file handle
=
==============
*/

int32 TOKEN_Load ( const char * filename );

#ifdef __cplusplus
};
#endif
#endif
