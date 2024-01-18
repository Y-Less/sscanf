/*
 *  sscanf 2.15.1
 *
 *  Version: MPL 1.1
 *
 *  The contents of this file are subject to the Mozilla Public License Version
 *  1.1 (the "License"); you may not use this file except in compliance with
 *  the License. You may obtain a copy of the License at
 *  http://www.mozilla.org/MPL/
 *
 *  Software distributed under the License is distributed on an "AS IS" basis,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 *  for the specific language governing rights and limitations under the
 *  License.
 *
 *  The Original Code is the sscanf 2.0 SA:MP plugin.
 *
 *  The Initial Developer of the Original Code is Alex "Y_Less" Cole.
 *  Portions created by the Initial Developer are Copyright (c) 2022
 *  the Initial Developer. All Rights Reserved.
 *
 *  Contributor(s):
 *
 *      Cheaterman
 *      DEntisT
 *      Emmet_
 *      karimcambridge
 *      kalacsparty
 *      Kirima
 *      leHeix
 *      maddinat0r
 *      Southclaws
 *      Y_Less
 *      ziggi
 *
 *  Special Thanks to:
 *
 *      SA:MP Team past, present, and future.
 *      maddinat0r, for hosting the repo for a very long time.
 *      Emmet_, for his efforts in maintaining it for almost a year.
 */

#include <malloc.h>
#include <string.h>
#include <stdarg.h>

#include "sscanf.h"
#include "args.h"
#include "specifiers.h"
#include "utils.h"
#include "data.h"
#include "array.h"
#include "enum.h"

#include "amx/plugincommon.h"
#include <sdk.hpp>
#include <Server/Components/Pawn/pawn.hpp>
#include "subhook/subhook.h"

#define DEFER_STRINGISE(n) #n
#define STRINGISE(n) DEFER_STRINGISE(n)

#define SSCANF_VERSION STRINGISE(SSCANF_VERSION_MAJOR) "." STRINGISE(SSCANF_VERSION_MINOR) "." STRINGISE(SSCANF_VERSION_BUILD)

//----------------------------------------------------------

logprintf_t
	logprintf,
	real_logprintf;

AMX_NATIVE
	SetPlayerName = NULL;

typedef int AMXAPI(*amx_Register_t)(AMX* amx, const AMX_NATIVE_INFO* nativelist, int number);

subhook_t amx_Register_hook;

// These are the pointers to all the functions currently used by sscanf.  If
// more are added, this table will need to be updated.
static void *
	NPC_AMX_FUNCTIONS[] = {
		NULL,                       // PLUGIN_AMX_EXPORT_Align16
		NULL,                       // PLUGIN_AMX_EXPORT_Align32
		NULL,                       // PLUGIN_AMX_EXPORT_Align64
		NULL,                       // PLUGIN_AMX_EXPORT_Allot
		NULL,                       // PLUGIN_AMX_EXPORT_Callback
		NULL,                       // PLUGIN_AMX_EXPORT_Cleanup
		NULL,                       // PLUGIN_AMX_EXPORT_Clone
		(void *)&npcamx_Exec,       // PLUGIN_AMX_EXPORT_Exec
		NULL,                       // PLUGIN_AMX_EXPORT_FindNative
		(void *)&npcamx_FindPublic, // PLUGIN_AMX_EXPORT_FindPublic
		NULL,                       // PLUGIN_AMX_EXPORT_FindPubVar
		NULL,                       // PLUGIN_AMX_EXPORT_FindTagId
		NULL,                       // PLUGIN_AMX_EXPORT_Flags
		(void *)&npcamx_GetAddr,    // PLUGIN_AMX_EXPORT_GetAddr
		NULL,                       // PLUGIN_AMX_EXPORT_GetNative
		NULL,                       // PLUGIN_AMX_EXPORT_GetPublic
		NULL,                       // PLUGIN_AMX_EXPORT_GetPubVar
		(void *)&npcamx_GetString,  // PLUGIN_AMX_EXPORT_GetString
		NULL,                       // PLUGIN_AMX_EXPORT_GetTag
		NULL,                       // PLUGIN_AMX_EXPORT_GetUserData
		NULL,                       // PLUGIN_AMX_EXPORT_Init
		NULL,                       // PLUGIN_AMX_EXPORT_InitJIT
		NULL,                       // PLUGIN_AMX_EXPORT_MemInfo
		NULL,                       // PLUGIN_AMX_EXPORT_NameLength
		NULL,                       // PLUGIN_AMX_EXPORT_NativeInfo
		NULL,                       // PLUGIN_AMX_EXPORT_NumNatives
		NULL,                       // PLUGIN_AMX_EXPORT_NumPublics
		NULL,                       // PLUGIN_AMX_EXPORT_NumPubVars
		NULL,                       // PLUGIN_AMX_EXPORT_NumTags
		NULL,                       // PLUGIN_AMX_EXPORT_Push
		NULL,                       // PLUGIN_AMX_EXPORT_PushArray
		(void *)&npcamx_PushString, // PLUGIN_AMX_EXPORT_PushString
		NULL,                       // PLUGIN_AMX_EXPORT_RaiseError
		(void *)&npcamx_Register,   // PLUGIN_AMX_EXPORT_Register
		(void *)&npcamx_Release,    // PLUGIN_AMX_EXPORT_Release
		NULL,                       // PLUGIN_AMX_EXPORT_SetCallback
		NULL,                       // PLUGIN_AMX_EXPORT_SetDebugHook
		(void *)&npcamx_SetString,  // PLUGIN_AMX_EXPORT_SetString
		NULL,                       // PLUGIN_AMX_EXPORT_SetUserData
		(void *)&npcamx_StrLen,     // PLUGIN_AMX_EXPORT_StrLen
		NULL,                       // PLUGIN_AMX_EXPORT_UTF8Check
		NULL,                       // PLUGIN_AMX_EXPORT_UTF8Get
		NULL,                       // PLUGIN_AMX_EXPORT_UTF8Len
		NULL,                       // PLUGIN_AMX_EXPORT_UTF8Put
};

extern "C" void *
	pAMXFunctions;

extern unsigned int
	g_iTrueMax,
	g_iInvalid,
	g_iMaxPlayerName;

extern int *
	g_iConnected;

extern int *
	g_iNPC;

extern char *
	g_szPlayerNames;

extern int
	gAlpha,
	gForms;

extern E_SSCANF_OPTIONS
	gOptions;

AMX *
	g_aCurAMX;

//extern char
//	** g_pServer;

#define SAVE_VALUE(m)       \
	if (doSave)             \
		*args.Next() = m

#define SAVE_VALUE_F(m)     \
	if (doSave) {           \
		float f = (float)m; \
		*args.Next() = amx_ftoc(f); }

static char
	emptyString[1] = { '\0' };

// Based on amx_StrParam but using 0 length strings.  This can't be inline as
// it uses alloca - it could be written to use malloc instead, but that would
// require memory free code all over the place!
#define STR_PARAM(amx,param,result)                                                          \
	do {                                                                                     \
		cell * amx_cstr_; int amx_length_;                                                   \
		amx_GetAddr((amx), (param), &amx_cstr_);                                             \
		amx_StrLen(amx_cstr_, &amx_length_);                                                 \
		if (amx_length_ > 0) {                                                               \
			if (((result) = (char *)alloca((amx_length_ + 1) * sizeof (*(result)))) != NULL) \
				amx_GetString((result), amx_cstr_, sizeof (*(result)) > 1, amx_length_ + 1); \
			else {                                                                           \
				SscanfError(43, "Unable to allocate memory.");                               \
				return SSCANF_FAIL_RETURN; } }                                               \
		else (result) = emptyString; }                                                       \
	while (false)

#define SAFE_STR_PARAM(amx,param,result)                                                     \
	do {                                                                                     \
		cell * amx_cstr_; int amx_length_;                                                   \
		amx_GetAddr((amx), (param), &amx_cstr_);                                             \
		amx_StrLen(amx_cstr_, &amx_length_);                                                 \
		if (amx_length_ > 0) {                                                               \
			if (((result) = (char *)alloca((amx_length_ + 1) * sizeof (*(result)))) != NULL) \
				amx_GetString((result), amx_cstr_, sizeof (*(result)) > 1, amx_length_ + 1); \
			else {                                                                           \
				logprintf("sscanf error: Unable to allocate memory.");                       \
				SetErrorCode(43);                                                            \
				return SSCANF_FAIL_RETURN; } }                                               \
		else (result) = emptyString; }                                                       \
	while (false)

#define LENGTH_STR_PARAM(amx,param,result,length)                                            \
	do {                                                                                     \
		cell * amx_cstr_;                                                                    \
		amx_GetAddr((amx), (param), &amx_cstr_);                                             \
		amx_StrLen(amx_cstr_, &length);                                                      \
		if (length > 0) {                                                                    \
			if (((result) = (char *)alloca((length + 1) * sizeof (*(result)))) != NULL)      \
				amx_GetString((result), amx_cstr_, sizeof (*(result)) > 1, length + 1);      \
			else {                                                                           \
				logprintf("sscanf error: Unable to allocate memory.");                       \
				SetErrorCode(43);                                                            \
				return SSCANF_FAIL_RETURN; } }                                               \
		else (result) = emptyString; }                                                       \
	while (false)

// Macros for the regular values.
#define DO(m,n)                                           \
	{m b;                                                 \
	if (Do##n(&string, &b)) {                             \
		SAVE_VALUE((cell)b);                              \
		break; }                                          \
	return SSCANF_FAIL_RETURN; }

#define DOV(m,n)                 \
	{m b;                        \
	Do##n(&string, &b);          \
	SAVE_VALUE((cell)b); }

#define DOF(m,n)                                          \
	{m b;                                                 \
	if (Do##n(&string, &b)) {                             \
		SAVE_VALUE_F(b)                                   \
		break; }                                          \
	return SSCANF_FAIL_RETURN; }

