require "spec_helper"
require "bigdecimal"
require "date"
require "uri"

describe RDO::MySQL::Driver, "type casting" do
  let(:options)    { connection_uri }
  let(:connection) { RDO.connect(options) }
  let(:value)      { connection.execute(sql).first_value }

  describe "null cast" do
    let(:sql) { "SELECT NULL" }

    it "returns nil" do
      value.should be_nil
    end
  end

  describe "varchar cast" do
    before(:each) do
      connection.execute("CREATE TEMPORARY TABLE test (v VARCHAR(16))")
      connection.execute("INSERT INTO test (v) VALUES ('bob')")
    end

    let(:sql) { "SELECT v FROM test" }

    it "returns a String" do
      value.should == "bob"
    end

    context "with utf-8 encoding" do
      let(:options) { URI.parse(connection_uri).tap{|u| u.query = "encoding=utf-8"}.to_s }

      it "has the correct encoding" do
        value.encoding.should == Encoding.find("utf-8")
      end
    end

    context "with iso-8859-1 encoding" do
      let(:options) { URI.parse(connection_uri).tap{|u| u.query = "encoding=iso-8859-1"}.to_s }

      it "has the correct encoding" do
        value.encoding.should == Encoding.find("iso-8859-1")
      end
    end
  end

  describe "char cast" do
    before(:each) do
      connection.execute("CREATE TEMPORARY TABLE test (v CHAR(4))")
      connection.execute("INSERT INTO test (v) VALUES ('bobby')")
    end

    let(:sql) { "SELECT v FROM test" }

    it "returns a String" do
      value.should == "bobb"
    end

    context "with utf-8 encoding" do
      let(:options) { URI.parse(connection_uri).tap{|u| u.query = "encoding=utf-8"}.to_s }

      it "has the correct encoding" do
        value.encoding.should == Encoding.find("utf-8")
      end
    end

    context "with iso-8859-1 encoding" do
      let(:options) { URI.parse(connection_uri).tap{|u| u.query = "encoding=iso-8859-1"}.to_s }

      it "has the correct encoding" do
        value.encoding.should == Encoding.find("iso-8859-1")
      end
    end
  end

  describe "text cast" do
    before(:each) do
      connection.execute("CREATE TEMPORARY TABLE test (v TEXT)")
      connection.execute("INSERT INTO test (v) VALUES ('bobby')")
    end

    let(:sql) { "SELECT v FROM test" }

    it "returns a String" do
      value.should == "bobby"
    end

    context "with utf-8 encoding" do
      let(:options) { URI.parse(connection_uri).tap{|u| u.query = "encoding=utf-8"}.to_s }

      it "has the correct encoding" do
        value.encoding.should == Encoding.find("utf-8")
      end
    end

    context "with iso-8859-1 encoding" do
      let(:options) { URI.parse(connection_uri).tap{|u| u.query = "encoding=iso-8859-1"}.to_s }

      it "has the correct encoding" do
        value.encoding.should == Encoding.find("iso-8859-1")
      end
    end
  end

  describe "binary cast" do
    before(:each) do
      connection.execute("CREATE TEMPORARY TABLE test (v BINARY(3))")
      connection.execute("INSERT INTO test (v) VALUES (?)", "\x00\x11\x22")
    end

    let(:sql) { "SELECT v FROM test" }

    it "returns a String" do
      value.should == "\x00\x11\x22"
    end

    it "has binary encoding" do
      value.encoding.should == Encoding.find("binary")
    end
  end

  describe "blob cast" do
    before(:each) do
      connection.execute("CREATE TEMPORARY TABLE test (v BLOB)")
      connection.execute("INSERT INTO test (v) VALUES (?)", "\x00\x11\x22\x33\x44")
    end

    let(:sql) { "SELECT v FROM test" }

    it "returns a String" do
      value.should == "\x00\x11\x22\x33\x44"
    end

    it "has binary encoding" do
      value.encoding.should == Encoding.find("binary")
    end
  end

  describe "integer cast" do
    let(:sql) { "SELECT 1234" }

    it "returns a Fixnum" do
      value.should == 1234
    end
  end

  describe "float cast" do
    before(:each) do
      connection.execute("CREATE TEMPORARY TABLE test (n FLOAT)")
      connection.execute("INSERT INTO test (n) VALUES (12.34)")
    end

    let(:sql) { "SELECT n FROM test" }

    it "returns a Float" do
      value.should == 12.34
    end
  end

  describe "decimal cast" do
    before(:each) do
      connection.execute("CREATE TEMPORARY TABLE test (n DECIMAL(6,2))")
      connection.execute("INSERT INTO test (n) VALUES ('1234.56')")
    end

    let(:sql) { "SELECT n FROM test" }

    it "returns a BigDecimal" do
      value.should == BigDecimal("1234.56")
    end
  end

  describe "date cast" do
    before(:each) do
      connection.execute("CREATE TEMPORARY TABLE test (d DATE)")
      connection.execute("INSERT INTO test (d) VALUES ('2012-09-30')")
    end

    let(:sql) { "SELECT d FROM test" }

    it "returns a Date" do
      value.should == Date.new(2012, 9, 30)
    end
  end

  describe "datetime cast" do
    before(:each) do
      connection.execute("CREATE TEMPORARY TABLE test (d DATETIME)")
      connection.execute("INSERT INTO test (d) VALUES ('2012-09-30 19:04:36')")
    end

    let(:sql) { "SELECT d FROM test" }

    it "returns a DateTime in the system time zone" do
      value.should == DateTime.new(2012, 9, 30, 19, 4, 36, DateTime.now.zone)
    end
  end

  describe "timestamp cast" do
    before(:each) do
      connection.execute("CREATE TEMPORARY TABLE test (d TIMESTAMP)")
      connection.execute("INSERT INTO test (d) VALUES ('2012-09-30 19:04:36')")
    end

    let(:sql) { "SELECT d FROM test" }

    it "returns a DateTime in the system time zone" do
      value.should == DateTime.new(2012, 9, 30, 19, 4, 36, DateTime.now.zone)
    end
  end
end
