/*
 *  sscanf 2.11.4
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

#if defined _INC_SSCANF
	#endinput
#endif
#define _INC_SSCANF

#if !defined _samp_included
	#error Please include <a_npc> or <a_samp> first.
#endif

/**
 * <library
 *   name="sscanf"
 *   summary="Extracts structured information from strings."
 *   license="Copyright (c) 2022 Alex &quot;Y_Less&quot; Cole.  Licensed under MPL 1.1"
 * >
 *   <summary pawndoc="true">
 *     This library uses the enhanced <em>pawndoc.xsl</em> from
 *     <a href="https://github.com/pawn-lang/pawndoc">pawn-lang/pawndoc</a>.
 *     This XSL has features such as library and markdown support, and will not
 *     render this message when used.
 *   </summary>
 *   <remarks>
 *     This is the <em>sscanf</em> plugin, which provides the <c>sscanf</c>
 *     function to extract basic structured data from strings.  This is slightly
 *     different to regular expressions, but both have their place.  A regular
 *     expression gives you total control over the exact structure of data down
 *     to the character level; however, extracting structured data like numbers
 *     using it is tricky.  Conversely this gives slightly higher-level
 *     <em>specifiers</em> which can easily extract data types, at the expense
 *     of fine-grained control.  To convert a string in to two numbers would
 *     look like:
 *     
 *     <code>
 *         new num1, num2;                                                          <br />
 *         sscanf("45 100", "ii", num1, num2);
 *     </code>
 *     
 *     <c>ii</c> is the specifier string, which here means "integer integer";
 *     stating that the input string should be two whole numbers in a row (which
 *     is - <c>"45 100"</c>).  <c>num1</c> and <c>num2</c> are the destination
 *     variables to store the found numbers in (after conversion from strings).
 *     You can check if the conversion failed by looking for a non-naught return
 *     value:
 *     
 *     <code>
 *         new num1, num2;                                                          <br />
 *         if (sscanf("hello 100", "ii", num1, num2))                               <br />
 *         {                                                                        <br /><indent />
 *             printf("The input was not two numbers.");                            <br />
 *         }
 *     </code>
 *     
 *     This will fail because <c>"hello"</c> is not a whole number (or indeed
 *     any type of number at all).  For more information on using the function
 *     refer to the tutorials or the reference documentation in
 *     <a href="https://github.com/Y-Less/sscanf/blob/master/README.md">the
 *     attached readme</a>.
 *   </remarks>
 * </library>
 */

///

/**
 * <library>sscanf</library>
 * <remarks>
 * Was sscanf built for an NPC mode or a normal mode?
 * </remarks>
 */
#if defined GetDistanceFromMeToPoint
	// NPC script.
	static stock SSCANF_NPC = 1;
	#define SSCANF_NPC (1)
	#pragma library sscanf
#else
	static stock SSCANF_NPC = 0;
	#define SSCANF_NPC (0)
#endif

#if !defined __PawnBuild && !defined SSCANF_NO_NICE_FEATURES
	#error sscanf likes the community compiler.  Use `#define SSCANF_NO_NICE_FEATURES` to ignore this.
#endif

/**
 * <library>sscanf</library>
 * <remarks>
 * The sscanf major version number.
 * </remarks>
 */
static stock SSCANF_VERSION_MAJOR =  2;
#define SSCANF_VERSION_MAJOR  2

/**
 * <library>sscanf</library>
 * <remarks>
 * The sscanf minor version number.
 * </remarks>
 */
static stock SSCANF_VERSION_MINOR = 11;
#define SSCANF_VERSION_MINOR 11

/**
 * <library>sscanf</library>
 * <remarks>
 * The sscanf build number.
 * </remarks>
 */
static stock SSCANF_VERSION_BUILD =  4;
#define SSCANF_VERSION_BUILD  4

/**
 * <library>sscanf</library>
 * <remarks>
 * The sscanf version as a string.  E.g. <c>"2.8.1"</c>.
 * </remarks>
 */
static stock SSCANF_VERSION_STRING[] = #SSCANF_VERSION_MAJOR "." #SSCANF_VERSION_MINOR "." #SSCANF_VERSION_BUILD;
#define SSCANF_VERSION_STRING #SSCANF_VERSION_MAJOR "." #SSCANF_VERSION_MINOR "." #SSCANF_VERSION_BUILD

/**
 * <library>sscanf</library>
 * <remarks>
 * Converts a version string to
 * <a href="https://en.wikipedia.org/wiki/Binary-coded_decimal">BCD</a>.  For example:
 *
 * <code>
 *   BCD(5.6.17);
 * </code>
 *
 * Returns:
 *
 * <code>
 *   0x050617
 * </code>
 *
 * Each section (between dots) is assigned a single byte and the last section is
 * always in the lowest byte.  This implies a maximum of four sections and two
 * digits per section.
 *
 * </remarks>
 */
static stock BCD(number)
{
	return number;
}

#define BCD(%0) (_:MORE_BCD:NO_MORE_BCD:%0.$0)
#define MORE_BCD:NO_MORE_BCD:%0.%1$%2) MORE_BCD:NO_MORE_BCD:%1$(%2) << 8 | DO_BCD(%0))
#define NO_MORE_BCD:$
#define DO_BCD(%0) _:(%0) / 10 << 4 | _:(%0) % 10

/**
 * <library>sscanf</library>
 * <remarks>
 * The sscanf version in BCD as a proper constant.  Example:
 *
 * <code>
 *         2.10.3  <br />
 *     => 02 10 03 <br />
 *     => 0x021003
 * </code>
 * </remarks>
 */
const SSCANF_VERSION = BCD(SSCANF_VERSION_MAJOR.SSCANF_VERSION_MINOR.SSCANF_VERSION_BUILD);

/**
 * <library>sscanf</library>
 * <remarks>
 * The sscanf version in BCD.  Example:
 *
 * <code>
 *         2.10.3  <br />
 *     => 02 10 03 <br />
 *     => 0x021003
 * </code>
 * </remarks>
 */
stock const SSCANF_VERSION_BCD = SSCANF_VERSION;

#define sscanf_%0\32; sscanf_
#define SSCANF:%0(%1) forward sscanf_%0(%1);public sscanf_%0(%1)

// The check for `SSCANF_Option` ensures that this is the first compiler pass
// and thus the check for `sscanf` only finds earlier definitions not our
// later fake definition made purely for documentation purposes.
#if defined sscanf && !defined SSCANF_Option
	#error sscanf (possibly the PAWN version) already defined.
#endif

/**
 * <library>sscanf</library>
 * <param name="file">The file in which this call is found.</param>
 * <param name="line">The line at which this call is found.</param>
 * <param name="data">The input string containing the data to parse out.</param>
 * <param name="format">The format description of what the input data should contain.</param>
 * <remarks>
 * The current true implementation of <c>sscanf</c> in the plugin.  This is
 * wrapped by macros to provide <c>sscanf</c> enhanced with filenames and line
 * numbers so that errors have more information.  The plugin also contains a
 * native function called <c>sscanf</c> which is only for backwards-
 * compatibility with older versions of this include.
 * </remarks>
 */
