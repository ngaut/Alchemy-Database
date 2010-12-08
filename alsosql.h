/*
 * This file implements the basic SQL commands of Alsosql (single row ops)
 *  and calls the range-query and join ops
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

#ifndef __ALSOSQL__H
#define __ALSOSQL__H

#include "adlist.h"
#include "redis.h"

#include "common.h"


typedef struct r_tbl {
    robj   *name;
    int     col_count;
    robj   *col_name[MAX_COLUMN_PER_TABLE];
    uchar   col_type[MAX_COLUMN_PER_TABLE];
    /* col_flags -> current usage only AUTO_INCREMENT */
    uchar   col_flags[MAX_COLUMN_PER_TABLE];
    int     virt_indx; /* TODO is this still used? */
} r_tbl_t;

typedef struct dual_lists {
    list *l1;
    list *l2;
    short num;
} d_l_t;

typedef struct r_ind {
    robj  *obj;
    int    table;
    int    column;
    uchar  type;
    bool   virt; /* virtual - i.e. on primary key */
    bool   nrl;  /* non relational index - i.e. redis command */
} r_ind_t;

typedef struct check_sql_where_clause {
    robj  *key;  
    robj  *low;  
    robj  *high; 
    list  *inl;  
    char  *stor; 
    char  *lvr;  
    int    imatch;
    int    tmatch;
    int    cmatch;
    int    obc;    /* ORDER BY col */
    int    obt;    /* ORDER BY tbl -> JOINS */
    bool   asc;   
    int    lim;   
    int    ofst;
    sds    token;    
    int    sto;
} cswc_t;

int find_table(char *tname);
int find_table_n(char *tname, int len);
int find_column(int tmatch, char *column);
int find_column_n(int tmatch, char *column, int len);
int get_all_cols(int tmatch, int cmatchs[]);

bool cCpyOrReply(redisClient *c, char *src, char *dest, unsigned int len);

/* TABLE_CHECK_OR_REPLY(char *TBL, RET) -
     creates (int tmatch) */
#define TABLE_CHECK_OR_REPLY(TBL, RET)        \
    int tmatch = find_table(TBL);             \
    if (tmatch == -1) {                       \
        addReply(c, shared.nonexistenttable); \
        return RET;                           \
    }

/* COLUMN_CHECK_OR_REPLY(char *cargv2ptr) -
     creates (char *cname, int cmatch)    */
#define COLUMN_CHECK_OR_REPLY(cargv2ptr, retval)   \
    char *cname  = cargv2ptr;                      \
    int   cmatch = find_column(tmatch, cname);     \
    if (cmatch == -1) {                            \
        addReply(c,shared.nonexistentcolumn);      \
        return retval;                             \
    }

bool parseCommaSpaceListReply(redisClient *c,
                              char        *y,
                              bool         col_check,
                              bool         tbl_check,
                              bool         join_check,
                              int          tmatch,    /* COL or TBL */
                              int          cmatchs[], /* COL or TBL */
                              int         *numt,      /* JOIN */
                              int          tmatchs[], /* JOIN */
                              int          j_tbls[],  /* JOIN */
                              int          j_cols[],  /* JOIN */
                              int         *qcols,
                              bool        *cstar);

bool parseSelectReply(redisClient *c,
                      bool         is_scan,
                      bool        *no_wc,
                      int         *tmatch,
                      int          cmatchs[MAX_COLUMN_PER_TABLE],
                      int         *qcols,
                      bool        *join,
                      bool        *cstar,
                      char        *clist,
                      char        *from,
                      char        *tlist,
                      char        *where);

void init_check_sql_where_clause(cswc_t *w, int tmatch, sds token);
void destroy_check_sql_where_clause(cswc_t *w);

void singleFKHack(cswc_t *w, uchar *wtype);

bool leftoverParsingReply(redisClient *c, char *x);

int parseUpdateOrReply(redisClient  *c,
                       int           tmatch,
                       char         *cname,
                       int           cmatchs[],
                       char         *vals   [],
                       unsigned int  vlens  []);

#define LEN_OBJ                                               \
    unsigned long  card   = 0;                                \
    robj          *lenobj = createObject(REDIS_STRING, NULL); \
    addReply(c, lenobj);                                      \
    decrRefCount(lenobj);

#define EMPTY_LEN_OBJ             \
    unsigned long  card   = 0;    \
    robj          *lenobj = NULL;

#define INIT_LEN_OBJ                           \
    lenobj = createObject(REDIS_STRING, NULL); \
    addReply(c, lenobj);                       \
    decrRefCount(lenobj);

void createTableCommitReply(redisClient *c,
                            char         cnames[][MAX_COLUMN_NAME_SIZE],
                            int          ccount,
                            char        *tname,
                            int          tlen);
void createCommand(redisClient *c);

void insertCommitReply(redisClient *c, 
                       char        *vals,
                       int          ncols,
                       int          tmatch,
                       int          matches,
                       int          indices[],
                       bool         ret_size);

void insertCommand(redisClient *c);
void sqlSelectCommand(redisClient *c);
void updateCommand(redisClient *c);
void deleteCommand(redisClient *c);
void dumpCommand(redisClient *c);

unsigned long emptyTable(redisDb *db, int tmatch);

/* FILLERS FILLERS FILLERS FILLERS FILLERS FILLERS FILLERS */
void tscanCommand(redisClient *c);  /* scan.h does not exist */
void normCommand(redisClient *c);   /* norm.h does not exist */
void denormCommand(redisClient *c); /* denorm.h does not exist */

#endif /*__ALSOSQL__H */ 
