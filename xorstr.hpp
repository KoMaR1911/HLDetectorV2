// Copyright (c) 2010-2014, Sebastien Andrivet
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from this
//    software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#pragma once
#include <Windows.h>
#include <string>
#include <array>
#include <utility>
#include <cstdarg>

#include <stdbool.h>
#include <stdint.h>

namespace xorstr_impl {

#ifdef _MSC_VER
#define XORSTR_INLINE __forceinline
#else
#define XORSTR_INLINE inline
#endif

	constexpr auto time = __TIME__;
	constexpr auto seed = static_cast<int>(time[7]) +
		static_cast<int>(time[6]) * 10 +
		static_cast<int>(time[4]) * 60 +
		static_cast<int>(time[3]) * 600 +
		static_cast<int>(time[1]) * 3600 +
		static_cast<int>(time[0]) * 36000;

	// 1988, Stephen Park and Keith Miller
	// "Random Number Generators: Good Ones Are Hard To Find", considered as "minimal standard"
	// Park-Miller 31 bit pseudo-random number generator, implemented with G. Carta's optimisation:
	// with 32-bit math and without division

	template <int N>
	struct random_generator
	{
	private:
		static constexpr unsigned a = 16807;  // 7^5
		static constexpr unsigned m = 2147483647;  // 2^31 - 1
		static constexpr unsigned s = random_generator<N - 1>::value;
		static constexpr unsigned lo = a * (s & 0xFFFF);  // multiply lower 16 bits by 16807
		static constexpr unsigned hi = a * (s >> 16);  // multiply higher 16 bits by 16807
		static constexpr unsigned lo2 = lo + ((hi & 0x7FFF) << 16);  // combine lower 15 bits of hi with lo's upper bits
		static constexpr unsigned hi2 = hi >> 15;  // discard lower 15 bits of hi
		static constexpr unsigned lo3 = lo2 + hi;

	public:
		static constexpr unsigned max = m;
		static constexpr unsigned value = lo3 > m ? lo3 - m : lo3;
	};

	template <>
	struct random_generator<0>
	{
		static constexpr unsigned value = seed;
	};

	template <int N, int M>
	struct random_int
	{
		static constexpr auto value = random_generator<N + 1>::value % M;
	};

	template <int N, class T>
	struct random_char
	{
		static constexpr T value = static_cast<T>(1 + random_int<N, (1 << ((sizeof(T) * 8) - 1)) - 1>::value);
	};

	template <class T, size_t N, int K>
	struct basic_string
	{
	private:
		using _T = std::decay_t<T>;

		const _T key_;
		std::array<_T, N + 1> encrypted_;

		constexpr _T enc(_T c) const
		{
			return c ^ key_;
		}

		_T dec(_T c) const
		{
			return c ^ key_;
		}

	public:
		template <size_t... Is>
		constexpr XORSTR_INLINE basic_string(const _T* str, std::index_sequence<Is...>) :
			key_(random_char<K, _T>::value), encrypted_{ { enc(str[Is])... } } {}

		XORSTR_INLINE decltype(auto) decrypt()
		{
			for (size_t i = 0; i < N; ++i) {
				encrypted_[i] = dec(encrypted_[i]);
			}
			encrypted_[N] = static_cast<_T>(0);
			return (const _T*)encrypted_.data();
		}
	};

#undef XORSTR_INLINE

}  // namespace xorstr_impl

#define xorenc(s) (xorstr_impl::basic_string<decltype(s[0]), sizeof(s) - 1, \
  __COUNTER__>(s, std::make_index_sequence<sizeof(s) - 1>()))

#define xorstr(s) (xorenc(s).decrypt())
