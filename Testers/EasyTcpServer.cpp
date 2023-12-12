#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;

int main()
{
	io_service ios;
	ip::tcp::acceptor acp{ios, {ip::make_address("127.0.0.1"), 8989}};
	acp.listen();
	while (true)
	{
		try {
			auto client = acp.accept();

			char buffer[2]{0};
			std::vector<char> dynamic_buffer{};
			int package_id{0};
			while (true)
			{
				if (read(client, boost::asio::buffer(buffer, 1)) < 1)
				{
					continue;
				}

				if (buffer[0] == '!')
				{
					std::cout << '[' << package_id++ << ']';
					for (auto& c: dynamic_buffer)
					{
						std::cout << static_cast<int>(c);
					}
					std::cout << std::endl;
					dynamic_buffer.clear();
				}
				dynamic_buffer.push_back(buffer[0]);
			}
		}
		catch (...)
		{
			std::cout << "Exception in client connection, closed and ready for next.";
		}
	}

}