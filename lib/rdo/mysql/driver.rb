##
# RDO MySQL driver.
# Copyright © 2012 Chris Corbyn.
#
# See LICENSE file for details.
##

module RDO
  module MySQL
    # Principal RDO driver class for MySQL.
    class Driver < RDO::Driver
      # implementation defined by ext/rdo_mysql/driver.c

      private

      %w[host user password database].each do |key|
        define_method(key) { options[key.intern].to_s }
      end

      def port
        options[:port].to_i
      end
    end
  end
end