native SSCANF__(const file[], const line, const data[], const format[], {Float, _}:...);

/**
 * <library>sscanf</library>
 * <param name="file">The file in which this call is found.</param>
 * <param name="line">The line at which this call is found.</param>
 * <param name="data">The input string containing the data to parse out.</param>
 * <param name="format">The format description of what the input data should contain.</param>
 * <remarks>
 * An alternative name for <c>SSCANF__</c>, used by <c>extract</c> so that the
 * name can be used as a macro.
 * </remarks>
 */
native UNFORMAT__(const file[], const line, const data[], const format[], {Float, _}:...) = SSCANF__;

/**
 * <library>sscanf</library>
 * <param name="players">The maximum players on the server.</param>
 * <param name="invalid">The invalid player ID.</param>
 * <param name="len"><c>MAX_PLAYER_NAME</c>.</param>
 * <remarks>
 * Initialise the plugin with real server information.
 * </remarks>
 */
native SSCANF_Init(players, invalid, len);

/**
 * <library>sscanf</library>
 * <param name="playerid">The ID of the player.</param>
 * <param name="name">The name of the player.</param>
 * <param name="npc">Is this player an NPC?</param>
 * <remarks>
 * Called when a player joins to inform the plugin of the connection.
 * </remarks>
 */
native SSCANF_Join(playerid, const name[], bool:npc);

/**
 * <library>sscanf</library>
 * <param name="playerid">The ID of the player.</param>
 * <remarks>
 * Called when a player leaves to inform the plugin of the disconnection.
 * </remarks>
 */
native SSCANF_Leave(playerid);

/**
 * <library>sscanf</library>
 * <param name="playerid">The ID of the player.</param>
 * <remarks>
 * Checks if the plugin knows about a given player ID.  Used when modes restart
 * to re-add players.
 * </remarks>
 */
native bool:SSCANF_IsConnected(playerid);

/**
 * <library>sscanf</library>
 * <param name="string1">The first string to compare.</param>
 * <param name="string2">The second string to compare.</param>
 * <remarks>
 * Computes the <a href="https://en.wikipedia.org/wiki/Levenshtein_distance">
 * Levenshtein Distance</a> between two strings.  This is simlar to
 * <c>strcmp</c> in usage, but is slightly more "fuzzy".  Distances are used to
 * work out which string is the most similar to another one, though they may not
 * be identical.  Useful in <c>k</c> callback functions to determine if the
 * entered string is close to a possible string.
 * </remarks>
 */
native SSCANF_Levenshtein(const string1[], const string2[]);

/**
 * <library>sscanf</library>
 * <param name="name">The sscanf option to look up.  For example <c>SSCANF_COLOUR_FORMS</c>.</param>
 * <remarks>
 * The old API used <c>SSCANF_Option</c> to both get and set parse options,
 * with an optional parameter for setting.  This was problematic if you wanted
 * to actually set an option to the default value - there was no way to
 * differentiate between getting an option and setting an option to the default
 * value.  Instead the new API has explicit <c>Get</c> and <c>Set</c> functions.
 * </remarks>
 */
native SSCANF_GetOption(const name[]) = SSCANF_Option;

/**
 * <library>sscanf</library>
 * <param name="name">The sscanf option to look up.  For example <c>MATCH_NAME_PARTIAL</c>.</param>
 * <remarks>
 * The old API used <c>SSCANF_Option</c> to both get and set parse options,
 * with an optional parameter for setting.  This was problematic if you wanted
 * to actually set an option to the default value - there was no way to
 * differentiate between getting an option and setting an option to the default
 * value.  Instead the new API uses true parameter counts to differentiate in
 * the legacy <c>SSCANF_Option</c> function, which are resolved through macros
 * to <c>Get__</c> and <c>Set__</c> functions.
 * </remarks>
 */
native SSCANF_GetOption__(const name[]) = SSCANF_Option;

/**
 * <library>sscanf</library>
 * <param name="name">The sscanf option to set.  For example <c>CELLMIN_ON_MATCHES</c>.</param>
 * <param name="value">The value to set the option to.</param>
 * <remarks>
 * The old API used <c>SSCANF_Option</c> to both get and set parse options,
 * with an optional parameter for setting.  This was problematic if you wanted
 * to actually set an option to the default value - there was no way to
 * differentiate between getting an option and setting an option to the default
 * value.  Instead the new API has explicit <c>Get</c> and <c>Set</c> functions.
 * </remarks>
 */
native SSCANF_SetOption(const name[], value) = SSCANF_Option;

/**
 * <library>sscanf</library>
 * <param name="name">The sscanf option to set.  For example <c>OLD_DEFAULT_CUSTOM</c>.</param>
 * <param name="value">The value to set the option to.</param>
 * <remarks>
 * The old API used <c>SSCANF_Option</c> to both get and set parse options,
 * with an optional parameter for setting.  This was problematic if you wanted
 * to actually set an option to the default value - there was no way to
 * differentiate between getting an option and setting an option to the default
 * value.  Instead the new API uses true parameter counts to differentiate in
 * the legacy <c>SSCANF_Option</c> function, which are resolved through macros
 * to <c>Get__</c> and <c>Set__</c> functions.
 * </remarks>
 */
native SSCANF_SetOption__(const name[], value) = SSCANF_Option;

/**
 * <library>sscanf</library>
 * <param name="name">The sscanf option to get or set.  For example <c>SSCANF_ARGB</c>.</param>
 * <param name="value">The optional value to use when setting the option.</param>
 * <remarks>
 * Backwards-compatibility with <c>SSCANF_Option</c>.  Usage:
 *
 * <code>
 *     SSCANF_Option(SSCANF_QUIET, 1);           <br />
 *     new quiet = SSCANF_Option(SSCANF_QUIET);
 * </code>
 *
 * This doesn't actually use a default parameter because a user may want to set
 * the option to whatever that parameter is.  Instead this is a macro that calls
 * <c>SSCANF_GetOption__</c> when called with one parameter (just a name), or
 * instead calls <c>SSCANF_SetOption__</c> when called with two - a name and a
 * value.
 * </remarks>
 */
static stock SSCANF_Option(const name[], value = -1)
{
	return value;
}

#define SSCANF_Option SSCANF_GetOption__
#define SSCANF_GetOption__(%0,%1) SSCANF_SetOption__(%0,%1)
#define _ALS_SSCANF_Option
#define _ALS_SSCANF_GetOption__

/**
 * <library>sscanf</library>
 * <param name="version">Pass-by-reference return value.</param>
 * <param name="size">The size of the destination array.</param>
 * <remarks>
 * Get the SSCANF plugin version as a string (e.g. <c>"2.11.2"</c>).  Compare
 * this value to the constant <c>SSCANF_VERSION_STRING</c>.
 * </remarks>
 */
native SSCANF_VersionString(version[], size = sizeof (version)) = SSCANF_Version;

