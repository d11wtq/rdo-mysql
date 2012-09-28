/*
 * RDO MySQL Driver.
 * Copyright © 2012 Chris Corbyn.
 *
 * See LICENSE file for details.
 */

#include "driver.h"
#include <stdio.h>
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

/** Initializer driver during extension initialization */
void Init_rdo_mysql_driver(void) {
  rb_require("rdo/mysql/driver");

  VALUE cMySQL = rb_path2class("RDO::MySQL::Driver");

  rb_define_alloc_func(cMySQL, rdo_mysql_driver_allocate);

  rb_define_method(cMySQL, "open", rdo_mysql_driver_open, 0);
  rb_define_method(cMySQL, "open?", rdo_mysql_driver_open_p, 0);
  rb_define_method(cMySQL, "close", rdo_mysql_driver_close, 0);
}

