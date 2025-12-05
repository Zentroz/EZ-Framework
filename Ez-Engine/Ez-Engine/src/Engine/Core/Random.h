#ifndef RANDOM_CLASS_H
#define RANDOM_CLASS_H

#include<random>

namespace CORE {
	class Random {
	public:
		static double Number() {
			std::uniform_real_distribution<double> dist(0.0, 1.0);
			return dist(rng);
		}

		static double Range(double min, double max) {
			std::uniform_real_distribution<double> dist(min, max);
			return dist(rng);
		}
	private:
		static std::default_random_engine rng;
	};
}

#endif