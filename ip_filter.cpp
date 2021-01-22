#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using ipPool = std::vector<std::vector<std::string> >;

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
auto split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    auto stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

void printIpPool(const ipPool& ip_pool)
{
    for(auto ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
    {
        for(auto ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
        {
            if (ip_part != ip->cbegin())
            {
                std::cout << ".";
            }
	    
            std::cout << *ip_part;
        }

        std::cout << std::endl;
    }
}

int main()
{
    try
    {
        ipPool ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            auto v = split(line, '\t');
            ip_pool.push_back(split(v.at(0), '.'));
        }

	auto compare = [](const std::vector<std::string>& ip_left,
			const std::vector<std::string>& ip_right) -> bool
	{
		for(auto left_part = ip_left.cbegin(),  right_part = ip_right.cbegin();
				left_part != ip_left.cend(); ++left_part, ++right_part)
		{
			int res = left_part->length() - right_part->length();
			if(res != 0)
				return res > 0 ? true : false;
			for(auto left_digit = left_part->cbegin(), right_digit = right_part->cbegin();
				       	left_digit != left_part->cend(); ++left_digit, ++right_digit)
			{
				int res = *left_digit - *right_digit;
				if(res != 0)
					return res > 0 ? true : false;
			}
		}
		return false;
	};

	std::sort(ip_pool.begin(), ip_pool.end(), compare);
	printIpPool(ip_pool);
	class Filter
	{
		public:

			Filter(const ipPool& pool) : ip_pool(pool)
			{}

			auto operator()(int num)
			{
				ipPool pool;
				for(auto& ip : ip_pool)
					if(ip.at(0) == std::to_string(num))
						pool.push_back(ip);
				return pool;
			}

			auto operator()(int first, int second)
			{
				ipPool pool;
				for(auto& ip : ip_pool)
					if(ip.at(0) == std::to_string(first) &&
			   	   	   ip.at(1) == std::to_string(second))
						pool.push_back(ip);
				return pool;
			}

		private:
			const ipPool& ip_pool;
	};
	Filter filter(ip_pool);
	auto ip = filter(1);
	printIpPool(ip);
	ip = filter(46, 70);
	printIpPool(ip);

	auto filter_any = [&ip_pool](int num)
	{
		ipPool pool;
		for(auto& ip : ip_pool)
			for(auto& oneByte : ip)
				if(oneByte == std::to_string(num))
				{
					pool.push_back(ip);
					break;
				}
		return pool;
	};
	ip = filter_any(46);
	printIpPool(ip);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
