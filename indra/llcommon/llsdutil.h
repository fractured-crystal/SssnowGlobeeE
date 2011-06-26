/** 
 * @file llsdutil.h
 * @author Phoenix
 * @date 2006-05-24
 * @brief Utility classes, functions, etc, for using structured data.
 *
 * $LicenseInfo:firstyear=2006&license=viewergpl$
 * 
 * Copyright (c) 2006-2009, Linden Research, Inc.
 * 
 * Second Life Viewer Source Code
 * The source code in this file ("Source Code") is provided by Linden Lab
 * to you under the terms of the GNU General Public License, version 2.0
 * ("GPL"), unless you have obtained a separate licensing agreement
 * ("Other License"), formally executed by you and Linden Lab.  Terms of
 * the GPL can be found in doc/GPL-license.txt in this distribution, or
 * online at http://secondlifegrid.net/programs/open_source/licensing/gplv2
 * 
 * There are special exceptions to the terms and conditions of the GPL as
 * it is applied to this Source Code. View the full text of the exception
 * in the file doc/FLOSS-exception.txt in this software distribution, or
 * online at
 * http://secondlifegrid.net/programs/open_source/licensing/flossexception
 * 
 * By copying, modifying or distributing this software, you acknowledge
 * that you have read and understood your obligations described above,
 * and agree to abide by those obligations.
 * 
 * ALL LINDEN LAB SOURCE CODE IS PROVIDED "AS IS." LINDEN LAB MAKES NO
 * WARRANTIES, EXPRESS, IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY,
 * COMPLETENESS OR PERFORMANCE.
 * $/LicenseInfo$
 */

#ifndef LL_LLSDUTIL_H
#define LL_LLSDUTIL_H

#include "llsd.h"

// U32
LL_COMMON_API LLSD ll_sd_from_U32(const U32);
LL_COMMON_API U32 ll_U32_from_sd(const LLSD& sd);

// U64
LL_COMMON_API LLSD ll_sd_from_U64(const U64);
LL_COMMON_API U64 ll_U64_from_sd(const LLSD& sd);

// IP Address
LL_COMMON_API LLSD ll_sd_from_ipaddr(const U32);
LL_COMMON_API U32 ll_ipaddr_from_sd(const LLSD& sd);

// Binary to string
LL_COMMON_API LLSD ll_string_from_binary(const LLSD& sd);

//String to binary
LL_COMMON_API LLSD ll_binary_from_string(const LLSD& sd);

// Serializes sd to static buffer and returns pointer, useful for gdb debugging.
LL_COMMON_API char* ll_print_sd(const LLSD& sd);

// Serializes sd to static buffer and returns pointer, using "pretty printing" mode.
LL_COMMON_API char* ll_pretty_print_sd(const LLSD& sd);

//compares the structure of an LLSD to a template LLSD and stores the
//"valid" values in a 3rd LLSD. Default values
//are pulled from the template.  Extra keys/values in the test
//are ignored in the resultant LLSD.  Ordering of arrays matters
//Returns false if the test is of same type but values differ in type
//Otherwise, returns true

LL_COMMON_API BOOL compare_llsd_with_template(
	const LLSD& llsd_to_test,
	const LLSD& template_llsd,
	LLSD& resultant_llsd);

// Simple function to copy data out of input & output iterators if
// there is no need for casting.
template<typename Input> LLSD llsd_copy_array(Input iter, Input end)
{
	LLSD dest;
	for (; iter != end; ++iter)
	{
		dest.append(*iter);
	}
	return dest;
}

#endif // LL_LLSDUTIL_H
