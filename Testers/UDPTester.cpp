#include <boost/asio.hpp>


int main(int argc, const char* argv[])
{
	boost::asio::io_service io_service{};

	boost::asio::ip::udp::socket socket{io_service};

	boost::asio::ip::udp::endpoint ep{boost::asio::ip::make_address("127.0.0.1"), 8989};
	socket.open(boost::asio::ip::udp::v4());
	char data[9] = "12345678";

	socket.send_to(boost::asio::buffer(data, 8), ep);
	return 0;
}