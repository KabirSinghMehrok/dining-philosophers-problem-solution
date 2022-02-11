# Dining Philosophers Problem Solution

This repository contains the solution to the Dining Philosophers Problem, a classic problem in deadlock management. The goal is to design a protocol that allows the philosophers to eat without encountering deadlock.

## Problem Description

The Dining Philosophers Problem involves a round table with five philosophers sitting around it. Each philosopher has a plate and a fork to their left and right. A philosopher can only eat if they can acquire both forks. The challenge is to design a solution that prevents deadlock, where no philosopher can eat because they can only access a single fork.

## Solution Approach

### Part 1: Waiter's Solution

-   The solution employs a waiter to allocate forks (resources) to the philosophers (threads).
-   Philosophers are assumed to be left-handed and will pick up the left fork first.
-   If a philosopher finds a suitable left fork, they pick it up. Otherwise, they look for a right fork.
-   If a philosopher cannot find any suitable fork, they enter a thinking state.
-   If there is only one fork available, the waiter only allows it to be the right-handed fork of a suitable philosopher.
-   Fork availability is managed using a boolean array.
-   Once a philosopher acquires both forks, they eat with those forks, return them, and enter a thinking state.
-   This approach avoids deadlocks by carefully coordinating fork allocation.

### Part 2: Extended Solution with Bowls

-   This part extends the first solution with the addition of sauce bowls.
-   The philosopher allocation and fork-handling process remains the same as in Part 1.
-   However, once a philosopher acquires both forks, they also check for the availability of a bowl.
-   The philosopher is free to choose any unoccupied bowl.
-   If a bowl is available, they eat with the forks and bowl, return them, and enter a thinking state.
-   This approach extends the solution to accommodate the use of sauce bowls.

## Repository Contents

-   **Part1:** C program implementing the waiter's solution to the Dining Philosophers Problem using synchronization primitives.
-   **Part2:** C program implementing a deadlock-free solution when philosophers require both forks and access to sauce bowls.
-   **Makefile:** A single Makefile to build each program.

## Usage
1.  Clone the repository
2.  Navigate to the desired part
3.  Build the program using the provided Makefile: `make`
4.  Run the executable: `./<part-name>.c`
