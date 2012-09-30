/*
 * RDO MySQL Driver.
 * Copyright © 2012 Chris Corbyn.
 *
 * See LICENSE file for details.
 */

#include "driver.h"
#include <stdio.h>
#include "tuples.h"
#include "macros.h"

/** Struct wrapped by RDO::MySQL::Driver */
typedef struct {
  MYSQL * conn;
  int     is_open;
} RDOMySQLDriver;

/** Free memory associated with connection during GC */
static void rdo_mysql_driver_free(RDOMySQLDriver * driver) {
  mysql_close(driver->conn);
  free(driver);
}

/** Extract result information from the given result */
static VALUE rdo_mysql_result_info_new(MYSQL * conn, MYSQL_RES * res) {
  unsigned long long count = (res == NULL) ? 0 : mysql_num_rows(res);

  VALUE info = rb_hash_new();
  rb_hash_aset(info, ID2SYM(rb_intern("count")), LL2NUM(count));
  rb_hash_aset(info, ID2SYM(rb_intern("insert_id")),
      LL2NUM(mysql_insert_id(conn)));
  rb_hash_aset(info, ID2SYM(rb_intern("affected_rows")),
      LL2NUM(mysql_affected_rows(conn)));

  return info;
}

/** Allocate memory, wrapping RDOMySQLDriver */
static VALUE rdo_mysql_driver_allocate(VALUE klass) {
  RDOMySQLDriver * driver = malloc(sizeof(RDOMySQLDriver));
  driver->conn    = NULL;
  driver->is_open = 0;

  return Data_Wrap_Struct(klass, 0, rdo_mysql_driver_free, driver);
}

/** Open a connection to MySQL */
static VALUE rdo_mysql_driver_open(VALUE self) {
  RDOMySQLDriver * driver;
  Data_Get_Struct(self, RDOMySQLDriver, driver);

  if (driver->is_open) {
    return Qtrue;
  }

  if (!(driver->conn = mysql_init(NULL))) {
    RDO_ERROR("Failed to connect to MySQL. Could not allocate memory.");
  }

  VALUE host = rb_funcall(self, rb_intern("host"), 0);
  VALUE port = rb_funcall(self, rb_intern("port"), 0);
  VALUE user = rb_funcall(self, rb_intern("user"), 0);
  VALUE pass = rb_funcall(self, rb_intern("password"), 0);
  VALUE db   = rb_funcall(self, rb_intern("database"), 0);

  Check_Type(host, T_STRING);
  Check_Type(port, T_FIXNUM);
  Check_Type(user, T_STRING);
  Check_Type(pass, T_STRING);
  Check_Type(db,   T_STRING);

  if (!mysql_real_connect(driver->conn,
        RSTRING_PTR(host),
        RSTRING_PTR(user),
        RSTRING_PTR(pass),
        RSTRING_PTR(db),
        NUM2INT(port),
        NULL, // UNIX socket
        0)) { // flags
    RDO_ERROR("MySQL connection failed: %s", mysql_error(driver->conn));
  } else {
    driver->is_open = 1;
  }

  return Qtrue;
}

/** Return true if the connection is open */
static VALUE rdo_mysql_driver_open_p(VALUE self) {
  RDOMySQLDriver * driver;
  Data_Get_Struct(self, RDOMySQLDriver, driver);

  return driver->is_open ? Qtrue : Qfalse;
}

/** Close the connection to MySQL */
static VALUE rdo_mysql_driver_close(VALUE self) {
  RDOMySQLDriver * driver;
  Data_Get_Struct(self, RDOMySQLDriver, driver);

  mysql_close(driver->conn);
  driver->conn    = NULL;
  driver->is_open = 0;

  return Qtrue;
}

/** Quote values for safe insertion into a statement */
static VALUE rdo_mysql_driver_quote(VALUE self, VALUE obj) {
  if (TYPE(obj) != T_STRING) {
    obj = RDO_OBJ_TO_S(obj);
  }

  RDOMySQLDriver * driver;
  Data_Get_Struct(self, RDOMySQLDriver, driver);

  if (!(driver->is_open)) {
    RDO_ERROR("Unable to quote value: connection is closed");
  }

  char quoted[RSTRING_LEN(obj) * 2 + 1];

  mysql_real_escape_string(driver->conn,
      quoted, RSTRING_PTR(obj), RSTRING_LEN(obj));

  return rb_str_new2(quoted);
}

/** Execute a statement with possible bind parameters */
static VALUE rdo_mysql_driver_execute(int argc, VALUE * args, VALUE self) {
  if (argc < 1) {
    rb_raise(rb_eArgError, "wrong number of arguments (0 for 1)");
  }

  RDOMySQLDriver * driver;
  Data_Get_Struct(self, RDOMySQLDriver, driver);

  if (!(driver->is_open)) {
    RDO_ERROR("Cannot execute query: connection is not open");
  }

  VALUE stmt = RDO_INTERPOLATE(self, args, argc);

  if (mysql_real_query(driver->conn,
        RSTRING_PTR(stmt),
        RSTRING_LEN(stmt)) != 0) {
    RDO_ERROR("Failed to execute query: %s", mysql_error(driver->conn));
  }

  MYSQL_RES * res = mysql_store_result(driver->conn);

  return RDO_RESULT(rdo_mysql_tuple_list_new(res),
      rdo_mysql_result_info_new(driver->conn, res));
}

/** Initializer driver during extension initialization */
void Init_rdo_mysql_driver(void) {
  rb_require("rdo/mysql/driver");

  VALUE cMySQL = rb_path2class("RDO::MySQL::Driver");

  rb_define_alloc_func(cMySQL, rdo_mysql_driver_allocate);

  rb_define_method(cMySQL, "open", rdo_mysql_driver_open, 0);
  rb_define_method(cMySQL, "open?", rdo_mysql_driver_open_p, 0);
  rb_define_method(cMySQL, "close", rdo_mysql_driver_close, 0);
  rb_define_method(cMySQL, "quote", rdo_mysql_driver_quote, 1);
  rb_define_method(cMySQL, "execute", rdo_mysql_driver_execute, -1);

  Init_rdo_mysql_tuples();
}
