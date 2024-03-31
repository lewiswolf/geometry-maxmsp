// dependencies
#include "c74_min.h"
#include <kac_core.hpp>

namespace c = c74::min;
namespace p = kac_core::physics;
namespace T = kac_core::types;

class rectangularSeries: public c::object<rectangularSeries> {
	public:
	MIN_DESCRIPTION {"Calculate the eigenmodes of a rectangle."};
	MIN_TAGS {""};
	MIN_AUTHOR {"Lewis Wolstanholme"};
	MIN_RELATED {"kac.rectangularAmplitudes"};

	c::inlet<> in {this, "(bang) calculate the rectangular eigenmodes."};
	c::outlet<> out {this, "(list) output the rectangular eigenmodes."};

	c::attribute<long> N {
		this,
		"N",
		10,
		c::title {"Nth Order"},
		c::description {"The maximum Nth order of the modes. (0, ∞]"}
	};
	c::attribute<long> M {
		this,
		"M",
		10,
		c::title {"Modes per Order"},
		c::description {"The maximum amount of modes per order. (0, ∞]"}
	};
	c::attribute<double> epsilon {
		this,
		"epsilon",
		1.0,
		c::title {"Aspect Ratio"},
		c::description {"The aspect ratio of the rectangle."}
	};

	c::message<> bang {
		this,
		"bang",
		"Calculate the rectangular eigenmodes.",
		[this](const c74::min::atoms& args, const int inlet) -> c74::min::atoms {
			// calculate rectangular eigenmodes
			T::Matrix_2D series_old = p::rectangularSeries(N, M, epsilon);
			c::atoms series(N * M);
			for (unsigned long n = 0; n < N; n++) {
				for (unsigned long m = 0; m < M; m++) { series[n * M + m] = series_old[n][m]; };
			};
			out.send(series);
			return {};
		}
	};
};

MIN_EXTERNAL(rectangularSeries);
