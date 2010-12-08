/*
 * This file implements the NonReleationIndex logic in AlchemyDB
 *

GPL License

Copyright (c) 2010 Russell Sullivan <jaksprats AT gmail DOT com>
ALL RIGHTS RESERVED 

   This file is part of AlchemyDatabase

    AlchemyDatabase is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    AlchemyDatabase is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with AlchemyDatabase.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __NRI__H
#define __NRI__H

#include "redis.h"

#include "alsosql.h"
#include "btreepriv.h"
#include "parser.h"
#include "common.h"

void freeNrlIndexObject(robj *o);

void nrlIndexAdd(robj *o, robj *pko, char *vals, uint32 cofsts[]);

bool parseNRLcmd(char *o_s, list *nrltoks, list *nrlcols, int tmatch);
sds rebuildOrigNRLcmd(robj *o);

void runNrlIndexFromStream(uchar *stream, d_l_t *nrlind, int itbl);

#endif /* __NRI__H */ 