// Macros for the default values.  None of these have ifs as the return value
// of GetReturnDefault is always true - we don't penalise users for the
// mistakes of the coder - they will get warning messages if they get the
// format wrong, and I don't know of any mistakes which aren't warned about
// (admittedly a silly statement as if I did I would have fixed them).
#define DE(m,n)                    \
	{m b;                          \
	switch (Do##n##D(&format, &b)) \
	{							   \
	case -1:					   \
		b = (m)*args.Next();       \
		if (!args.HasMore()) {     \
			SscanfWarning(47, "Format specifier does not match parameter count."); \
			return SSCANF_FAIL_RETURN; } \
	case 1:						   \
		SAVE_VALUE((cell)b);       \
	}							   \
	break; }

#define DEF(m,n)                    \
	{m b;                           \
	switch (Do##n##D(&format, &b))  \
	{							    \
	case -1:					    \
		b = amx_ctof(*args.Next()); \
		if (!args.HasMore()) {     \
			SscanfWarning(47, "Format specifier does not match parameter count."); \
			return SSCANF_FAIL_RETURN; } \
	case 1:						    \
		SAVE_VALUE_F(b);            \
	}							    \
	break; }

// Macros for the default values in the middle of a string so you can do:
// 
// sscanf("hello, , 10", "p<,>sI(42)i", str, var0, var1);
// 
// Note that optional parameters in the middle of a string only work with
// explicit (i.e. not whitespace) delimiters.
#define DX(m,n)                        \
	if (IsDelimiter(*string)) {        \
		m b;                           \
		switch (Do##n##D(&format, &b)) \
		{							   \
		case -1:					   \
			b = (m)*args.Next();       \
			if (!args.HasMore()) {     \
				SscanfWarning(47, "Format specifier does not match parameter count."); \
				return SSCANF_FAIL_RETURN; } \
		case 1:						   \
			SAVE_VALUE((cell)b);       \
		}							   \
		break; }                       \
	if (SkipDefault(&format))          \
		args.Next();

#define DXF(m,n)                        \
	if (IsDelimiter(*string)) {         \
		m b;                            \
		switch (Do##n##D(&format, &b))  \
		{							    \
		case -1:					    \
			b = amx_ctof(*args.Next()); \
			if (!args.HasMore()) {     \
				SscanfWarning(47, "Format specifier does not match parameter count."); \
				return SSCANF_FAIL_RETURN; } \
		case 1:						    \
			SAVE_VALUE_F(b);            \
		}							    \
		break; }                        \
	if (SkipDefault(&format))           \
		args.Next();

bool
	DoK(AMX * amx, char ** defaults, char ** input, cell * cptr, bool optional, bool all);

void
	SetOptions(char *, cell);

cell
	GetOptions(char *);

char
	* gFormat = 0;

const char
	* gCallFile = 0;

int
	gCallLine = 0;

cell
	* gCallResolve = 0;

bool SscanfErrLine()
{
	if (gCallResolve == 0)
	{
		gCallFile = "sscanf";
		gCallLine = -1;
	}
	else if (gCallFile == 0)
	{
		// Extract the filename.
		int length;
		amx_StrLen(gCallResolve, &length);
		if (length > 0)
		{
			if ((gCallFile = (const char *)malloc((length + 1))) != NULL)
			{
				amx_GetString((char *)gCallFile, gCallResolve, false, length + 1);
			}
			else
			{
				logprintf("sscanf error: Unable to allocate memory.");
				SetErrorCode(43);
				gCallFile = "sscanf";
				gCallLine = -1;
				gCallResolve = 0;
			}
		}
		else
		{
			gCallFile = "sscanf";
			gCallLine = -1;
			gCallResolve = 0;
		}
	}
	return gCallLine > 0;
}

static cell
	Sscanf(AMX * amx, char * string, char * format, cell const * params, const int paramCount)
{
	struct args_s
		args{ amx, params, 0, 0, paramCount };
	// Check for CallRemoteFunction style null strings and correct.
	if (string[0] == '\1' && string[1] == '\0')
	{
		string[0] = '\0';
	}
	// Save the default options so we can have local modifications.
	InitialiseDelimiter();
	// Skip leading space.
	SkipWhitespace(&string);
	bool
		doSave;
	// Code for the rare cases where the WHOLE format is quiet.
	if (*format == '{')
	{
		++format;
		doSave = false;
	}
	else
	{
		doSave = true;
	}
	g_aCurAMX = amx;
	// Now do the main loop as long as there are variables to store the data in
	// and input string remaining to get the data from.
	while (*string && (args.HasMore() || !doSave))
	{
		if (!*format)
		{
			// End of the format string - if we're here we've got all the
			// parameters but there is extra string or variables, which may
			// indicate their code needs fixing, for example:
			// sscanf(data, "ii", var0, var1, var3, var4);
			// There is only two format specifiers, but four returns.  This may
			// also be reached if there is too much input data, but that is
			// considered OK as that is likely a user's fault.
			if (args.HasMore())
			{
				SscanfWarning(47, "Format specifier does not match parameter count.");
			}
			if (!doSave)
			{
				// Started a quiet section but never explicitly ended it.
				SscanfWarning(48, "Unclosed quiet section.");
			}
			return SSCANF_TRUE_RETURN;
		}
		else if (IsWhitespace(*format))
		{
			++format;
		}
		else
		{
			IncErrorSpecifier();
			switch (*format++)
			{
			case 'L':
				DX(bool, L)
				// FALLTHROUGH
			case 'l':
				DOV(bool, L)
				break;
			case 'B':
				DX(int, B)
				// FALLTHROUGH
			case 'b':
				DO(int, B)
			case 'N':
				DX(int, N)
				// FALLTHROUGH
			case 'n':
				DO(int, N)
			case 'C':
				DX(char, C)
				// FALLTHROUGH
			case 'c':
				DO(char, C)
			case 'I':
			case 'D':
				DX(int, I)
				// FALLTHROUGH
			case 'i':
			case 'd':
				DO(int, I)
			case 'H':
			case 'X':
				DX(int, H)
				// FALLTHROUGH
			case 'h':
			case 'x':
				DO(int, H)
			case 'M':
				DX(unsigned int, M)
				// FALLTHROUGH
			case 'm':
				DO(unsigned int, M)
			case 'O':
				DX(int, O)
				// FALLTHROUGH
			case 'o':
				DO(int, O)
			case 'F':
				DXF(double, F)
				// FALLTHROUGH
			case 'f':
				DOF(double, F)
			case 'G':
				DXF(double, G)
				// FALLTHROUGH
			case 'g':
				DOF(double, G)
			case '{':
				if (doSave)
				{
					doSave = false;
				}
				else
				{
					// Already in a quiet section.
					SscanfWarning(20, "Can't have nestled quiet sections.");
				}
				continue;
			case '}':
				if (doSave)
				{
					SscanfWarning(21, "Not in a quiet section.");
				}
				else
				{
					doSave = true;
				}
				continue;
			case 'P':
				{
					ResetDelimiter();
					char *
						t = GetMultiType(&format);
					if (t) AddDelimiters(t);
					else return SSCANF_FAIL_RETURN;
					continue;
				}
				// FALLTHROUGH
			case 'p':
				// 'P' doesn't exist.
				// Theoretically, for compatibility, this should be:
				// p<delimiter>, but that will break backwards
				// compatibility with anyone doing "p<" to use '<' as a
				// delimiter (doesn't matter how rare that may be).  Also,
				// writing deprecation code and both the new and old code
				// is more trouble than it's worth, and it's slow.
				// UPDATE: I wrote the "GetSingleType" code for 'a' and
				// figured out a way to support legacy and new code, while
				// still maintaining support for the legacy "p<" separator,
				// so here it is:
				ResetDelimiter();
				AddDelimiter(GetSingleType(&format));
				continue;
			case 'S':
				if (IsDelimiter(*string))
				{
					char *
						dest;
					int
						length;
					DoSD(&format, &dest, &length, args);
					// Send the string to PAWN.
					if (doSave)
					{
						if (!args.HasMore())
						{
							SscanfWarning(47, "Format specifier does not match parameter count.");
							return SSCANF_FAIL_RETURN;
						}
						amx_SetString(args.Next(), dest, 0, 0, length);
					}
					break;
				}
				// Implicit "else".
				if (SkipDefault(&format))
				{
					SscanfError(64, "(*) is not supported in strings/arrays yet.");
					args.Next();
				}
				// FALLTHROUGH
			case 's':
				{
					// Get the length.
					int
						length = GetLength(&format, args);
					char *
						dest;
					DoS(&string, &dest, length, IsEnd(*format) || (!doSave && *format == '}' && IsEnd(*(format + 1))));
					// Send the string to PAWN.
					if (doSave)
					{
						if (!args.HasMore())
						{
							SscanfWarning(47, "Format specifier does not match parameter count.");
							return SSCANF_FAIL_RETURN;
						}
						amx_SetString(args.Next(), dest, 0, 0, length);
					}
				}
				break;
			case 'Z':
				if (IsDelimiter(*string))
				{
					char *
						dest;
					int
						length;
					DoSD(&format, &dest, &length, args);
					// Send the string to PAWN.
					if (doSave)
					{
						if (!args.HasMore())
						{
							SscanfWarning(47, "Format specifier does not match parameter count.");
							return SSCANF_FAIL_RETURN;
						}
						amx_SetString(args.Next(), dest, 1, 0, length);
					}
					break;
				}
				// Implicit "else".
				if (SkipDefault(&format))
				{
					SscanfError(64, "(*) is not supported in strings/arrays yet.");
					args.Next();
				}
				// FALLTHROUGH
			case 'z':
				{
					// Get the length.
					int
						length = GetLength(&format, args);
					char *
						dest;
					DoS(&string, &dest, length, IsEnd(*format) || (!doSave && *format == '}' && IsEnd(*(format + 1))));
					// Send the string to PAWN.
					if (doSave)
					{
						if (!args.HasMore())
						{
							SscanfWarning(47, "Format specifier does not match parameter count.");
							return SSCANF_FAIL_RETURN;
						}
						amx_SetString(args.Next(), dest, 1, 0, length);
					}
				}
				break;
			case 'U':
				if (IsDelimiter(*string))
				{
					int
						b;
					switch (DoUD(&format, &b))
					{
					case -1:
						b = *args.Next();
						if (!args.HasMore())
						{
							SscanfWarning(47, "Format specifier does not match parameter count.");
							return SSCANF_FAIL_RETURN;
						}
					}
					if (*format == '[')
					{
						int
							len = GetLength(&format, args);
						if (gOptions & OLD_DEFAULT_NAME)
						{
							// Incompatible combination.
							SscanfError(55, "'U(name)[len]' is incompatible with OLD_DEFAULT_NAME.");
							return SSCANF_FAIL_RETURN;
						}
						else if (len < 2)
						{
							SscanfError(56, "'U(num)[len]' length under 2.");
							return SSCANF_FAIL_RETURN;
						}
						else if (doSave)
						{
							if (!args.HasMore())
							{
								SscanfWarning(47, "Format specifier does not match parameter count.");
								return SSCANF_FAIL_RETURN;
							}
							cell *
								cptr = args.Next();
							*cptr++ = b;
							*cptr = g_iInvalid;
						}
					}
					else
					{
						SAVE_VALUE((cell)b);
					}
					break;
				}
				if (SkipDefault(&format))
					args.Next();
				// FALLTHROUGH
			case 'u':
				if (*format == '[')
				{
					int
						len = GetLength(&format, args);
					if (len < 2)
					{
						SscanfError(57, "'u[len]' length under 2.");
						return SSCANF_FAIL_RETURN;
					}
					else
					{
						int
							b = -1;
						E_SSCANF_OPTIONS
							od = gOptions;
						if (doSave)
						{
							char *
								tstr;
							// Don't detect multiple results.
							gOptions = (E_SSCANF_OPTIONS)(gOptions & ~CELLMIN_ON_MATCHES);
							cell *
								cptr = args.Next();
							while (--len)
							{
								tstr = string;
								if (!DoU(&tstr, &b, b + 1))
								{
									*cptr++ = b;
									b = g_iInvalid;
									break;
								}
								if (b == g_iInvalid) break;
								*cptr++ = b;
							}
							if (b == g_iInvalid)
							{
								*cptr = g_iInvalid;
							}
							else
							{
								tstr = string;
								DoU(&tstr, &b, b + 1);
								if (b == g_iInvalid) *cptr = g_iInvalid;
								else *cptr = 0x80000000;
							}
							// Restore results detection.
							gOptions = od;
							string = tstr;
						}
						else
						{
							DoU(&string, &b, 0);
						}
					}
				}
				else
				{
					int
						b;
					DoU(&string, &b, 0);
					SAVE_VALUE((cell)b);
				}
				break;
			case 'Q':
				if (IsDelimiter(*string))
				{
					int
						b;
					switch (DoQD(&format, &b))
					{
					case -1:
						b = *args.Next();
						if (!args.HasMore())
						{
							SscanfWarning(47, "Format specifier does not match parameter count.");
							return SSCANF_FAIL_RETURN;
						}
					}
					if (*format == '[')
					{
						int
							len = GetLength(&format, args);
						if (gOptions & OLD_DEFAULT_NAME)
						{
							// Incompatible combination.
							SscanfError(58, "'Q(name)[len]' is incompatible with OLD_DEFAULT_NAME.");
							return SSCANF_FAIL_RETURN;
						}
						else if (len < 2)
						{
							SscanfError(59, "'Q(num)[len]' length under 2.");
							return SSCANF_FAIL_RETURN;
						}
						else if (doSave)
						{
							if (!args.HasMore())
							{
								SscanfWarning(47, "Format specifier does not match parameter count.");
								return SSCANF_FAIL_RETURN;
							}
							cell *
								cptr = args.Next();
							*cptr++ = b;
							*cptr = g_iInvalid;
						}
					}
					else
					{
						SAVE_VALUE((cell)b);
					}
					break;
				}
				if (SkipDefault(&format))
					args.Next();
				// FALLTHROUGH
			case 'q':
				if (*format == '[')
				{
					int
						len = GetLength(&format, args);
					if (len < 2)
					{
						SscanfError(60, "'q[len]' length under 2.");
						return SSCANF_FAIL_RETURN;
					}
					else
					{
						int
							b = -1;
						E_SSCANF_OPTIONS
							od = gOptions;
						if (doSave)
						{
							char *
								tstr;
							// Don't detect multiple results.
							gOptions = (E_SSCANF_OPTIONS)(gOptions & ~CELLMIN_ON_MATCHES);
							cell *
								cptr = args.Next();
							while (--len)
							{
								tstr = string;
								if (!DoQ(&tstr, &b, b + 1))
								{
									*cptr++ = b;
									b = g_iInvalid;
									break;
								}
								if (b == g_iInvalid) break;
								*cptr++ = b;
							}
							if (b == g_iInvalid)
							{
								*cptr = g_iInvalid;
							}
							else
							{
								tstr = string;
								DoQ(&tstr, &b, b + 1);
								if (b == g_iInvalid) *cptr = g_iInvalid;
								else *cptr = 0x80000000;
							}
							// Restore results detection.
							gOptions = od;
							string = tstr;
						}
						else
						{
							DoQ(&string, &b, 0);
						}
					}
				}
				else
				{
					int
						b;
					DoQ(&string, &b, 0);
					SAVE_VALUE((cell)b);
				}
				break;
			case 'R':
				if (IsDelimiter(*string))
				{
					int
						b;
					switch (DoRD(&format, &b))
					{
					case -1:
						b = *args.Next();
						if (!args.HasMore())
						{
							SscanfWarning(47, "Format specifier does not match parameter count.");
							return SSCANF_FAIL_RETURN;
						}
					}
					if (*format == '[')
					{
						int
							len = GetLength(&format, args);
						if (gOptions & OLD_DEFAULT_NAME)
						{
							// Incompatible combination.
							SscanfError(61, "'R(name)[len]' is incompatible with OLD_DEFAULT_NAME.");
							return SSCANF_FAIL_RETURN;
						}
						else if (len < 2)
						{
							SscanfError(62, "'R(num)[len]' length under 2.");
							return SSCANF_FAIL_RETURN;
						}
						else if (doSave)
						{
							if (!args.HasMore())
							{
								SscanfWarning(47, "Format specifier does not match parameter count.");
								return SSCANF_FAIL_RETURN;
							}
							cell *
								cptr = args.Next();
							*cptr++ = b;
							*cptr = g_iInvalid;
						}
					}
					else
					{
						SAVE_VALUE((cell)b);
					}
					break;
				}
				if (SkipDefault(&format))
					args.Next();
				// FALLTHROUGH
			case 'r':
				if (*format == '[')
				{
					int
						len = GetLength(&format, args);
					if (len < 2)
					{
						SscanfError(63, "'r[len]' length under 2.");
						return SSCANF_FAIL_RETURN;
					}
					else
					{
						int
							b = -1;
						E_SSCANF_OPTIONS
							od = gOptions;
						if (doSave)
						{
							char *
								tstr;
							cell *
								cptr = args.Next();
							// Don't detect multiple results.
							gOptions = (E_SSCANF_OPTIONS)(gOptions & ~CELLMIN_ON_MATCHES);
							while (--len)
							{
								tstr = string;
								if (!DoR(&tstr, &b, b + 1))
								{
									*cptr++ = b;
									b = g_iInvalid;
									break;
								}
								if (b == g_iInvalid) break;
								*cptr++ = b;
							}
							if (b == g_iInvalid)
							{
								*cptr = g_iInvalid;
							}
							else
							{
								tstr = string;
								DoR(&tstr, &b, b + 1);
								if (b == g_iInvalid) *cptr = g_iInvalid;
								else *cptr = 0x80000000;
							}
							// Restore results detection.
							gOptions = od;
							string = tstr;
						}
						else
						{
							DoR(&string, &b, 0);
						}
					}
				}
				else
				{
					int
						b;
					DoR(&string, &b, 0);
					SAVE_VALUE((cell)b);
				}
				break;
			case 'A':
				// We need the default values here.
				if (DoA(&format, &string, args, true, doSave))
				{
					break;
				}
				return SSCANF_FAIL_RETURN;
			case 'a':
				if (DoA(&format, &string, args, false, doSave))
				{
					break;
				}
				return SSCANF_FAIL_RETURN;
			case 'E':
				// We need the default values here.
				if (DoE(&format, &string, args, true, doSave))
				{
					break;
				}
				return SSCANF_FAIL_RETURN;
			case 'e':
				if (DoE(&format, &string, args, false, doSave))
				{
					break;
				}
				return SSCANF_FAIL_RETURN;
			case 'K':
			{
				const char *first_closing_lace = FindFirstOf(format, '>');
				const char *first_closing_bracket = FindFirstOf(format, ')');
				const char *after_spec = (first_closing_lace < first_closing_bracket) 
					? first_closing_bracket : first_closing_lace;
				if (*after_spec != '\0')
					after_spec += 1;
				bool consume_all = IsEnd(*after_spec)
					|| (!doSave && *after_spec == '}' && IsEnd(*(after_spec + 1)));
				// We need the default values here.
				if (doSave)
				{
					if (DoK(amx, &format, &string, args.Next(), true, consume_all))
					{
						break;
					}
				}
				else
				{
					// Pass a NULL pointer so data isn't saved anywhere.
					if (DoK(amx, &format, &string, NULL, true, consume_all))
					{
						break;
					}
				}
				return SSCANF_FAIL_RETURN;
			}
			case 'k':
			{
				const char *after_spec = FindFirstOf(format, '>') + 1;
				bool consume_all = IsEnd(*after_spec)
					|| (!doSave && *after_spec == '}' && IsEnd(*(after_spec + 1)));
				if (doSave)
				{
					if (DoK(amx, &format, &string, args.Next(), false, consume_all))
					{
						break;
					}
				}
				else
				{
					// Pass a NULL pointer so data isn't saved anywhere.
					if (DoK(amx, &format, &string, NULL, false, consume_all))
					{
						break;
					}
				}
				return SSCANF_FAIL_RETURN;
			}
			case '\'':
				// Find the end of the literal.
				{
					char
						* str = format,
						* write = format;
					bool
						escape = false;
					while (!IsEnd(*str) && (escape || *str != '\''))
					{
						if (*str == '\\')
						{
							if (escape)
							{
								// "\\" - Go back a step to write this
								// character over the last character (which
								// just happens to be the same character).
								--write;
							}
							escape = !escape;
						}
						else
						{
							if (*str == '\'')
							{
								// Overwrite the escape character with the
								// quote character.  Must have been
								// preceeded by a slash or it wouldn't have
								// got to here in the loop.
								--write;
							}
							escape = false;
						}
						// Copy the string over itself to get rid of excess
						// escape characters.
						// Not sure if it's faster in the average case to
						// always do the copy or check if it's needed.
						// This write is always safe as it makes the string
						// shorter, so we'll never run out of space.  It
						// will also not overwrite the original string.
						*write++ = *str++;
					}
					if (*str == '\'')
					{
						// Correct end.  Make a shorter string to search
						// for.
						*write = '\0';
						// Find the current section of format in string.
						char *
							find = strstr(string, format);
						if (!find)
						{
							// Didn't find the string
							SetErrorCode(1003);
							return SSCANF_FAIL_RETURN;
						}
						// Found the string.  Update the current string
						// position to the length of the search term
						// further along from the start of the term.  Use
						// "write" here as we want the escaped string
						// length.
						string = find + (write - format);
						// Move to after the end of the search string.  Use
						// "str" here as we want the unescaped string
						// length.
						format = str + 1;
					}
					else
					{
						SscanfWarning(24, "Unclosed string literal.");
						char *
							find = strstr(string, format);
						if (!find)
						{
							SetErrorCode(1003);
							return SSCANF_FAIL_RETURN;
						}
						string = find + (write - format);
						format = str;
					}
				}
				break;
			case '?':
				{
					char *
						t = GetMultiType(&format);
					if (t) SetOptions(t, -1);
					else return SSCANF_FAIL_RETURN;
					continue;
				}
			case '!':
				if (args.HasMore())
				{
					SscanfWarning(47, "Format specifier does not match parameter count.");
				}
				if (!doSave)
				{
					// Started a quiet section but never explicitly ended it.
					SscanfWarning(48, "Unclosed quiet section.");
				}
				return SSCANF_TRUE_RETURN;
			case '%':
				SscanfWarning(25, "sscanf specifiers do not require '%%' before them.");
				continue;
			default:
				SscanfWarning(9, "Unknown format specifier '%c', skipping.", *(format - 1));
				continue;
			}
			// Loop cleanup - only skip one spacer so that we can detect
			// multiple explicit delimiters in a row, for example:
			// 
			// hi     there
			// 
			// is NOT multiple explicit delimiters in a row (they're
			// whitespace).  This however is:
			// 
			// hi , , , there
			// 
			SkipOneSpacer(&string);
		}
	}
	// Temporary to the end of the code.
	ResetDelimiter();
	AddDelimiter(')');
	// We don't need code here to handle the case where args.Pos was reached,
	// but the end of the string wasn't - if that's the case there's no
	// problem as we just ignore excess string data.
	while (args.HasMore() || !doSave)
	{
		// Loop through if there's still parameters remaining.
		if (!*format)
		{
			SscanfWarning(47, "Format specifier does not match parameter count.");
			if (!doSave)
			{
				// Started a quiet section but never explicitly ended it.
				SscanfWarning(48, "Unclosed quiet section.");
			}
			return SSCANF_TRUE_RETURN;
		}
		else if (IsWhitespace(*format))
		{
			++format;
		}
		else
		{
			// Do the main switch again.
			IncErrorSpecifier();
			switch (*format++)
			{
			case 'L':
				DE(bool, L)
			case 'B':
				DE(int, B)
			case 'N':
				DE(int, N)
			case 'C':
				DE(char, C)
			case 'I':
			case 'D':
				DE(int, I)
			case 'H':
			case 'X':
				DE(int, H)
			case 'M':
				DE(unsigned int, M)
			case 'O':
				DE(int, O)
			case 'F':
				DEF(double, F)
			case 'G':
				DEF(double, G)
			case 'U':
				DE(int, U)
			case 'Q':
				DE(int, Q)
			case 'R':
				DE(int, R)
			case 'A':
				if (DoA(&format, NULL, args, true, doSave))
				{
					break;
				}
				return SSCANF_FAIL_RETURN;
			case 'E':
				if (DoE(&format, NULL, args, true, doSave))
				{
					break;
				}
				return SSCANF_FAIL_RETURN;
			case 'K':
				if (doSave)
				{
					if (DoK(amx, &format, NULL, args.Next(), true, false))
					{
						break;
					}
				}
				else
				{
					// Pass a NULL pointer so data isn't saved anywhere.
					// Also pass NULL data so it knows to only collect the
					// default values.
					if (DoK(amx, &format, NULL, NULL, true, false))
					{
						break;
					}
				}
				return SSCANF_FAIL_RETURN;
			case '{':
				if (doSave)
				{
					doSave = false;
				}
				else
				{
					// Already in a quiet section.
					SscanfWarning(20, "Can't have nestled quiet sections.");
				}
				break;
			case '}':
				if (doSave)
				{
					SscanfWarning(21, "Not in a quiet section.");
				}
				else
				{
					doSave = true;
				}
				break;
			case 'S':
				{
					char *
						dest;
					int
						length;
					if (DoSD(&format, &dest, &length, args))
					{
						// Send the string to PAWN.
						if (doSave)
						{
							amx_SetString(args.Next(), dest, 0, 0, length);
						}
					}
				}
				break;
			case 'Z':
				{
					char *
						dest;
					int
						length;
					if (DoSD(&format, &dest, &length, args))
					{
						// Send the string to PAWN.
						if (doSave)
						{
							amx_SetString(args.Next(), dest, 1, 0, length);
						}
					}
				}
				break;
			case 'P':
				//SscanfWarning("You can't have an optional delimiter.");
				GetMultiType(&format);
				continue;
				// FALLTHROUGH
			case 'p':
				// Discard delimiter.  This only matters when they have
				// real inputs, not the default ones used here.
				GetSingleType(&format);
				continue;
			case '\'':
				// Implicitly optional if the specifiers after it are
				// optional.
				{
					bool
						escape = false;
					while (!IsEnd(*format) && (escape || *format != '\''))
					{
						if (*format == '\\')
						{
							escape = !escape;
						}
						else
						{
							escape = false;
						}
						++format;
					}
					if (*format == '\'')
					{
						++format;
					}
					else
					{
						SscanfWarning(24, "Unclosed string literal.");
					}
				}
				break;
				// Large block of specifiers all together.
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
			case 'h':
			case 'i':
			case 'k':
			case 'l':
			case 'n':
			case 'o':
			case 'q':
			case 'r':
			case 's':
			case 'z':
			case 'u':
			case 'x':
			case 'm':
				// These are non optional items, but the input string
				// didn't include them, so we fail - this is in fact the
				// most basic definition of a fail (the original)!  We
				// don't need any text warnings here - admittedly we don't
				// know if the format specifier is well formed (there may
				// not be enough return variables for example), but it
				// doesn't matter - the coder should have tested for those
				// things, and the more important thing is that the user
				// didn't enter the correct data.
				SetErrorCode(1004);
				return SSCANF_FAIL_RETURN;
			case '?':
				GetMultiType(&format);
				continue;
			case '!':
				SscanfWarning(47, "Format specifier does not match parameter count.");
				if (!doSave)
				{
					// Started a quiet section but never explicitly ended it.
					SscanfWarning(48, "Unclosed quiet section.");
				}
				return SSCANF_TRUE_RETURN;
			case '%':
				SscanfWarning(25, "sscanf specifiers do not require '%%' before them.");
				break;
			default:
				SscanfWarning(9, "Unknown format specifier '%c', skipping.", *(format - 1));
				break;
			}
			// Don't need any cleanup here.
		}
	}
	if (*format)
	{
		do
		{
			if (*format == '\'')
			{
				IncErrorSpecifier();
				// Allow trailing string literals.
				++format;
				char
					* str = format,
					* write = format;
				bool
					escape = false;
				while (!IsEnd(*str) && (escape || *str != '\''))
				{
					if (*str == '\\')
					{
						if (escape)
						{
							// "\\" - Go back a step to write this
							// character over the last character (which
							// just happens to be the same character).
							--write;
						}
						escape = !escape;
					}
					else
					{
						if (*str == '\'')
						{
							// Overwrite the escape character with the
							// quote character.  Must have been
							// preceeded by a slash or it wouldn't have
							// got to here in the loop.
							--write;
						}
						escape = false;
					}
					// Copy the string over itself to get rid of excess
					// escape characters.
					// Not sure if it's faster in the average case to
					// always do the copy or check if it's needed.
					// This write is always safe as it makes the string
					// shorter, so we'll never run out of space.  It
					// will also not overwrite the original string.
					*write++ = *str++;
				}
				if (*str == '\'')
				{
					// Correct end.  Make a shorter string to search
					// for.
					*write = '\0';
					// Find the current section of format in string.
					char *
						find = strstr(string, format);
					if (!find)
					{
						// Didn't find the string
						SetErrorCode(1003);
						return SSCANF_FAIL_RETURN;
					}
					// Found the string.  Update the current string
					// position to the length of the search term
					// further along from the start of the term.  Use
					// "write" here as we want the escaped string
					// length.
					string = find + (write - format);
					// Move to after the end of the search string.  Use
					// "str" here as we want the unescaped string
					// length.
					format = str + 1;
				}
				else
				{
					SscanfWarning(24, "Unclosed string literal.");
					char *
						find = strstr(string, format);
					if (!find)
					{
						SetErrorCode(1003);
						return SSCANF_FAIL_RETURN;
					}
					string = find + (write - format);
					format = str;
				}
			}
			else
			{
				if (!IsWhitespace(*format))
				{
					IncErrorSpecifier();
					// Only print this warning if the remaining characters are
					// not spaces - spaces are allowed, and sometimes required,
					// on the ends of formats (e.g. to stop the final 's'
					// specifier collecting all remaining characters and only
					// get one word).  We could check that the remaining
					// specifier is a valid one, but this is only a guide - they
					// shouldn't even have other characters IN the specifier so
					// it doesn't matter - it will point to a bug, which is the
					// important thing.
					if (doSave)
					{
						if (*format == '}')
						{
							SscanfWarning(21, "Not in a quiet section.");
						}
						else if (*format == '!')
						{
							// Check we are at the end of the input.
							SkipWhitespace(&string);
							if (*string)
							{
								// Still non-whitespace text, but we don't want any more.
								SetErrorCode(1009);
								return SSCANF_FAIL_RETURN;
							}
							break;
						}
						else if (*format == '{')
						{
							doSave = true;
						}
						else
						{
							// Fix the bad display bug.
							SscanfWarning(47, "Format specifier does not match parameter count.");
							// Only display it once.
							break;
						}
					}
					else
					{
						if (*format == '}')
						{
							doSave = true;
						}
						else
						{
							SscanfWarning(47, "Format specifier does not match parameter count.");
							break;
						}
					}
				}
				++format;
			}
		}
		while (*format);
	}
	if (!doSave)
	{
		// Started a quiet section but never explicitly ended it.
		SscanfWarning(48, "Unclosed quiet section.");
	}
	// No more parameters and no more format specifiers which could be read
	// from - this is a valid return!
	return SSCANF_TRUE_RETURN;
}

#define E_SEARCH_STATE_ESCAPE  (1) // \escape
#define E_SEARCH_STATE_QUIET   (2) // {quiet}
#define E_SEARCH_STATE_SKIP    (4) // -skip
#define E_SEARCH_STATE_SUBTYPE (8) // <subtype>
#define E_SEARCH_STATE_LENGTH  (16) // [length]
#define E_SEARCH_STATE_DEFAULT (32) // (default)
#define E_SEARCH_STATE_STRING  (64) // 'string'

static cell
	SpecifierLookahead(char ** format, int * specifiers)
{
	// A very simple skip for specifiers.  Barely does any checks, just finds the
	// end of the current segment, checks if there is an alternate, and counts how
	// many specifiers write data in this section.
	char *
		searcher = *format;
	int
		state = 0,
		arg = 0;
	char
		specifier = '\0';
	while (*searcher)
	{
		if (state & E_SEARCH_STATE_ESCAPE)
		{
			state &= ~E_SEARCH_STATE_ESCAPE;
		}
		else if (*searcher == '\\')
		{
			state |= E_SEARCH_STATE_ESCAPE;
		}
		else if (state & E_SEARCH_STATE_STRING)
		{
			if (*searcher == '\'')
			{
				state &= ~E_SEARCH_STATE_STRING;
			}
		}
		else if (state & E_SEARCH_STATE_DEFAULT)
		{
			if (*searcher == ')')
			{
				state &= ~E_SEARCH_STATE_DEFAULT;
			}
		}
		else if (state & E_SEARCH_STATE_LENGTH)
		{
			if (*searcher == ']')
			{
				state &= ~E_SEARCH_STATE_LENGTH;
			}
		}
		else if (state & E_SEARCH_STATE_SUBTYPE)
		{
			if (*searcher == '>')
			{
				state &= ~E_SEARCH_STATE_SUBTYPE;
			}
		}
		else switch (*searcher)
		{
		case '<':
			state |= E_SEARCH_STATE_SUBTYPE;
			break;
		case '-':
			// Really doesn't need to do anything.  We aren't writing anyway.
			//state |= E_SEARCH_STATE_SKIP;
			break;
		case '(':
			if (specifier != 'a' && specifier != 's' && specifier != 'e' && *(searcher + 1) == '*' && *(searcher + 2) == ')')
			{
				arg = arg + 1;
				searcher = searcher + 2;
			}
			else
			{
				state |= E_SEARCH_STATE_DEFAULT;
			}
			break;
		case '\'':
			++(*specifiers);
			state |= E_SEARCH_STATE_STRING;
			break;
		case '[':
			if (*(searcher + 1) == '*' && *(searcher + 2) == ']')
			{
				// These are used even in quiet mode.
				arg = arg + 1;
				searcher = searcher + 2;
			}
			else
			{
				state |= E_SEARCH_STATE_LENGTH;
			}
			break;
		case '{':
			++(*specifiers);
			state |= E_SEARCH_STATE_QUIET;
			break;
		case '}':
			state &= ~E_SEARCH_STATE_QUIET;
			break;
		case '|':
			// Do something more.
			*format = searcher;
			return arg;
		case 'w':
			// Wide specifier.
			++(*specifiers);
			do
			{
				++searcher;
				specifier = *searcher;
			}
			while (specifier == 'w');
			if (specifier == '\0')
			{
				// Invalid.
				*format = searcher;
				return arg;
			}
			break;
		case '%':
		case ' ':
		case '\t':
			// Skip.
			break;
		default:
			++(*specifiers);
			// A normal specifier.
			specifier = *searcher;
			if (!(state & E_SEARCH_STATE_QUIET))
			{
				arg = arg + 1;
			}
			break;
		}
		++searcher;
	}
	*format = searcher;
	return arg;
}

static cell
	CallSscanf(AMX* amx, char* string, char* format, cell const* params, const int paramCount)
{
	// Check the lookahead to see if there are any alternates.
	char*
		end = format;
	int
		specifiers = 0,
		partArgs = SpecifierLookahead(&end, &specifiers);
	if (*end == '|')
	{
		// Do alternates.
		if (paramCount == 0)
		{
			SscanfError(73, "No alternate destination.");
			if ((gOptions & ERROR_CODE_IN_RET))
			{
				// Add the code.
				if ((gOptions & ERROR_CATEGORY_ONLY))
				{
					return 1 | GetErrorCategory(73);
				}
				else
				{
					return 1 | (73 << 16);
				}
			}
			return 1;
		}
		cell
			ret = 0,
			* cptr;
		bool
			more = true;
		int
			error = 1,
			offset = 1,
			alternate = 1;
		// Write the (lack of) alternate to the first slot.
		amx_GetAddr(amx, params[0], &cptr);
		*cptr = 0;
		for ( ; ; )
		{
			int
				defaultAlpha = gAlpha,
				defaultForms = gForms;
			E_SSCANF_OPTIONS
				defaultOpts = gOptions;
			int
				count = paramCount - offset;
			if (count < 0)
			{
				count = 0;
			}
			else if (partArgs < count)
			{
				count = partArgs;
			}
			SetErrorCode(0, error);
			ret = Sscanf(amx, string, format, params + offset, count);
			if (ret || (gOptions & WARNINGS_AS_ERRORS))
			{
				ret = GetErrorSpecifier();
				// Error.
				if ((gOptions & ERROR_CODE_IN_RET))
				{
					// Add the code.
					if ((gOptions & ERROR_CATEGORY_ONLY))
					{
						ret |= GetErrorCategory(GetErrorCode());
					}
					else
					{
						ret |= GetErrorCode() << 16;
					}
				}
			}
			RestoreOpts(defaultOpts, defaultAlpha, defaultForms);
			if (ret == 0)
			{
				// Passed.
				*cptr = alternate;
				return 0;
			}
			if (more)
			{
				format = end + 1;
				end = format;
				offset += partArgs;
				++alternate;
				error += specifiers;
				specifiers = 0;
				partArgs = SpecifierLookahead(&end, &specifiers);
				more = *end == '|';
				// Temporary end.
				*end = '\0';
			}
			else
			{
				// Didn't find an alternate.
				ret = 1;
				if ((gOptions & ERROR_CODE_IN_RET))
				{
					// Add the code.
					if ((gOptions & ERROR_CATEGORY_ONLY))
					{
						ret |= GetErrorCategory(1010);
					}
					else
					{
						ret |= 1010 << 16;
					}
				}
				return ret;
			}
		}
	}
	else
	{
		int
			defaultAlpha = gAlpha,
			defaultForms = gForms;
		E_SSCANF_OPTIONS
			defaultOpts = gOptions;
		cell
			ret = Sscanf(amx, string, format, params, paramCount);
		if (ret || (gOptions & WARNINGS_AS_ERRORS))
		{
			ret = GetErrorSpecifier();
			// Error.
			if ((gOptions & ERROR_CODE_IN_RET))
			{
				// Add the code.
				if ((gOptions & ERROR_CATEGORY_ONLY))
				{
					ret |= GetErrorCategory(GetErrorCode());
				}
				else
				{
					ret |= GetErrorCode() << 16;
				}
			}
		}
		RestoreOpts(defaultOpts, defaultAlpha, defaultForms);
		return ret;
	}
}

static cell AMX_NATIVE_CALL
	n_old_sscanf(AMX * amx, cell const * params)
{
	SetErrorCode(0);
	if (g_iTrueMax == 0)
	{
		logprintf("sscanf error: System not initialised.");
		SetErrorCode(1);
		return SSCANF_FAIL_RETURN;
	}
	// Friendly note, the most complex set of specifier additions is:
	// 
	//  A<i>(10, 11)[5]
	// 
	// In that exact order - type, default, size.  It's very opposite to how
	// it's done in code, where you would do the eqivalent to:
	// 
	//  <i>[5] = {10, 11}
	// 
	// But this method is vastly simpler to parse in this context!  Technically
	// you can, due to legacy support for 'p', do:
	// 
	//  Ai(10, 11)[5]
	// 
	// But you will get an sscanf warning, and I may remove that ability from
	// the start - that will mean a new function, but an easy to write one.
	// In fact the most complex will probably be something like:
	// 
	//  E<ifs[32]s[8]d>(10, 12.3, Hello there, Hi, 42)
	// 
	// Get the number of parameters passed.  We add one as the indexes are out
	// by one (OBOE - Out By One Error) due to params[0] being the parameter
	// count, not an actual parameter.
	const int
		paramCount = ((int)params[0] / sizeof (cell)) + 1;
	// Could add a check for only 3 parameters here - I can't think of a time
	// when you would not want any return values at all, but that doesn't mean
	// they don't exist - you could just want to check but not save the format.
	// Update - that is now a possibility with the '{...}' specifiers.
	if (paramCount < (2 + 1))
	{
		logprintf("sscanf error: Missing required parameters.");
		SetErrorCode(39);
		return SSCANF_FAIL_RETURN;
	}
	// Bacup up the file/line data for nested calls.
	char *
		px = gFormat;
	const char *
		pf = gCallFile;
	int
		pl = gCallLine;
	cell *
		pp = gCallResolve;
	gFormat = 0;
	gCallFile = 0;
	gCallLine = -1;
	gCallResolve = 0;
	SscanfWarning(49, "Include / plugin mismatch, please recompile your script for the latest features.");
	// Set up function wide values.
	// Get and check the main data.
	// Pointer to the current format specifier.
	// Doesn't use `SscanfError`, because we don't have the format specifier yet.
	SAFE_STR_PARAM(amx, params[2], gFormat);
	// Pointer to the current input data.
	char *
		string;
	STR_PARAM(amx, params[1], string);
	cell
		ret = CallSscanf(amx, string, gFormat, params + 3, paramCount - 3);
	// Restore and free the error data, if it wasn't constant.
	if (gCallFile && gCallResolve)
	{
		free((void *)gCallFile);
	}
	gCallResolve = pp;
	gCallLine = pl;
	gCallFile = pf;
	gFormat = px;
	return ret;
}

// native sscanf(const data[], const format[], (Float,_}:...);
static cell AMX_NATIVE_CALL
	n_sscanf(AMX * amx, cell const * params)
{
	SetErrorCode(0);
	if (g_iTrueMax == 0)
	{
		logprintf("sscanf error: System not initialised.");
		SetErrorCode(1);
		return SSCANF_FAIL_RETURN;
	}
	// Friendly note, the most complex set of specifier additions is:
	// 
	//  A<i>(10, 11)[5]
	// 
	// In that exact order - type, default, size.  It's very opposite to how
	// it's done in code, where you would do the eqivalent to:
	// 
	//  <i>[5] = {10, 11}
	// 
	// But this method is vastly simpler to parse in this context!  Technically
	// you can, due to legacy support for 'p', do:
	// 
	//  Ai(10, 11)[5]
	// 
	// But you will get an sscanf warning, and I may remove that ability from
	// the start - that will mean a new function, but an easy to write one.
	// In fact the most complex will probably be something like:
	// 
	//  E<ifs[32]s[8]d>(10, 12.3, Hello there, Hi, 42)
	// 
	// Get the number of parameters passed.  We add one as the indexes are out
	// by one (OBOE - Out By One Error) due to params[0] being the parameter
	// count, not an actual parameter.
	const int
		paramCount = ((int)params[0] / sizeof (cell)) + 1;
	// Could add a check for only 3 parameters here - I can't think of a time
	// when you would not want any return values at all, but that doesn't mean
	// they don't exist - you could just want to check but not save the format.
	// Update - that is now a possibility with the '{...}' specifiers.
	if (paramCount < (4 + 1))
	{
		logprintf("sscanf error: Missing required parameters.");
		SetErrorCode(39);
		return SSCANF_FAIL_RETURN;
	}
	// Bacup up the file/line data for nested calls.
	char*
		px = gFormat;
	const char *
		pf = gCallFile;
	int
		pl = gCallLine;
	cell *
		pp = gCallResolve;
	gFormat = 0;
	gCallFile = 0;
	gCallLine = params[2];
	amx_GetAddr(amx, params[1], &gCallResolve);
	SAFE_STR_PARAM(amx, params[4], gFormat);
	// Pointer to the current input data.
	char *
		string;
	STR_PARAM(amx, params[3], string);
	cell
		ret = CallSscanf(amx, string, gFormat, params + 5, paramCount - 5);
	// Restore and free the error data, if it wasn't constant.
	if (gCallFile && gCallResolve)
	{
		free((void *)gCallFile);
	}
	gCallResolve = pp;
	gCallLine = pl;
	gCallFile = pf;
	gFormat = px;
	return ret;
}

// native sscanf(const data[], const format[], (Float,_}:...);
PAWN_NATIVE_EXPORT cell PAWN_NATIVE_API
	PawnSScanf(AMX * amx, char * string, char * format, cell const * params, int paramCount, char * file, int line)
{
	if (g_iTrueMax == 0)
	{
		logprintf("sscanf error: System not initialised.");
		SetErrorCode(1);
		return SSCANF_FAIL_RETURN;
	}
	// Bacup up the file/line data for nested calls.
	char*
		px = gFormat;
	const char *
		pf = gCallFile;
	int
		pl = gCallLine;
	cell *
		pp = gCallResolve;
	gFormat = format;
	gCallFile = file;
	gCallLine = line;
	cell
		ret = CallSscanf(amx, string, gFormat, params + 5, paramCount - 5);
	// Restore and free the error data, if it wasn't constant.
	gCallResolve = pp;
	gCallLine = pl;
	gCallFile = pf;
	gFormat = px;
	return ret;
}

void
	qlog(char const * str, ...)
{
	// Do nothing
}

static cell AMX_NATIVE_CALL
	n_SSCANF_Init(AMX * amx, cell const * params)
{
	SetErrorCode(0);
	if (params[0] != 3 * sizeof (cell))
	{
		logprintf("sscanf error: SSCANF_Init has incorrect parameters.");
		SetErrorCode(32);
		g_iTrueMax = 0;
		return 0;
	}
	if (g_iTrueMax != 0)
	{
		// Already initialised.
		return 1;
	}
	g_iTrueMax = (int)params[1];
	g_iInvalid = (int)params[2];
	g_iMaxPlayerName = (int)params[3] + 1;
	g_szPlayerNames = new char [g_iTrueMax * g_iMaxPlayerName];
	g_iConnected = new int [g_iTrueMax];
	// FINALLY fixed this bug - I didn't know "new []" didn't initialise...
	memset(g_iConnected, 0, sizeof (int) * g_iTrueMax);
	g_iNPC = new int [g_iTrueMax];
	return 1;
}

static void
	DoName(AMX * amx, cell playerid, cell name)
{
	cell *
		str;
	int
		len;
	amx_GetAddr(amx, name, &str);
	amx_StrLen(str, &len);
	if ((unsigned int)len >= g_iMaxPlayerName)
	{
		len = (int)g_iMaxPlayerName - 1;
	}
	amx_GetString(g_szPlayerNames + (g_iMaxPlayerName * playerid), str, 0, len + 1);
}

static cell AMX_NATIVE_CALL
	n_SSCANF_IsConnected(AMX * amx, cell const * params)
{
	SetErrorCode(0);
	cell playerid;

	playerid = params[1];
	if (playerid < 0 || playerid >= (int)g_iTrueMax)
	{
		SetErrorCode(1008);
		return 0;
	}
	if (g_iConnected[playerid] != 0)
	{
		return 1;
	}
	return 0;
}

static cell AMX_NATIVE_CALL
	n_SSCANF_Join(AMX * amx, cell const * params)
{
	SetErrorCode(0);
	if (params[0] != 3 * sizeof (cell))
	{
		logprintf("sscanf error: SSCANF_Join has incorrect parameters.");
		SetErrorCode(33);
		return 0;
	}
	cell
		playerid = params[1];
	if (playerid < 0 || playerid >= (int)g_iTrueMax)
	{
		SetErrorCode(1002);
		return 0;
	}
	++g_iConnected[playerid];
	DoName(amx, playerid, params[2]);
	g_iNPC[playerid] = params[3];
	return 1;
}

static cell AMX_NATIVE_CALL
	n_SSCANF_Leave(AMX * amx, cell const * params)
{
	SetErrorCode(0);
	if (params[0] != 1 * sizeof (cell))
	{
		logprintf("sscanf error: SSCANF_Leave has incorrect parameters.");
		SetErrorCode(34);
		return 0;
	}
	int
		playerid = params[1];
	if (playerid < 0 || playerid >= (int)g_iTrueMax)
	{
		SetErrorCode(1005);
		return 0;
	}
	// To be correct for multiple scripts with loads and unloads (unloadfs).
	--g_iConnected[playerid];
	return 1;
}

static cell AMX_NATIVE_CALL
	n_SSCANF_Option(AMX * amx, cell const * params)
{
	SetErrorCode(0);
	if (params[0] == 1 * sizeof (cell))
	{
		// Get.
		char *
			string;
		SAFE_STR_PARAM(amx, params[1], string);
		return GetOptions(string);
	}
	else if (params[0] == 2 * sizeof (cell))
	{
		// Set.
		char *
			string;
		SAFE_STR_PARAM(amx, params[1], string);
		SetOptions(string, params[2]);
		return 1;
	}
	else
	{
		logprintf("sscanf error: SSCANF_Option has incorrect parameters.");
		SetErrorCode(37);
		return 0;
	}
}

static cell AMX_NATIVE_CALL
	n_SSCANF_Version(AMX * amx, cell const * params)
{
	SetErrorCode(0);
	if (params[0] == 2 * sizeof(cell))
	{
		// Return the version as a string.
		cell length = params[2];
		if (length > 0)
		{
			cell * cptr;
			amx_GetAddr(amx, params[1], &cptr);
			amx_SetString(cptr, SSCANF_VERSION, 1, 0, length);
		}
	}
	else if (params[0] != 0 * sizeof(cell))
	{
		logprintf("sscanf error: SSCANF_Version has incorrect parameters.");
		SetErrorCode(36);
		return 0;
	}
	// Return the version in BCD.
	return
		((SSCANF_VERSION_MAJOR / 10) << 20) |
		((SSCANF_VERSION_MAJOR % 10) << 16) |
		((SSCANF_VERSION_MINOR / 10) << 12) |
		((SSCANF_VERSION_MINOR % 10) << 8) |
		((SSCANF_VERSION_BUILD / 10) << 4) |
		((SSCANF_VERSION_BUILD % 10) << 0);
}

static cell AMX_NATIVE_CALL
	n_SSCANF_SetPlayerName(AMX * amx, cell const * params)
{
	SetErrorCode(0);
	if (g_iTrueMax == 0)
	{
		logprintf("sscanf error: System not initialised.");
		SetErrorCode(1);
		return 0;
	}
	// Hook ALL AMXs, even if they don't use sscanf, by working at the plugin
	// level.  This allows us to intercept name changes.
	//cell
	//	result;
	//amx_Callback(amx, 0, &result, params);
	if (params[0] != 2 * sizeof (cell))
	{
		logprintf("sscanf error: SetPlayerName has incorrect parameters.");
		SetErrorCode(38);
		return 0;
	}
	cell ret = SetPlayerName(amx, params);
	if (ret == 1) // Name has been successfully set.
	{
		DoName(amx, params[1], params[2]);
	}
	return ret;
}

#if !defined min
	inline int min(int a, int b)
	{
		return a < b ? a : b;
	}
#endif

static cell AMX_NATIVE_CALL
	n_SSCANF_Levenshtein(AMX * amx, cell const * params)
{
	SetErrorCode(0);
	if (params[0] != 2 * sizeof(cell))
	{
		logprintf("sscanf error: SSCANF_Levenshtein has incorrect parameters.");
		return -1;
	}
	// Get the two strings to compare.
	cell
		* string1,
		* string2;
	int
		len1,
		len2;
	amx_GetAddr(amx, params[1], &string1);
	amx_StrLen(string1, &len1);
	amx_GetAddr(amx, params[2], &string2);
	amx_StrLen(string2, &len2);
	if (len1 == 0)
	{
		return len2;
	}
	if (len2 == 0)
	{
		return len1;
	}
	int *
		matrix = (int *)malloc(sizeof(int) * (len1 + 1) * (len2 + 1));
	if (matrix == NULL)
	{
		SetErrorCode(1006);
		return 0;
	}
	int step = len2 + 1;
	for (int i = 0; i <= len1; i++)
	{
		matrix[i * step + 0] = i;
	}
	for (int j = 0; j <= len2; j++)
	{
		matrix[0 * step + j] = j;
	}
	for (int i = 1; i <= len1; i++)
	{
		for (int j = 1; j <= len2; j++)
		{
			matrix[i * step + j] =
				min(
					min(
						matrix[(i - 1) * step + j] + 1,
						matrix[i * step + (j - 1)] + 1
					),
					matrix[(i - 1) * step + (j - 1)] + ((string1[i - 1] == string2[j - 1]) ? 0 : 1)
				);
		}
	}
	int ret = matrix[len1 * step + len2];
	free(matrix);
	return ret;
}

static cell AMX_NATIVE_CALL
	n_SSCANF_TextSimilarity(AMX * amx, cell const * params)
{
	SetErrorCode(0);
	if (params[0] != 2 * sizeof(cell))
	{
		logprintf("sscanf error: SSCANF_TextSimilarity has incorrect parameters.");
		SetErrorCode(68);
		return 0x7FFFFFFF;
	}
	// Get the two strings to compare.
	cell
		* string1,
		* string2;
	amx_GetAddr(amx, params[1], &string1);
	amx_GetAddr(amx, params[2], &string2);
	char matrix[36 * 36];
	memset(matrix, 0, 36 * 36);
	int
		pair = 0,
		ch,
		unique1 = 0,
		unique2 = 0,
		ngrams1 = -1,
		ngrams2 = -1;
	while ((ch = *string1++))
	{
		// This code only looks at numbers and letters, and ignores case.
		if ('0' <= ch && ch <= '9')
		{
			pair = (pair / 36) | ((ch - '0') * 36);
		}
		else if ('a' <= ch && ch <= 'z')
		{
			pair = (pair / 36) | ((ch - ('a' - 10)) * 36);
		}
		else if ('A' <= ch && ch <= 'Z')
		{
			pair = (pair / 36) | ((ch - ('A' - 10)) * 36);
		}
		else
		{
			// Not a character we are interested in.
			continue;
		}
		if (++ngrams1)
		{
			// Got at least TWO characters.
			++matrix[pair];
			++unique1;
		}
	}
	while ((ch = *string2++))
	{
		// This code only looks at numbers and letters, and ignores case.
		if ('0' <= ch && ch <= '9')
		{
			pair = (pair / 36) | ((ch - '0') * 36);
		}
		else if ('a' <= ch && ch <= 'z')
		{
			pair = (pair / 36) | ((ch - ('a' - 10)) * 36);
		}
		else if ('A' <= ch && ch <= 'Z')
		{
			pair = (pair / 36) | ((ch - ('A' - 10)) * 36);
		}
		else
		{
			// Not a character we are interested in.
			continue;
		}
		// The original version had positive numbers as pairs in `string1` but
		// not `string2`, and vice-versa, but I already optimised it.
		if (!++ngrams2)
		{
		}
		else if (matrix[pair])
		{
			--matrix[pair];
			--unique1;
		}
		else
		{
			++unique2;
		}
	}
	if (ngrams1 < 1 || ngrams2 < 1)
	{
		// There just aren't enough letters to compare.
		SetErrorCode(1007);
		return 0;
	}
	// Normalise the number of matching pairs and multiply.
	float
		result = (1.0f - ((float)unique1 / (float)ngrams1)) * (1.0f - ((float)unique2 / (float)ngrams2));
	return amx_ftoc(result);
}

static cell AMX_NATIVE_CALL
	n_SSCANF_NOP(AMX * amx, cell const * params)
{
	SetErrorCode(0);
	return 1;
}

static cell AMX_NATIVE_CALL
	n_SSCANF_GetLastError(AMX * amx, cell const * params)
{
	return GetErrorCode();
}

static cell AMX_NATIVE_CALL
	n_SSCANF_GetErrorSpecifier(AMX * amx, cell const * params)
{
	return GetErrorSpecifier();
}

static cell AMX_NATIVE_CALL
	n_SSCANF_GetErrorCategory(AMX * amx, cell const * params)
{
	SetErrorCode(0);
	if (params[0] != 1 * sizeof(cell))
	{
		logprintf("sscanf error: SSCANF_GetErrorCategory has incorrect parameters.");
		SetErrorCode(69);
		return -1;
	}
	// Return the error category.
	return GetErrorCategory(params[1]);
}

//----------------------------------------------------------
// The AmxLoad() function gets called when a new gamemode or
// filterscript gets loaded with the server. In here we register
// the native functions we like to add to the scripts.

AMX_NATIVE_INFO
	sscanfSAMPNatives[] =
		{
			{"sscanf", n_old_sscanf},
			{"SSCANF__", n_sscanf},
			{"SSCANF_Init", n_SSCANF_Init},
			{"SSCANF_ClearLastError", n_SSCANF_NOP},
			{"SSCANF_Join", n_SSCANF_Join},
			{"SSCANF_Leave", n_SSCANF_Leave},
			{"SSCANF_IsConnected", n_SSCANF_IsConnected},
			{"SSCANF_Option", n_SSCANF_Option},
			{"SSCANF_Version", n_SSCANF_Version},
			{"SSCANF_Levenshtein", n_SSCANF_Levenshtein},
			{"SSCANF_TextSimilarity", n_SSCANF_TextSimilarity},
			{"SSCANF_GetLastError", n_SSCANF_GetLastError},
			{"SSCANF_GetErrorSpecifier", n_SSCANF_GetErrorSpecifier},
			{"SSCANF_GetErrorCategory", n_SSCANF_GetErrorCategory},
			{0, 0}
		};

extern AMX_NATIVE_INFO
	sscanfOMPNatives[];

//----------------------------------------------------------
// The Support() function indicates what possibilities this
// plugin has. The SUPPORTS_VERSION flag is required to check
// for compatibility with the server. 

PLUGIN_EXPORT unsigned int PLUGIN_CALL
	Supports() 
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

#define CALL_HOOKED_REGISTER(a, n, c) \
	(((amx_Register_t)subhook_get_trampoline(amx_Register_hook))(a, n, c))

int AMXAPI SSCANF_Register(AMX* amx, const AMX_NATIVE_INFO* nativelist, int number)
{
	int ret = AMX_ERR_NONE;
	int err = AMX_ERR_NONE;
	if (SetPlayerName == NULL)
	{
		// If `number` is `-1` loop until a null entry.
		for (int i = 0; number == -1 ? nativelist[i].name != nullptr : i != number; ++i)
		{
			if (strcmp(nativelist[i].name, "SetPlayerName") == 0)
			{
				// Save the pointer to the original function.
				SetPlayerName = nativelist[i].func;
				// Found the original version, inject ours first.
				AMX_NATIVE_INFO
					natives[2] = {
						{ "SetPlayerName", n_SSCANF_SetPlayerName },
						{ 0, 0 },
					};
				// Always keep the most specific error.
				if ((err = CALL_HOOKED_REGISTER(amx, natives, -1)) != AMX_ERR_NONE)
					ret = err;
				// Register all the other natives before `SetPlayerName`.
				for (int j = 0; j != i; ++j)
				{
					natives[0].name = nativelist[j].name;
					natives[0].func = nativelist[j].func;
					if ((err = CALL_HOOKED_REGISTER(amx, natives, -1)) != AMX_ERR_NONE)
						ret = err;
				}
				// Register all the other natives after `SetPlayerName`.
				++i;
				if (number == -1)
				{
					if ((err = CALL_HOOKED_REGISTER(amx, nativelist + i, -1)) != AMX_ERR_NONE)
						ret = err;
				}
				else if (i != number)
				{
					if ((err = CALL_HOOKED_REGISTER(amx, nativelist + i, number - i)) != AMX_ERR_NONE)
						ret = err;
				}
				subhook_install(amx_Register_hook);
				return ret;
			}
		}
	}
	// We already have `SetPlayerName`, don't search again just pass straight through.
	return CALL_HOOKED_REGISTER(amx, nativelist, number);
}

//----------------------------------------------------------
// The Load() function gets passed on exported functions from
// the SA-MP Server, like the AMX Functions and logprintf().
// Should return true if loading the plugin has succeeded.

int Init(AMX * amx)
{
	return amx_Register(amx, sscanfSAMPNatives, -1);
}

//----------------------------------------------------------
// When a gamemode is over or a filterscript gets unloaded, this
// function gets called. No special actions needed in here.

int Cleanup(AMX * amx) 
{
	return AMX_ERR_NONE;
}

// Standard gamemode/filterscript functions.
PLUGIN_EXPORT int PLUGIN_CALL
	AmxLoad(AMX * amx) 
{
	return Init(amx);
}

PLUGIN_EXPORT int PLUGIN_CALL
	AmxUnload(AMX * amx) 
{
	return Cleanup(amx);
}

//----------------------------------------------------------
// The Load() function gets passed on exported functions from
// the SA-MP Server, like the AMX Functions and logprintf().
// Should return true if loading the plugin has succeeded.

PLUGIN_EXPORT bool PLUGIN_CALL
	Load(void ** ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];

	amx_Register_hook = subhook_new((void*)((amx_Register_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_Register], (void*)&SSCANF_Register, SUBHOOK_64BIT_OFFSET);
	subhook_install(amx_Register_hook);

	real_logprintf = logprintf;
	
	logprintf("");
	logprintf(" ===============================");
	logprintf("");
	logprintf("      sscanf plugin loaded.     ");
	logprintf("");
	logprintf("         Version: " SSCANF_VERSION "");
	logprintf("");
	logprintf("   (c) 2022 Alex \"Y_Less\" Cole  ");
	logprintf("");
	logprintf(" ===============================");
	logprintf("");

	return true;
}

//----------------------------------------------------------
// The Unload() function is called when the server shuts down,
// meaning this plugin gets shut down with it.

PLUGIN_EXPORT void PLUGIN_CALL
	Unload()
{
	logprintf("");
	logprintf(" ===============================");
	logprintf("");
	logprintf("     sscanf plugin unloaded.    ");
	logprintf("");
	logprintf(" ===============================");
	logprintf("");
}

int NpcInit(AMX * amx)
{
	pAMXFunctions = NPC_AMX_FUNCTIONS;
	logprintf = qlog;
	real_logprintf = qlog;
	
	logprintf("");
	logprintf(" ===============================");
	logprintf("");
	logprintf("      sscanf library loaded.    ");
	logprintf("");
	logprintf("         Version: " SSCANF_VERSION "");
	logprintf("");
	logprintf("   (c) 2022 Alex \"Y_Less\" Cole  ");
	logprintf("");
	logprintf(" ===============================");
	logprintf("");

	return Init(amx);
}

int NpcCleanup(AMX * amx)
{
	int ret = Cleanup(amx);

	logprintf("");
	logprintf(" ===============================");
	logprintf("");
	logprintf("     sscanf library unloaded.   ");
	logprintf("");
	logprintf(" ===============================");
	logprintf("");

	return ret;
}

PLUGIN_EXPORT int PLUGIN_CALL amx_sscanfInit(AMX * amx)
{
	// NPC plugin init functions.  Relies on the plugin name.
	return NpcInit(amx);
}

PLUGIN_EXPORT int PLUGIN_CALL amx_sscanf2Init(AMX * amx)
{
	// NPC plugin init functions.  Relies on the plugin name.
	return NpcInit(amx);
}

PLUGIN_EXPORT int PLUGIN_CALL amx_amxsscanfInit(AMX * amx)
{
	// NPC plugin init functions.  Relies on the plugin name.
	return NpcInit(amx);
}

PLUGIN_EXPORT int PLUGIN_CALL amx_amxsscanf2Init(AMX * amx)
{
	// NPC plugin init functions.  Relies on the plugin name.
	return NpcInit(amx);
}

PLUGIN_EXPORT int PLUGIN_CALL amx_sscanfCleanup(AMX * amx)
{
	// NPC plugin init functions.  Relies on the plugin name.
	return NpcCleanup(amx);
}

PLUGIN_EXPORT int PLUGIN_CALL amx_sscanf2Cleanup(AMX * amx)
{
	// NPC plugin init functions.  Relies on the plugin name.
	return NpcCleanup(amx);
}

PLUGIN_EXPORT int PLUGIN_CALL amx_amxsscanfCleanup(AMX * amx)
{
	// NPC plugin init functions.  Relies on the plugin name.
	return NpcCleanup(amx);
}

PLUGIN_EXPORT int PLUGIN_CALL amx_amxsscanf2Cleanup(AMX * amx)
{
	// NPC plugin init functions.  Relies on the plugin name.
	return NpcCleanup(amx);
}

class SScanFComponent;

SScanFComponent *
	sscanfComponent = nullptr;

// TODO: Subscribe to player events.
class SScanFComponent final : public IComponent, public PawnEventHandler, public PlayerConnectEventHandler, public PlayerChangeEventHandler
{
private:
	static ICore * core;
	static IPlayerPool * players;
	IPawnComponent * pawnComponent = nullptr;

	static void openmplog(char const * format, ...)
	{
		// Convert from `logprintf` to `core->logLn`.
		va_list params;
		va_start(params, format);
		core->vlogLn(LogLevel::Message, format, params);
		va_end(params);

	}

	void SaveName(IPlayer& player)
	{
		StringView name = player.getName();
		size_t
			playerid = player.getID(),
			len = name.length();
		if (len >= g_iMaxPlayerName)
		{
			len = g_iMaxPlayerName - 1;
		}
		strncpy(g_szPlayerNames + (g_iMaxPlayerName * playerid), name.data(), len);
		*(g_szPlayerNames + (g_iMaxPlayerName * playerid) + len) = '\0';
	}

public:
	static cell AMX_NATIVE_CALL
		n_OMP_Init(AMX * amx, cell const * params)
	{
		SetErrorCode(0);
		if (params[0] != 3 * sizeof (cell))
		{
			logprintf("sscanf error: SSCANF_Init has incorrect parameters.");
			SetErrorCode(32);
			g_iTrueMax = 0;
			return 0;
		}
		// Special `open.mp` return value.
		return -1;
	}

	// This should be `onPlayerNameChange`, but the SDK repo is out of date.
	void onPlayerNameChange(IPlayer& player, StringView oldName) override
	{
		SaveName(player);
	}

	// I hate using lower-case letters in HEX.  But I had to differentiate between `a` and `A1`, the
	// former being `A` and the latter being `N` (if you squint REALLY hard).  Granted not as hard
	// as you have to squint to see `7` as `X`...
	PROVIDE_UID(0xA1e7'C01e'55caA1f'2);

	StringView componentName() const override
	{
		return "sscanf";
	}

	SemanticVersion componentVersion() const override
	{
		return SemanticVersion(SSCANF_VERSION_MAJOR, SSCANF_VERSION_MINOR, SSCANF_VERSION_BUILD, 0);
	}

	void onLoad(ICore * c) override
	{
		core = c;
		players = &core->getPlayers();
		players->getPlayerConnectDispatcher().addEventHandler(this);
		players->getPlayerChangeDispatcher().addEventHandler(this);
		logprintf = SScanFComponent::openmplog;
		real_logprintf = logprintf;

		logprintf("");
		logprintf(" ===============================");
		logprintf("");
		logprintf("     sscanf component loaded.   ");
		logprintf("");
		logprintf("         Version: " SSCANF_VERSION "");
		logprintf("");
		logprintf("   (c) 2022 Alex \"Y_Less\" Cole  ");
		logprintf("");
		logprintf(" ===============================");
		logprintf("");

		g_iTrueMax = *core->getConfig().getInt("max_players");
		g_iInvalid = INVALID_PLAYER_ID;
		g_iMaxPlayerName = MAX_PLAYER_NAME + 1;
		g_szPlayerNames = new char[g_iTrueMax * g_iMaxPlayerName];
		g_iConnected = new int[g_iTrueMax];
		memset(g_iConnected, 0, sizeof(int) * g_iTrueMax);
		g_iNPC = new int[g_iTrueMax];
	}

	// This should be `onPlayerConnect`, but the SDK repo is out of date.
	void onPlayerConnect(IPlayer & player) override
	{
		int
			playerid = player.getID();
		g_iConnected[playerid] = 1;
		g_iNPC[playerid] = player.isBot();
		SaveName(player);
	}
	
	// This should be `onPlayerDisconnect`, but the SDK repo is out of date.
	void onPlayerDisconnect(IPlayer & player, PeerDisconnectReason reason) override
	{
		g_iConnected[player.getID()] = 0;
	}

	void onInit(IComponentList * components) override
	{
		pawnComponent = components->queryComponent<IPawnComponent>();

		if (pawnComponent)
		{
			pAMXFunctions = (void *)&pawnComponent->getAmxFunctions();
			pawnComponent->getEventDispatcher().addEventHandler(this, EventPriority_FairlyHigh);
		}
		else
		{
			core->logLn(LogLevel::Error, "sscanf error: Pawn component not loaded.");
		}
	}

	void onReady() override
	{
	}

	void onAmxLoad(IPawnScript& script) override
	{
		// The SA:MP version does extra bits with `SetPlayerName` etc.  This one doesn't. 
		amx_Register(script.GetAMX(), sscanfOMPNatives, -1);
	}

	void onAmxUnload(IPawnScript& script) override
	{
	}

	void onFree(IComponent * component) override
	{
		if (component == pawnComponent)
		{
			pawnComponent = nullptr;
			pAMXFunctions = nullptr;
		}
	}

	void free() override
	{
		// Could also be done in the destructor.
		if (pawnComponent)
		{
			pawnComponent->getEventDispatcher().removeEventHandler(this);
		}
		if (players)
		{
			players->getPlayerConnectDispatcher().removeEventHandler(this);
			players->getPlayerChangeDispatcher().removeEventHandler(this);
		}
		// Deinitialise the system.
		g_iTrueMax = 0;

		logprintf("");
		logprintf(" ===============================");
		logprintf("");
		logprintf("    sscanf component unloaded.  ");
		logprintf("");
		logprintf(" ===============================");
		logprintf("");

		delete this;
	}

	void reset() override
	{
		// Nothing to reset for now.
	}
};

ICore * SScanFComponent::core = nullptr;
IPlayerPool * SScanFComponent::players = nullptr;

COMPONENT_ENTRY_POINT()
{
	sscanfComponent = new SScanFComponent();
	return sscanfComponent;
}

AMX_NATIVE_INFO
	sscanfOMPNatives[] =
{
	{"sscanf", n_old_sscanf},
	{"SSCANF__", n_sscanf},
	{"SSCANF_Init", SScanFComponent::n_OMP_Init},
	{"SSCANF_ClearLastError", n_SSCANF_NOP},
	{"SSCANF_Join", n_SSCANF_NOP},
	{"SSCANF_Leave", n_SSCANF_NOP},
	{"SSCANF_IsConnected", n_SSCANF_NOP},
	{"SSCANF_Option", n_SSCANF_Option},
	{"SSCANF_Version", n_SSCANF_Version},
	{"SSCANF_Levenshtein", n_SSCANF_Levenshtein},
	{"SSCANF_TextSimilarity", n_SSCANF_TextSimilarity},
	{"SSCANF_GetLastError", n_SSCANF_GetLastError},
	{"SSCANF_GetErrorSpecifier", n_SSCANF_GetErrorSpecifier},
	{"SSCANF_GetErrorCategory", n_SSCANF_GetErrorCategory},
	{0, 0}
};

