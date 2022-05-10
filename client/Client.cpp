#include "Client.hpp"

int current_time_nanoseconds()
{
	struct timespec tm;
	clock_gettime(CLOCK_REALTIME, &tm);
	return tm.tv_nsec;
}

double Client::get_random_value(std::string type) const
{
	double	d_val = 0.0;
	int 	delay = 0;

	boost::random::mt19937 defEngine(current_time_nanoseconds());
	if (type == "DOUBLE")
	{
		std::uniform_real_distribution<double> dblDistro(-90.0, 90.0); // uniform_real_distribution for double
		d_val = dblDistro(defEngine);
		return (d_val);
	}
	else
	{
		boost::random::uniform_int_distribution<int> dblDistro(5,30);
		delay = dblDistro(defEngine);
		std::cout << "Delay: " << delay << " sec" << std::endl;
		return (delay);
	}
}

void Client::start_messaging(std::string &host, std::string &port)
{
	io_context context;
	tcp::socket sock(context);

	// provides the ability to resolve a query to a list of endpoints
	tcp::resolver resolver_t(context);
	connect(sock, resolver_t.resolve(host, port));

	std::string rqst_msg;
	double x = 0.0;
	double y = 0.0;
	int delay = 0;
	while (1)
	{
		x = get_random_value("DOUBLE");
		delay = get_random_value("INTEGER");
		sleep(delay);
		y = get_random_value("DOUBLE");

//		rqst_msg = std::string("X:");
		rqst_msg = std::to_string(x);
		rqst_msg.append(" ");
		rqst_msg.append(std::to_string(y));
//		rqst_msg.append(" delay: ");
//		rqst_msg.append(std::to_string(delay));
		boost::asio::write(sock, boost::asio::buffer(rqst_msg, rqst_msg.length()));
//		rqst_msg.clear();

//		std::cout << "[Client] Enter a message: ";
//		char request[max_length];
//		std::cin.getline(request, max_length);
//		size_t request_length = std::strlen(request);
//		boost::asio::write(sock, boost::asio::buffer(request, request_length));

		char reply[max_length];
		size_t reply_length = boost::asio::read(sock, boost::asio::buffer(reply, 11));
		if (reply[reply_length - 1] != '\n')
			std::cout << "\n";
		std::cout << "[Client] Reply message is: ";
		std::cout.write(reply, reply_length);
		std::cout << "\n";
	}
}