/**
 * <library>sscanf</library>
 * <param name="version">Pass-by-reference return value.</param>
 * <param name="size">The size of the destination array.</param>
 * <remarks>
 * Get the SSCANF plugin version as a string (e.g. <c>"2.11.2"</c>).  Compare
 * this value to the constant <c>SSCANF_VERSION_STRING</c>.  This internal
 * alternate spelling is used by the <c>SSCANF_Version</c> macro, which will
 * return a string when called with parameters, or a BCD value when not.
 * </remarks>
 */
native SSCANF_VersionString__(version[], size = sizeof (version)) = SSCANF_Version;

/**
 * <library>sscanf</library>
 * <remarks>
 * Get the SSCANF plugin version as a BCD-encoded value (e.g. <c>0x021102</c>).
 * Compare this value to the constant <c>SSCANF_VERSION_BCD</c>.
 * </remarks>
 */
native SSCANF_VersionBCD() = SSCANF_Version;

/**
 * <library>sscanf</library>
 * <remarks>
 * Get the SSCANF plugin version as a BCD-encoded value (e.g. <c>0x021102</c>).
 * Compare this value to the constant <c>SSCANF_VERSION_BCD</c>.  This internal
 * alternate spelling is used by the <c>SSCANF_Version</c> macro, which will
 * return a string when called with parameters, or a BCD value when not.
 * </remarks>
 */
native SSCANF_VersionBCD__() = SSCANF_Version;

// The `SSCANF_Version` function is overloaded to take 0 or 2 parameters (or 1 plus a default).

/**
 * <library>sscanf</library>
 * <param name="version">Pass-by-reference return value.</param>
 * <param name="size">The size of the destination array.</param>
 * <remarks>
 * Get the SSCANF plugin version as a string (e.g. <c>"2.11.2"</c>) <em>or</em>
 * a BCD-encoded value (e.g. <c>0x021102</c>).  This function returns a string
 * (by reference) when called with parameters, or a number (directly) when not.
 * </remarks>
 */
static stock SSCANF_Version(version[] = "", size = sizeof (version))
{
	return SSCANF_VERSION_BCD;
}

#define SSCANF_Version SSCANF_VersionString__
#define SSCANF_VersionString__() SSCANF_VersionBCD__()
#define _ALS_SSCANF_Version
#define _ALS_SSCANF_VersionString__

/**
 * <library>sscanf</library>
 * <param name="data">The input string containing the data to parse out.</param>
 * <param name="format">The format description of what the input data should contain.</param>
 * <remarks>
 * The main entry point.  See the readme for vast amounts of information on how
 * to call this function and all the details on what it does.  This is a macro
 * that calls <c>SSCANF__</c> and passes the current file and line number as
 * well for improved error messages.
 * </remarks>
 */
stock sscanf(const data[], const format[], {Float, _}:...)
{
	return 0;
}

#if defined __PawnBuild
	#define sscanf( SSCANF__(__file,__line,
#else
	// `-1` because the old compiler doesn't have `__line` so we can't know
	// where this function was used.  Tell the plugin this.  Actually, we *can*
	// know, if we look at the CIP and try extract the file/line information
	// from any included debug information.  `SSCANF_UNK_` tells the plugin that
	// this was called via `sscanf` not `unformat`, because they're actually the
	// same function once compiled.
	#define sscanf( SSCANF__(SSCANF_UNK_,-1,
#endif

/**
 * <library>sscanf</library>
 * <param name="data">The input string containing the data to parse out.</param>
 * <param name="format">The format description of what the input data should contain.</param>
 * <remarks>
 * An alternative spelling of <c>sscanf</c>, requested by Kalcor because the
 * original doesn't match the C specification for the format descriptor.  This
 * is a macro that calls <c>UNFORMAT__</c> and passes the current file and line
 * number as well for improved error messages.
 * </remarks>
 */
stock unformat(const data[], const format[], {Float, _}:...)
{
	return 0;
}

#if defined __PawnBuild
	#define unformat( SSCANF__(__file,__line,
#else
	// `-1` because the old compiler doesn't have `__line` so we can't know
	// where this function was used.  Tell the plugin this.  Actually, we *can*
	// know, if we look at the CIP and try extract the file/line information
	// from any included debug information.  `SSCANF_FOM_` tells the plugin that
	// this was called via `unformat` not `sscanf`, because they're actually the
	// same function once compiled.
	#define unformat( SSCANF__(SSCANF_FOM_,-1,
#endif

/**
 * <library>sscanf</library>
 * <remarks>
 * The fallback for the filename in <c>sscanf</c> on the old compiler, which
 * doesn't have the inbuilt <c>__file</c> macro.  This is the "feature" enabled
 * by <c>SSCANF_NO_NICE_FEATURES</c>.  Appends <c>"unknown file"</c> in the
 * plugin when line number <c>&lt; 0</c>.
 * </remarks>
 */
stock const SSCANF_UNK_[] = "sscanf";

/**
 * <library>sscanf</library>
 * <remarks>
 * The fallback for the filename in <c>unformat</c> on the old compiler, which
 * doesn't have the inbuilt <c>__file</c> macro.  This is the "feature" enabled
 * by <c>SSCANF_NO_NICE_FEATURES</c>.  Appends <c>"unknown file"</c> in the
 * plugin when line number <c>&lt; 0</c>.
 * </remarks>
 */
stock const SSCANF_FOM_[] = "unformat";

/**
 * <library>sscanf</library>
 * <remarks>
 * The fallback for the filename in <c>extract</c> on the old compiler, which
 * doesn't have the inbuilt <c>__file</c> macro.  This is the "feature" enabled
 * by <c>SSCANF_NO_NICE_FEATURES</c>.  Appends <c>"unknown file"</c> in the
 * plugin when line number <c>&lt; 0</c>.
 * </remarks>
 */
stock const SSCANF_EXT_[] = "extract";

/**
 * <library>sscanf</library>
 * <remarks>
 * The <c>SSCANF_QUIET</c> option as a constant string so you can get compile-
 * time spell checking on the name.  Don't print any errors to the console.
 * <em>Really</em> not recommended unless you <em>know</em> your code is stable
 * and in production.
 * </remarks>
 */
stock const SSCANF_QUIET[] = "SSCANF_QUIET";

/**
 * <library>sscanf</library>
 * <remarks>
 * The <c>OLD_DEFAULT_NAME</c> option as a constant string so you can get
 * compile-time spell checking on the name.  The behaviour of <c>U</c>, <c>Q</c>
 * and <c>R</c> have been changed to take any number as a default, instead of a
 * connected player.  Setting <c>OLD_DEFAULT_NAME</c> to <c>1</c> will revert to
 * the old version.
 * </remarks>
 */
stock const OLD_DEFAULT_NAME[] = "OLD_DEFAULT_NAME";

