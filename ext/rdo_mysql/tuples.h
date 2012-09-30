/*
 * RDO MySQL Driver.
 * Copyright © 2012 Chris Corbyn.
 *
 * See LICENSE file for details.
 */

#include <ruby.h>
#include <mysql.h>

/** Constructor to return a new TupleList wrapping res */
VALUE rdo_mysql_tuple_list_new(MYSQL_RES * res);

/** Called during extension initialization to create the TupleList class */
void Init_rdo_mysql_tuples(void);
