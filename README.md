# Monkey Rooms

My daughter brought from her school this problem:

_There is a string of 17 pairwise connected rooms, where a monkey hides.
Each day you can check two rooms to see if the monkey is in there.
At night, the monkey runs into a connected room.
The monkey thinks it is better to always change the room not to be caught,
so she does not stay in the same room.
The task is to make sure to find her in as few days as possible._

```
┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐
              🐒
└───┘ └───┘ └───┘ └───┘ └───┘ └───┘ └───┘
```

## A few comments

While interesting theoretically, I thought this is a wonderful programming
exercise that would touch many important concepts, such as information representation,
algorithms and optimization.

This is also a great example to illustrate common issues of a direct brute-force approach,
that in this case would involve seemingly small amount of calculations at just a few steps,
but would lead to an extremely long running time. Indeed, even for say 15 rooms,
we would need to consider about 100 possible choices of rooms for 9 days before we find
a solution. These numbers are very small: 15 rooms, 9 days. Moreover, for each day we need
to consider about 100 pairs of rooms, and it is entirely possible to write a program that
would check, say, 100 million pairs of rooms in a second. Yet, if we run such a program
for our 15 rooms, we would need to wait for the answer "10 days" for about 500 years!

Another huge problem of the brute-force approach is memory. First, we need to track our
moves. We want to be able to reconstruct any solution after we find the minimal number
of days required to find the monkey in the worst case scenario. One way, of course,
would be to keep track of the previous state and our move for any state we get into.
However, the problem is that we can get from one fixed state into another fixed state
in many ways. Second, the number of possible solutions also grows pretty fast.
For example, for 15 rooms as above the number of solutions is greater than 25000. While
by itself this number is not large, it indicates that the solution graph is far from being
a tree, and is rather a highly connected graph. If we were to keep all "parent" nodes for
each node, this would grow up in memory pretty fast.

The memory problem here is also related to the general problem of keeping a good balance
between memory usage and efficiency. For example, should we use an array or a dictionary,
where the first would be much faster but the second much more efficient on memory?

## Modifications

A few modifications and generalizations come into mind. First, we may, of course, vary
the number of rooms. Second, we may consider two cases depending on whether the same room
can be checked more than once in a day (this would not affect the solutions much rather
their number). More interestingly, third, we may change the possible number of room checks.
Say, how solutions would change if we are now allowed to open 3 or 4 rooms? Finally, fourth,
I thought it would be interesting to consider a modification rather than a generalization of
the problem where the rooms are placed in a circle.

This modification is interesting also because unlike the original problem it is not even
clear in general whether a solution exists. In the linear case with two checks per day
there is always a "naive" strategy to first check rooms 1 and 2 in day 1, then in day 2
check rooms 2 and 3 etc. With this strategy we force the monkey in the worst case scenario
towards the other end of the chain of rooms. But there is no such obvious approach in
the case where the rooms form a circle. In this case, it is only pretty clear that, with
1 check per day we cannot catch the monkey if we have at least 3 rooms, and with 2 checks
per day we can catch it in some cases with a small number of rooms, but what in general?
Do we need more than 2 checks per day when we have many rooms in a circle?

## The challenge

There are a few approaches to the original problem that can significantly reduce computation.
However, I wanted to keep this problem a programming challenge. First, I did not want
to waste too much time on debugging numerous problems that would arise if the algorithm
was too much overcomplicated. Second, more importantly, the algorithm should be able to solve
all modifications and generalizations without too much work. At the same time, it should be
generous on memory, and much more efficient than the "500 years" algorithm.

In fact, the challenge was _to write an algorithm that would solve the problem with 2 checks
for all cases from 2 rooms up to 35 rooms in a reasonable time, and find all solutions
in each case keeping them internally in a manner that would allow it to list them all
in a linear time_. At the same time the algorithm should be a) universal enough to take care
of the modifications and generalizations listed above; and b) as much straightforward as
possible. The latter can be understood as follows: if the brute-force algorithm was efficient
enough to tackle the problem in a reasonable time, it would be a perfect candidate; however,
it is not, so the algorithm we are looking for should be efficient and smart just enough
to solve the problem in a reasonable time. There is no need to waste time on writing and
debugging a fancy algorithm if a simpler one works just fine according to the set criteria.

## The results

The current code solves the problem with 2 checks and finds all solutions in all cases from
1 room to 35 rooms in less than 30 seconds in total on a ~i5 6th generation computer, and in
less than 12 seconds on an i3 8th generation computer. For example, the case of 33 rooms is
solved and all its 556992 solutions are found in about 3.3 seconds on ~i5 6th generation,
and in about 1.7 seconds on i3 8th generation using only 1GB of memory.

At the same time, the code is universal enough so that it takes care of all generalizations
by design (it depends on a bunch of variables such as the number of rooms and the number
of possible checks per day, that can be set arbitrary), and to add the circular case all
that was needed was to add a function corresponding to the new way information is updated
after a room is open, and do a few other minor modifications so that the function is called
if the circular case is chosen.