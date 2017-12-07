#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <unistd.h>

using namespace boost::asio;
using namespace std;
using ip::tcp;
using boost::asio::deadline_timer;

class tcp_connection :    public boost::enable_shared_from_this<tcp_connection>  {
private:  
	tcp::socket sock_;
	std::string msg_;
	boost::asio::streambuf input_buffer_;
	deadline_timer deadline_;
	char send_buffer_[1024];
	bool stopped_;
	tcp_connection(io_service&  io) :   sock_(io)   {}
	void handle_write(const boost::system::error_code& error)     
	{ 

	}

public: 
	typedef boost::shared_ptr<tcp_connection> pointer;

	static pointer create(io_service& io) {
		return pointer(new tcp_connection(io));
	}

	tcp::socket&  socket()  {return sock_;}

	
	
// --------------------------------------------------

	void stop(){
		stopped_  = true;
		boost::system::error_code ignored_ec;
		sock_.close(ignored_ec);
		deadline_.cancel();
		//heartbeat_timer_.cancel();
	}

	void start()
	{
    // Set a deadline for the read operation.
		deadline_.expires_from_now(boost::posix_time::seconds(30));

    // Start an asynchronous operation to read a newline-delimited message.
		async_read_until(sock_, input_buffer_, '\n',
			boost::bind(&tcp_connection::handle_read, this, _1));
	}

	void handle_read(const boost::system::error_code& ec)
	{
		if (stopped_)
			return;

		if (!ec)
		{
      // Extract the newline-delimited message from the buffer.
			string line;
			istream is(&input_buffer_);
			getline(is, line);

      // Empty messages are heartbeats and so ignored.
			if (!line.empty())
			{
				cout << "Received: " << line << endl;
			}

			start();
		}
		else
		{
			cout << "Error on receive: " << ec.message() << endl;

			stop();
		}
	}


// --------------------------------------------------


		/*void start()  {
		async_read(sock_,buffer(carla,carla.size()),
			boost::bind(&tcp_connection::handle_write,
				shared_from_this(),boost::asio::placeholders::error));

cout << "passei" << endl;
		string carla = "leva_natas\n";
		async_write(sock_,buffer(carla,carla.size()),
			boost::bind(&tcp_connection::handle_write,
				shared_from_this(),boost::asio::placeholders::error)); 


	}*/
};



class tcp_server {
	private:    tcp::acceptor acceptor_;
	
	public:	tcp_server(io_service&  io)
	: acceptor_(io, tcp::endpoint(tcp::v4(),  10000))   {
		start_accept();
	}

private:  
	void start_accept() {
		tcp_connection::pointer new_con =
		tcp_connection::create(acceptor_.get_io_service());
		acceptor_.async_accept(new_con->socket(),
			boost::bind(&tcp_server::handle_accept, this, 
				new_con, boost::asio::placeholders::error));
	}

	void handle_accept(tcp_connection::pointer new_con,
		const boost::system::error_code& error)   
	{
		if (!error)
		{
			new_con->start();
			start_accept();
		}else{
			cout << "error" << endl;
		}

	}

};


int main()	{
	try
	{
		io_service io;
		tcp_server server(io);
		io.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;

}