#include <iostream>
#include <algorithm>
#include <vector>

#include "adventofcode.h"
#include "util.h"
#include "graph.h"

#ifdef TEST
#include <catch2/catch.hpp>
#include <sstream>
#endif

int64_t day10_1(std::istream &file)
{
	auto numbers = read_numbers(file);
	std::sort(numbers.begin(), numbers.end());
	int jolt1 = 0, jolt3 = 1; // always include last adapter to device
	int prev = 0;			  // start at outlet
	for (int i = 0; i < (int)numbers.size(); i++)
	{
		int diff = numbers[i] - prev;
		if (diff == 1)
			jolt1++;
		if (diff == 3)
			jolt3++;
		prev = numbers[i];
	}
	return jolt1 * jolt3;
}

// recursively count all possible routes to destination
long foobar(const std::vector<int> &numbers, int src, int dest)
{
	if (src == dest)
		return 1;
	long count = 0;
	for (int i = src + 1; i < (int)numbers.size() && i < src + 4; i++)
	{
		if (numbers[i] - numbers[src] > 3)
			break;
		count += foobar(numbers, i, dest);
	}
	return count;
}

int64_t day10_2(std::istream &file)
{
	auto numbers = read_numbers(file);
	// sort numbers
	std::sort(numbers.begin(), numbers.end());

	// add outlet at 0 jolts
	numbers.insert(numbers.begin(), 0);

	// add device at max+3 jolts
	int max = *std::max_element(numbers.begin(), numbers.end());
	numbers.push_back(max + 3);

	// detect all indices with a 3-jolt jump
	std::vector<int> jolt3;
	for (int i = 0; i < (int)numbers.size() - 1; i++)
		if (numbers[i + 1] - numbers[i] == 3)
			jolt3.push_back(i);

	// find the number of arrangements between each 3-jolt jump,
	// and then multiply all of the arrangements together
	long answer = 1;
	int prev = 0;
	for (int &curr : jolt3)
	{
		answer *= foobar(numbers, prev, curr);
		prev = curr;
	}

	return answer;
}

#ifdef TEST
TEST_CASE("Day10 part1", "[day10]")
{
	std::istringstream iss(R"(28
33
18
42
31
14
46
20
48
47
24
23
49
45
19
38
39
11
1
32
25
35
8
17
7
9
4
2
34
10
3)");
	REQUIRE(day10_1(iss) == 220);
}

// resetting the stringstream wasn't working, so I duplicated the test code
TEST_CASE("Day10 part2", "[day10]")
{
	std::istringstream iss1(R"(16
10
15
5
1
11
7
19
6
12
4)");
	std::istringstream iss2(R"(28
33
18
42
31
14
46
20
48
47
24
23
49
45
19
38
39
11
1
32
25
35
8
17
7
9
4
2
34
10
3)");
	REQUIRE(day10_2(iss1) == 8);
	REQUIRE(day10_2(iss2) == 19208);
}
#endif
