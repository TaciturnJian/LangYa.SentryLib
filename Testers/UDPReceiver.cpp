#include <iostream>
#include <boost/asio.hpp>

#include "LangYa/SentryLib/Common/MemoryView.hpp"

int main()
{
	boost::asio::io_service io_service{};

	boost::asio::ip::udp::socket socket{io_service, {boost::asio::ip::udp::v4(), 8989}};

	char buffer[9]{0};

	while (true)
	{
		std::cout << "<";
		auto bytes = socket.receive(boost::asio::buffer(buffer, 8));
		LangYa::SentryLib::MemoryView view{buffer, bytes};
		std::cout << view << '>' << std::endl;
	}
}
