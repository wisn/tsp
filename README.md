# Traveling Salesman Problem

Traveling Salesman Problem (TSP) is a popular optimization problem in computer science.
To put it simply, TSP is a problem where a salesman want to traverse all cities exactly once
with an optimal cost.

## Prerequisite

- `g++` 7.4.0 or later. We are using `C++11` standard.
- OpenMP installed.

## How to Use

Take a look on the code that you want to run.
Make sure to change `dataset` variable if necessary.
It is used for fetching the `inputs` and the `outputs` folder.
When everything is done, run this command.

```
$ g++ <filename>.cpp -std=c++11 -fopenmp -o exe && ./exe
```

Change `<filename>` with your chosen approach.
The result will be something like below.

```
TSP serial with brute-force approach.
Computed 5 nodes from "own" dataset.

Optimal cost is 20.
Optinal path:

0   -[4]->   1
1   -[5]->   2
2   -[6]->   4
4   -[2]->   3
3   -[3]->   0

Took about 0.000167 seconds.
Also, the optimal cost is CORRECT.
```

## License

The code that written in this repository is licensed under [The MIT License](LICENSE).

As for the datasets in the `inputs` and in the `outputs` folder (except for `own`)
are retrieved from:

```
Gerhard Reinelt,
TSPLIB - A Traveling Salesman Problem Library,
ORSA Journal on Computing,
Volume 3, Number 4, Fall 1991, pages 376-384.
```

It is available on <https://people.sc.fsu.edu/~jburkardt/datasets/tsp/tsp.html>.
