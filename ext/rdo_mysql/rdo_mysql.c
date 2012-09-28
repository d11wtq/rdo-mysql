/*
 * RDO MySQL Driver.
 * Copyright © 2012 Chris Corbyn.
 *
 * See LICENSE file for details.
 */

#include <ruby.h>
#include "driver.h"

/** Extension initializer, called when .so is loaded */
void Init_rdo_mysql(void) {
  rb_require("rdo");
  Init_rdo_mysql_driver();
}
