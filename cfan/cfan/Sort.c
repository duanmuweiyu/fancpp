/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#include "cfan/Sort.h"



#define cf_cmopFunc(v1, v2) ((v1) - (v2))
cf_SortTemplate_impl(cf_ArrayI, int)
#undef cf_cmopFunc