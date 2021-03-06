# RDO MySQL Driver

This is the MySQL driver for [RDO—Ruby Data Objects](https://github.com/d11wtq/rdo).

[![Build Status](https://secure.travis-ci.org/d11wtq/rdo-mysql.png?branch=master)](http://travis-ci.org/d11wtq/rdo-mysql)

Refer to the [RDO project README](https://github.com/d11wtq/rdo) for full
usage information.

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

## Type support

The following table lists the way MySQL types are mapped to Ruby types:

<table>
  <thead>
    <tr>
      <th>MySQL Type</th>
      <th>Ruby Type</th>
      <th>Notes</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <th>NULL</th>
      <td>NilClass</td>
      <td></td>
    </tr>
    <tr>
      <th>INT, TINYINT, SMALLINT, MEDIUMINT, BIGINT, INT</th>
      <td>Fixnum</td>
      <td>Ruby may use a Bignum of required</td>
    </tr>
    <tr>
      <th>CHAR, VARCHAR</th>
      <td>String</td>
      <td>The encoding specified on the connection is used</td>
    </tr>
    <tr>
      <th>TEXT, TINYTEXT, MEDIUMTEXT, LONGTEXT</th>
      <td>String</td>
      <td>The encoding specified on the connection is used</td>
    </tr>
    <tr>
      <th>BLOB, TINYBLOB, MEDIUMBLOB, LONGBLOB</th>
      <td>String</td>
      <td>The encoding is set to ASCII-8BIT/BINARY</td>
    </tr>
    <tr>
      <th>BINARY, VARBINARY</th>
      <td>String</td>
      <td>The encoding is set to ASCII-8BIT/BINARY</td>
    </tr>
    <tr>
      <th>FLOAT, DOUBLE</th>
      <td>Float</td>
      <td></td>
    </tr>
    <tr>
      <th>DECIMAL/NUMERIC</th>
      <td>BigDecimal</td>
      <td></td>
    </tr>
    <tr>
      <th>DATE</th>
      <td>Date</td>
      <td></td>
    </tr>
    <tr>
      <th>DATETIME, TIMESTAMP</th>
      <td>DateTime</td>
      <td>MySQL does not store or return a time zone; the system time zone is used</td>
    </tr>
    <tr>
      <th>SET</th>
      <td>Set</td>
      <td>MySQL does not allow values containing commas to be included in a SET</td>
    </tr>
  </tbody>
</table>

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
request from the master branch of your fork.

Contributors will be credited in this README.

## Copyright & Licensing

Written by Chris Corbyn.

Licensed under the MIT license. See the LICENSE file for full details.
