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

      def after_open
        set_time_zone
        set_encoding
      end

      def set_time_zone
        execute(
          "SET time_zone = ?",
          options.fetch(:time_zone, RDO::Util.system_time_zone)
        )
      end

      def set_encoding
        execute("SET NAMES ?", charset_name(encoding))
      end

      def encoding
        options.fetch(:encoding, 'utf8')
      end

      # mysql uses a restrictive syntax for setting the encoding
      def charset_name(name)
        case name.to_s.downcase
        when "utf-8"
          "utf8"
        when /^iso-8859-[0-9]+$/
          name.to_s.gsub(/^.*?-([0-9]+)$/, "latin\\1")
        else
          name
        end
      end
    end
  end
end
