/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#include "cfan/Array.h"



#define cmopFunc(v1, v2) cf_Array_defaultCmopFunc(v1, v2)
cf_ArrayTemplate_impl(cf_ArrayI, int)
cf_ArrayTemplate_impl(cf_ArrayP, void*)
#undef cmopFunc