/**
 * <library>sscanf</library>
 * <remarks>
 * The <c>MATCH_NAME_PARTIAL</c> option as a constant string so you can get
 * compile-time spell checking on the name.  Currently sscanf will search for
 * players by name, and will <em>always</em> search for player whose name
 * <em>starts</em> with the specified string.  If someone types <c>Y_Less</c>,
 * sscanf will not find say <c>[CLAN]Y_Less</c> because there name doesn't start
 * with the specified text.  This option, when set to <c>1</c>, will search
 * <em>anywhere</em> in the player's name for the given string.
 * </remarks>
 */
stock const MATCH_NAME_PARTIAL[] = "MATCH_NAME_PARTIAL";

/**
 * <library>sscanf</library>
 * <remarks>
 * The <c>CELLMIN_ON_MATCHES</c> option as a constant string so you can get
 * compile-time spell checking on the name.  Whatever the value of
 * <c>MATCH_NAME_PARTIAL</c>, the first found player will always be returned,
 * so if you do a search for <c>_</c> on an RP server, you could get almost
 * anyone.  To detect this case, if more than one player will match the
 * specified string then <em>sscanf</em> will return an ID of <c>cellmin</c>
 * instead.  This can be combined with <c>U</c> for a lot more power:
 * 
 * <code>
 *     sscanf(params, "?&lt;CELLMIN_ON_MATCHES=1&gt;U(-1)", id);                <br />
 *     if (id == -1)                                                            <br />
 *     {                                                                        <br /><indent />
 *         // No player was entered.                                            <br />
 *     }                                                                        <br />
 *     else if (id == cellmin)                                                  <br />
 *     {                                                                        <br /><indent />
 *         // Multiple matches found                                            <br />
 *     }                                                                        <br />
 *     else if (id == INVALID_PLAYER_ID)                                        <br />
 *     {                                                                        <br /><indent />
 *         // Entered player is not connected.                                  <br />
 *     }                                                                        <br />
 *     else                                                                     <br />
 *     {                                                                        <br /><indent />
 *         // Found just one player.                                            <br />
 *     }
 * </code>
 * </remarks>
 */
stock const CELLMIN_ON_MATCHES[] = "CELLMIN_ON_MATCHES";

/**
 * <library>sscanf</library>
 * <remarks>
 * The <c>OLD_DEFAULT_KUSTOM</c> option as a constant string so you can get
 * compile-time spell checking on the name.  As with <c>U</c>, <c>K</c> used to
 * require a valid identifier as the default and would parse it using the
 * specified callback, so this would <em>not</em> work:
 * 
 * <code>
 *     K&lt;vehicle&gt;(Veyron)
 * </code>
 *
 * Because that is not a valid vehicle name in GTA.  The new version now just
 * takes a number and returns that regardless:
 * 
 * <code>
 *     K&lt;vehicle&gt;(999)
 * </code>
 *
 * This setting reverts to the old behaviour.
 * </remarks>
 */
stock const OLD_DEFAULT_KUSTOM[] = "OLD_DEFAULT_KUSTOM";

/**
 * <library>sscanf</library>
 * <remarks>
 * The <c>OLD_DEFAULT_CUSTOM</c> option as a constant string so you can get
 * compile-time spell checking on the name.  This is the same as
 * <c>OLD_DEFAULT_KUSTOM</c>, but with an alternate spelling.
 * </remarks>
 */
stock const OLD_DEFAULT_CUSTOM[] = "OLD_DEFAULT_CUSTOM";

/**
 * <library>sscanf</library>
 * <remarks>
 * The <c>SSCANF_COLOUR_FORMS</c> option as a constant string so you can get
 * compile-time spell checking on the name.  There are multiple valid colour
 * input formats, which you can enable or disable here.  The parameter is a
 * bit map (flags) for all the following values:
 *
 * <ul>
 * <li><c>1</c> - <c>#RGB</c></li>
 * <li><c>2</c> - <c>#RRGGBB</c></li>
 * <li><c>4</c> - <c>0xRRGGBB</c></li>
 * <li><c>8</c> - <c>RRGGBB</c></li>
 * <li><c>16</c> - <c>{RRGGBB}</c></li>
 * <li><c>32</c> - <c>0xRRGGBBAA</c></li>
 * <li><c>64</c> - <c>RRGGBBAA</c></li>
 * </ul>
 *
 * So to ONLY accept SA:MP `SendClientMessage` colours use:
 *
 * <code>
 *     SSCANF_Option(SSCANF_COLOUR_FORMS, 16);
 * </code>
 *
 * To only accept 8-digit values use:
 *
 * <code>
 *     SSCANF_Option(SSCANF_COLOUR_FORMS, 96);
 * </code>
 *
 * Default values (those specified between <c>()</c>s for <c>M</c>) ignore this
 * setting - they can always use any form.
 * </remarks>
 */
stock const SSCANF_COLOUR_FORMS[] = "SSCANF_COLOUR_FORMS";

/**
 * <library>sscanf</library>
 * <remarks>
 * The <c>SSCANF_ALPHA</c> option as a constant string so you can get compile-
 * time spell checking on the name.  Specify the default alpha value for colours
 * (<c>m</c>) which don't manually specify an alpha channel.  The alpha values
 * are specified as a <em>decimal</em> number, <em>not</em> a <em>hex</em>
 * number, so setting an alpha of <c>0x80</c> would be:
 *
 * <code>
 *     SSCANF_Option(SSCANF_ALPHA, 128);
 * </code>
 * </remarks>
 */
stock const SSCANF_ALPHA[] = "SSCANF_ALPHA";

/**
 * <library>sscanf</library>
 * <remarks>
 * The <c>SSCANF_ARGB</c> option as a constant string so you can get compile-
 * time spell checking on the name.  Specify whether the returned colour is
 * <c>ARGB</c> or <c>RGBA</c>:
 *
 * <code>
 *     SSCANF_Option(SSCANF_ARGB, 1); // Set 3- and 6-digit colour outputs to `AARRGGBB`. <br />
 *     SSCANF_Option(SSCANF_ARGB, 0); // Set 3- and 6-digit colour outputs to `RRGGBBAA` (default).
 * </code>
 * </remarks>
 */
stock const SSCANF_ARGB[] = "SSCANF_ARGB";

/**
 * <library>sscanf</library>
 * <remarks>
 * Has sscanf been initialised at least once already in this script?  Avoids the
 * need to define <c>FILTERSCRIPT</c> by including <c>OnFilterScriptInit</c>
 * and <c>OnGameModeInit</c> (plus <c>OnNPCModeInit</c>) and seeing which one
 * gets called first.
 * </remarks>
 */
static stock bool:SSCANF_gInit = false;

// Pre-hook data.
#if !defined CHAIN_ORDER
	#define CHAIN_ORDER() 0
#endif

#define CHAIN_HOOK(%0) forward @CO_%0();public @CO_%0(){return CHAIN_ORDER()+1;}
#define CHAIN_NEXT(%0) @CO_%0

#define CHAIN_FORWARD:%0_%2(%1)=%3; \
	forward %0_%2(%1); \
	public %0_%2(%1) <_ALS : _ALS_x0, _ALS : _ALS_x1> { return (%3); } \
	public %0_%2(%1) <> { return (%3); }

