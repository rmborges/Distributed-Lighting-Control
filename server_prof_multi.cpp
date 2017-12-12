#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <unistd.h>
#include "serial_send.h"

#define USB "/dev/ttyACM0"

using namespace boost::asio;
using ip::tcp;
using boost::asio::deadline_timer;

class tcp_connection :		public
boost::enable_shared_from_this<tcp_connection>	{
private:	
	tcp_connection(io_service&	io)	:
	socket_(io),
	heartbeat_timer_(io),
	deadline_(io),
	stopped_(false)
	{}


	void handle_write(const boost::system::error_code& error)
	{
		if (stopped_)
			return;

		if (!error)
		{
      		// Wait 10 seconds before sending the next heartbeat.
			heartbeat_timer_.expires_from_now(boost::posix_time::seconds(10));
			heartbeat_timer_.async_wait(boost::bind(&tcp_connection::start_write, this));
		}
		else
		{
			std::cout << "Error on heartbeat: " << error.message() << std::endl;

			stop();
		}
	}

	void handle_read(const boost::system::error_code& error)
	{
		if (stopped_)
			return;

		if (!error)
		{
			// Extract the newline-delimited message from the buffer.
			std::string line;
			std::istream is(&input_buffer_);
			std::getline(is, line);

      		// Empty messages are heartbeats and so ignored.
			if (!line.empty())
			{
				std::cout << "Received: " << line << "\n";
			}
			std::cout << "passei no deadline" << std::endl;
			start_read();
		}
		else
		{
			std::cout << "Error on receive: " << error.message() << "\n";

			stop();
		}
	}

	void check_deadline()
	{
		if (stopped_)
			return;

    // Check whether the deadline has passed. We compare the deadline against
    // the current time since a new asynchronous operation may have moved the
    // deadline before this actor had a chance to run.
		if (deadline_.expires_at() <= deadline_timer::traits_type::now())
		{
      // The deadline has passed. The socket is closed so that any outstanding
      // asynchronous operations are cancelled.
			socket_.close();

      // There is no longer an active deadline. The expiry is set to positive
      // infinity so that the actor takes no action until a new deadline is set.
			deadline_.expires_at(boost::posix_time::pos_infin);
		}

    // Put the actor back to sleep.
		deadline_.async_wait(boost::bind(&tcp_connection::check_deadline, this));
	}

	tcp::socket socket_;
	std::string msg_;
	enum {max_length=1024};
	char send_buffer_[max_length];
	deadline_timer heartbeat_timer_;
	boost::asio::streambuf input_buffer_;
	deadline_timer deadline_;
	bool stopped_;


public:	
	typedef boost::shared_ptr<tcp_connection>	pointer;
	static pointer create(io_service&	io)	{
		return pointer(new tcp_connection(io));
	}

	tcp::socket&	socket()	{return socket_;}

	  // This function terminates all the actors to shut down the connection. It
	  // may be called by the user of the client class, or by the class itself in
	  // response to graceful termination or an unrecoverable error.
	void stop()
	{
		stopped_ = true;
		boost::system::error_code ignored_ec;
		socket_.close(ignored_ec);
		deadline_.cancel();
		heartbeat_timer_.cancel();
	}
	
	// funçao que é chamada qd se cria nova conexão
	// fica à espera de pedido do client
	void start() {

		start_read();
		start_write();
		//deadline_.async_wait(boost::bind(&tcp_connection::check_deadline, this));

	}

	void start_write()	{
		if (stopped_)
			return;

    // SEM O SHARED_FROM_THIS
		std::cout << "enviei heartbeat" << std::endl;
    //boost::asio::async_write(socket_, boost::asio::buffer("\n", 1),
    //  boost::bind(&tcp_connection::handle_write, this, _1));

	// Start an asynchronous operation to send a heartbeat message.
		boost::asio::async_write(socket_, boost::asio::buffer("\n", 1),
			boost::bind(&tcp_connection::handle_write, shared_from_this(),
				boost::asio::placeholders::error));

		/*
		std::cout << "passei" << std::endl;
		std::string carla = "leva_natas";
		std::string terminated_line = carla + std::string("\n");
		std::size_t n = terminated_line.size();
		terminated_line.copy(send_buffer_, n);
		async_write(socket_,boost::asio::buffer(send_buffer_,n),
			boost::bind(&tcp_connection::handle_write,
				shared_from_this(),boost::asio::placeholders::error)); 

		*/
	}

	void start_read()	{
		/*
		async_read(socket_,buffer(msg_),
			boost::bind(&tcp_connection::handle_read,
				shared_from_this(),boost::asio::placeholders::error));
		*/
		/*async_read_until(socket_, input_buffer_, '\n',
			boost::bind(&tcp_connection::handle_read,
				shared_from_this(),boost::asio::placeholders::error));*/



	// Set a deadline for the read operation.
		deadline_.expires_from_now(boost::posix_time::seconds(30));

    // Start an asynchronous operation to read a newline-delimited message.
		boost::asio::async_read_until(socket_, input_buffer_, '\n',
			boost::bind(
				&tcp_connection::handle_read, shared_from_this(),
				boost::asio::placeholders::error));
	}
};

class tcp_server {
	public: tcp_server(io_service&	io)
	:	acceptor_(io,	tcp::endpoint(tcp::v4(),	10000))		{
		start_accept();
	}

private:	
	void start_accept()	{
		tcp_connection::pointer new_con =
		tcp_connection::create(acceptor_.get_io_service());
		acceptor_.async_accept(new_con->socket(),
			boost::bind(&tcp_server::handle_accept,	this,	
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
			std::cout << "error" << std::endl;
		}
	}

	tcp::acceptor acceptor_;
};



int main()		{
	try
	{
		io_service io;
		tcp_server server(io);
		serial_send serial_write(io);
		io.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception:  " << e.what() << std::endl;
	}

	return 0;
}