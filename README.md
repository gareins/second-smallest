# Comparison of algorithms for finding second smallest element

Finding this algorithm is a classical question at job interviews. 
When I was asked this question, I was able to satisfy the recruiter,
but was a bit dissapointed, that a simple loop was "the best solution"
to quote the technical part of the duo, that was asking me these 
questions. So after the end of interview, I want to the webs to find
better solutions and here are a few solutions, that could be considered
good enough answers.

## Algorithms

Overview of algorithms used.

### Sort and then second element

This is the simplest answer, but is very slow and the only one with
time complexity of O(n logn). By my measurements, it is about ten times 
slower then other methods.

### Find smallest element, remove it and find the smallest one again

In graphs, this one is named *two equal loops*. Even though it is very 
simple and quite fast, it was not good enough at an interview. By my 
measurements, it is about 20-30% slower than the solution with just
one loop. This makes 2n comparisons and 2n loads.

    m, idx = min(list)
    list[0], list[idx] = list[idx], list[0]
    m, _ = min(list[1:])
    return m

### Use one loop, to find the two smallest elements

This was indeed the expected solution at interview. It is simple and 
has the most stable performance, though not the fastest. This method
makes 2n comparisons and n loads.

    small = max(list[0], list[1])
    smaller = min(list[0], list[1])
    for element in list[2:]:
      if element < smaller:
        small = smaller
        smaller = element
      else if element < small:
        small = element
    return small
    
### Use one loop, with only one main if statement

As per Jsdemonsim in #1, I implemented this and this is indeed
the fastest most of the time. It's interesting, how much a small
change, that would be considered less readable and unnecesary,
can impact performance for the better.

    small = max(list[0], list[1])
    smaller = min(list[0], list[1])
    for element in list[2:]:
      if element < small:
        if element < smaller:
          small = smaller
          smaller = element
        else:
          small = element
    return small

### Two runs of bubble sort and then second element

Even though this solution is more than two times slower than the last
method, it is a elegant in its own way. Number of operations here is 
2n loads, 2n comparisons and most importantly, lots of swaps, which 
is what makes this algotithm slow. It also modifies the array.

    for i in range(2):
      for i in reversed(range(1, len(list))):
        if list[i] >= list[i - 1]
          list[i], list[i - 1] = list[i - 1], list[i]
    return list[1]

### Two different loops

This is an algorithm, that I wrote myself, is very simple and fastest
on the list. It makes on average 3n/2 loads and 3n/2 comparisons.

    small = max(list[0], list[1])
    smaller = min(list[0], list[1])
    smaller_idx = index of smaller in list
  
    for element in list[2:]:
      if element < smaller:
        small = smaller
        smaller = element
        smaller_idx = index of element in list
    
    for element in list[smaller_idx:]:
      if element < small:
        small = element
    
    return small

It very much depends on the machine, but in on my i5, this was the fastest
method of the bunch, where reasons probably lie in cache and jump prediction,
but I really don't know why. If anyone knows, shoot!

Negative property of this method would be the fact, that it is very unstable, 
very fast sometimes and slower other times.

### Tournament

This is a very nice algorithm, that is indeed very fast, but only in cases,
where the number of elements in the list is exactly or just below 2^n. Thus
we see the unconsistent speeds, where performance increases only when we cross
powers of n.

How does it work: We orginize a tournament, where adjecent pairs compete.
After first round, there are only half the numbers left and we repeat.
In the end, the smallest element will win. Then, in order to find second 
fastest, we have to check all the numbers that lost to the winner.

This method performs 2n + log(n) loads, n + logn comparisons and n stores.

TODO: pseudo code.

## Results

In the results we discard first two rounds, to discard for the time, the
tournament algorithm needs to alocate space.

We have plots for average, best and worst case scenarios, we have relative 
plot of average speeds and the number of times an algorithm won in at a
given size.

[See this file](out/1/RESULTS.md).

## Do it yourself

I used gcc 6.3.1 with -O3 optimizations and python3 with matplotlib for 
visualization. You can edit mytype and other constants in main.cpp for 
different test scenarios. Constants:

* MIN_ITER minimal number of iteration of a given algorithm at a given size
           iteration will stop if MIN_ITER is not reached in MAX_SEC,
* MAX_ITER maximal number of iterations, before continuing to the next size,
* MAX_SEC longest time period for algorithms at a given size.
* MIN_SIZE size of the array in the first round,
* STEP_SIZE next_size = STEP_SIZE * previous_size,
* mytype type of elements in array.
