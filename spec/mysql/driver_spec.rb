require "spec_helper"
require "uri"

describe RDO::MySQL::Driver do
  let(:options)    { connection_uri }
  let(:connection) { RDO::Connection.new(options) }

  after(:each) { connection.close rescue nil }

  describe "#initialize" do
    context "with valid settings" do
      it "opens a connection to the server" do
        connection.should be_open
      end
    end

    context "with invalid settings" do
      let(:options) { URI.parse(connection_uri).tap{|u| u.user = "bad_user"}.to_s }

      it "raises an RDO::Exception" do
        expect { connection }.to raise_error(RDO::Exception)
      end

      it "provides a meaningful error" do
        begin
          connection && fail("RDO::Exception should be raised")
        rescue RDO::Exception => e
          e.message.should =~ /mysql.*?bad_user/i
        end
      end
    end
  end

  describe "#close" do
    it "closes the connection to the server" do
      connection.close
      connection.should_not be_open
    end

    it "returns true" do
      connection.close.should == true
    end

    context "called multiple times" do
      it "has no negative side-effects" do
        5.times { connection.close }
        connection.should_not be_open
      end
    end
  end

  describe "#open" do
    it "opens a connection to the server" do
      connection.close && connection.open
      connection.should be_open
    end

    it "returns true" do
      connection.close
      connection.open.should == true
    end

    context "called multiple times" do
      before(:each) { connection.close }

      it "has no negative side-effects" do
        5.times { connection.open }
        connection.should be_open
      end
    end
  end
end
