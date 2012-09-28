require "rdo"
require "rdo/mysql"

ENV["CONNECTION"] ||= "mysql://rdo:rdo@localhost/rdo?encoding=utf-8"

RSpec.configure do |config|
  def connection_uri
    ENV["CONNECTION"]
  end
end
