#include "PIDSequence.hpp"

PIDSequence::PIDSequence(const std::string& seed)
{
	std::seed_seq seedSeq(seed.begin(), seed.end());
	generator_.seed(seedSeq);
}
