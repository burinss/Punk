#pragma once

#include <random>
#include <string>

class PIDSequence
{
private:

	std::mt19937 generator_;

public:

	PIDSequence() {}
	PIDSequence(const std::string& seed);
	~PIDSequence() {}

	// TODO: Use a random number generator to sample from a
	// list of tokens and use a few tokens as the prefix
	std::string Next() { return std::to_string(generator_() % 100000); }

protected:

};