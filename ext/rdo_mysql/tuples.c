/*
 * RDO MySQL Driver.
 * Copyright © 2012 Chris Corbyn.
 *
 * See LICENSE file for details.
 */

#include "tuples.h"
#include <stdio.h>
#include "macros.h"

/** RDO::MySQL::TupleList class */
static VALUE rdo_mysql_cTupleList;

/** Struct wrapped by TupleList class */
typedef struct {
  MYSQL_RES * res;
} RDOMySQLTupleList;

/** Free memory allocated to list during GC */
static void rdo_mysql_tuple_list_free(RDOMySQLTupleList * list) {
  mysql_free_result(list->res);
  free(list);
}

/** Constructor to create a new TupleList for the given result */
VALUE rdo_mysql_tuple_list_new(MYSQL_RES * res) {
  RDOMySQLTupleList * list = malloc(sizeof(RDOMySQLTupleList));
  list->res = res;

  VALUE obj = Data_Wrap_Struct(rdo_mysql_cTupleList,
      0, rdo_mysql_tuple_list_free, list);

  rb_obj_call_init(obj, 0, NULL);

  return obj;
}

/** Cast the given value to a Ruby type */
static VALUE rdo_mysql_cast_value(char * v, MYSQL_FIELD f) {
  switch (f.type) {
    case MYSQL_TYPE_TINY:
    case MYSQL_TYPE_SHORT:
    case MYSQL_TYPE_LONG:
    case MYSQL_TYPE_INT24:
    case MYSQL_TYPE_LONGLONG:
      return RDO_FIXNUM(v);

    default:
      return RDO_STRING(v, strlen(v), 1);
  }
}

/** Iterate over all tuples in the list */
static VALUE rdo_mysql_tuple_list_each(VALUE self) {
  RDOMySQLTupleList * list;
  Data_Get_Struct(self, RDOMySQLTupleList, list);

  if (!rb_block_given_p() || list->res == NULL) {
    return self;
  }

  mysql_data_seek(list->res, 0);

  unsigned int   nfields = mysql_num_fields(list->res);
  MYSQL_FIELD  * fields  = mysql_fetch_fields(list->res);
  MYSQL_ROW      row;

  while ((row = mysql_fetch_row(list->res))) {
    VALUE hash     = rb_hash_new();
    unsigned int i = 0;

    for (; i < nfields; ++i) {
      rb_hash_aset(hash,
          ID2SYM(rb_intern(fields[i].name)),
          rdo_mysql_cast_value(row[i], fields[i]));
    }

    rb_yield(hash);
  }

  return self;
}

/** Initializer for the TupleList class */
void Init_rdo_mysql_tuples(void) {
  rb_require("rdo/mysql");

  VALUE mMySQL = rb_path2class("RDO::MySQL");

  rdo_mysql_cTupleList = rb_define_class_under(mMySQL,
      "TupleList", rb_cObject);

  rb_define_method(rdo_mysql_cTupleList,
      "each", rdo_mysql_tuple_list_each, 0);

  rb_include_module(rdo_mysql_cTupleList, rb_mEnumerable);
}