#define CHAIN_PUBLIC:%0(%1) %0(%1) <_ALS : _ALS_go>

CHAIN_HOOK(SSCANF)
#undef CHAIN_ORDER
#define CHAIN_ORDER CHAIN_NEXT(SSCANF)

/**
 * <library>sscanf</library>
 * <remarks>
 * Common ALS boilerplate to ensure that all the <c>&lt;_ALS : &gt;</c> states
 * are known to the compiler.
 * </remarks>
 */
static stock _SSCANF_IncludeStates() <_ALS : _ALS_x0, _ALS : _ALS_x1, _ALS : _ALS_x2, _ALS : _ALS_x3> {}
static stock _SSCANF_IncludeStates() <_ALS : _ALS_go> {}

forward SSCANF_PlayerCheck();

/**
 * <library>sscanf</library>
 * <remarks>
 * const-correct wrapper for <c>SetTimer</c>, mainly for NPC modes.
 * </remarks>
 */
native SSCANF_SetTimer(const funcname[], interval, bool:repeating) = SetTimer;

///
/// <library>sscanf</library>
/// <remarks>
/// Generic initialisation code called from a range of different init publics.
/// </remarks>
///
/// <!--
#if !defined __PawnBuild ///
	forward SSCANF_RunInit(); ///
	/// There's a bug in the old compiler with the pawndoc generation for
	/// functions containing <c>state</c>.  This little trick starts an XML
	/// comment at the end of the documentation <c>SSCANF_RunInit</c> and
	/// immediately closes it again in a dedicated function
	/// <c>SSCANF_RunInit0</c>, which is sorted next lexicographically.
	/// -->
	static stock SSCANF_RunInit0()
	{
	}
#endif
static stock SSCANF_RunInit()
{
	#if SSCANF_NPC
		state _ALS : _ALS_go;
		SSCANF_Init(MAX_PLAYERS, INVALID_PLAYER_ID, MAX_PLAYER_NAME);
		#if !defined SSCANF_NO_PLAYERS
			// Initialise the system.
			SSCANF_PlayerCheck();
			// Will be run once per frame.
			SSCANF_SetTimer("SSCANF_PlayerCheck", 0, true);
		#endif
	#else
		state _ALS : _ALS_go;
		new
			name[MAX_PLAYER_NAME + 1];

		SSCANF_Init(GetMaxPlayers(), INVALID_PLAYER_ID, MAX_PLAYER_NAME);
		SSCANF_gInit = true;

		// Check if there are any players that aren't initialized.
		for (new i = 0; i < MAX_PLAYERS; i ++)
		{
			if (IsPlayerConnected(i) && !SSCANF_IsConnected(i))
			{
				GetPlayerName(i, name, sizeof (name));
				SSCANF_Join(i, name, bool:IsPlayerNPC(i));
			}
		}
	#endif
}

/**
 * <library>sscanf</library>
 * <remarks>
 * Called when the script starts if it is a NPC mode, sets up the system, then
 * calls the "real" OnNPCModeInit (using the new ALS 2 hook method).
 * </remarks>
 */
#if SSCANF_NPC
	public OnNPCModeInit()
	{
		SSCANF_RunInit();
		SSCANF_OnNPCModeInit();
		return 1;
	}

	#if defined _ALS_OnNPCModeInit
		#undef OnNPCModeInit
	#else
		#define _ALS_OnNPCModeInit
	#endif
	#define OnNPCModeInit(%0) CHAIN_PUBLIC:SSCANF_OnNPCModeInit(%0)
	CHAIN_FORWARD:SSCANF_OnNPCModeInit() = 1;
#else
	native SSCANF_OnNPCModeInit();
	#define SSCANF_OnNPCModeInit()
#endif

/**
 * <library>sscanf</library>
 * <remarks>
 * NPC modes have no "OnPlayerConnect callback, so we need to simulate one.
 * </remarks>
 */
#if SSCANF_NPC && !defined SSCANF_NO_PLAYERS
	public SSCANF_PlayerCheck()
	{
		new
			name[MAX_PLAYER_NAME + 1];
		for (new i = 0; i != MAX_PLAYERS; ++i)
		{
			if (IsPlayerConnected(i))
			{
				if (!SSCANF_IsConnected(i))
				{
					GetPlayerName(i, name, sizeof (name));
					// We have no way to know if they are an NPC or not!
					SSCANF_Join(i, name, false);
				}
			}
			else
			{
				if (SSCANF_IsConnected(i))
				{
					SSCANF_Leave(i);
				}
			}
		}
	}
#else
	native SSCANF_PlayerCheck();
	#define SSCANF_PlayerCheck()
#endif

// `SSCANF_BRACKETS` to avoid macro replacements.
#define SSCANF_BRACKETS ()
forward OnScriptInit SSCANF_BRACKETS;
#undef SSCANF_BRACKETS

/**
 * <library>sscanf</library>
 * <remarks>
 * Called for earlier initialisation by YSI.
 * </remarks>
 */
#if SSCANF_NPC
	native SSCANF_OnScriptInit();
	#define SSCANF_OnScriptInit()
#else
	public OnScriptInit()
	{
		if (!SSCANF_gInit)
		{
			SSCANF_RunInit();
		}
		SSCANF_OnScriptInit();
		return 1;
	}

	#if defined _ALS_OnScriptInit
		#undef OnScriptInit
	#else
		#define _ALS_OnScriptInit
	#endif
	#define OnScriptInit(%0) CHAIN_PUBLIC:SSCANF_OnScriptInit(%0)
	CHAIN_FORWARD:SSCANF_OnScriptInit() = 1;
#endif

/**
 * <library>sscanf</library>
 * <remarks>
 * Called when the script starts if it is a filterscript, sets up the system,
 * then calls the "real" OnFilterScriptInit (using the new ALS 2 hook method).
 * </remarks>
 */
#if SSCANF_NPC
	native SSCANF_OnFilterScriptInit();
	#define SSCANF_OnFilterScriptInit()
#else
	public OnFilterScriptInit()
	{
		if (!SSCANF_gInit)
		{
			SSCANF_RunInit();
		}
		SSCANF_OnFilterScriptInit();
		return 1;
	}

	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif
	#define OnFilterScriptInit(%0) CHAIN_PUBLIC:SSCANF_OnFilterScriptInit(%0)
	CHAIN_FORWARD:SSCANF_OnFilterScriptInit() = 1;
#endif

/**
 * <library>sscanf</library>
 * <remarks>
 * Called when the script starts if it is a gamemode.  This callback is also
 * called in filterscripts so we don't want to reinitialise the system in that
 * case.
 * </remarks>
 */
#if SSCANF_NPC
	native SSCANF_OnGameModeInit();
	#define SSCANF_OnGameModeInit()
#else
	public OnGameModeInit()
	{
		if (!SSCANF_gInit)
		{
			SSCANF_RunInit();
		}
		SSCANF_OnGameModeInit();
		return 1;
	}

	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif
	#define OnGameModeInit(%0) CHAIN_PUBLIC:SSCANF_OnGameModeInit(%0)
	CHAIN_FORWARD:SSCANF_OnGameModeInit() = 1;
