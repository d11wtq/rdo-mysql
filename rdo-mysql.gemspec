# -*- encoding: utf-8 -*-
require File.expand_path('../lib/rdo/mysql/version', __FILE__)

Gem::Specification.new do |gem|
  gem.authors       = ["d11wtq"]
  gem.email         = ["chris@w3style.co.uk"]
  gem.description   = "Provides access to MySQL using the RDO interface"
  gem.summary       = "MySQL Driver for RDO"
  gem.homepage      = "https://github.com/d11wtq/rdo-mysql"

  gem.files         = `git ls-files`.split($\)
  gem.executables   = gem.files.grep(%r{^bin/}).map{ |f| File.basename(f) }
  gem.test_files    = gem.files.grep(%r{^(test|spec|features)/})
  gem.name          = "rdo-mysql"
  gem.require_paths = ["lib"]
  gem.version       = RDO::MySQL::VERSION
  gem.extensions    = ["ext/rdo_mysql/extconf.rb"]

  gem.add_runtime_dependency "rdo", "~> 0.1.0"

  gem.add_development_dependency "rspec"
  gem.add_development_dependency "rake-compiler"
end
