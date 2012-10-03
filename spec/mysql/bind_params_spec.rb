require "spec_helper"
require "date"

describe RDO::MySQL::Driver, "bind params" do
  let(:options)    { connection_uri }
  let(:connection) { RDO.connect(options) }

  let(:table) do
    "CREATE TABLE test (id INT NOT NULL PRIMARY KEY AUTO_INCREMENT)"
  end

  let(:tuple) { connection.execute("SELECT * FROM test").first }

  before(:each) do
    connection.execute("DROP TABLE IF EXISTS test")
    connection.execute(table)
    connection.execute(*insert)
  end

  after(:each) do
    begin
      connection.execute("DROP TABLE IF EXISTS test")
    ensure
      connection.close
    end
  end

  describe "String param" do
    context "against a varchar field" do
      let(:table) do
        <<-SQL
        CREATE TABLE test (
          id    INT PRIMARY KEY AUTO_INCREMENT,
          value VARCHAR(32)
        )
        SQL
      end
      let(:insert) { ["INSERT INTO test (value) VALUES (?)", "bob"] }

      it "is inferred correctly" do
        tuple.should == {id: 1, value: "bob"}
      end
    end

    context "against a char field" do
      let(:table) do
        <<-SQL
        CREATE TABLE test (
          id    INT PRIMARY KEY AUTO_INCREMENT,
          value CHAR(3)
        )
        SQL
      end
      let(:insert) { ["INSERT INTO test (value) VALUES (?)", "bob"] }

      it "is inferred correctly" do
        tuple.should == {id: 1, value: "bob"}
      end
    end

    context "against a text field" do
      let(:table) do
        <<-SQL
        CREATE TABLE test (
          id    INT PRIMARY KEY AUTO_INCREMENT,
          value TEXT
        )
        SQL
      end
      let(:insert) { ["INSERT INTO test (value) VALUES (?)", "bob"] }

      it "is inferred correctly" do
        tuple.should == {id: 1, value: "bob"}
      end
    end

    context "against a blob field" do
      let(:table) do
        <<-SQL
        CREATE TABLE test (
          id    INT PRIMARY KEY AUTO_INCREMENT,
          value BLOB
        )
        SQL
      end
      let(:insert) { ["INSERT INTO test (value) VALUES (?)", "bob"] }

      it "is inferred correctly" do
        tuple.should == {id: 1, value: "bob"}
      end
    end

    context "against an integer field" do
      let(:table) do
        <<-SQL
        CREATE TABLE test (
          id    INT PRIMARY KEY AUTO_INCREMENT,
          value INT
        )
        SQL
      end
      let(:insert) { ["INSERT INTO test (value) VALUES (?)", "42"] }

      it "is inferred correctly" do
        tuple.should == {id: 1, value: 42}
      end
    end

    context "against a float field" do
      let(:table) do
        <<-SQL
        CREATE TABLE test (
          id    INT PRIMARY KEY AUTO_INCREMENT,
          value FLOAT
        )
        SQL
      end
      let(:insert) { ["INSERT INTO test (value) VALUES (?)", "42.4"] }

      it "is inferred correctly" do
        tuple.should == {id: 1, value: 42.4}
      end
    end

    context "against a date field" do
      let(:table) do
        <<-SQL
        CREATE TABLE test (
          id    INT PRIMARY KEY AUTO_INCREMENT,
          value DATE
        )
        SQL
      end
      let(:insert) { ["INSERT INTO test (value) VALUES (?)", "2012-10-03"] }

      it "is inferred correctly" do
        tuple.should == {id: 1, value: Date.new(2012, 10, 3)}
      end
    end

    context "against a datetime field" do
      let(:table) do
        <<-SQL
        CREATE TABLE test (
          id    INT PRIMARY KEY AUTO_INCREMENT,
          value DATETIME
        )
        SQL
      end
      let(:insert) { ["INSERT INTO test (value) VALUES (?)", "2012-10-03 00:09:36"] }

      it "is inferred correctly" do
        tuple.should == {id: 1, value: DateTime.new(2012, 10, 3, 0, 9, 36, DateTime.now.zone)}
      end
    end

    context "against a timestamp field" do
      let(:table) do
        <<-SQL
        CREATE TABLE test (
          id    INT PRIMARY KEY AUTO_INCREMENT,
          value TIMESTAMP
        )
        SQL
      end
      let(:insert) { ["INSERT INTO test (value) VALUES (?)", "2012-10-03 00:09:36"] }

      it "is inferred correctly" do
        tuple.should == {id: 1, value: DateTime.new(2012, 10, 3, 0, 9, 36, DateTime.now.zone)}
      end
    end
  end
end