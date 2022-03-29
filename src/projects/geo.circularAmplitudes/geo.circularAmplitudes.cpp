/// @file
///	@ingroup 	minexamples
///	@copyright	Copyright 2018 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

// core
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

// dependencies
#include "c74_min.h"
#include "geometry.hpp"

namespace c = c74::min;
namespace g = geometry;

class circularAmplitudes : public c::object<circularAmplitudes> {
public:
	MIN_DESCRIPTION	{"Calculate the modal amplitudes relative to a strike location in polar coordinates."};
	MIN_TAGS		{""};
	MIN_AUTHOR		{"Lewis Wolf"};
	MIN_RELATED		{"geo.circularAmplitudes"};

	c::inlet<>  in1	{ this, "(float) the radial component of the circular strike location" };
	c::inlet<>  in2	{ this, "(float) the theta component of the circular strike location" };
	c::outlet<> out	{ this, "(float) output the modal amplitudes" };


	c::argument<int> set_N { this, "N", "Update the maximum Nth order of the modes.",
		MIN_ARGUMENT_FUNCTION { 
			N = arg;
			series = g::calculateCircularSeries(N, M);
		}
	};
	c::attribute<int> N { this, "N", 10,
		c::description {"The maximum Nth order of the modes."}
	};

	c::argument<int> set_M { this, "M", "Update the amount of modes per order.",
		MIN_ARGUMENT_FUNCTION { 
			M = arg;
			series = g::calculateCircularSeries(N, M);
		}
	};
	c::attribute<int> M { this, "M", 10,
		c::description {"The amount of modes per order."}
	};


	c::message<> number { this, "number", "Calculate the modal amplitudes.",
		MIN_FUNCTION {
			double angular;
			c::atoms amplitudes(N * M);
			switch (inlet) {
				case 0:
					r = c::from_atoms<std::vector<double>>(args)[0];
					for (unsigned int n = 0; n < N; n++) {
						angular = n != 0 ? M_SQRT2 * sin(n * theta + M_PI_4) : 1.0;
						for (unsigned int m = 0; m < M; m++) {
							amplitudes[n * M + m] = g::besselJ(n, series[n][m] + r) * angular;
						};	
					}
					out.send(amplitudes);
					break;
				case 1:
					theta = c::from_atoms<std::vector<double>>(args)[0];
					break;
				default:
					break;
			}
			return {};
		}
	};


private:
	std::vector<std::vector<double>> series = g::calculateCircularSeries(N, M);
	double r;
	double theta;
};

MIN_EXTERNAL(circularAmplitudes);