#endif

forward OnCachedInit();

/**
 * <library>sscanf</library>
 * <remarks>
 * Called when the script starts if it is (legacy) YSI cached mode.
 * </remarks>
 */
#if SSCANF_NPC
	native SSCANF_OnCachedInit();
	#define SSCANF_OnCachedInit()
#else
	public OnCachedInit()
	{
		SSCANF_RunInit();
		SSCANF_OnCachedInit();
		return 1;
	}

	#if defined _ALS_OnCachedInit
		#undef OnCachedInit
	#else
		#define _ALS_OnCachedInit
	#endif
	#define OnCachedInit(%0) CHAIN_PUBLIC:SSCANF_OnCachedInit(%0)
	CHAIN_FORWARD:SSCANF_OnCachedInit() = 1;
#endif

/**
 * <library>sscanf</library>
 * <remarks>
 * Called when a player connects.  Actually increments an internal count so that
 * if a script ends and <c>OnPlayerDisconnect</c> is called then <c>sscanf</c>
 * still knows that the player is really connected.  Also stores their name
 * internally.
 * </remarks>
 */
#if SSCANF_NPC
	native SSCANF_OnPlayerConnect();
	#define SSCANF_OnPlayerConnect()
#else
	public OnPlayerConnect(playerid)
	{
		new
			name[MAX_PLAYER_NAME + 1];
		GetPlayerName(playerid, name, sizeof(name));
		SSCANF_Join(playerid, name, bool:IsPlayerNPC(playerid));
		SSCANF_OnPlayerConnect(playerid);
		return 1;
	}

	#if defined _ALS_OnPlayerConnect
		#undef OnPlayerConnect
	#else
		#define _ALS_OnPlayerConnect
	#endif
	#define OnPlayerConnect(%0) CHAIN_PUBLIC:SSCANF_OnPlayerConnect(%0)
	CHAIN_FORWARD:SSCANF_OnPlayerConnect(playerid) = 1;
#endif

/**
 * <library>sscanf</library>
 * <remarks>
 * Called when a player disconnects, or when a script is ended.
 * </remarks>
 */
#if SSCANF_NPC
	native SSCANF_OnPlayerDisconnect();
	#define SSCANF_OnPlayerDisconnect()
#else
	public OnPlayerDisconnect(playerid, reason)
	{
		SSCANF_OnPlayerDisconnect(playerid, reason);
		SSCANF_Leave(playerid);
		return 1;
	}

	#if defined _ALS_OnPlayerDisconnect
		#undef OnPlayerDisconnect
	#else
		#define _ALS_OnPlayerDisconnect
	#endif
	#define OnPlayerDisconnect(%0) CHAIN_PUBLIC:SSCANF_OnPlayerDisconnect(%0)
	CHAIN_FORWARD:SSCANF_OnPlayerDisconnect(playerid, reason) = 1;
#endif

// Ensure that these purely internal natives can't be called from outside this
// include.
#define SSCANF_Init
#define SSCANF_Join
#define SSCANF_Leave
#define SSCANF_IsConnected

#if defined __PawnBuild
	#define extract%0->%1; EXTRN%1;UNFORMAT__(__file,__line,_:EXTRV:EXTRX:%0,""#,%1,,);
#else
	#define extract%0->%1; EXTRN%1;UNFORMAT__(SSCANF_EXT_,-1,_:EXTRV:EXTRX:%0,""#,%1,,);
#endif

#define UNFORMAT__(%7,%8,_:EXTRV:EXTRX:%0,""#,%1);%2else if (UNFORMAT__(%7,%8,_:EXTRV:EXTRX:%0,""#,%1))

#define EXTRV:EXTRX:%0<%3>%4#,%9new%1,%2) EXTRZ:EXTRY:%0%4#P<%3>,|||%1|||%2)
#define EXTRZ:EXTRY:%0#P<,> EXTRY:%0"P<,>"#
#define EXTRX:%0#,%9new%1,%2) EXTRY:%0#,|||%1|||%2)
#define EXTRY: EXTR8:EXTR9:EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:

#define EXTR8:EXTR9:EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:%0#%1,%2|||%6:%3=%9|||%4) %6_EXTRO:%0#%1,%2|||%3=%9|||%4)
#define EXTR9:EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:%0#%1,%2|||%3=%9|||%4) __EXTRO:%0#%1,%2|||%3=%9|||%4)
#define EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:%0#%1,%2|||%6:%3[%7]|||%4) %6_EXTRW:%0#%1,%2|||%3[%7]|||%4)
#define EXTR1:EXTR2:EXTR3:EXTR4:%0#%1,%2|||%3[%7]|||%4) __EXTRW:%0#%1,%2|||%3[%7]|||%4)
#define EXTR2:EXTR3:EXTR4:%0#%1,%2|||%6:%3|||%4) %6_EXTRN:%0#%1,%2|||%3|||%4)
#define EXTR3:EXTR4:%0#%1,,%2||||||%4) %0#%1,%2)
#define EXTR4:%0#%1,%2|||%3|||%4) __EXTRN:%0#%1,%2|||%3|||%4)

// Optional specifiers.
#define __EXTRO:%0#%1,%2|||%3=%9|||%4,%5) EXTRY:%0#%1I"("#%9")"#,%2,%3|||%4|||%5)
#define Float_EXTRO:%0#%1,%2|||%3=%9|||%4,%5) EXTRY:%0#%1F"("#%9")"#,%2,%3|||%4|||%5)
#define player_EXTRO:%0#%1,%2|||%3=%9|||%4,%5) EXTRY:%0#%1U"("#%9")"#,%2,%3|||%4|||%5)
#define string_EXTRO:%0#%1,%2|||%3[%7]=%9|||%4,%5) EXTRY:%0#%1S"("#%9")[*]",%2,(%7),%3|||%4|||%5)

// Normal specifiers (the double underscore is to work for "_:".
#define __EXTRN:%0#%1,%2|||%3|||%4,%5) EXTRY:%0#%1i,%2,%3|||%4|||%5)
#define Float_EXTRN:%0#%1,%2|||%3|||%4,%5) EXTRY:%0#%1f,%2,%3|||%4|||%5)
#define player_EXTRN:%0#%1,%2|||%3|||%4,%5) EXTRY:%0#%1u,%2,%3|||%4|||%5)
//#define string_EXTRW:%0#%1,%2|||%3[%7]|||%4,%5) EXTRY:%0#%1s[%7],%2,%3|||%4|||%5)

// Array versions of normal specifiers.
#define __EXTRW:%0#%1,%2|||%3[%7]|||%4,%5) EXTRY:%0#%1a<i>[*],%2,(%7),%3|||%4|||%5)
#define Float_EXTRW:%0#%1,%2|||%3[%7]|||%4,%5) EXTRY:%0#%1a<f>[*],%2,(%7),%3|||%4|||%5)
#define player_EXTRW:%0#%1,%2|||%3[%7]|||%4,%5) EXTRY:%0#%1a<u>[*],%2,(%7),%3|||%4|||%5)
#define string_EXTRW:%0#%1,%2|||%3[%7]|||%4,%5) EXTRY:%0#%1s[*],%2,(%7),%3|||%4|||%5)

