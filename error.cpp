/*
 *  sscanf 2.13.8
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

#include "error.h"

int
	g_iErrorCode = 0,
	g_iErrorSpecifier = 0; // No error.

int
	GetErrorCode()
{
	if (g_iErrorSpecifier == 0)
	{
		return 0;
	}
	return g_iErrorCode;
}

void
	SetErrorCode(int error)
{
	g_iErrorCode = error;
}

int
	GetErrorSpecifier()
{
	return g_iErrorSpecifier;
}

void
	SetErrorSpecifier(int spec)
{
	g_iErrorSpecifier = spec;
}
