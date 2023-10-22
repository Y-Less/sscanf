# sscanf 2.15.1

## Introduction

This is the `sscanf` plugin, which provides the `sscanf` function to extract basic structured data from strings.  This is slightly different to regular expressions, but both have their place.  A regular expression gives you total control over the exact structure of data down to the character level; however, extracting structured data like numbers using it is tricky.  Conversely this gives slightly higher-level "specifiers" which can easily extract data types, at the expense of fine-grained control.  To convert a string in to two numbers would look like:

```pawn
new num1, num2;
sscanf("45 100", "ii", num1, num2);
```

`ii` is the specifier string, which here means "integer integer"; stating that the input string should be two whole numbers in a row (which is - `"45 100"`).  `num1` and `num2` are the destination variables to store the found numbers in (after conversion from strings).  You can check if the conversion failed by looking for a non-naught return value:

```pawn
new num1, num2;
if (sscanf("hello 100", "ii", num1, num2))
{
	printf("The input was not two numbers.");
}
```

This will fail because `"hello"` is not a whole number (or indeed any type of number at all).  For more information on using the function refer to the tutorials or the reference documentation below:

## Contents

* 1 [Introduction](#introduction)
* 2 [Contents](#contents)
* 3 [Downloads](#downloads)
* 4 [Use](#use)
	* 4.1 [Scripting](#scripting)
	* 4.2 [open.mp](#openmp)
	* 4.3 [SA:MP Windows](#samp-windows)
	* 4.4 [SA:MP Linux](#samp-linux)
	* 4.5 [NPC Modes](#npc-modes)
* 5 [Tutorials](#tutorials)
    * 5.1 [`/sendcash` Command](#sendcash-command)
    * 5.2 [INI Parser](#ini-parser)
    * 5.3 [Error Detection](#error-detection)
    * 5.4 [A Basic Shop](#a-basic-shop)
* 6 [Specifiers](#specifiers)
    * 6.1 [Strings](#strings)
    * 6.2 [Packed Strings](#packed-strings)
    * 6.3 [Arrays](#arrays)
    * 6.4 [Enums](#enums)
    * 6.5 [Provided Lengths](#provided-lengths)
    * 6.6 [Quiet](#quiet)
    * 6.7 [Searches](#searches)
    * 6.8 [Enums](#enums)
    * 6.9 [Delimiters](#delimiters)
    * 6.10 [Optional specifiers](#optional-specifiers)
    * 6.11 [Users](#users)
    * 6.12 [Custom (kustom) specifiers](#custom-kustom-specifiers)
    * 6.13 [Colours](#colours)
        * 6.13.1 [3 digits](#3-digits)
        * 6.13.2 [6 digits](#6-digits)
        * 6.13.3 [8 digits](#8-digits)
    * 6.14 [End Of Input](#end-of-input)
* 7 [Options](#options)
    * 7.1 [OLD_DEFAULT_NAME:](#old_default_name)
    * 7.2 [MATCH_NAME_PARTIAL:](#match_name_partial)
    * 7.3 [CELLMIN_ON_MATCHES:](#cellmin_on_matches)
    * 7.4 [SSCANF_QUIET:](#sscanf_quiet)
    * 7.5 [OLD_DEFAULT_KUSTOM:](#old_default_kustom)
    * 7.6 [SSCANF_ALPHA:](#sscanf_alpha)
    * 7.7 [SSCANF_COLOUR_FORMS:](#sscanf_colour_forms)
    * 7.8 [SSCANF_ARGB:](#sscanf_argb)
    * 7.9 [MATCH_NAME_FIRST:](#match_name_first)
    * 7.10 [MATCH_NAME_SIMILARITY:](#match_name_similarity)
    * 7.11 [ERROR_CODE_IN_RET:](#error_code_in_ret)
    * 7.12 [WARNINGS_AS_ERRORS:](#warnings_as_errors)
    * 7.13 [ERROR_CATEGORY_ONLY:](#error_category_only)
* 8 [`extract`](#extract)
* 9 [Similarity](#similarity)
* 10 [Error Returns](#error-returns)
    * 10.1 [Additional Codes](#additional-codes)
    * 10.2 [Error Categories](#error-categories)
* 11 [Alternates](#alternates)
* 12 [All Specifiers](#all-specifiers)
* 13 [Full API](#full-api)
    * 13.1 [`sscanf(const data[], const format[], {Float, _}:...);`](#sscanfconst-data-const-format-float-_)
    * 13.2 [`unformat(const data[], const format[], {Float, _}:...);`](#unformatconst-data-const-format-float-_)
    * 13.3 [`SSCANF_Option(const name[], value);`](#sscanf_optionconst-name-value)
    * 13.4 [`SSCANF_Option(const name[]);`](#sscanf_optionconst-name)
    * 13.5 [`SSCANF_SetOption(const name[], value);`](#sscanf_setoptionconst-name-value)
    * 13.6 [`SSCANF_GetOption(const name[], value);`](#sscanf_getoptionconst-name-value)
    * 13.7 [`SSCANF_Version(version[], size = sizeof (version));`](#sscanf_versionversion-size--sizeof-version)
    * 13.8 [`SSCANF_Version();`](#sscanf_version)
    * 13.9 [`SSCANF_VersionString(version[], size = sizeof (version));`](#sscanf_versionstringversion-size--sizeof-version)
    * 13.10 [`SSCANF_VersionBCD();`](#sscanf_versionbcd)
    * 13.11 [`SSCANF_Levenshtein(const string1[], const string2[]);`](#sscanf_levenshteinconst-string1-const-string2)
    * 13.12 [`Float:SSCANF_TextSimilarity(const string1[], const string2[]);`](#floatsscanf_textsimilarityconst-string1-const-string2)
    * 13.13 [`SSCANF_GetClosestString(const input[], const candidates[][], threshold = cellmax, count = sizeof (candidates));`](#sscanf_getcloseststringconst-input-const-candidates-threshold--cellmax-count--sizeof-candidates)
    * 13.14 [`SSCANF_GetClosestValue(const input[], const candidates[][], const results[], fail = cellmin, threshold = cellmax, count = sizeof (candidates), check = sizeof (results));`](#sscanf_getclosestvalueconst-input-const-candidates-const-results-fail--cellmin-threshold--cellmax-count--sizeof-candidates-check--sizeof-results)
    * 13.15 [`SSCANF_GetSimilarString(const input[], const candidates[][], Float:threshold = 0.111111, count = sizeof (candidates));`](#sscanf_getsimilarstringconst-input-const-candidates-floatthreshold--0111111-count--sizeof-candidates)
    * 13.16 [`SSCANF_GetSimilarValue(const input[], const candidates[][], const results[], fail = cellmin, Float:threshold = 0.111111, count = sizeof (candidates), check = sizeof (results));`](#sscanf_getsimilarvalueconst-input-const-candidates-const-results-fail--cellmin-floatthreshold--0111111-count--sizeof-candidates-check--sizeof-results)
    * 13.17 [`SSCANF_VERSION_STRING`](#sscanf_version_string)
    * 13.18 [`SSCANF_VERSION_BCD`](#sscanf_version_bcd)
    * 13.19 [`SSCANF_VERSION`](#sscanf_version-1)
    * 13.20 [`SSCANF_NO_K_VEHICLE`](#sscanf_no_k_vehicle)
    * 13.21 [`SSCANF_NO_K_WEAPON`](#sscanf_no_k_weapon)
    * 13.22 [`SSCANF_NO_NICE_FEATURES`](#sscanf_no_nice_features)
    * 13.23 [`SSCANF_GetLastError();`](#sscanf_getlasterror)
    * 13.24 [`SSCANF_ClearLastError();`](#sscanf_clearlasterror)
    * 13.25 [`sscanf_error:SSCANF_GetErrorCategory(error);`](#sscanf_errorsscanf_geterrorcategoryerror)
    * 13.26 [`SSCANF_GetErrorSpecifier();`](#sscanf_geterrorspecifier)
    * 13.27 [`SSCANF_Debug`();`](#sscanf_debug)
* 14 [Errors/Warnings](#errorswarnings)
    * 14.1 [MSVRC100.dll not found](#msvrc100dll-not-found)
    * 14.2 [sscanf error: System not initialised](#sscanf-error-system-not-initialised)
    * 14.3 [sscanf warning: String buffer overflow.](#sscanf-warning-string-buffer-overflow)
    * 14.4 [sscanf warning: Optional types invalid in array specifiers, consider using 'A'.](#sscanf-warning-optional-types-invalid-in-array-specifiers-consider-using-a)
    * 14.5 [sscanf warning: Optional types invalid in enum specifiers, consider using 'E'.](#sscanf-warning-optional-types-invalid-in-enum-specifiers-consider-using-e)
    * 14.6 [sscanf error: Multi-dimensional arrays are not supported.](#sscanf-error-multi-dimensional-arrays-are-not-supported)
    * 14.7 [sscanf error: Search strings are not supported in arrays.](#sscanf-error-search-strings-are-not-supported-in-arrays)
    * 14.8 [sscanf error: Delimiters are not supported in arrays.](#sscanf-error-delimiters-are-not-supported-in-arrays)
    * 14.9 [sscanf error: Quiet sections are not supported in arrays.](#sscanf-error-quiet-sections-are-not-supported-in-arrays)
    * 14.10 [sscanf error: Unknown format specifier '?'.](#sscanf-error-unknown-format-specifier-)
    * 14.11 [sscanf warning: Empty default values.](#sscanf-warning-empty-default-values)
    * 14.12 [sscanf warning: Unclosed default value.](#sscanf-warning-unclosed-default-value)
    * 14.13 [sscanf warning: No default value found.](#sscanf-warning-no-default-value-found)
    * 14.14 [sscanf warning: Unenclosed specifier parameter.](#sscanf-warning-unenclosed-specifier-parameter)
    * 14.15 [sscanf warning: No specified parameter found.](#sscanf-warning-no-specified-parameter-found)
    * 14.16 [sscanf warning: Missing string length end.](#sscanf-warning-missing-string-length-end)
    * 14.17 [sscanf warning: Missing length end.](#sscanf-warning-missing-length-end)
    * 14.18 [sscanf error: Invalid data length.](#sscanf-error-invalid-data-length)
    * 14.19 [sscanf error: Invalid character in data length.](#sscanf-error-invalid-character-in-data-length)
    * 14.20 [sscanf error: String/array must include a length, please add a destination size.](#sscanf-error-stringarray-must-include-a-length-please-add-a-destination-size)
    * 14.21 [sscanf warning: Can't have nestled quiet sections.](#sscanf-warning-cant-have-nestled-quiet-sections)
    * 14.22 [sscanf warning: Not in a quiet section.](#sscanf-warning-not-in-a-quiet-section)
    * 14.23 [sscanf warning: Can't remove quiet in enum.](#sscanf-warning-cant-remove-quiet-in-enum)
    * 14.24 [sscanf error: Arrays are not supported in enums.](#sscanf-error-arrays-are-not-supported-in-enums)
    * 14.25 [sscanf warning: Unclosed string literal.](#sscanf-warning-unclosed-string-literal)
    * 14.26 [sscanf warning: sscanf specifiers do not require '%' before them.](#sscanf-warning-sscanf-specifiers-do-not-require--before-them)
    * 14.27 [sscanf error: Insufficient default values.](#sscanf-error-insufficient-default-values)
    * 14.28 [sscanf error: Options are not supported in enums.](#sscanf-error-options-are-not-supported-in-enums)
    * 14.29 [sscanf error: Options are not supported in arrays.](#sscanf-error-options-are-not-supported-in-arrays)
    * 14.30 [sscanf error: No option value.](#sscanf-error-no-option-value)
    * 14.31 [sscanf error: Unknown option name.](#sscanf-error-unknown-option-name)
    * 14.32 [sscanf warning: Could not find function SSCANF:?.](#sscanf-warning-could-not-find-function-sscanf)
    * 14.33 [sscanf error: SSCANF_Init has incorrect parameters.](#sscanf-error-sscanf_init-has-incorrect-parameters)
    * 14.34 [sscanf error: SSCANF_Join has incorrect parameters.](#sscanf-error-sscanf_join-has-incorrect-parameters)
    * 14.35 [sscanf error: SSCANF_Leave has incorrect parameters.](#sscanf-error-sscanf_leave-has-incorrect-parameters)
    * 14.36 [sscanf error: SSCANF_IsConnected has incorrect parameters.](#sscanf-error-sscanf_isconnected-has-incorrect-parameters)
    * 14.37 [sscanf error: SSCANF_Version has incorrect parameters.](#sscanf-error-sscanf_version-has-incorrect-parameters)
    * 14.38 [sscanf error: SSCANF_Option has incorrect parameters.](#sscanf-error-sscanf_option-has-incorrect-parameters)
    * 14.39 [sscanf error: SetPlayerName has incorrect parameters.](#sscanf-error-setplayername-has-incorrect-parameters)
    * 14.40 [sscanf error: Missing required parameters.](#sscanf-error-missing-required-parameters)
    * 14.41 [`fatal error 111: user error: sscanf already defined, or used before inclusion.`](#fatal-error-111-user-error-sscanf-already-defined-or-used-before-inclusion)
    * 14.42 [`error 004: function "sscanf" is not implemented`](#error-004-function-sscanf-is-not-implemented)
    * 14.43 [`error 004: function "sscanf" is not implemented - include <sscanf2> first.`](#error-004-function-sscanf-is-not-implemented---include-sscanf2-first)
    * 14.44 [sscanf error: Pawn component not loaded.](#sscanf-error-pawn-component-not-loaded)
    * 14.45 [sscanf warning: Unknown `player->setName()` return.](#sscanf-warning-unknown-player-setname-return)
    * 14.46 [sscanf error: This script was built with the component version of the include.](#sscanf-error-this-script-was-built-with-the-component-version-of-the-include)
    * 14.47 [sscanf error: Unable to allocate memory.](#sscanf-error-unable-to-allocate-memory)
    * 14.48 [sscanf warning: User arrays are not supported in arrays.](#sscanf-warning-user-arrays-are-not-supported-in-arrays)
    * 14.49 [sscanf warning: Invalid values in array defaults.](#sscanf-warning-invalid-values-in-array-defaults)
    * 14.50 [sscanf warning: Excess array defaults found.](#sscanf-warning-excess-array-defaults-found)
    * 14.51 [sscanf warning: Format specifier does not match parameter count.](#sscanf-warning-format-specifier-does-not-match-parameter-count)
    * 14.52 [sscanf warning: Unclosed quiet section.](#sscanf-warning-unclosed-quiet-section)
    * 14.53 [sscanf warning: Include / plugin mismatch, please recompile your script for the latest features.](#sscanf-warning-include--plugin-mismatch-please-recompile-your-script-for-the-latest-features)
    * 14.54 [sscanf warning: A minus minus makes no sense.](#sscanf-warning-a-minus-minus-makes-no-sense)
    * 14.55 [sscanf warning: A minus option makes no sense.](#sscanf-warning-a-minus-option-makes-no-sense)
    * 14.56 [sscanf warning: A minus delimiter makes no sense.](#sscanf-warning-a-minus-delimiter-makes-no-sense)
    * 14.57 [sscanf warning: A minus quiet section makes no sense.](#sscanf-warning-a-minus-quiet-section-makes-no-sense)
    * 14.58 [sscanf warning: User arrays are not supported in enums.](#sscanf-warning-user-arrays-are-not-supported-in-enums)
    * 14.59 [sscanf error: 'U(name)[len]' is incompatible with OLD_DEFAULT_NAME.](#sscanf-error-u-name-len-is-incompatible-with-old_default_name)
    * 14.60 [sscanf error: 'U(num)[len]' length under 2.](#sscanf-error-u-num-len-length-under-2)
    * 14.61 [sscanf error: 'u[len]' length under 2.](#sscanf-error-u-len-length-under-2)
    * 14.62 [sscanf error: 'Q(name)[len]' is incompatible with OLD_DEFAULT_NAME.](#sscanf-error-q-name-len-is-incompatible-with-old_default_name)
    * 14.63 [sscanf error: 'Q(num)[len]' length under 2.](#sscanf-error-q-num-len-length-under-2)
    * 14.64 [sscanf error: 'q[len]' length under 2.](#sscanf-error-q-len-length-under-2)
    * 14.65 [sscanf error: 'R(name)[len]' is incompatible with OLD_DEFAULT_NAME.](#sscanf-error-r-name-len-is-incompatible-with-old_default_name)
    * 14.66 [sscanf error: 'R(num)[len]' length under 2.](#sscanf-error-r-num-len-length-under-2)
    * 14.67 [sscanf error: 'r[len]' length under 2.](#sscanf-error-r-len-length-under-2)
    * 14.68 [sscanf error: (*) is not supported in strings/arrays yet.](#sscanf-error-is-not-supported-in-strings-arrays-yet)
    * 14.69 [sscanf error: Unclosed specifier parameters.](#sscanf-error-unclosed-specifier-parameters)
    * 14.70 [sscanf error: No specified parameters found.](#sscanf-error-no-specified-parameters-found)
    * 14.71 [sscanf error: Enums are not supported in enums.](#sscanf-error-enums-are-not-supported-in-enums)
    * 14.72 [sscanf error: SSCANF_TextSimilarity has incorrect parameters.](#sscanf-error-sscanf_textsimilarity-has-incorrect-parameters)
    * 14.73 [sscanf error: SSCANF_GetErrorCategory has incorrect parameters.](#sscanf-error-sscanf_geterrorcategory-has-incorrect-parameters)
    * 14.74 [sscanf error: End of text is not supported in arrays.](#sscanf-error-end-of-text-is-not-supported-in-arrays)
    * 14.75 [sscanf error: End of text is not supported in enums.](#sscanf-error-end-of-text-is-not-supported-in-enums)
    * 14.76 [sscanf warning: A minus end of text makes no sense.](#sscanf-warning-a-minus-end-of-text-makes-no-sense)
    * 14.77 [sscanf error: No alternate destination.](#sscanf-error-no-alternate-destination)
* 14 [Future Plans](#future-plans)
    * 14.1 [Reserved Specifiers](#reserved-specifiers)
    * 14.2 [Validity Ranges](#validity-ranges)
    * 14.3 [Enums And Arrays](#enums-and-arrays)
    * 14.4 [Compilation](#compilation)
* 15 [Building](#building)
    * 15.1 [Getting The Code](#getting-the-code)
    * 15.2 [Building On Windows](#building-on-windows)
    * 15.3 [Building On Linux](#building-on-linux)
    * 15.4 [Building With Docker](#building-with-docker)
* 16 [License](#license)
    * 16.1 [Version: MPL 1.1](#version-mpl-11)
    * 16.2 [Contributor(s):](#contributors)
    * 16.3 [Special Thanks to:](#special-thanks-to)
* 17 [Changelog](#changelog)
    * 17.1 [sscanf 2.8.2 - 18/04/2015](#sscanf-282---18042015)
    * 17.2 [sscanf 2.8.3 - 02/10/2018](#sscanf-283---02102018)
    * 17.3 [sscanf 2.9.0 - 04/11/2019](#sscanf-290---04112019)
    * 17.4 [sscanf 2.10.0 - 27/06/2020](#sscanf-2100---27062020)
    * 17.5 [sscanf 2.10.1 - 27/06/2020](#sscanf-2101---27062020)
    * 17.6 [sscanf 2.10.2 - 28/06/2020](#sscanf-2102---28062020)
    * 17.7 [sscanf 2.10.3 - 28/04/2021](#sscanf-2103---28042021)
    * 17.8 [sscanf 2.10.4 - 17/01/2022](#sscanf-2104---17012022)
    * 17.9 [sscanf 2.11.1 - 25/01/2022](#sscanf-2111---25012022)
    * 17.9 [sscanf 2.11.2 - 04/02/2022](#sscanf-2112---04022022)
    * 17.10 [sscanf 2.11.3 - 05/02/2022](#sscanf-2113---05022022)
    * 17.10 [sscanf 2.11.4 - 02/03/2022](#sscanf-2114---02032022)
    * 17.11 [sscanf 2.11.5 - 31/03/2022](#sscanf-2115---31032022)
    * 17.12 [sscanf 2.12.1 - 05/05/2022](#sscanf-2121---05052022)
    * 17.13 [sscanf 2.12.2 - 11/05/2022](#sscanf-2122---11052022)
    * 17.14 [sscanf 2.13.1 - 25/06/2022](#sscanf-2131---25062022)
    * 17.15 [sscanf 2.13.2 - 07/09/2022](#sscanf-2132---07092022)
    * 17.16 [sscanf 2.13.3 - 04/12/2022](#sscanf-2133---04122022)
    * 17.17 [sscanf 2.13.4 - 20/12/2022](#sscanf-2134---20122022)
    * 17.18 [sscanf 2.13.5 - 28/12/2022](#sscanf-2135---28122022)
    * 17.19 [sscanf 2.13.6 - 28/12/2022](#sscanf-2136---28122022)
    * 17.20 [sscanf 2.13.7 - 02/01/2023](#sscanf-2137---02012023)
    * 17.21 [sscanf 2.13.8 - 05/01/2023](#sscanf-2138---05012023)
    * 17.21 [sscanf 2.14.1 - 08/09/2023](#sscanf-2141---08092023)
    * 17.22 [sscanf 2.15.1 - 10/09/2023](#sscanf-2151---10092023)

## Downloads

GitHub repo:

https://github.com/Y-Less/sscanf/

## Use

### Scripting

This behaves exactly as the old sscanf did, just MUCH faster and much more flexibly.  To use it add:

```pawn
#include <sscanf2>
```

To your modes and remove the old sscanf (the new include will detect the old version and throw an error if it is detected).

The basic code looks like:

```pawn
if (sscanf(params, "ui", giveplayerid, amount))
{
    return SendClientMessage(playerid, 0xFF0000AA, "Usage: /givecash <playerid/name> <amount>");
}
```

However it should be noted that sscanf can be used for any text processing you like.  For example an ini processor could look like (don't worry about what the bits mean at this stage):

```pawn
if (sscanf(szFileLine, "p<=>s[8]s[32]", szIniName, szIniValue))
{
    printf("Invalid INI format line");
}
```

There is also an alternate function name to avoid confusion with the C standard sscanf:

```pawn
if (unformat(params, "ui", giveplayerid, amount))
{
    return SendClientMessage(playerid, 0xFF0000AA, "Usage: /givecash <playerid/name> <amount>");
}
```

### open.mp

The `sscanf` binary (`sscanf.dll` on Windows, or `sscanf.so` on Linux) works as both a legacy (SA:MP) plugin or an *open.mp* component.  The recommended method is to use it as a component - just place the file in the `components` directory in the server root and *open.mp* will load it automatically.

If you wish to use it as a legacy plugin for some reason (there is no need if you are on version `2.12.1` or higher) place it in the `plugins` directory in the open.mp server root and either follow the *SA:MP*-specific instructions for `server.cfg` on your platform or add `"sscanf"` to `"pawn.legacy_plugins"` in `config.json`:

```json
{
	"pawn":
	{
		"legacy_plugins":
		[
			"sscanf"
		]
	}
}
```

### SA:MP Windows

Add `sscanf` to the start of the `plugins` line in `server.cfg`.  For example:

```
plugins sscanf streamer crashdetect
```

If there isn't a `plugins` line already, add one:

```pawn
plugins sscanf
```

You must also place `sscanf.dll` in the `plugins` subdirectory of the server.  If there isn't a `plugins` directory, create one.

### SA:MP Linux

Add `sscanf.so` to the start of the `plugins` line in `server.cfg`.  For example:

```
plugins sscanf.so streamer.so crashdetect.so
```

If there isn't a `plugins` line already, add one:

```pawn
plugins sscanf.so
```

You must also place `sscanf.so` in the `plugins` subdirectory of the server.  If there isn't a `plugins` directory, create one.

### NPC modes

To use sscanf in an NPC mode save the plugin as `amxsscanf.dll` (on Windows) or `amxsscanf.so` (on Linux) in the same directory as `samp-npc(.exe)` (i.e. the server root).  This allows NPC modes to automatically find and load the library.  The only tiny differences between this sscanf and the normal sscanf are that there are no prints; and `u`, `r`, and `q` don't know if a user is a bot or not thus just assume they are all players.

## Tutorials

### `/sendcash` Command

Send some of your money to another player.  This command allows you to specify the target player using their name or ID so you can type `/sendcash Y_Less 500` to send me $500 (*hint hint...*), or `/sendcash 27 12` to send $12 to whoever player ID 27 is.  Note that if a player has a numeric name such as `69` you will have to use their ID only, typing that name will always select the player with ID 69, not the player with name "69":

```pawn
@cmd() sendcash(playerid, params[], help)
{
	if (help)
	{
		SendClientMessage(playerid, COLOUR_HELP, "Send money to another player.  Usage: /sendcash <name/id> <amount>");
		return 1;
	}
	new targetid, money;
	if (sscanf(params, "ui", targetid, money) != 0)
	{
		SendClientMessage(playerid, COLOUR_HELP, "Missing parameters.  Usage: /sendcash <name/id> <amount>");
		return 1;
	}
	if (targetid == INVALID_PLAYER_ID)
	{
		SendClientMessage(playerid, COLOUR_HELP, "Unknown target player.");
		return 1;
	}
	if (money > GetPlayerMoney(playerid))
	{
		SendClientMessage(playerid, COLOUR_HELP, "You don't have enough money.");
		return 1;
	}
	GivePlayerMoney(playerid, -money);
	GivePlayerMoney(targetid, money);
	SendClientMessage(playerid, COLOUR_HELP, "You sent money.");
	SendClientMessage(targetid, COLOUR_HELP, "You receieved money.");
	return 1;
}
```

### INI Parser

This very basic file reader uses *sscanf* to split the keys and values in an INI file by `=`.

```pawn
bool:ReadINIString(const filename[], const key[], &value)
{
	new File:f = fopen(filename, io_read);
	if (!f)
	{
		return false;
	}
	new line[128];
	new k[32], v[96];
	while ((fread(f, line)))
	{
		if (sscanf(line, "p<=>s[32]s[96]", k, v) == 0)
		{
			if (strcmp(key, k, true) == 0)
			{
				value = strval(v);
				fclose(f);
				return true;
			}
		}
	}
	fclose(f);
	return false;
}
```

### Error Detection

We can write a command and use error returns to know exactly what happened when the user entered something wrong.  See the section on [Error Returns](#error-returns) for more information on what all the values mean.  This will expand the earlier `/sendcash` command with even more checks:

```pawn
@cmd() sendcash(playerid, params[], help)
{
	if (help)
	{
		SendClientMessage(playerid, COLOUR_HELP, "Send money to another player.  Usage: /sendcash <name/id> <amount> [optional reason]");
		return 1;
	}
	new targetid, money, reason;
	SSCANF_Option(ERROR_CATEGORY_ONLY, 1);
	switch (sscanf(params, "?<ERROR_CODE_IN_RET=1>?<WARNINGS_AS_ERRORS=1>uiS[16]()", targetid, money, reason))
	{
		case SSCANF_ERROR(3, MISSING):
		{
			SendClientMessage(playerid, COLOUR_HELP, "No player specified.");
			return 1;
		}
		case SSCANF_ERROR(3, INVALID):
		{
			SendClientMessage(playerid, COLOUR_HELP, "Invalid player specified.");
			return 1;
		}
		case SSCANF_ERROR(4, MISSING):
		{
			SendClientMessage(playerid, COLOUR_HELP, "No amount specified.");
			return 1;
		}
		case SSCANF_ERROR(4, INVALID):
		{
			SendClientMessage(playerid, COLOUR_HELP, "Invalid amount specified.");
			return 1;
		}
		case SSCANF_ERROR(5, OVERFLOW):
		{
			SendClientMessage(playerid, COLOUR_HELP, "Reason too long, please shorten it.");
			return 1;
		}
	}
	if (targetid == INVALID_PLAYER_ID)
	{
		SendClientMessage(playerid, COLOUR_HELP, "Unknown target player.");
		return 1;
	}
	if (money < 0)
	{
		SendClientMessage(playerid, COLOUR_HELP, "You can't steal money.");
		return 1;
	}
	if (money > GetPlayerMoney(playerid))
	{
		SendClientMessage(playerid, COLOUR_HELP, "You don't have enough money.");
		return 1;
	}
	GivePlayerMoney(playerid, -money);
	GivePlayerMoney(targetid, money);
	SendClientMessage(playerid, COLOUR_HELP, "You sent money.");
	if (IsNull(reason))
	{
		SendClientMessage(targetid, COLOUR_HELP, "You receieved money.");
	}
	else
	{
		SendClientMessage(targetid, COLOUR_HELP, "You receieved money because: %s.", reason);
	}
	return 1;
}
```

### A Basic Shop

This example will use alternates to make a `/buy` command in a simple manner:

```pawn
@cmd() buy(playerid, params[], help)
{
	new
		alt,
		weapon,
		ammo,
		vehicle,
		colour1,
		colour2;
	if (help || sscanf(params, "'weapon'ii|'armour'|'health'|'vehicle'k<vehicle>I(-1)I(-1)", alt, weapon, ammo, vehicle, colour1, colour2))
	{
		SendClientMessage(playerid, COLOUR_ERROR, "Usage:");
		SendClientMessage(playerid, COLOUR_ERROR, " ");
		SendClientMessage(playerid, COLOUR_ERROR, "	 /buy weapon <id> <ammo>");
		SendClientMessage(playerid, COLOUR_ERROR, "	 /buy armour");
		SendClientMessage(playerid, COLOUR_ERROR, "	 /buy health");
		SendClientMessage(playerid, COLOUR_ERROR, "	 /buy vehicle <type> <colour1> <colour2>");
		SendClientMessage(playerid, COLOUR_ERROR, " ");
		return 1;
	}
	else switch (alt)
	{
		case 1:
		{
			SendClientMessage(playerid, COLOUR_OK, "You bought weapon %d with %d ammo", weapon, ammo);
		}
		case 2:
		{
			SendClientMessage(playerid, COLOUR_OK, "You bought armour");
		}
		case 3:
		{
			SendClientMessage(playerid, COLOUR_OK, "You bought health");
		}
		case 4:
		{
			SendClientMessage(playerid, COLOUR_OK, "You bought vehicle %d with colours %d, %d", vehicle, colour1, colour2);
		}
	}
	return 1;
}
```

These command variants all now work:

```
/weapon 4 5
/armour
/buy health
/buy vehicle 500 9 10
```

This also works thanks to using `k<vehicle>` instead of `i` for the vehicle model ID:

```
/buy vehicle Infernus 5 6
```

As does this, using default random colours:

```
/buy vehicle PCJ-600
```

This one doesn't work, and displays the help message instead:

```
/buy food
```

Note that the actual task of giving the player the items is left as an exercise for the reader.

We can do one better for the vehicle colours.  Currently they are optional so you can provide naught, one or two colours.  But what if we want the player to provide both or neither.  Using `I` won't work for that because it makes both optional.  Instead we can use alternatives with two or no integer parameters:

```pawn
@cmd() buy(playerid, params[], help)
{
	new
		alt,
		weapon,
		ammo,
		vehicle,
		colour1,
		colour2;
	if (help || sscanf(params, "'weapon'ii|'armour'|'health'|'vehicle'k<vehicle>ii|'vehicle'k<vehicle>", alt, weapon, ammo, vehicle, colour1, colour2, vehicle))
	{
		// ...
		return 1;
	}
	else switch (alt)
	{
		// ...
		case 4:
		{
			SendClientMessage(playerid, COLOUR_OK, "You bought vehicle %d with colours %d, %d", vehicle, colour1, colour2);
		}
		case 5:
		{
			colour1 = -1;
			colour2 = -1;
			SendClientMessage(playerid, COLOUR_OK, "You bought vehicle %d with random colours", vehicle);
		}
	}
	return 1;
}
```

The alternative with two integer parameters must come first as the one with no integers could also match that and simply ignore the data (unless you use `!` for strict text ending, i.e. `'vehicle'k<vehicle>!`).  The `vehicle` parameter is also used twice in the `sscanf` call so that we only need a single variable for both alternatives.

## Specifiers

The basic specifiers (the letters `u`, `i`, `s` etc. in the codes above) here.  There are more advanced ones in a later table.

|  Specifier(s)  |               Name                |                      Example values                       |
| -------------- | --------------------------------- | --------------------------------------------------------- |
|  `i`, `d`      |  Integer                          |  `1`, `42`, `-10`                                         |
|  `c`           |  Character                        |  `a`, `o`, `*`                                            |
|  `l`           |  Logical                          |  `true`, `false`                                          |
|  `b`           |  Binary                           |  `01001`, `0b1100`                                        |
|  `h`, `x`      |  Hex                              |  `1A`, `0x23`                                             |
|  `o`           |  Octal                            |  `045`, `12`                                              |
|  `n`           |  Number                           |  `42`, `0b010`, `0xAC`, `045`                             |
|  `f`           |  Float                            |  `0.7`, `-99.5`                                           |
|  `g`           |  IEEE Float                       |  `0.7`, `-99.5`, `INFINITY`, `-INFINITY`, `NAN`, `NAN_E`  |
|  `u`           |  User name/id (bots and players)  |  `Y_Less`, `0`                                            |
|  `q`           |  Bot name/id                      |  `ShopBot`, `27`                                          |
|  `r`           |  Player name/id                   |  `Y_Less`, `42`                                           |
|  `m`           |  Colour                           |  `{FF00AA}`, `0xFFFFFFFF`, `444`                          |

### Strings

The specifier `s` is used, as before, for strings - but they are now more advanced.  As before they support collection, so doing:

```pawn
sscanf("hello 27", "si", str, val);
```

Will give:

```
hello
27
```

Doing:

```pawn
sscanf("hello there 27", "si", str, val);
```

Will fail as `there` is not a number.  However doing:

```pawn
sscanf("hello there", "s", str);
```

Will give:

```
hello there
```

Because there is nothing after `s` in the specifier, the string gets everything.  To stop this simply add a space:

```pawn
sscanf("hello there", "s ", str);
```

Will give:

```
hello
```

You can also escape parts of strings with `\\` - note that this is two backslashes as 1 is used by the compiler:

```pawn
sscanf("hello\\ there 27", "si", str, val);
```

Will give:

```
hello there
27
```

All these examples however will give warnings in the server as the new version has array sizes.  The above code should be:

```pawn
new
    str[32],
    val;
sscanf("hello\\ there 27", "s[32]i", str, val);
```

As you can see - the format specifier now contains the length of the target string, ensuring that you can never have your strings overflow and cause problems.  This can be combined with the SA:MP compiler's stringizing:

```pawn
#define STR_SIZE 32
new
    str[STR_SIZE],
    val;
sscanf("hello\\ there 27", "s
#STR_SIZE "]i", str, val);
```

Or better yet, you can now use `[*]` to pass a string length as an additional parameter (see "Provided Lengths" below).

So when you change your string size you don't need to change your specifiers.

### Packed Strings

`z` and `Z` return packed strings.  They are otherwise identical to `s` and `S`, so see the `Strings` documentation above for more details.

### Arrays

One of the advanced new specifiers is `a`, which creates an array, obviously.  The syntax is similar to that of strings and, as you will see later, the delimiter code:

```pawn
new
    arr[5];
sscanf("1 2 3 4 5", "a<i>[5]", arr);
```

The `a` specifier is immediately followed by a single type enclosed in angle brackets - this type can be any of the basic types listed above.  It is the followed, as with strings now, by an array size.  The code above will put the numbers 1 to 5 into the 5 indexes of the `arr` array variable.

Arrays can now also be combined with strings (see below), specifying the string size in the array type:

```
a<s[10]>[12]
```

This will produce an array of 12 strings, each up to 10 characters long (9 + NULL).  Optional string arrays still follow the optional array syntax:

```
A<s[10]>(hello)[12]
```

However, unlike numbers you can't specify a progression and have it fill up.  This code:

```
A<i>(0, 1)[4]
```

Will by default produce:

```
0, 1, 2, 3
```

However, this code:

```
A<s[10]>(hi, there)[4]
```

Will by default produce:

```
"hi, there", "hi, there", "hi, there", "hi, there"
```

As normal, you can add brackets in to the default string value with `\)`:

```
A<s[10]>(hi (code\))[4]
```

It should also be noted that there is NO length checking on default strings.  If you do:

```
A<s[10]>(This is longer than 10 characters)[4]
```

You will probably just corrupt the PAWN stack.  The length checking is to ensure no users enter malicious data; however, in this case it is up to the scripter to ensure that the data is correct as they are the only one affecting it and shouldn't be trying to crash their own server.  Interestingly, arrays of strings actually also work with jagged arrays and arrays that have been shuffled by Slice's quicksort function (this isn't a side-effect, I specifically wrote them to do so).

### Enums

This is possibly the most powerful addition to sscanf ever.  This gives you the ability to define the structure of an enum within your specifier string and read any data straight into it.  The format takes after that of arrays, but with more types - and you can include strings in enums (but not other enums or arrays):

```pawn
enum
    E_DATA
{
    E_DATA_C,
    Float:E_DATA_X,
    E_DATA_NAME[32],
    E_DATA_Z
}

main
{
    new
        var[E_DATA];
    sscanf("1 12.0 Bob c", "e<ifs[32]c>", var);
}
```

Now I'll be impressed if you can read that code straight off, so I'll explain it slowly:

```pawn
e - Start of the `enum` type
< - Starts the specification of the structure of the enum
i - An integer, corresponds with E_DATA_C
f - A float, corresponds with E_DATA_X
s[32] - A 32 cell string, corresponds with E_DATA_NAME
c - A character, corresponds with E_DATA_Z
> - End of the enum specification
```

Note that an enum doesn't require a size like arrays and strings - it's size is determined by the number and size of the types.  Most, but not all, specifiers can be used inside enums (notably arrays and other enums can't be).

### Provided Lengths

Both strings and arrays take a length, normally specified in the string with (say) `s[32]`.  However, this system has some extreme limitations - most notably macros.  This code will not work:

```pawn
#define LEN 32
sscanf(params, "s[LEN]", str);
```

This code will work:

```pawn
#define LEN 32
sscanf(params, "s["#LEN"]", str);
```

But this code won't even compile due to a compiler issue stringifying brackets before version 3.10.11.

```pawn
#define LEN (32)
sscanf(params, "s["#LEN"]", str);
```

This code will work, but is a bit awkward:

```pawn
sscanf(params, "s[(32)]", str);
```

This code will compile, but then also won't work:

```pawn
#define LEN 8*4
sscanf(params, "s["#LEN"]", str);
```

For this reason you can pass string and array lengths as additional parameters using `*` for the length:

```pawn
#define LEN 8*4
sscanf(params, "s[*]", LEN, str);
```

The lengths appear BEFORE the destination, arrays first then strings:

```pawn
new int, arr[5][10], str[32];
sscanf(params, "ia<s[*]>[*]s[*]", int, sizeof (arr), sizeof (arr[]), arr, 32, str);
```

There are three main specifiers here - `i`, `a<s[*]>[*]`, and `s[*]`; and each is handled entirely independently.  So the first parameter (`int`) is for `i`, the next three (`sizeof (arr), sizeof (arr[]), arr`) are for `a<s[*]>[*]`, and the final two (`32, str`) are for `s[*]`.  The first and last are easy to understand, the second one not so much.  The sizes are in reverse order, outer to inner, so `sizeof (arr)` is for the `*` in the outer `a<...>[*]` and `sizeof (arr[])` is for the `*` in the inner `s[*]`  Then the destination variable is given after all information about the specifier has been derived.

The same applies to strings in enums:

```pawn
enum E_EXAMPLE
{
	Float:FLOAT,
	STR_1[32],
	STR_2[64],
	INT,
}

new dest[E_EXAMPLE];
sscanf(params, "e<fs[*]s[*]i>", _:STR_2 - _:STR_1, 64, dest);
```

And to arrays of users (see below):

```pawn
new ids[3], i;
sscanf(params, "u[*]", sizeof (ids), ids);
```

This allows you to pass variable lengths if you don't want to use all of a string, and use the full power of the pre-processor to generate lengths at compile-time.  It also bypasses the compiler stringise bug with brackets in strings.  `extract` (see below) now uses `*` for all strings and arrays as well, so will similarly fully use the pre-processor.

### Quiet

The two new specifiers `{` and `}` are used for what are known as `quiet` specifiers.  These are inputs which are read and checked, but not saved.  For example:

```pawn
sscanf("42 -100", "{i}i", var);
```

Clearly there are two numbers and two `i`, but only one return variable.  This is because the first `i` is quiet so is not saved, but affects the return value.  The code above makes `var` `-100`.  The code below will fail in an if check:

```pawn
sscanf("hi -100", "{i}i", var);
```

Although the first integer is not saved it is still read - and `hi` is not an integer.  Quiet zones can be as long as you like, even for the whole string if you only want to check values are right, not save them:

```pawn
sscanf("1 2 3", "i{ii}", var);
sscanf("1 2 3", "{iii}");
sscanf("1 2 3", "i{a<i>[2]}", var);
```

You can also embed quiet sections inside enum specifications:

```pawn
sscanf("1 12.0 Bob 42 INFINITY c", "e<ifs[32]{ig}c>", var);
```

Quiet sections cannot contain other quiet sections, however they can include enums which contain quiet sections.

### Searches

Searches were in the last version of sscanf too, but I'm explaining them again anyway.  Strings enclosed in single quotes (') are scanned for in the main string and the position moved on.  Note that to search for a single quote you escape it as above using `\\`:

```pawn
sscanf("10 11 woo 12", "i'woo'i", var0, var1);
```

Gives:

```
10
12
```

You could achieve the same effect with:

```pawn
sscanf("10 11 woo 12", "i{is[1000]}i", var0, var1);
```

But that wouldn't check that the string was `woo`.  Also note the use of `1000` for the string size.  Quiet strings must still have a length, but as they aren't saved anywhere you can make this number as large as you like to cover any eventuality.  Enum specifications can include search strings.

### Enums

This is a feature similar to quiet sections, which allows you to skip overwriting certain parts of an enum:

```
e<ii-i-ii>
```

Here the `-` is a `minus`, and tells sscanf that there is an enum element there, but not to do anything, so if you had:

```pawn
enum E
{
    E_A,
    E_B,
    E_C,
    E_D,
    E_E
}
```

And you only wanted to update the first two and the last fields and leave all others untouched you could use that specifier above.  This way sscanf knows how to skip over the memory, and how much memory to skip.  Note that this doesn't read anything, so you could also combine this with quiet sections:

```
e<ii-i-i{ii}i>
```

That will read two values and save them, skip over two memory locations, read two values and NOT save them, then read and save a last value.  In this way you can have written down all the values for every slot in the enum, but have only used 3 of them.  Note that this is the same with `E` - if you do:

```
E<ii-i-ii>
```

You should ONLY specify THREE defaults, not all five:

```
E<ii-i-ii>(11, 22, 55)
```

### Delimiters

The previous version of sscanf had `p` to change the symbol used to separate tokens.  This specifier still exists but it has been formalised to match the array and enum syntax.  What was previously:

```pawn
sscanf("1,2,3", "p,iii", var0, var1, var2);
```

Is now:

```pawn
sscanf("1,2,3", "p<,>iii", var0, var1, var2);
```

The old version will still work, but it will give a warning.  Enum specifications can include delimiters, and is the only time `<>`s are contained in other `<>`s:

```pawn
sscanf("1 12.0 Bob,c", "e<ifp<,>s[32]c>", var);
```

Note that the delimiter will remain in effect after the enum is complete.  You can even use `>` as a specifier by doing `p<\>>` (or the older `p>`).

When used with strings, the collection behaviour is overruled.  Most specifiers are still space delimited, so for example this will work:

```pawn
sscanf("1 2 3", "p<;>iii", var0, var1, var2);
```

Despite the fact that there are no `;`s.  However, strings will ONLY use the specified delimiters, so:

```pawn
sscanf("hello 1", "p<->s[32]i", str, var);
```

Will NOT work - the variable `str` will contain `hello 1`.  On the other hand, the example from earlier, slightly modified:

```pawn
sscanf("hello there>27", "p<>>s[32]i", str, var);
```

WILL work and will give an output of:

```
hello there
27
```

You can now have optional delimiters using `P` (upper case `p` to match other `optional` specifiers).  These are optional in the sense that you specify multiple delimiters and any one of them can be used to end the next symbol:

```pawn
sscanf("(4, 5, 6, 7)", "P<(),>{s[2]}iiii", a, b, c, d);
```

This uses a `quiet section` to ignore anything before the first `(`, and then uses multiple delimiters to end all the text.  Example:

```pawn
sscanf("42, 43; 44@", "P<,;@>a<i>[3]", arr);
```

### Optional specifiers

EVERY format specifier (that is, everything except `''`, `{}` and `p`) now has an optional equivalent - this is just their letter capitalised.  In addition to optional specifiers, there are also now default values:

```pawn
sscanf("", "I(12)", var);
```

The `()`s (round brackets) contain the default value for the optional integer and, as the main string has no data, the value of `var` becomes `12`.  Default values come before array sizes and after specifications, so an optional array would look like:

```pawn
sscanf("1 2", "A<i>(3)[4]", arr);
```

Note that the size of the array is `4` and the default value is `3`.  There are also two values which are defined, so the final value of `arr` is:

```
1, 2, 3, 3
```

Array default values are clever, the final value of:

```pawn
sscanf("", "A<i>(3,6)[4]", arr);
```

Will be:

```
3, 6, 9, 12
```

The difference between `3` and `6` is `3`, so the values increase by that every index.  Note that it is not very clever, so:

```pawn
sscanf("", "A<i>(1,2,2)[4]", arr);
```

Will produce:

```
1, 2, 2, 2
```

The difference between `2` and `2` (the last 2 numbers in the default) is 0, so there will be no further increase.  For `l` (logical) arrays, the value is always the same as the last value, as it is with `g` if the last value is one of the special values (INFINITY, NEG_INFINITY (same as -INFINITY), NAN or NAN_E).  Note that:

```pawn
sscanf("", "a<I>(1,2,2)[4]", arr);
```

Is invalid syntax, the `A` must be the capital part.

Enums can also be optional:

```pawn
sscanf("4", "E<ifs[32]c>(1, 12.0, Bob, c)", var);
```

In that code all values except `4` will be default.  Also, again, you can escape commas with `\\` in default enum strings.  Some final examples:

```pawn
sscanf("1", "I(2)I(3)I(4)", var0, var1, var2);
sscanf("", "O(045)H(0xF4)B(0b0100)U(Y_Less)", octnum, hexnum, binnum, user);
sscanf("0xFF", "N(0b101)");
```

That last example is of a specifier not too well described yet - the `number` specifier, which will work out the format of the number from the leading characters (0x, 0b, 0 or nothing).  Also note that the second example has changed - see the next section.

### Users

The `u`, `q`, and `r` specifiers search for a user by name or ID.  The method of this search has changed in the latest versions of `sscanf`.

Additionally `U`, `Q`, and `R` used to take a name or ID as their default value - this has since been changed to JUST a number, and sscanf will not try and determine if this number is online:

Previous:

```pawn
sscanf(params, "U(Y_Less)", id);
if (id == INVALID_PLAYER_ID)
{
    // Y_Less or the entered player is not connected.
}
```

New:

```pawn
sscanf(params, "U(-1)", id);
if (id == -1)
{
    // No player was entered.
}
else if (id == INVALID_PLAYER_ID)
    // Entered player is not connected.
}
```

See the section on options for more details.

Users can now optionally return an ARRAY of users instead of just one.  This array is just a list of matched IDs, followed by `INVALID_PLAYER_ID`.  Given the following players:

```
0) Y_Less
1) [CLAN]Y_Less
2) Jake
3) Alex
4) Hass
```

This code:

```pawn
new ids[3], i;
if (sscanf("Le", "?<MATCH_NAME_PARTIAL=1>u[3]", ids)) printf("Error in input");
for (i = 0; ids[i] != INVALID_PLAYER_ID; ++i)
{
    if (ids[i] == cellmin)
    {
        printf("Too many matches");
        break;
    }
    printf("id = %d", ids[i]);
}
if (i == 0) printf("No matching players found.");
```

Will output:

```
id = 0
id = 1
Too many matches
```

Searching `Les` instead will give:

```
id = 0
id = 1
```

And searching without `MATCH_NAME_PARTIAL` will give:

```
No matching players found.
```

Basically, if an array of size `N` is passed, this code will return the first N-1 results.  If there are less than `N` players whose name matches the given name then that many players will be returned and the next slot will be `INVALID_PLAYER_ID` to indicate the end of the list.  On the other hand if there are MORE than `N - 1` players whose name matches the given pattern, then the last slot will be `cellmin` to indicate this fact.

When combined with `U` and returning the default, the first slot is always exactly the default value (even if that's not a valid connected player) and the next slot is always `INVALID_PLAYER_ID`.

Note also that user arrays can't be combined with normal arrays or enums, but normal single-return user specifiers still can be.

Just like with array sizes you can use `*` to pass an additional variable for the default, so you can do:

```pawn
sscanf(params, "I(*)", INVALID_ID, output);
```

As with arrays the default value comes *before* the output value in the parameter list.  When combined with `[*]` the order is *default*, then *size*, then *output*:

```pawn
sscanf(params, "A<i>(*)[*]", DEFAULT_VALUE, ARRAY_SIZE, output);
```

String defaults can use `\` to escape `)` or `*` within them if you want a string to contain those values.

However, currently neither arrays nor strings use the value from the `*` parameter, but both will still skip the parameter.

### Custom (kustom) specifiers

The latest version of sscanf adds a new `k` specifier to allow you to define your own specifers in PAWN:

```pawn
sscanf(params, "uk<playerstate>", playerid, state);
```

`k<playerstate>` allows you to type a number or the textual name of a player state.  To convert this string name to a value we need a conversion function:

```pawn
@kustom() playerstate(string[])
{
    if ('0' <= string[0] <= '9')
    {
        new
            ret = strval(string);
        if (0 <= ret <= 9)
        {
            return ret;
        }
    }
    else if (!strcmp(string, "PLAYER_STATE_NONE")) return 0;
    else if (!strcmp(string, "PLAYER_STATE_ONFOOT")) return 1;
    else if (!strcmp(string, "PLAYER_STATE_DRIVER")) return 2;
    else if (!strcmp(string, "PLAYER_STATE_PASSENGER")) return 3;
    else if (!strcmp(string, "PLAYER_STATE_WASTED")) return 7;
    else if (!strcmp(string, "PLAYER_STATE_SPAWNED")) return 8;
    else if (!strcmp(string, "PLAYER_STATE_SPECTATING")) return 9;
}
```

The code above, when added to the top level of your mode, will add the `playerstate` specifier used in `k<playerstate>`.  With this a user can either type a number or the full name of a state and the function will convert the name or number to a number.

This system supports optional custom specifiers (`K`) with no additional PAWN code.  This optional kustom specifier takes a default value that is NOT (as of sscanf 2.8) parsed by the given callback:

```pawn
sscanf(params, "uK<playerstate>(0)", playerid, state);
```

So in this example `999` is NOT a valid vehicle model, but if no other value is supplied then 999 will be returned, allowing you to differentiate between the user entering an invalid vehicle and not entering anything at all:

```
K<vehicle>(999)
```

The new version of `sscanf2.inc` includes functions for `k<weapon>` and `k<vehicle>` allowing you to enter either the ID or name and get the ID back, but both are VERY basic at the moment and I expect other people will improve on them.

Note that custom specifiers always take a string input and always return a number, but this can be a Float, bool, or any other single cell tag type.

Also as of sscanf 2.8, `k` can be used in both arrays and enums.  Prior to sscanf 2.13.3 `@kustom()` was `SSCANF:`; [this is bad for many reasons](https://github.com/pawn-lang/YSI-Includes/blob/5.x/annotations.md) but the old version still works of course (because backwards compatibility is important).

### Colours

sscanf 2.10.0 introduced colours in addition to normal hex numbers.  They are parsed almost identically, but have slightly more constraints on their forms.  Colours must be HEX values exactly 3, 6, or 8 digits long.  3 digit numbers are as in CSS - `#RGB` becomes `0xRRGGBBAA` with default alpha, 6 digit numbers are already `0xRRGGBBAA` with default alpha, 8 digit numbers are the full colour with alpha.  The default default alpha is `255` (`FF`), but this can be changed with the `SSCANF_ALPHA` option; for example setting the default alpha to `AA` would be `?<SSCANF_ALPHA=170>`.  Why do they use `m`, not some sane letter?  Simple - all the good descriptive letters were already used.

The different lengths have slightly different semantics in what is accepted, to reduce the changes of incorrect values being parsed.  You can also customise exactly which input types you accept with the `SSCANF_COLOUR_FORMS` option.

#### 3 digits

A 3-digit hex value MUST be prefixed with `#` as in CSS, and each component is multiplied by `0x11` to give the final component value.  `#FAB` would become `0xFFAABBFF`, `#123` would become `0x112233FF`, `000` would be rejected because there is no `#`.

#### 6 digits

A 6-digit hex colour MAY be prefixed by `#` as in CSS, but doesn't have to be; it can also be prefixed by `0x` or nothing at all.  `#123456`, `0x123456`, and `123456` are all the same value, all valid, and will all give an output of `0x123456FF` with the default alpha value.  Furthermore, a 6-digit hex value may be optionally enclosed in `{}`s - `{8800DD}` is valid, but no other length in `{}`s are valid.

More valid examples:

* `FFFFFF`
* `0x000000`
* `0x010101`
* `#EEEEEE`
* `{000000}`

More invalid examples:

* `FFFFFFF` - 7 digits
* `0x00000` - 5 digits
* `#EEEE` - 4 digits
* `{}` - 0 digits
* `{BBB}` - 3 digits, but not `#` prefix
* `{12345678}` - 8 digits, but inside `{}`s`
* `{123456` - 6 digits, but no closing `}`.

#### 8 digits

8-digit colours are the simplest - the alpha is specified explicitly and there are only two possible input forms - `0x` prefix and no prefix.  I.e. either `0x88995566` or `88995566`.

### End Of Input

The `!` specifier must come at the end of a specifier and makes the match much stricter.  This will pass:

```pawn
sscanf("4 5 6 7 8", "iii", a, b, c);
```

This will not:

```pawn
sscanf("4 5 6 7 8", "iii!", a, b, c);
```

The `!` at the end checks that there is no input (except whitespace) remaining.

## Options

The latest version of sscanf introduces several options that can be used to customise the way in which sscanf operates.  There are two ways of setting these options - globally and locally:

```pawn
SSCANF_Option(SSCANF_QUIET, 1);
```

This sets the `SSCANF_QUIET` option globally.  Every time `sscanf` is called the option (see below) will be in effect.  Note that the use of `SSCANF_QUIET` instead of the string `"SSCANF_QUIET"` is entirely valid here - all the options are defined in the sscanf2 include already (but you can use the string if you want).

Alternatively you can use `?` to specify an option locally - i.e. only for the current sscanf call:

```pawn
sscanf(params, "si", str, num);
sscanf(params, "?<SSCANF_QUIET=1>si", str, num);
sscanf(params, "si", str, num);
```

`s` without a length is wrong, and the first and last `sscanf` calls will give an error in the console, but the second one won't as for just that one call prints have been disabled.  The following code disables prints globally then enables them locally:

```pawn
SSCANF_Option(SSCANF_QUIET, 1);
sscanf(params, "si", str, num);
sscanf(params, "?<SSCANF_QUIET=0>si", str, num);
sscanf(params, "si", str, num);
```

Note that disabling prints is a VERY bad idea when developing code as you open yourself up to unreported buffer overflows when no length is specified on strings.

To specify multiple options requires multiple calls:

```pawn
SSCANF_Option(SSCANF_QUIET, 1);
SSCANF_Option(MATCH_NAME_PARTIAL, 0);
sscanf(params, "?<SSCANF_QUIET=1>?<MATCH_NAME_PARTIAL=0>s[10]i", str, num);
```

You can also read the current value of an option by ommitting the second parameter:

```pawn
new quiet = SSCANF_Option(SSCANF_QUIET);
```

The options are:

### OLD_DEFAULT_NAME:

The behaviour of `U`, `Q`, and `R` have been changed to take any number as a default, instead of a connected player.  Setting `OLD_DEFAULT_NAME` to `1` will revert to the old version.

### MATCH_NAME_PARTIAL:

Currently sscanf will search for players by name, and will ALWAYS search for player whose name STARTS with the specified string.  If someone types `Y_Less`, sscanf will not find say `[CLAN]Y_Less` because there name doesn't start with the specified text.  This option, when set to 1, will search ANYWHERE in the player's name for the given string.

### CELLMIN_ON_MATCHES:

Whatever the value of `MATCH_NAME_PARTIAL`, the first found player will always be returned, so if you do a search for `_` on an RP server, you could get almost anyone.  To detect this case, if more than one player will match the specified string then sscanf will return an ID of `cellmin` instead.  This can be combined with `U` for a lot more power:

```pawn
sscanf(params, "?<CELLMIN_ON_MATCHES=1>U(-1)", id);
if (id == -1)
{
	// No player was entered.
}
else if (id == cellmin)
{
	// Multiple matches found
}
else if (id == INVALID_PLAYER_ID)
{
	// Entered player is not connected.
}
else
{
	// Found just one player.
}
```

### SSCANF_QUIET:

Don't print any errors to the console.  REALLY not recommended unless you KNOW your code is stable and in production.

### OLD_DEFAULT_KUSTOM:

As with `U`, `K` used to require a valid identifier as the default and would parse it using the specified callback, so this would NOT work:

```
K<vehicle>(Veyron)
```

Because that is not a valid vehicle name in GTA.  The new version now JUST takes a number and returns that regardless:

```
K<vehicle>(9999)
```

This setting reverts to the old behaviour.

### SSCANF_ALPHA:

Specify the default alpha value for colours (`m`) which don't manually specify an alpha channel.  The alpha values are specified as a ***DECIMAL*** number, ***NOT*** a ***HEX*** number, so setting an alpha of `0x80` would be:

```pawn
SSCANF_Option(SSCANF_ALPHA, 128);
```

### SSCANF_COLOUR_FORMS:

There are multiple valid colour input formats, which you can enable or disable here.  The parameter is a bit map (flags) for all the following values:

* `1` - `#RGB`
* `2` - `#RRGGBB`
* `4` - `0xRRGGBB`
* `8` - `RRGGBB`
* `16` - `{RRGGBB}`
* `32` - `0xRRGGBBAA`
* `64` - `RRGGBBAA`

So to ONLY accept SA:MP `SendClientMessage` colours use:

```pawn
SSCANF_Option(SSCANF_COLOUR_FORMS, 16);
```

To only accept 8-digit values use:

```pawn
SSCANF_Option(SSCANF_COLOUR_FORMS, 96);
```

Default values (those specified between `()`s for `M`) ignore this setting - they can always use any form.

### SSCANF_ARGB:

Specify whether the returned colour is `ARGB` or `RGBA`:

```pawn
SSCANF_Option(SSCANF_ARGB, 1); // Set 3- and 6-digit colour outputs to `AARRGGBB`.
SSCANF_Option(SSCANF_ARGB, 0); // Set 3- and 6-digit colour outputs to `RRGGBBAA` (default).
```

### MATCH_NAME_SIMILARITY:

Use the same text similarity metrics as in kustom matchers to find the best name match to a given input.  The value given is the cutoff threshold for matches.  A value of `-1` disables this setting:

```pawn
sscanf("Y_Lass", "?<MATCH_NAME_SIMILARITY=0.3>u", id);
```

Will probably find `Y_Less` as the closest matching name.  A similarity of `1.0` would return only exact matches; a similarity of `0.0` will always return something, even if the input is total gibberish.  When set (i.e. not `-1`) this option overrides `MATCH_NAME_PARTIAL`, but works well with all the other name matching options.  This is the only float option, and so needs a tag override to read:

```pawn
new Float:similarity = Float:SSCANF_Option(MATCH_NAME_SIMILARITY);
```

### ERROR_CODE_IN_RET:

The return value from `sscanf` is `0` for no error, or the index of the specifier that failed.  If you get a failure you can call `SSCANF_GetLastError` to get the exact error code, or you can set this option to `true` to have the return value also include this error value in the return, ORed with the index.

### WARNINGS_AS_ERRORS:

This setting allows `sscanf` to fail when a warning is given, as well as an error.

### ERROR_CATEGORY_ONLY:

When `ERROR_CODE_IN_RET` is enabled returns the error category instead of the exact error code.

## `extract`

I've written some (extendable) macros so you can do:

```pawn
extract params -> new a, string:b[32], Float:c; else
{
    return SendClientMessage(playerid, COLOUR_RED, "FAIL!");
}
```

This will compile as:

```pawn
new a, string:b[32], Float:c;
if (unformat(params, "is[32]f", a, b, c))
{
    return SendClientMessage(playerid, COLOUR_RED, "FAIL!");
}
```

Note that `unformat` is the same as `sscanf`, also note that the `SendClientMessage` part is optional:

```pawn
extract params -> new a, string:b[32], Float:c;
```

Will simply compile as:

```pawn
new a, string:b[32], Float:c;
unformat(params, "is[32]f", a, b, c);
```

Basically it just simplifies sscanf a little bit (IMHO).  I like new operators and syntax, hence this, examples:

```pawn
// An int and a float.
extract params -> new a, Float:b;
// An int and an OPTIONAL float.
extract params -> new a, Float:b = 7.0;
// An int and a string.
extract params -> new a, string:s[32];
// An int and a playerid.
extract params -> new a, player:b;
```

As I say, the syntax is extendable, so to add hex numbers you would do:

```pawn
#define hex_EXTRO:%0##%1,%2|||%3=%9|||%4,%5) EXTRY:%0##%1H"("#%9")"#,%2,%3|||%4|||%5)
#define hex_EXTRN:%0##%1,%2|||%3|||%4,%5) EXTRY:%0##%1h,%2,%3|||%4|||%5)
#define hex_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<h>[*],%2,(%7),%3|||%4|||%5)
```

That will add the tag `hex` to the system.  Yes, the lines look complicated (because they are), but the ONLY things you need to change are the name before the underscore and the letter near the middle (`H`, `h` and `a<h>` in the examples above for `optional`, `required` and `required array` (no optional arrays yet besides strings)).

New examples (with `hex` added):

```pawn
// A hex number and a player.
extract params -> new hex:a, player:b;
// 32 numbers then 32 players.
extract params -> new a[32], player:b[32];
// 11 floats, an optional string, then an optional hex number.
extract params -> new Float:f[11], string:s[12] = "optional", hex:end = 0xFF;
```

The code is actually surprisingly simple (I developed another new technique to simplify my `tag` macros and it paid off big style here).  By default `Float`, `string`, `player` and `_` (i.e. no tag) are supported, and their individual letter definitions take up the majority of the code as demonstrated with the `hex` addition above.  Note that `string:` is now used extensively in my code to differentiate from tagless arrays in cases like this, it is removed automatically but `player:` and `hex:` are not so you may wish to add:

```pawn
#define player:
#define hex:
```

To avoid tag mismatch warnings (to remove them AFTER the compiler has used them to determine the correct specifier).

The very first example had an `else`, this will turn:

```pawn
unformat(params, "ii", a, b);
```

In to:

```pawn
if (unformat(params, "ii", a, b))
```

You MUST put the `else` on the same line as `extract` for it to be detected, but then you can use normal single or multi-line statements.  This is to cover common command use cases, you can even leave things on the same line:

```pawn
else return SendClientMessage(playerid, 0xFF0000AA, "Usage: /cmd <whatever>");
```

There is now the ability to split by things other than space (i.e. adds `P<?>` to the syntax - updated from using `p` to `P`):

```pawn
extract params<|> -> new a, string:b[32], Float:c;
```

Will simply compile as:

```pawn
new a, string:b[32], Float:c;
unformat(params, "P<|>is[32]f", a, b, c);
```

Note that for technical reasons you can use `<->` (because it looks like the arrow after the `extract` keyword).  You also can't use `<;>`, `<,>`, or `<)>` because of a bug with `#`, but you can use any other character (most notably `<|>`, as is popular with SQL scripts).  I'm thinking of adding enums and existing variables (currently you HAVE to declare new variables), but not right now.

## Similarity

A lot of the code uses a concept of "similarity" when comparing two strings.  This is less accurate, but thus more forgiving, than an exact string comparison.  For example if a player types `nrg` they probably mean the `NRG-500` bike, and if they type `mac10` they probably mean the `Mac 10` gun.  Using standard comparisons these inputs would never match, and using the common "Levenshtein Distance" (as old versions of the code did) can produce some strange results.  For example typing `NRG` as a vehicle input will return `TUG` using Levenshtein, because converting `NRG` to `TUG` only requires three replacements, whereas converting `NRG` to `NRG-500` requires four additions, thus is technically further away despite being more similar to a person.

Thus the new version of sscanf uses a "similarity" metric instead of a "distance" metric to work out which strings are probably the same.  This is used by the `k<vehicle>` and `k<weapon>` pre-defined custom specifiers, and for the `MATCH_NAME_SIMILARITY` option used with `u`, `r`, and `q` (i.e. for names).  The algorithm is roughly based on one of "Bigram Similarity":

* Split the two input strings up in to letter pairs, ignoring punctuation and spaces.  For example `"Shotty"` becomes `"sh", "ho", "ot", "tt", "ty"` and `"Combat Shotgun"` becomes `"co", "om", "mb", "ba", "at", "ts", "sh", "ho", "ot", "tg", "gu", "un"`.

* For each pair of letters from the first word, count how many of them appear in the second word.  For example `"Combat Shotgun"` contains `"sh", "ho", "ot"` from `"Shotty"`, 3/5 (60%) of the available letter pairs.

* Repeat in the reverse, so `"Shotty"` contains `"sh", "ho", "ot"` from `"Combat Shotgun"`, 3/12 (25%) of the available letter pairs (the fact that the two result lists are the same here is pure fluke and irrelevant).

* Invert the fractions to get the percentage of pairs not in the other word, multiply the two numbers, and subtract from `1.0` to give the final result.  So for the example above:

```
similarity = 1 - ((1 - 60%) * (1 - 25%))
similarity = 1 - (0.4 * 0.75)
similarity = 1 - 0.3
similarity = 0.7
```

A similarity is always between `0.0` (totally different) and `1.0` (absolutely identical), and many functions will return the string from a list with the highest similarity.  So `k<vehicle>` will find the vehicle with the best similarity to the given input.  However, this doesn't always make sense.  If the input is `"dkaoiingsjk"`, that is obviously not any sane vehicle, but it still has a similarity rating to every vehicle name; the ratings will be very low, but one must be the highest (it is the `"Sandking"`, with a similarity rating of `0.228571`).  Just calling the similarity functions blindly will thus always return a result, even if that result is gibberish, thus the functions also optionally accept a *threshold*, a similarity value that the result must be better than.  So for the above example passing a threshold of `0.5` would result in no valid vehicle being returned.

## Error Returns

When everything is fine, `sscanf` returns `0` - meaning "no error".  When it isn't fine the return value will tell you exactly where and what the error was.  By default the return value is just the index (plus one) of the specifier that failed:

```pawn
index = sscanf("bad input", "ii", a, b); // Returns `1`
```

Here the return value is `1` because the first `i` failed to parse a number.  Normally this should be index `0`, but that would mean "no error", so we must increase the offset by one.  If the first specifier passes but the second one fails, then the return value will be `2`:

```pawn
index = sscanf("45 input", "ii", a, b); // Returns `2`
```

The same is true for missing data instead of bad data:

```pawn
index = sscanf("45", "ii", a, b); // Returns `2`
```

If you want more information about exactly what went wrong you can request this using `SSCANF_GetLastError`:

```pawn
index = sscanf("45", "ii", a, b); // Returns `2`
if (index)
{
	error = SSCANF_GetLastError(); // Returns `1004` ("out of input")
}
```

The error codes will persist internally as the "last" error until a new sscanf function is called (with the exception of `SSCANF_GetLastError`, hence why `SSCANF_ClearLastError` also exists to reset this value without any other side-effects).

Alternatively you can set an option for `sscanf` to return both the index and error code together:

```pawn
SSCANF_Option(ERROR_CODE_IN_RET, 1);

error = sscanf("45 hello", "ii", a, b); // Returns `SSCANF_ERROR(2, 1011)`
error = sscanf("45", "ii", a, b); // Returns `SSCANF_ERROR(2, 1004)`
```

Now instead of returning just `2` to indicate the index of the specifier at which the error was encountered; the call also returns an error code representing exactly what the problem was.  `1004` means "out of input".  All the other possible codes are documented in this file in the sections [Errors/Warnings](#errorswarnings) and [Additional Codes](#additional-codes).

If you want slighty more information than just the index, but also not quite as much information as the exact error code you can use the error categories instead.  These categories group the (currently around eighty) error codes in to a few manageable subdivisions.  See [Error Categories](#error-categories) for the full list.  Use `SSCANF_GetErrorCategory` to get this reduced information:

```pawn
index = sscanf("45", "ii", a, b); // Returns `2`
if (index)
{
	error = SSCANF_GetLastError(); // Returns `1004` ("out of input")
	category = SSCANF_GetErrorCategory(error); // Returns `SSCANF_ERROR_MISSING`
}
```

Or again, using options in the previous example:

```pawn
SSCANF_Option(ERROR_CODE_IN_RET, 1);
SSCANF_Option(ERROR_CATEGORY_ONLY, 1);

error = sscanf("45 hello", "ii", a, b); // Returns `SSCANF_ERROR(2, SSCANF_ERROR_INVALID)`
error = sscanf("45", "ii", a, b); // Returns `SSCANF_ERROR(2, SSCANF_ERROR_MISSING)`
```

The `SSCANF_ERROR` macro is slightly clever and allows you to avoid a tiny bit of repetition when using categories:

```pawn
SSCANF_Option(ERROR_CODE_IN_RET, 1);
SSCANF_Option(ERROR_CATEGORY_ONLY, 1);

error = sscanf("45 hello", "ii", a, b); // Returns `SSCANF_ERROR(2, INVALID)`
error = sscanf("45", "ii", a, b); // Returns `SSCANF_ERROR(2, MISSING)`
```

sscanf has both "errors", which stop processing, and "warnings", which can be continued on from but may give strange results.  The distinction is currently a little blurred since some errors actually continue like warnings (so should be renamed).  The error system will not report warnings, including things like `sscanf warning: String buffer overflow.`.  By default.  This can be changed with yet a third setting:

```pawn
new str[8]
error = sscanf("a very long string", "?<ERROR_CODE_IN_RET=1>?<ERROR_CATEGORY_ONLY=1>?<WARNINGS_AS_ERRORS=1>?<SSCANF_QUIET=1>s[8]", str);
```

This code will return an error of `SSCANF_ERROR(5, OVERFLOW)` and will not (thanks to `SSCANF_QUIET`) print anything in the console.  This problem can finally be dealt with entirely in-code.  Note that the index here is `5`, not `1`, because EVERY specifier counts towards this offset.  There are four `?` specifiers in the string, hence the `s` is `5`.

It should be noted that `sscanf` and `SSCANF_GetLastError` will return the last error/warning generated by the call.  If there is more than one problem only the final one will be reported.

### Additional Codes

Error codes under 1000 relate to printed messages, and are documented along-side their messages in [Section 14](#errorswarnings).  Error codes above 1000 are regular failures:

* *1001* - A colour was given, but it doesn't match an allowed format:

```pawn
sscanf(input, "?<SSCANF_COLOUR_FORMS=12>m", "#112233", colour);
```

* *1002* - `SSCANF_Join` was called with an invalid player ID:

```pawn
SSCANF_Join(134234);
```

* *1003* - The search string was not found in the input:

```pawn
sscanf("some input", "'hello'");
```

* *1004* - Arguably the most basic failure - not enough input:

```pawn
sscanf("6", "ii", a, b);
```

* *1005* - `SSCANF_Leave` was called with an invalid player ID:

```pawn
SSCANF_Leave(134234);
```

* *1006* - The matrix was not allocated correctly in `SSCANF_Levenshtein`.

* *1007* - The text compared with `SSCANF_TextSimilarity` is too short:

```pawn
SSCANF_TextSimilarity("a", "b");
```

* *1008* - `SSCANF_IsConnected` was called with an invalid player ID:

```pawn
SSCANF_IsConnected(134234);
```

* *1009* - Excess data seen, with a specifier ending `!`.

```pawn
sscanf("4 5 6", "ii!", a, b);
```

* *1010* - No matching alternate found:

```pawn
sscanf("0x11 0x22", "ii|bb", variant, a, b, c, d);
```

* *1011* - An int was wanted, but the input didn't match:

```pawn
sscanf("hello", "i", output);
```

* *1012* - A number was wanted, but the input didn't match:

```pawn
sscanf("hello", "n", output);
```

* *1013* - A hex was wanted, but the input didn't match:

```pawn
sscanf("hello", "h", output);
```

* *1014* - A colour was wanted, but the input didn't match:

```pawn
sscanf("hello", "m", output);
```

* *1015* - An octal was wanted, but the input didn't match:

```pawn
sscanf("0xFF", "o", output);
```

* *1016* - A float was wanted, but the input didn't match:

```pawn
sscanf("NAN", "f", output);
```

* *1017* - A character was wanted, but the input didn't match:

```pawn
sscanf("long", "c", output);
```

* *1018* - A binary was wanted, but the input didn't match:

```pawn
sscanf("44", "b", output);
```

* *1019* - An extended (IEEE 754) float was wanted, but the input didn't match:

```pawn
sscanf("IsANumber", "g", output);
```

### Error Categories

There are almost 100 different unique error codes, but many of them can often be dealt with in the same way.  For example - "***1011* - An int was wanted, but the input didn't match**" and "***1012* - A number was wanted, but the input didn't match**" can probably usually use the same result processing (especially since they can never be returned by the same specifier).  Hence the errors are grouped together in to a few large groups for ease of processing.  These categories are:

* `SSCANF_ERROR_NONE` - Not an error.
* `SSCANF_ERROR_NATIVE` - A generic problem with calling a native, such as invalid parameters or out of memory.
* `SSCANF_ERROR_SPECIFIER` - The specifier itself is wrong in some way.  This is always a coding error.
* `SSCANF_ERROR_INVALID` - The parsed input was present, but invalid.  For example `hello` for an integer.
* `SSCANF_ERROR_MISSING` - Required input data was missing.
* `SSCANF_ERROR_EXCESS` - There was too much input data (with `!`).
* `SSCANF_ERROR_COLOUR` - A colour was parsed, but was not one of the accepted formats.
* `SSCANF_ERROR_OVERFLOW` - The infamous "string buffer overflow" warning.  Enable warnings as errors to get this in code.
* `SSCANF_ERROR_NOT_FOUND` - The search string (`''`) wasn't found.
* `SSCANF_ERROR_NO_ALTS` - None of the alternatives (`|`) matched.

You may notice that the last few "groups" are very small compared to the others, but they cover issues that are both unique and easy to rectify in code.

## Alternates

Alternates allow you to create two different specifier strings and find the best (or first) match.  Using this you could for example detect whether something is a boolean, a decimal, or a hexadecimal.  The `|` specifier splits the different options, and will report the branch taken (or `0` if it can't find one) in the first return parameter.  Each other specifier must then appear in the parameter list separately:

```pawn
new alt, b, i, h;
if (sscanf("45", "b|i|h", alt, b, i, h)) == 0)
{
  switch (alt)
	{
		case 1: printf("Typed a binary number.");
		case 2: printf("Typed a decimal number.");
		case 3: printf("Typed a hexadecimal number.");
	}
}
```

`45` is not a boolean number, therefore the first branch fails.  `45` is a decimal number, therefore the second branch succeedes, the variable `i` is set to `45`, and the variable `alt` is set to `2` because the second branch was successful.  `b` and `h` may be un-initialised after this call.  You could re-use the same variable for all three numeric outputs:

```pawn
new alt, num;
if (sscanf("45", "b|i|h", alt, num, num, num)) == 0)
{
  switch (alt)
	{
		case 1: printf("Typed a binary number.");
		case 2: printf("Typed a decimal number.");
		case 3: printf("Typed a hexadecimal number.");
	}
}
```

The order here is important  - `45` is a valid integer, but it is also a valid hexadecimal number; `111` is valid for all three.  Thus we go most specific to least specific.  In the other order every single call will return *hexadecimal*:

```pawn
new alt, num;
if (sscanf("45", "h|i|b", alt, num, num, num)) == 0)
{
  switch (alt)
	{
		case 1: printf("Typed a hexadecimal number.");
		case 2: printf("Will never happen.");
		case 3: printf("Will never happen.");
	}
}
```

The order also matters a lot if you have default values:

```pawn
sscanf("42", "iS[*](hello)|i", alt, num, sizeof (str), str, num);
```

That will return `alt == 1` and set `str` to `"hello"`, despite the fact that there is only one numeric input and the second branch is a better match.  Alternates do not look for the *best* match, they look for the *first* match.

One very good use for alternates is string matching.  See [A Basic Shop](#a-basic-shop) for an example of using `''` in multiple alternates.

Temporary settings will NOT persist betwen alternate branches:

```pawn
SSCANF_SetOption(SSCANF_COLOUR_FORMS, 2);
sscanf("#567890", "?<SSCANF_COLOUR_FORMS=16>m|m", alt, colour1, colour2);
// `alt` is `2`.
// `colour1` is undefined.
// `colour2` is `0x56789000`
```

The first branch will fail because the input is not in colour form `16` (`{RRGGBB}`), but after the `|` the local options are reset to the global options and the colour *is* in form `2` (`#RRGGBB`).  Weirdly this means that errors relating to the alternate itself (such as `SSCANF_ERROR(1, 1010)`) will always use the global settings, because no local settings can ever apply to `|`.

Note that if you combine alternates and error returns you probably want to disable `ERROR_CODE_IN_RET` and just use `SSCANF_GetLastError`, and you absolutely want to disable `WARNINGS_AS_ERRORS`.  Enabling warnings as errors can make alternates that would normally pass, fail:

```pawn
sscanf("5 a-long-string", "is[8]|ii", alt, int, string, a, b);
```

That code will pass and set `alt` to `1` when `WARNINGS_AS_ERRORS` is disabled, but will fail with code `1010` when `WARNINGS_AS_ERRORS` is enabled because the first alternative will give a string buffer overflow warning, then turn that in to an error.  An alternate with an error can't possibly be correct.  However, `SSCANF_GetLastError` will still report the overflow while allowing the alternate to be found.

The errors will also be the last found error, probably in the final alternate section.  The specifier index is relative to the start of the format string, plus `1` for the alternates.  In the specifier `ii|x` the `x` has index `4` - `+1` for the eternal error base, `+1` for the alternate specifier (regardless of the number of `|`s) and `+2` for `ii`.  In `bb|ii|x` the `x` has index `6`, because despite there being two `|`s, alternates are just one thing.  They are also always specifier index `1` (despite the fact that they don't appear at the start of the string); so if `sscanf` returns `1`, you know it means "no alternate match found".

## All Specifiers

For quick reference, here is a list of ALL the specifiers and their use:

|                  Format                  |                   Use                  |
| ---------------------------------------- | -------------------------------------- |
|  `A<type>(default)[length]`              |  Optional array of given type          |
|  `a<type>[length]`                       |  Array of given type                   |
|  `B(binary)`                             |  Optional binary number                |
|  `b`                                     |  Binary number                         |
|  `C(character)`                          |  Optional character                    |
|  `c`                                     |  Character                             |
|  `D(integer)`                            |  Optional integer                      |
|  `d`                                     |  Integer                               |
|  `E<specification>(default)`             |  Optional enumeration of given layout  |
|  `e<specification>`                      |  Enumeration of given layout           |
|  `F(float)`                              |  Optional floating point number        |
|  `f`                                     |  Floating point number                 |
|  `G(float/INFINITY/-INFINITY/NAN/NAN_E)` |  Optional float with IEEE definitions  |
|  `g`                                     |  Float with IEEE definitions           |
|  `H(hex value)`                          |  Optional hex number                   |
|  `h`                                     |  Hex number                            |
|  `I(integer)`                            |  Optional integer                      |
|  `i`                                     |  Integer                               |
|  `K<callback>(any format number)`        |  Optional custom operator              |
|  `k<callback>`                           |  Custom operator                       |
|  `L(true/false)`                         |  Optional logical truthity             |
|  `l`                                     |  Logical truthity                      |
|  `M(hex value)`                          |  Optional colour                       |
|  `m`                                     |  Colour                                |
|  `N(any format number)`                  |  Optional number                       |
|  `n`                                     |  Number                                |
|  `O(octal value)`                        |  Optional octal value                  |
|  `o`                                     |  Octal value                           |
|  `P<delimiters>`                         |  Multiple delimiters change            |
|  `p<delimiter>`                          |  Delimiter change                      |
|  `Q(any format number)`                  |  Optional bot (bot)                    |
|  `q`                                     |  Bot (bot)                             |
|  `R(any format number)`                  |  Optional player (player)              |
|  `r`                                     |  Player (player)                       |
|  `S(string)[length]`                     |  Optional string                       |
|  `s[length]`                             |  String                                |
|  `U(any format number)`                  |  Optional user (bot/player)            |
|  `u`                                     |  User (bot/player)                     |
|  `X(hex value)`                          |  Optional hex number                   |
|  `x`                                     |  Hex number                            |
|  `Z(string)[length]`                     |  Optional packed string                |
|  `z[length]`                             |  Packed string                         |
|  `'string'`                              |  Search string                         |
|  `{`                                     |  Open quiet section                    |
|  `}`                                     |  Close quiet section                   |
|  `%`                                     |  Deprecated optional specifier prefix  |
|  `?`                                     |  Local options specifier               |
|  `!`                                     |  Strict end of input check             |
|  `|`                                     |  Alterates                             |

## Full API

### `sscanf(const data[], const format[], {Float, _}:...);`

The main sscanf function.

### `unformat(const data[], const format[], {Float, _}:...);`

An alternate name for the main `sscanf` function, since the sscanf format specifiers do not conform
to the C API of the same name.

### `SSCANF_Option(const name[], value);`

Set an option.

### `SSCANF_Option(const name[]);`

Get an option.

### `SSCANF_SetOption(const name[], value);`

Set an option explicitly (no overloaded `SSCANF_Option` call).

### `SSCANF_GetOption(const name[], value);`

Get an option explicitly (no overloaded `SSCANF_Option` call).

### `SSCANF_Version(version[], size = sizeof (version));`

Get the SSCANF plugin version as a string (e.g. `"2.11.2"`).  Compare to the macro `SSCANF_VERSION_STRING`.

### `SSCANF_Version();`

Get the SSCANF plugin version as binary coded decimal (BCD) number (e.g. `0x021003`).  Compare to the macro `SSCANF_VERSION_BCD`.

### `SSCANF_VersionString(version[], size = sizeof (version));`

Get the SSCANF plugin version as a string explicitly (no overloaded `SSCANF_Version` call).

### `SSCANF_VersionBCD();`

Get the SSCANF plugin version as BCD explicitly (no overloaded `SSCANF_Version` call).

### `SSCANF_Levenshtein(const string1[], const string2[]);`

Computes the [Levenshtein Distance](https://en.wikipedia.org/wiki/Levenshtein_distance) between the two input strings.  Useful in `k` callback functions to determine if the entered string is close to a possible string.

### `Float:SSCANF_TextSimilarity(const string1[], const string2[]);`

This works out the similarity between two strings.  The Levenshtein distance often produces results that seem weird to people, for example by that measure `NRG` is closer to `TUG` than `NRG-500`.  Instead this function uses various other (unfixed) algorithms; currently comparing all pairs of letters between the two strings to work out what percentage of each string is in the other string, then multiplying the results to get the final similarity.  This algorithm produces much more human sane results, and can handle things like `ls police` matching`Police Car (LSPD)`.  It ignores all punctuation and case as well.

### `SSCANF_GetClosestString(const input[], const candidates[][], threshold = cellmax, count = sizeof (candidates));`

Takes an input string and an array of string possibilities (candidates) and returns the index of the string closest to the input string.  If no valid match is found, `-1` is returned.  Note that this will always return the closest, even if the closest is not that close; which is why an optional `threshold` parameter is available.  When this parameter is provided the closest match must be closer in Levenshtein distance than the threshold, otherwise again `-1` is returned.  Deprecated in favour of `SSCANF_GetSimilarString`.

### `SSCANF_GetClosestValue(const input[], const candidates[][], const results[], fail = cellmin, threshold = cellmax, count = sizeof (candidates), check = sizeof (results));`

Similar to [`SSCANF_GetClosestString`](#sscanf_getcloseststringconst-input-const-candidates-threshold--cellmax-count--sizeof-candidates) in that it searches the `candidates` array for the string most closely matching the `input` and bounded by `threshold`.  But instead of returning the index this function returns the value in the second `results` array at that index; and instead of returning `-1` on failure it returns the value of `fail`.  The two arrays must match in size and an `assert` in the function checks for this.  Deprecated in favour of `SSCANF_GetSimilarValue`.

### `SSCANF_GetSimilarString(const input[], const candidates[][], Float:threshold = 0.111111, count = sizeof (candidates));`

Like `SSCANF_GetClosestString`, but using a more human-friendly text comparison function.

### `SSCANF_GetSimilarValue(const input[], const candidates[][], const results[], fail = cellmin, Float:threshold = 0.111111, count = sizeof (candidates), check = sizeof (results));`

Like `SSCANF_GetClosestValue`, but using a more human-friendly text comparison function.

### `SSCANF_VERSION_STRING`

The SSCANF include version as a string.

### `SSCANF_VERSION_BCD`

The SSCANF include version as BCD, as a `stock const` variable.

### `SSCANF_VERSION`

The SSCANF include version as BCD, as a `const` to work at compile-time.

### `SSCANF_NO_K_VEHICLE`

Exclude the default `k<vehicle>` kustom specifier from being compiled when this is defined before including the include file.

### `SSCANF_NO_K_WEAPON`

Exclude the default `k<weapon>` kustom specifier from being compiled when this is defined before including the include file.

### `SSCANF_NO_NICE_FEATURES`

Several sscanf features, such as file and line numbers in errors, only work on the new compiler.  If you want to use the old compiler those nice features won't work.  If you want to compile anyway without those features you need to define this symbol before inclusion.

### `SSCANF_GetLastError();`

Gets the error code set by the most recent call to any other sscanf function.  Calling this function does <b>not</b> clear the error, unlike some other implementations.  To do that call `SSCANF_ClearLastError`.

### `SSCANF_ClearLastError();`

Resets the error code from any previous *sscanf* function call.  Note that calling any other function also resets this value, but with more side-effects.

### `sscanf_error:SSCANF_GetErrorCategory(error);`

There are almost 100 different unique error codes, but many of them can often be dealt with in the same way.  For example - **`1011` - An int was wanted, but the input didn't match** and **`1012` - A number was wanted, but the input didn't match** can probably usually use the same result processing (especially since they can never be returned by the same specifier).  Hence the errors are grouped together in to a few large groups for ease of processing.

### `SSCANF_GetErrorSpecifier();`

Gets the index of the specifier that generated the last error.  When the error was in an alternate this will return the final underlying specifier that had an error, while `sscanf` itself will just return an index of `1` for "no matching alternative".  This is the only place where the two may be different.

### `SSCANF_Debug();`

Print a lot of useful debugging information.

## Errors/Warnings

### MSVRC100.dll not found

If you get this error, DO NOT just download the dll from a random website.  This is part of the `Microsoft Visual Studio Redistributable Package`.  This is required for many programs, but they often come with it.  Download it here:

http://www.microsoft.com/download/en...s.aspx?id=5555

### sscanf error: System not initialised

*Error Code: 1*

If you get this error, you need to make sure that you have recompiled ALL your scripts using the LATEST version of `sscanf2.inc`.  Older versions didn't really require this as they only had two natives - `sscanf` and `unformat`, the new version has some other functions - you don't need to worry about them, but you must use `sscanf2.inc` so that they are correctly called.  If you think you have done this and STILL get the error then try again - make sure you are using the correct version of PAWNO for example.

### sscanf warning: String buffer overflow.

*Error Code: 2*

This error comes up when people try and put too much data in to a string.  For example:

```pawn
new str[10];
sscanf("Hello there, how are you?", "s[10]", str);
```

That code will try and put the string `Hello there, how are you?` in to the variable called `str`.  However, `str` is only 10 cells big and can thus only hold the string `Hello ther` (with a NULL terminator).  In this case, the rest of the data is ignored - which could be good or bad:

```pawn
new str[10], num;
sscanf("Hello there you|42", "p<|>s[10]i", str, num);
```

In this case `num` is still correctly set to `42`, but the warning is given for lost data (`e you`).

Currently there is nothing you can do about this from a programming side (you can't even detect it - that is a problem I intend to address), as long as you specify how much data a user should enter this will simply discard the excess, or make the destination variable large enough to handle all cases.

### sscanf warning: Optional types invalid in array specifiers, consider using 'A'.

*Error Code: 3*

A specifier such as:

```
a<I(5)>[10]
```

Has been written - here indicating an array of optional integers all with the default value `5`.  Instead you should use:

```
A<i>(5)[10]
```

This is an optional array of integers all with the default value `5`, the advantage of this is that arrays can have multiple defaults:

```
A<i>(5, 6)[10]
```

That will set the array to `5, 6, 7, 8, 9, 10, 11, 12, 13, 14` by default, incrementing by the found difference each time.

### sscanf warning: Optional types invalid in enum specifiers, consider using 'E'.

*Error Code: 4*

Similar to the previous warning, A specifier such as:

```
e<I(5)f>
```

Is invalid, instead use:

```
E<if>(42, 11.0)
```

This forces ALL the parts of an enum to be optional - anything less is not possible.

### sscanf error: Multi-dimensional arrays are not supported.

*Error Code: 5*

This is not allowed:

```pawn
sscanf(params, "a<a<i>[5]>[10]", arr);
```

A work-around can be done using:

```pawn
sscanf(params, "a<i>[50]", arr[0]);
```

That will correctly set up the pointers for the system.

### sscanf error: Search strings are not supported in arrays.

*Error Code: 6*

This is not allowed (see the section on search strings):

```
a<'hello'i>[10]
```

### sscanf error: Delimiters are not supported in arrays.

*Error Code: 7*

This is not allowed:

```
a<p<,>i>[10]
```

Instead use:

```
p<,>a<i>[10]
```

### sscanf error: Quiet sections are not supported in arrays.

*Error Code: 8*

This is not allowed:

```
a<{i}>[10]
```

Instead use:

```
{a<i>[10]}
```

### sscanf error: Unknown format specifier '?'.

*Error Code: 9*

The given specifier is not known (this post contains a full list of all the specifiers near the bottom).

### sscanf warning: Empty default values.

*Error Code: 10*

An optional specifier has been set as (for example):

```
I()
```

Instead of:

```
I(42)
```

This does not apply to strings as they can be legitimately empty.

### sscanf warning: Unclosed default value.

*Error Code: 11*

You have a default value on an optional specifier that looks like:

```
I(42
```

Instead of:

```
I(42)
```

### sscanf warning: No default value found.

*Error Code: 12*

You have no default value on an optional specifier:

```
I
```

Instead of:

```
I(42)
```

### sscanf warning: Unenclosed specifier parameter.

*Error Code: 13*

You are using the old style:

```
p,
```

Instead of:

```
p<,>
```

Alternatively a custom delimiter of:

```
p<
```

Was found with no matching `>` after one character.  Instead use:

```
p<,>
```

Or, if you really do want a delimiter of `<` then use:

```
p<<>
```

Note that this does not need to be escaped; however, a delimiter of `>` does:

```
p<\>>
```

The `\` may also need to be escaped when writing actual PAWN strings, leading to:

```
p<\\>>
```

This also applies to array types (`a<` vs `a<i>`), and will result in an invalid array type.

### sscanf warning: No specified parameter found.

*Error Code: 14*

The format specifier just ends with:

```
p
```

This also applies to array types (`a` vs `a<i>`).

### sscanf warning: Missing string length end.

*Error Code: 15*

See below.

### sscanf warning: Missing length end.

*Error Code: 16*

A string has been written as:

```
s[10
```

Instead of:

```
s[10]
```

I.e. the length has not been closed.

### sscanf error: Invalid data length.

*Error Code: 17*

An invalid array or string size has been specified (0, negative, or not a number).

### sscanf error: Invalid character in data length.

*Error Code: 18*

A string or array has been given a length that is not a number.

### sscanf error: String/array must include a length, please add a destination size.

*Error Code: 19*

Arrays are newer than strings, so never had an implementation not requiring a length, so there is no compatibility problems in REQUIRING a length to be given.

### sscanf warning: Can't have nestled quiet sections.

*Error Code: 20*

You have tried writing something like this:

```
{i{x}}
```

This has a quiet section (`{}`) inside another one, which makes no sense.

### sscanf warning: Not in a quiet section.

*Error Code: 21*

`}` was found with no corresponding `{`:

```
i}
```

### sscanf warning: Can't remove quiet in enum.

*Error Code: 22*

This is caused by specifiers such as:

```
{fe<i}x>
```

Where the quiet section is started before the enum, but finishes part way through it rather than after it.  This can be emulated by:

```
{f}e<{i}x>
```

### sscanf error: Arrays are not supported in enums.

*Error Code: 23*

Basically, you can't do:

```
e<fa<i>[5]f>
```

You can, however, still do:

```
e<fiiiiif>
```

This is a little more awkward, but is actually more technically correct given how enums are compiled.

### sscanf warning: Unclosed string literal.

*Error Code: 24*

A specifier starts a string with `'`, but doesn't close it:

```
i'hello
```

### sscanf warning: sscanf specifiers do not require '%' before them.

*Error Code: 25*

`format` uses code such as `%d`, sscanf only needs `d`, and confusingly the C equivalent function (also called `sscanf`) DOES require `%`.  Sorry.

### sscanf error: Insufficient default values.

*Error Code: 26*

Default values for arrays can be partially specified and the remainder will be inferred from the pattern of the last two:

```
A<i>(0, 1)[10]
```

That specifier will default to the numbers `0` to `9`.  However, because enums have a mixture of types, all the default values for `E` must ALWAYS be specified:

```
E<iiff>(0, 1, 0.0, 1.0)
```

This will not do:

```
E<iiff>(0, 1)
```

### sscanf error: Options are not supported in enums.

*Error Code: 27*

The `?` specifier for local options must appear outside any other specifier.

### sscanf error: Options are not supported in arrays.

*Error Code: 28*

The `?` specifier for local options must appear outside any other specifier.

### sscanf error: No option value.

*Error Code: 29*

An option was specified with no value:

```
?<OLD_DEFAULT_NAME>
```

### sscanf error: Unknown option name.

*Error Code: 30*

The given option was not recognised.  Check spelling and case:

```
?<NOT_A_VALID_NAME=1>
```

### sscanf warning: Could not find function SSCANF:?.

*Error Code: 31*

A `k` specifier has been used, but the corresponding function could not be found.  If you think it is there check the spelling matches exactly - including the case.

### sscanf error: SSCANF_Init has incorrect parameters.

*Error Code: 32*

You edited something in the sscanf2 include - undo it or redownload it.

### sscanf error: SSCANF_Join has incorrect parameters.

*Error Code: 33*

You edited something in the sscanf2 include - undo it or redownload it.

### sscanf error: SSCANF_Leave has incorrect parameters.

*Error Code: 34*

You edited something in the sscanf2 include - undo it or redownload it.

### sscanf error: SSCANF_IsConnected has incorrect parameters.

*Error Code: 35*

You edited something in the sscanf2 include - undo it or redownload it.

### sscanf error: SSCANF_Version has incorrect parameters.

*Error Code: 36*

You edited something in the sscanf2 include - undo it or redownload it.

### sscanf error: SSCANF_Option has incorrect parameters.

*Error Code: 37*

You edited something in the sscanf2 include - undo it or redownload it.

### sscanf error: SetPlayerName has incorrect parameters.

*Error Code: 38*

You somehow managed to call `SetPlayerName` without passing all the parameters.  This can only happen by redefining the native declaration itself, so undo any edits to it.

### sscanf error: Missing required parameters.

*Error Code: 39*

`sscanf` itself was called without sufficient parameters.  I.e. the input and specifier strings are missing.  This can also happen when you edit the include itself to mess with the file/line macros.

You somehow managed to call `SetPlayerName` without passing all the parameters.  This can only happen by redefining the native declaration itself, so undo any edits to it.

### `fatal error 111: user error: sscanf already defined, or used before inclusion.`

There are two ways to trigger this:  The first is to have another copy of `sscanf` defined before you include the file.  This used to be the only known way to trigger this error, so the error said:

> `sscanf (possibly the PAWN version) already defined.`

However, there is a second way to trigger it - using `sscanf` before including it.  This used to be possible, but isn't any more as `sscanf` is now a macro that inserts additional data in to the call.  So this will fail:

```pawn
#include <a_samp>

main()
{
	sscanf("hi", "hi");sscanf (possibly the PAWN version) already defined.
}

#include <sscanf2>
```

To fix this, just include `<sscanf2>` before you use `sscanf`.

### `error 004: function "sscanf" is not implemented`

See below.

### `error 004: function "sscanf" is not implemented - include <sscanf2> first.`

These are the same error, the only difference being compilers and settings.  Obviously the more useful (second) error which tells you how to solve this problem.  Similar to [the previous error](#fatal-error-111-user-error-sscanf-already-defined-or-used-before-inclusion) this happens when `sscanf` is used before being included, but in a slightly different way:

```pawn
#include <a_samp>

main()
{
	#if defined sscanf
		sscanf("hi", "hi");
	#endif
}

#include <sscanf2>
```

This code tries to be slightly clever, but fails.  The correct way to check for sscanf inclusion is:

```pawn
#if !defined _INC_SSCANF
	#error You need sscanf
#endif
```

There is a third version of this error which looks like:

```
error 004: function "sscanf" is not implemented <library>sscanf</library>      <remarks>  The main entry point.  See the readme for vast amounts of information on how  to call this function and all the details on what it does.  This is a macro  that calls <c>SSCANF__</c> and passes the current file and line number as  well for improved error messages.  </remarks> 
```

For more information on why, see [this compiler issue](https://github.com/pawn-lang/compiler/issues/705).

### sscanf error: Pawn component not loaded.

*Error Code: 40*

When loading sscanf as a component on open.mp the Pawn component is also required.  Ensure `pawn.dll` or `pawn.so` is in the `components/` directory.

### sscanf warning: Unknown `player->setName()` return.

*Error Code: 41*

The open.mp sscanf component was probably built against an old version of the SDK.  Check for an updated version at https://www.github.com/Y-Less/sscanf/.

### sscanf error: This script was built with the component version of the include.

*Error Code: 42*

When compiling a script with the open.mp official includes the sscanf2 include file compiles different code, which assumes that the natives will be loaded as a component.  This error comes when the natives are loaded as a plugin instead, as certain features like `SSCANF_Init` are no longer required in the component case.  Move `sscanf.dll` or `sscanf.so` from the `plugins/` directory to the `components/` directory and remove the legacy plugin name from `plugins` (in server.cfg) or `pawn.legacy_plugins` (in config.json).

### sscanf error: Unable to allocate memory.

*Error Code: 43*

You ran out of RAM.  Unfortunately there's not much that can be done about this error.  `sscanf` only allocates very small amounts of memory, and only temporarilly, so you must be *really* low to get this.

### sscanf warning: User arrays are not supported in arrays.

*Error Code: 44*

This is not allowed:

```
a<u[5]>[10]
```

Instead use:

```
u[50]
```

### sscanf warning: Invalid values in array defaults.

*Error Code: 45*

The optional array default values do not parse to the given type.  For example you've provided floats instead of integers:

```
A<i>(6.6, 7.7)[10]
```

### sscanf warning: Excess array defaults found.

*Error Code: 46*

More optional array default values were given than needed:

```
A<i>(0, 1, 2, 3, 4, 5, 6)[3]
```

### sscanf warning: Format specifier does not match parameter count.

*Error Code: 47*

There are more (or fewer) specifiers in the format string than there are destination parameters:

```pawn
sscanf(input, "iiiiiiii", a, b, c);
```

### sscanf warning: Unclosed quiet section.

*Error Code: 48*

A quiet section was opened but not closed:

```
{i
```

### sscanf warning: Include / plugin mismatch, please recompile your script for the latest features.

*Error Code: 49*

Your mode was compiled using a (possibly very) old version of `sscanf2.inc`, which doesn't have the new natives or features.  However, the plugin does have those and can give far better diagnostics (such as error line numbers) if you recompile.

### sscanf warning: A minus minus makes no sense.

*Error Code: 50*

`-` in an `enum` declaration skips the next element in the `enum` so that you can parse partial data.  However, you've used two in a row:

```
e<i--f>
```

### sscanf warning: A minus option makes no sense.

*Error Code: 51*

`-` in an `enum` declaration skips the next element in the `enum` so that you can parse partial data.  However, you've used `-` on an optional specifier, which means you don't care if it is there, and won't save it - just skip it entirely:

```
e<i-F(2.2)>
```

### sscanf warning: A minus delimiter makes no sense.

*Error Code: 52*

`-` in an `enum` declaration skips the next element in the `enum` so that you can parse partial data.  However, `p` isn't a specifier for reading in data, so there's no type to skip:

```
e<i-p<,>>
```

### sscanf warning: A minus quiet section makes no sense.

*Error Code: 53*

`-` in an `enum` declaration skips the next element in the `enum` so that you can parse partial data.  A quiet section reads in input but doesn't save it.  I guess these do make sense together in retrospect, but instead of doing:

```
e<i-{i}>
```

Do:

```
e<i-i{i}>
```

To skip data without writing it out.

### sscanf warning: User arrays are not supported in enums.

*Error Code: 54*

You can't do this:

```
e<u[5]>
```

### sscanf error: 'U(name)[len]' is incompatible with OLD_DEFAULT_NAME.

*Error Code: 55*

Player arrays end the list of results with `INVALID_PLAYER_ID`.  `OLD_DEFAULT_NAME` checks if the given player is connected by name.  Therefore if you try combine these two features you may end the list early:

```
?<OLD_DEFAULT_NAME=1>U(Y_Less)[5]
```

### sscanf error: 'U(num)[len]' length under 2.

*Error Code: 56*

Use arrays have a sentinel, i.e. they search for all matching users and end the list with `INVALID_PLAYER_ID`.  If the array size is smaller than `2` elements then the only value that can be returned is the sentinel, i.e. you won't get any useful results:

```
U(4)[1]
```

### sscanf error: 'u[len]' length under 2.

*Error Code: 57*

Use arrays have a sentinel, i.e. they search for all matching users and end the list with `INVALID_PLAYER_ID`.  If the array size is smaller than `2` elements then the only value that can be returned is the sentinel, i.e. you won't get any useful results:

```
u[1]
```

### sscanf error: 'Q(name)[len]' is incompatible with OLD_DEFAULT_NAME.

*Error Code: 58*

Player arrays end the list of results with `INVALID_PLAYER_ID`.  `OLD_DEFAULT_NAME` checks if the given player is connected by name.  Therefore if you try combine these two features you may end the list early:

```
?<OLD_DEFAULT_NAME=1>Q(Y_Less)[5]
```

### sscanf error: 'Q(num)[len]' length under 2.

*Error Code: 59*

Use arrays have a sentinel, i.e. they search for all matching users and end the list with `INVALID_PLAYER_ID`.  If the array size is smaller than `2` elements then the only value that can be returned is the sentinel, i.e. you won't get any useful results:

```
Q(4)[1]
```

### sscanf error: 'q[len]' length under 2.

*Error Code: 60*

Use arrays have a sentinel, i.e. they search for all matching users and end the list with `INVALID_PLAYER_ID`.  If the array size is smaller than `2` elements then the only value that can be returned is the sentinel, i.e. you won't get any useful results:

```
q[1]
```

### sscanf error: 'R(name)[len]' is incompatible with OLD_DEFAULT_NAME.

*Error Code: 61*

Player arrays end the list of results with `INVALID_PLAYER_ID`.  `OLD_DEFAULT_NAME` checks if the given player is connected by name.  Therefore if you try combine these two features you may end the list early:

```
?<OLD_DEFAULT_NAME=1>R(Y_Less)[5]
```

### sscanf error: 'R(num)[len]' length under 2.

*Error Code: 62*

Use arrays have a sentinel, i.e. they search for all matching users and end the list with `INVALID_PLAYER_ID`.  If the array size is smaller than `2` elements then the only value that can be returned is the sentinel, i.e. you won't get any useful results:

```
R(4)[1]
```

### sscanf error: 'r[len]' length under 2.

*Error Code: 63*

Use arrays have a sentinel, i.e. they search for all matching users and end the list with `INVALID_PLAYER_ID`.  If the array size is smaller than `2` elements then the only value that can be returned is the sentinel, i.e. you won't get any useful results:

```
r[1]
```

### sscanf error: (*) is not supported in strings/arrays yet.

*Error Code: 64*

You can't use `*` to pass default string values in additional parameters:

```pawn
sscanf(input, "A<s>(*)[5]", "default", output)
```

### sscanf error: Unclosed specifier parameters.

*Error Code: 65*

Some parameter was not closed by `>`:

```
e<iiif
```

### sscanf error: No specified parameters found.

*Error Code: 66*

Some parameter was not opened by `<`:

```
eiiif>
```

### sscanf error: Enums are not supported in enums.

*Error Code: 67*

Basically, you can't do:

```
e<fe<iii>f>
```

You can, however, still do:

```
e<fiiif>
```

Nested enums compile to the same as a single flat enum.  It may make less sense compared to the code, but is at least easier to write.

### sscanf error: SSCANF_TextSimilarity has incorrect parameters.

*Error Code: 68*

You edited something in the sscanf2 include - undo it or redownload it.

### sscanf error: SSCANF_GetErrorCategory has incorrect parameters.

*Error Code: 69*

You edited something in the sscanf2 include - undo it or redownload it.

### sscanf error: End of text is not supported in arrays.

*Error Code: 70*

You can't use `!` as the type of an array:

```
a<!>[10]
```

### sscanf error: End of text is not supported in enums.

*Error Code: 71*

You can't use `!` as a type in an enum:

```
e<ii!>
```

Just put it after:

```
e<ii>!
```

### sscanf warning: A minus end of text makes no sense.

*Error Code: 72*

`-` in an `enum` declaration skips the next element in the `enum` so that you can parse partial data.  However, `p` isn't a specifier for reading in data, so there's no type to skip:

```
e<i-p<,>>
```

### sscanf error: No alternate destination.

*Error Code: 73*

`-` in an `enum` declaration skips the next element in the `enum` so that you can parse partial data.  However, `p` isn't a specifier for reading in data, so there's no type to skip:

```
e<i-p<,>>
```

## Future Plans

### Reserved Specifiers

The currently used specifiers are:

```
abcdefghiklmnopqrsuxz
```

This leaves only the following specifiers:

```
jtvwy
```

* `t` is for time - some sort of date/time processing that returns a unix timestamp.
* `y` is for "YID" - the YSI user ID.  Don't use YSI?  Tough.
* `v` I figure is for something to do with varargs, similar to `a` but for extra parameters.
* `j` no idea yet.
* `w` is the most important one to reserve - it is for ***w**ide* specifiers.  Since there are so few left it is important to establish future compatibility.  Thus `w` is a prefix that indicates that the following specifier has an alternate meaning.  So `i` is "integer" but `wi` is something else entirely (don't know what yet).  This scheme does recurse endlessly so `wwi` and `wwwwwi` are also different.  In this way we will never run out and can start adding support for more obscure items like iterators and jagged arrays (the original idea for `j`).  There's also a suggestion for this as `words` - `w<5>` for five words, but I think that makes sense as an extension to `s`.

### Validity Ranges

Add a feature to simple specifiers (mainly numbers like `i`, `f`, `c`, etc) to specify what the valid values are.  So you could write an IP parser as:

```
p<.>i<0-256>i<0-256>i<0-256>i<0-256>
```

Obvioulys there is upper-bound is exclusive.  Multiple valid ranges, and open-ended ranges, can be given with `x<0-10,20,22,50-90,1000->

### Enums And Arrays

More of these: nested arrays in enums, 2d/3d arrays, strings in enums and arrays, etc.

### Compilation

Basically pre-defining specifier strings for use later:

```pawn
new Specifier:spec = SSCANF_Compile("is[32]");

SSCANF_Run(input, spec, int, string);
```

You could define the base `sscanf` as:

```pawn
sscanf(const input, const specifier, ...)
{
	return SSCANF_Run(input, SSCANF_Compile(specifier), ___(2));
}
```

## Building

If you want to compile sscanf for yourself.

### Getting The Code

You can clone the code using github.  Note that there are multiple submodules involved, so you are best using a recursive checkout:

```
git clone --recursive https://github.com/Y-Less/sscanf/
```

Note the use of the `--recursive` argument, because this repository contains submodules.  A useful setting when cloning recursive repos is:

```bash
git config --global url."git@github.com:".insteadOf "https://github.com/"
```

Which allows you to push `https://` repos you have permissions on.

### Building On Windows

```bash
mkdir build
cd build
cmake .. -A Win32 -T ClangCL
```

Open Visual Studio and build the solution.

### Building On Linux

```bash
mkdir build
cd build
# May need to configure this line.
export CC=/usr/lib/llvm/13/bin/clang CXX=/usr/lib/llvm/13/bin/clang++
cmake ..
make
```

Change `Debug` to `Release` for final versions.

### Building With Docker

```bash
cd docker
.\build.sh
```

You may need to set up some directories first:

```bash
mkdir build
mkdir conan
sudo chown 1000 build
sudo chown 1000 conan
```

Instead you run the script as root, and target a specific distro:

```bash
UBUNTU_VERSION=18.04 sudo .\build.sh
```

The output is in `docker/build/`

## License

### Version: MPL 1.1

The contents of this file are subject to the Mozilla Public License Version
1.1 (the "License"); you may not use this file except in compliance with
the License.  You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied.  See the License
for the specific language governing rights and limitations under the
License.

The Original Code is the sscanf 2.0 SA:MP plugin.

The Initial Developer of the Original Code is Alex "Y_Less" Cole.
Portions created by the Initial Developer are Copyright (c) 2022
the Initial Developer.  All Rights Reserved.

### Contributor(s):

* Cheaterman
* DEntisT
* Emmet_
* karimcambridge
* kalacsparty
* Kirima
* leHeix
* maddinat0r
* Southclaws
* Y_Less
* ziggi

### Special Thanks to:

* SA:MP Team past, present, and future.
* maddinat0r, for hosting the repo for a very long time.
* Emmet_, for his efforts in maintaining it for almost a year.

## Changelog

### sscanf 2.8.2 - 18/04/2015

* Fixed a bug where `u` wasn't working correctly after a server restart.

### sscanf 2.8.3 - 02/10/2018

* Allow `k` in arrays.
* Allow `k` to consume the rest of the line (like strings) when they are the last specifier.

### sscanf 2.9.0 - 04/11/2019

* Added `[*]` support.
* Fixed bracketed lengths (`[(32)]`).
* Ported readme to markdown.
* Added `z` and `Z` for packed strings (thus officially removing their deprecated optional use).
* Remove missing string length warnings - its now purely an error.
* Remove `p,` warnings - its now purely an error.

### sscanf 2.10.0 - 27/06/2020

* Added `m` for colours (ran out of useful letters).
* Added file and line details for errors.

### sscanf 2.10.1 - 27/06/2020

* Plugin backwards compatibility with older includes.

### sscanf 2.10.2 - 28/06/2020

* Fix bug in parameter counts.

### sscanf 2.11.2 - 28/04/2021

* Use prehooks in include.
* Export `PawnSScanf` function from dll to other plugins.
* `SSCANF_SetOption()` and `SSCANF_GetOption()` for more control of options.
* `SSCANF_VERSION` and `SSCANF_Version()` to compare include and plugin versions.
* Hide more internal functions.
* Fix the license.

### sscanf 2.10.4 - 17/01/2022

* Fix trailing string literals, to allow `"x'!'"` for example.
* Added `SSCANF_VERSION` for compile-time checks.

### sscanf 2.11.1 - 25/01/2022

* Re-added NPC mode support.

### sscanf 2.11.2 - 04/02/2022

* Minor Linux build fixes.

### sscanf 2.11.3 - 05/02/2022

* Added `SSCANF_Levenshtein` for better string candidate processing.
* Added `SSCANF_GetClosestString` for better string candidate processing.
* Added `SSCANF_GetClosestValue` for better string candidate processing.
* Added `SSCANF_NO_K_VEHICLE` to disable the default `k<vehicle>` specifier code.
* Added `SSCANF_NO_K_WEAPON` to disable the default `k<weapon>` specifier code.

### sscanf 2.11.4 - 02/03/2022

* Documentation comments on all functions via pawndoc.

### sscanf 2.11.5 - 31/03/2022

* Improve some errors caused by using `sscanf` before including it.

### sscanf 2.12.1 - 05/05/2022

* Integrate open.mp component support.

### sscanf 2.12.2 - 11/05/2022

* Switch to a different (semi made-up) word similarity function.
* Added `SSCANF_TextSimilarity` for best string candidate processing.
* Added `SSCANF_GetSimilarString` for best string candidate processing.
* Added `SSCANF_GetSimilarValue` for best string candidate processing.
* Use `OnPlayerNameChange` in the open.mp component code version.
* Switch `SSCANF_Levenshtein` internally to use direct AMX access.

### sscanf 2.13.1 - 25/06/2022

* Enable similarity-based name comparisons (`MATCH_NAME_SIMILARITY`).
* Return the best matching name by default.
* Add `MATCH_NAME_FIRST` to revert best name match behaviour.
* Improve `MATCH_NAME_PARTIAL`.
* Internal cleanup and fixes.

### sscanf 2.13.2 - 07/09/2022

* Rebuild for open.mp beta 9 SDK changes.

### sscanf 2.13.3 - 04/12/2022

* Add `@kustom()` decorator for kustom specifiers.
* Use *subhook* to hook `amx_Register` and find `SetPlayerName`.

### sscanf 2.13.4 - 20/12/2022

* Update open.mp SDK.
* Added `(*)` for dynamic default values.

### sscanf 2.13.5 - 28/12/2022

* Move builds to CMake.
* Add docker builds.
* Fix `[*]` again.

### sscanf 2.13.6 - 28/12/2022

* Fixed a crash with `[*]` when there aren't enough parameters.

### sscanf 2.13.7 - 02/01/2023

* Improve GDK plugin compatibility.

### sscanf 2.13.8 - 05/01/2023

* "Final" SDK update.

### sscanf 2.14.1 - 08/09/2023

* Fixed a bug where setting `MATCH_NAME_FIRST` also disabled logging.
* Re-introduced the return value of `sscanf` giving the index of the failed specifier.
* `SSCANF_GetLastError` to get the error code in failure cases.
* `SSCANF_ClearLastError` to reset the error code from previous failures.
* `SSCANF_GetErrorCategory` to get the category of an error.
* Added `WARNINGS_AS_ERRORS` option to treat warnings as errors.
* Added `ERROR_CODE_IN_RET` option to return error codes along-side error indexes.
* Added `ERROR_CATEGORY_ONLY` option to return error categories instead of error codes.
* Stop defining `__PawnBuild` and use `__pawn_build` instead.
* Add `sscanf_error` to define all error codes.
* Add `SSCANF_ERROR` macro to combine specifier indexes and error codes for `ERROR_CODE_IN_RET`.
* Add `!` specifier to check for strict end of input.

### sscanf 2.15.1 - 10/09/2023

* Added alternates via `|`.
* `SSCANF_GetErrorSpecifier` to get the error position in failure cases.
* `SSCANF_Debug` to dump a load of useful debugging information.