// Get rid of excess leading space which causes warnings.
#define EXTRN%0new%1; new%1;

#if !defined string
    #define string:
#endif

#define player:%0;UNFORMAT__(%1) %0;UNFORMAT__(%1)

#define hex:%0;UNFORMAT__(%1) %0;UNFORMAT__(%1)
#define hex_EXTRO:%0#%1,%2|||%3=%9|||%4,%5) EXTRY:%0#%1H"("#%9")"#,%2,%3|||%4|||%5)
#define hex_EXTRN:%0#%1,%2|||%3|||%4,%5) EXTRY:%0#%1h,%2,%3|||%4|||%5)
#define hex_EXTRW:%0#%1,%2|||%3[%7]|||%4,%5) EXTRY:%0#%1a<h>[*],%2,(%7),%3|||%4|||%5)

#define bin:%0;UNFORMAT__(%1) %0;UNFORMAT__(%1)
#define bin_EXTRO:%0#%1,%2|||%3=%9|||%4,%5) EXTRY:%0#%1B"("#%9")"#,%2,%3|||%4|||%5)
#define bin_EXTRN:%0#%1,%2|||%3|||%4,%5) EXTRY:%0#%1b,%2,%3|||%4|||%5)
#define bin_EXTRW:%0#%1,%2|||%3[%7]|||%4,%5) EXTRY:%0#%1a<b>[*],%2,(%7),%3|||%4|||%5)

#define colour:%0;UNFORMAT__(%1) %0;UNFORMAT__(%1)
#define colour_EXTRO:%0#%1,%2|||%3=%9|||%4,%5) EXTRY:%0#%1M"("#%9")"#,%2,%3|||%4|||%5)
#define colour_EXTRN:%0#%1,%2|||%3|||%4,%5) EXTRY:%0#%1m,%2,%3|||%4|||%5)
#define colour_EXTRW:%0#%1,%2|||%3[%7]|||%4,%5) EXTRY:%0#%1a<m>[*],%2,(%7),%3|||%4|||%5)

#define kustom:%0<%1> %0
#define kustom_EXTRO:%0#%1,%2|||%3<%8>=%9|||%4,%5) EXTRY:%0#%1K<%8>"("#%9")"#,%2,%3|||%4|||%5)
#define kustom_EXTRN:%0#%1,%2|||%3<%8>|||%4,%5) EXTRY:%0#%1k<%8>,%2,%3|||%4|||%5)
//#define bin_EXTRW:%0#%1,%2|||%3[%7]|||%4,%5) EXTRY:%0#%1a<b>[*],%2,(%7),%3|||%4|||%5)

/**
 * <library>sscanf</library>
 * <param name="input">The first string to compare.</param>
 * <param name="candidates">A list of other strings to compare against.</param>
 * <param name="threshold">How similar the strings must be to be considered a match.</param>
 * <param name="count">The number of candidates.</param>
 * <remarks>
 * Takes an input string and an array of string possibilities (candidates) and
 * returns the index of the string closest to the input string.  If no valid
 * match is found, <c>-1</c> is returned.  Note that this will always return the
 * closest, even if the closest is not that close; which is why an optional
 * <c>threshold</c> parameter is available.  When this parameter is provided the
 * closest match must be closer in Levenshtein distance than the threshold,
 * otherwise again <c>-1</c> is returned.
 * </remarks>
 */
stock SSCANF_GetClosestString(const input[], const candidates[][], threshold = cellmax, count = sizeof (candidates))
{
	new
		closest = cellmin,
		distance;
	while (count--)
	{
		distance = SSCANF_Levenshtein(input, candidates[count]);
		if (distance < threshold)
		{
			closest = count,
			threshold = distance;
		}
	}
	return closest;
}

/**
 * <library>sscanf</library>
 * <param name="input">The first string to compare.</param>
 * <param name="candidates">A list of other strings to compare against.</param>
 * <param name="results">The values to return when the corresponding candidate (by index) is the closest match.</param>
 * <param name="fail">The value to return when there is no good match.</param>
 * <param name="threshold">How similar the strings must be to be considered a match.</param>
 * <param name="count">The number of candidates (must match <c>check</c>).</param>
 * <param name="check">The number of results (must match <c>count</c>).</param>
 * <remarks>
 * Similar to <c>SSCANF_GetClosestString</c> in that it searches the
 * <c>candidates</c> array for the string most closely matching the <c>input</c>
 * and bounded by <c>threshold</c>.  But instead of returning the index this
 * function returns the value in the second <c>results</c> array at that index;
 * and instead of returning <c>-1</c> on failure it returns the value of
 * <c>fail</c>.  The two arrays must match in size and an <c>assert</c> in the
 * function checks for this.
 * </remarks>
 */
stock SSCANF_GetClosestValue(const input[], const candidates[][], const results[], fail = cellmin, threshold = cellmax, count = sizeof (candidates), check = sizeof (results))
{
	assert(count == check);
	new closest = SSCANF_GetClosestString(input, candidates, threshold, count);
	if (closest == cellmin)
	{
		return fail;
	}
	return results[closest];
}

/**
 * <library>sscanf</library>
 * <param name="string">The current word being parsed out of the <c>sscanf</c> input.</param>
 * <remarks>
 * The default implementation of <c>k&lt;weapon&gt;</c>.  Finds the closest
 * weapon by Levenshtein distance to the input.
 * </remarks>
 */
#if defined SSCANF_NO_K_WEAPON
	native SSCANF_no_k_weapon();
	#define SSCANF_no_k_weapon()
#else
	SSCANF:weapon(const string[])
	{
		static const results[] = {
			0, 0, 1, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 22, 23, 24, 25,
			26, 27, 28, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46
		};

		static const candidates[][] = {
			"Fists",
			"Unarmed",
			"Knuckles",
			"Knuckledusters",
			"Brass Knuckles",
			"Golf Club",
			"Night Stick",
			"Knife",
			"Baseball Bat",
			"Shovel",
			"Pool cue",
			"Katana",
			"Chainsaw",
			"Purple Dildo",
			"White Dildo",
			"Long White Dildo",
			"White Dildo 2",
			"Flowers",
			"Cane",
			"Grenades",
			"Tear Gas",
			"Molotovs",
			"Pistol",
			"Silenced Pistol",
			"Desert Eagle",
			"Shotgun",
			"Sawn Off Shotgun",
			"Combat Shotgun",
			"Micro Uzi",
			"Mac 10",
			"MP5",
			"AK47",
			"M4",
			"Tec9",
			"Rifle",
			"Sniper Rifle",
			"RPG",
			"Missile Launcher",
			"Flame Thrower",
			"Minigun",
			"Sachel Charges",
			"Detonator",
			"Spray Paint",
			"Fire Extinguisher",
			"Camera",
			"Nightvision Goggles",
			"Thermal Goggles",
			"Parachute"
		};
		
		// This function is VERY basic, needs VASTLY improving to detect variations.
		if ('0' <= string[0] <= '9')
		{
			new
				ret = strval(string);
			if (0 <= ret <= 18 || 22 <= ret <= 46)
			{
				return ret;
			}
		}
		else
		{
			return SSCANF_GetClosestValue(string, candidates, results, -1);
		}
		return -1;
	}
