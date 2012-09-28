require "bundler/gem_tasks"
require "rspec/core/rake_task"
require "rake/extensiontask"

Rake::ExtensionTask.new('rdo_mysql') do |ext|
  ext.lib_dir = File.join('lib', 'rdo_mysql')
end

desc "Run the full RSpec suite"
RSpec::Core::RakeTask.new('spec') do |t|
  t.pattern     = 'spec/'
end

Rake::Task['spec'].prerequisites << :compile
