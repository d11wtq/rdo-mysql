/*
 * RDO MySQL Driver.
 * Copyright © 2012 Chris Corbyn.
 *
 * See LICENSE file for details.
 */

#include "tuples.h"
#include <stdio.h>
#include "macros.h"

/* I hate magic numbers */
#define RDO_MYSQL_BINARY_ENC 63

/** RDO::MySQL::TupleList class */
static VALUE rdo_mysql_cTupleList;

/** Struct wrapped by TupleList class */
typedef struct {
  MYSQL_RES * res;
  int         encoding;
} RDOMySQLTupleList;

/** Free memory allocated to list during GC */
static void rdo_mysql_tuple_list_free(RDOMySQLTupleList * list) {
  mysql_free_result(list->res);
  free(list);
}

/** Parse the string as a Set */
static VALUE rdo_mysql_parse_set(char * v, unsigned long len, int enc) {
  return rb_funcall(rb_path2class("Set"),
      rb_intern("new"), 1, rb_funcall(RDO_STRING(v, len, enc),
        rb_intern("split"), 1, rb_str_new2(",")));
}

/** Constructor to create a new TupleList for the given result */
VALUE rdo_mysql_tuple_list_new(MYSQL_RES * res, int encoding) {
  RDOMySQLTupleList * list = malloc(sizeof(RDOMySQLTupleList));
  list->res      = res;
  list->encoding = encoding;

  VALUE obj = Data_Wrap_Struct(rdo_mysql_cTupleList,
      0, rdo_mysql_tuple_list_free, list);

  rb_obj_call_init(obj, 0, NULL);

  return obj;
}

/** Cast the given value to a Ruby type */
static VALUE rdo_mysql_cast_value(char * v, unsigned long len, MYSQL_FIELD f, int enc) {
  if (v == NULL)
    return Qnil;

  switch (f.type) {
    case MYSQL_TYPE_NULL:
      return Qnil;

    case MYSQL_TYPE_TINY:
    case MYSQL_TYPE_SHORT:
    case MYSQL_TYPE_LONG:
    case MYSQL_TYPE_INT24:
    case MYSQL_TYPE_LONGLONG:
      return RDO_FIXNUM(v);

    case MYSQL_TYPE_STRING:
    case MYSQL_TYPE_VAR_STRING:
    case MYSQL_TYPE_MEDIUM_BLOB:
    case MYSQL_TYPE_TINY_BLOB:
    case MYSQL_TYPE_LONG_BLOB:
    case MYSQL_TYPE_BLOB:
      if (f.flags & SET_FLAG)
        return rdo_mysql_parse_set(v, len, enc);
      else if (f.charsetnr == RDO_MYSQL_BINARY_ENC)
        return RDO_BINARY_STRING(v, len);
      else
        return RDO_STRING(v, len, enc);

    case MYSQL_TYPE_DECIMAL:
    case MYSQL_TYPE_NEWDECIMAL:
      return RDO_DECIMAL(v);

    case MYSQL_TYPE_FLOAT:
    case MYSQL_TYPE_DOUBLE:
      return RDO_FLOAT(v);

    case MYSQL_TYPE_DATE:
    case MYSQL_TYPE_NEWDATE:
      return RDO_DATE(v);

    case MYSQL_TYPE_TIMESTAMP:
    case MYSQL_TYPE_DATETIME:
      return RDO_DATE_TIME_WITHOUT_ZONE(v);

    default:
      return RDO_BINARY_STRING(v, len);
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

  unsigned int    nfields = mysql_num_fields(list->res);
  MYSQL_FIELD   * fields  = mysql_fetch_fields(list->res);
  MYSQL_ROW       row;

  while ((row = mysql_fetch_row(list->res))) {
    unsigned long * lengths = mysql_fetch_lengths(list->res);
    VALUE           hash    = rb_hash_new();
    unsigned int    i       = 0;

    for (; i < nfields; ++i) {
      rb_hash_aset(hash,
          ID2SYM(rb_intern(fields[i].name)),
          rdo_mysql_cast_value(row[i], lengths[i], fields[i], list->encoding));
    }

    rb_yield(hash);
  }

  return self;
}

/** Initializer for the TupleList class */
void Init_rdo_mysql_tuples(void) {
  rb_require("rdo/mysql");
  rb_require("set");

  VALUE mMySQL = rb_path2class("RDO::MySQL");

  rdo_mysql_cTupleList = rb_define_class_under(mMySQL,
      "TupleList", rb_cObject);

  rb_define_method(rdo_mysql_cTupleList,
      "each", rdo_mysql_tuple_list_each, 0);

  rb_include_module(rdo_mysql_cTupleList, rb_mEnumerable);
}
