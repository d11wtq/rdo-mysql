##
# RDO MySQL driver.
# Copyright © 2012 Chris Corbyn.
#
# See LICENSE file for details.
##

require "rdo"
require "rdo/mysql/version"
require "rdo/mysql/driver"

# c extension
require "rdo_mysql/rdo_mysql"

RDO::Connection.register_driver(:mysql, RDO::MySQL::Driver)
