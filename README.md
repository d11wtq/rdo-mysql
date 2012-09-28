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