#endif

/**
 * <library>sscanf</library>
 * <param name="string">The current word being parsed out of the <c>sscanf</c> input.</param>
 * <remarks>
 * The default implementation of <c>k&lt;vehicle&gt;</c>.  Finds the closest
 * vehicle by Levenshtein distance to the input.
 * </remarks>
 */
#if defined SSCANF_NO_K_VEHICLE
	native SSCANF_no_k_vehicle();
	#define SSCANF_no_k_vehicle()
#else
	SSCANF:vehicle(const string[])
	{
		static const results[] = {
			400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417,
			418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435,
			436, 437, 438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453,
			454, 455, 456, 457, 458, 459, 460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471,
			472, 473, 474, 475, 476, 477, 478, 479, 480, 481, 482, 483, 484, 485, 486, 487, 488, 489,
			490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507,
			508, 509, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525,
			526, 527, 528, 529, 530, 531, 532, 533, 534, 535, 536, 537, 538, 539, 540, 541, 542, 543,
			544, 545, 546, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561,
			562, 563, 564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575, 576, 577, 578, 579,
			580, 581, 582, 583, 584, 585, 586, 587, 588, 589, 590, 591, 592, 593, 594, 595, 596, 597,
			598, 599, 600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611
		};

		static const candidates[][] = {
			"Landstalker",
			"Bravura",
			"Buffalo",
			"Linerunner",
			"Perenniel",
			"Sentinel",
			"Dumper",
			"Firetruck",
			"Trashmaster",
			"Stretch",
			"Manana",
			"Infernus",
			"Voodoo",
			"Pony",
			"Mule",
			"Cheetah",
			"Ambulance",
			"Leviathan",
			"Moonbeam",
			"Esperanto",
			"Taxi",
			"Washington",
			"Bobcat",
			"Mr Whoopee",
			"BF Injection",
			"Hunter",
			"Premier",
			"Enforcer",
			"Securicar",
			"Banshee",
			"Predator",
			"Bus",
			"Rhino",
			"Barracks",
			"Hotknife",
			"Article Trailer",
			"Previon",
			"Coach",
			"Cabbie",
			"Stallion",
			"Rumpo",
			"RC Bandit",
			"Romero",
			"Packer",
			"Monster",
			"Admiral",
			"Squallo",
			"Seasparrow",
			"Pizzaboy",
			"Tram",
			"Article Trailer 2",
			"Turismo",
			"Speeder",
			"Reefer",
			"Tropic",
			"Flatbed",
			"Yankee",
			"Caddy",
			"Solair",
			"Berkley's RC Van",
			"Skimmer",
			"PCJ-600",
			"Faggio",
			"Freeway",
			"RC Baron",
			"RC Raider",
			"Glendale",
			"Oceanic",
			"Sanchez",
			"Sparrow",
			"Patriot",
			"Quad",
			"Coastguard",
			"Dinghy",
			"Hermes",
			"Sabre",
			"Rustler",
			"ZR-350",
			"Walton",
			"Regina",
			"Comet",
			"BMX",
			"Burrito",
			"Camper",
			"Marquis",
			"Baggage",
			"Dozer",
			"Maverick",
			"SAN News Maverick",
			"Rancher",
			"FBI Rancher",
			"Virgo",
			"Greenwood",
			"Jetmax",
			"Hotring Racer",
			"Sandking",
			"Blista Compact",
			"Police Maverick",
			"Boxville",
			"Benson",
			"Mesa",
			"RC Goblin",
			"Hotring Racer",
			"Hotring Racer",
			"Bloodring Banger",
			"Rancher",
			"Super GT",
			"Elegant",
			"Journey",
			"Bike",
			"Mountain Bike",
			"Beagle",
			"Cropduster",
			"Stuntplane",
			"Tanker",
			"Roadtrain",
			"Nebula",
			"Majestic",
			"Buccaneer",
			"Shamal",
			"Hydra",
			"FCR-900",
			"NRG-500",
			"HPV1000",
			"Cement Truck",
			"Towtruck",
			"Fortune",
			"Cadrona",
			"FBI Truck",
			"Willard",
			"Forklift",
			"Tractor",
			"Combine Harvester",
			"Feltzer",
			"Remington",
			"Slamvan",
			"Blade",
			"Freight (Train)",
			"Brownstreak (Train)",
			"Vortex",
			"Vincent",
			"Bullet",
			"Clover",
			"Sadler",
			"Firetruck LA",
			"Hustler",
			"Intruder",
			"Primo",
			"Cargobob",
			"Tampa",
			"Sunrise",
			"Merit",
			"Utility Van",
			"Nevada",
			"Yosemite",
			"Windsor",
			"Monster \"A\"",
			"Monster \"B\"",
			"Uranus",
			"Jester",
			"Sultan",
			"Stratum",
			"Elegy",
			"Raindance",
			"RC Tiger",
			"Flash",
			"Tahoma",
			"Savanna",
			"Bandito",
			"Freight Flat Trailer (Train)",
			"Streak Trailer (Train)",
			"Kart",
			"Mower",
			"Dune",
			"Sweeper",
			"Broadway",
			"Tornado",
			"AT400",
			"DFT-30",
			"Huntley",
			"Stafford",
			"BF-400",
			"Newsvan",
			"Tug",
			"Petrol Trailer",
			"Emperor",
			"Wayfarer",
			"Euros",
			"Hotdog",
			"Club",
			"Freight Box Trailer (Train)",
			"Article Trailer 3",
			"Andromada",
			"Dodo",
			"RC Cam",
			"Launch",
			"Police Car (LSPD)",
			"Police Car (SFPD)",
			"Police Car (LVPD)",
			"Police Ranger",
			"Picador",
			"S.W.A.T.",
			"Alpha",
			"Phoenix",
			"Glendale Shit",
			"Sadler Shit",
			"Baggage Trailer \"A\"",
			"Baggage Trailer \"B\"",
			"Tug Stairs Trailer",
			"Boxville",
			"Farm Trailer",
			"Utility Trailer"
		};

		// This function is VERY basic, needs VASTLY improving to detect variations.
		if ('0' <= string[0] <= '9')
		{
			new
				ret = strval(string);
			if (400 <= ret <= 611)
			{
				return ret;
			}
		}
		else
		{
			return SSCANF_GetClosestValue(string, candidates, results, -1);
		}
		return -1;
	}
#endif

// Fix the compiler crash when both the PAWN and Plugin versions of sscanf are
// found by renaming the old version at declaration.  (fixes.inc compatible
// naming scheme: "BAD_Function()").
#define SSCANF__(%0:...) BAD_sscanf(%0:...)

