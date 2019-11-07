////////////////////////////////////////////////////////////
//
// Copyright (c) 2018 Jan Filipowicz, Filip Turobos
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
////////////////////////////////////////////////////////////

#ifndef GENETIC_ALGORITHM_LIBRARY_DEFAULT_LOGGER_H
#define GENETIC_ALGORITHM_LIBRARY_DEFAULT_LOGGER_H

#include <ostream>
#include <vector>

template<class CharT, class Traits>
class default_logger {
public:
	using ostream_type = std::basic_ostream<CharT, Traits>;
	explicit default_logger(ostream_type& out);
	explicit default_logger(ostream_type&&) = delete;
	template<class Algorithm>
	void operator()(const Algorithm&, typename Algorithm::stage_type stage, const std::vector<typename Algorithm::evaluated_specimen_type>& specimens) const;
private:
	ostream_type& out;
};

#endif

template<class CharT, class Traits>
inline default_logger<CharT, Traits>::default_logger(ostream_type& out)
	: out(out) {}

template<class CharT, class Traits>
template<class Algorithm>
inline void default_logger<CharT, Traits>::operator()(const Algorithm&, typename Algorithm::stage_type stage, const std::vector<typename Algorithm::evaluated_specimen_type>& specimens) const {
	using stage_type = typename Algorithm::stage_type;
	switch (stage) {
		case stage_type::generated:
			out << "Generated new specimens:\n";
			break;
		case stage_type::selected:
			out << "Selected the fittest specimens:\n";
			break;
		case stage_type::bred:
			out << "Created a new generation:\n";
			break;
	}
	for (const auto& specimen : specimens) {
		out << specimen.value() << " rated " << specimen.rating() << '\n';
	}
}
