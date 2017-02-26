//============================================================================
// Name        : SecondBiggest.cpp
// Author      : Ozbolt Menegatti
// Version     : 0.1
// Copyright   : GPL
// Description : Algorithms for finding second smallest element
//============================================================================

#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <map>
#include <cmath>

template<typename T>
std::vector<T> make_vec(size_t N) {
	std::vector<T> vec;
	for (size_t i = 0; i < N; i++) {
		vec.push_back(i);
	}
	return vec;
}

template<typename T>
int benchmark(const std::vector<T>& vec, T (*f)(std::vector<T>&)) {
	std::vector<T> vec_copy = vec;

	std::chrono::steady_clock::time_point begin =
			std::chrono::steady_clock::now();
	T second = f(vec_copy);
	std::chrono::steady_clock::time_point end =
			std::chrono::steady_clock::now();

	assert(second == 1);
	return int((end - begin).count());
}

template<typename T>
T m_one_for_loop(std::vector<T>& lst) {
	T first = std::min(lst[0], lst[1]);
	T second = std::max(lst[0], lst[1]);

	for (auto iter = lst.begin() + 2; iter != lst.end(); ++iter) {
		if (*iter < first) {
			second = first;
			first = *iter;
		} else if (*iter < second) {
			second = *iter;
		}
	}

	return second;
}

template<typename T>
T m_two_different_loops(std::vector<T>& lst) {
	T first = std::min(lst[0], lst[1]);
	T second = std::max(lst[0], lst[1]);
	auto first_ptr = lst[0] < lst[1] ? lst.begin() : lst.begin() + 1;

	for (auto iter = lst.begin() + 2; iter != lst.end(); ++iter) {
		if (*iter < first) {
			second = first;
			first = *iter;
			first_ptr = iter;
		}
	}

	for (auto iter = first_ptr + 1; iter != lst.end(); ++iter) {
		if (*iter < second) {
			second = *iter;
		}
	}

	return second;
}

template<typename T>
T m_nth_element(std::vector<T>& lst) {
	std::nth_element(lst.begin(), lst.begin() + 1, lst.end());
	return lst[1];
}

template<typename T>
T twice_bubble(std::vector<T>& lst) {
	for(int i = 0; i < 2; ++i) {
		for(auto iter = lst.end() - 1; iter != lst.begin(); --iter) {
			if(*(iter - 1) > *iter) {
				std::iter_swap(iter - 1, iter);
			}
		}
	}
	return lst[1];
}

template<typename T>
T m_two_identical_loops(std::vector<T>& lst) {
	auto smallest = lst.begin();
	for (auto iter = lst.begin() + 1; iter != lst.end(); ++iter) {
		if (*smallest > *iter) {
			smallest = iter;
		}
	}

	std::iter_swap(lst.begin(), smallest);
	smallest = lst.begin() + 1;
	for (auto iter = lst.begin() + 2; iter != lst.end(); ++iter) {
		if (*smallest > *iter) {
			smallest = iter;
		}
	}

	return *smallest;
}

template<typename T>
T m_sort(std::vector<T>& lst) {
	std::sort(lst.begin(), lst.end());
	return lst[1];
}

template<typename T>
T m_tournament(std::vector<T>& lst) {
	static std::vector<T> tree;
	size_t tree_size = pow(2, ceil(log2(lst.size())));
	tree.resize(tree_size - 1, 1);

	// making number of elements even
	T second = std::max(lst[0], lst[1]);
	lst.resize(lst.size() + (lst.size() % 2), second);

	// first round of tournament
	for (size_t i = 0; i < lst.size(); i += 2) {
		tree[i / 2] = std::min(lst[i], lst[i + 1]);
	}

	// first round of tournament, fill to 2^n
	for (size_t i = lst.size(); i < tree_size; i += 2) {
		tree[i / 2] = second;

	}

	// rest of tournament (last round is not really needed, but what is two or three more instructions)
	size_t offset_in = 0;
	for (size_t offset_out = tree_size / 2; offset_out < tree_size - 1;
			++offset_out) {
		T first = tree[offset_in++];
		T second = tree[offset_in++];
		tree[offset_out] = std::min(first, second);
	}

	// check who lost to the winner in last round
	bool cmp = tree[tree_size - 2 - 1] > tree[tree_size - 3 - 1];
	size_t zero_pair_idx = 2 * (cmp + 2);
	T second_smallest = tree[tree_size - 2 - 1 - !cmp];

	// check who lost to the winner in rounds 2 .. semifinal
	while (zero_pair_idx < tree_size) {
		size_t pair_idx = tree_size - zero_pair_idx - 1;
		size_t new_zero = tree[pair_idx - 1] <= tree[pair_idx];

		second_smallest = std::min(second_smallest, tree[pair_idx - !new_zero]);
		zero_pair_idx = 2 * (zero_pair_idx + new_zero);
	}

	// check the first round loser
	return std::min(second_smallest,
			std::max(lst[2 * tree_size - zero_pair_idx - 2],
					lst[2 * tree_size - zero_pair_idx - 1]));
}

typedef long long mytype;
typedef mytype (*fun_t)(std::vector<mytype>&);

int main() {
	std::vector<std::pair<std::string, fun_t>> method_map;
	method_map.push_back(
			std::pair<std::string, fun_t>("tournament", m_tournament));
	method_map.push_back(
			std::pair<std::string, fun_t>("one_for_loop", m_one_for_loop));
	method_map.push_back(
			std::pair<std::string, fun_t>("two_different_loops",
					m_two_different_loops));
	method_map.push_back(
			std::pair<std::string, fun_t>("two_equal_loops",
					m_two_identical_loops));
	method_map.push_back(
			std::pair<std::string, fun_t>("twice_bubble", twice_bubble));

	srand(time(NULL));
	const size_t MIN_ITER = 100;
	const size_t MAX_ITER = 300;
	const size_t MAX_SEC = 200;
	const size_t MIN_SIZE = 100;
	const double STEP_SIZE = 1.2;

	std::vector<int> methods(method_map.size());
	std::iota(std::begin(methods), std::end(methods), 0);

	std::vector<std::vector<int>> results;
	bool to_break = false;

	for (double n = MIN_SIZE; !to_break; n *= STEP_SIZE) {
		// reset results
		results.resize(0, std::vector<int>());
		results.resize(method_map.size(), std::vector<int>());

		// make vec to work on
		std::vector<mytype> vec = make_vec<mytype>(n);

		size_t iter = 0;
		std::chrono::steady_clock::time_point begin =
				std::chrono::steady_clock::now();
		// repeat MAX_ITER every method
		for (; iter < MAX_ITER && !to_break; ++iter) {

			// shuffle order of methods before starting
			std::random_shuffle(methods.begin(), methods.end());
			std::random_shuffle(vec.begin(), vec.end());

			for (int method : methods) {
				// actually perform the method
				int time = benchmark(vec, method_map[method].second);
				results[method].push_back(time);

				// check if running to long already...
				using namespace std::chrono;
				duration<double> d = duration_cast<duration<double>>(
						steady_clock::now() - begin);
				if (d.count() > MAX_SEC) {
					to_break = true;
					break;
				}
			}
		}

		// if last one lasted for too long!
		to_break |= iter < MIN_ITER;

		std::cerr << "N: " << n << std::endl;
		// print results
		for (size_t i = 0; i < method_map.size(); i++) {
			std::cout << method_map[i].first << ";" << n;
			for (int t : results[i]) {
				std::cout << ";" << t;
			}
			std::cout << std::endl;
		}
	}
}

