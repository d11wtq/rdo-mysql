# RDO MySQL Driver

This is the MySQL driver for [RDO—Ruby Data Objects](https://github.com/d11wtq/rdo).

Refer to the RDO project [README](https://github.com/d11wtq/rdo) for full usage information.

## Installation

Via rubygems:

    $ gem install rdo-mysql

Or add this line to your application's Gemfile:

    gem "rdo-mysql"

And then execute:

    $ bundle

## Usage

``` ruby
require "rdo-mysql"

conn = RDO.connect("mysql://user:pass@localhost/db_name?encoding=utf-8")
```

## Prepared statements

MySQL, in theory supports prepared statements. But really you would not want
to use them. There are still a number of known limitations with MySQL prepared
statements, as outlined here:

  - http://dev.mysql.com/doc/refman/5.0/en/c-api-prepared-statement-problems.html
  - http://dev.mysql.com/doc/refman/5.1/en/c-api-prepared-statement-problems.html
  - http://dev.mysql.com/doc/refman/5.5/en/c-api-prepared-statement-problems.html

rdo-mysql uses RDO's emulated prepared statement support instead. If people
shout loud enough that they'd prefer to use MySQL's problematic prepared
statements anyway, I'll implement them.

## Contributing

If you find any bugs, please send a pull request if you think you can
fix it, or file in an issue in the issue tracker.

When sending pull requests, please use topic branches—don't send a pull
request from the master branch of your fork, as that may change
unintentionally.

Contributors will be credited in this README.

## Copyright & Licensing

Written by Chris Corbyn.

Licensed under the MIT license. See the LICENSE file for full details.
test
