require "mkmf"

if ENV["CC"]
  RbConfig::MAKEFILE_CONFIG["CC"] = ENV["CC"]
end

def config_value(type, flag)
  IO.popen("mysql_config --#{type}").
    readline.chomp.
    split(/\s+/).select{|s| s =~ /#{flag}/}.
    map{|s| s.sub(/^#{flag}/, "")}.uniq
rescue
  Array[]
end

def have_build_env
  [
    have_header("mysql.h"),
    (p config_value("libs", "-l")).all?{|lib| have_library(lib)}
  ].all?
end

dir_config(
  "mysqlclient",
  p config_value("include", "-I"),
  p config_value("libs", "-L")
)

unless have_build_env
  puts "Unable to find mysqlclient libraries and headers. Not building."
  exit(1)
end

create_makefile("rdo_mysql/rdo_mysql")
