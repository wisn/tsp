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
Computing 5 nodes from "own5" dataset...

Optimal cost is 20.
Optimal route:

3   -[3]->   0
0   -[4]->   1
1   -[5]->   2
2   -[6]->   4

Took about 0.000245 seconds.
Also, the optimal cost is CORRECT based on the dataset.

Thread frequencies:
#0 ran 240 time(s).
#1 ran 0 time(s).
#2 ran 0 time(s).
#3 ran 0 time(s).
```

## Comparison Result

This is tested on Intel Core i3-5005U 2.0 GHz with 12 GB RAM.

### Brute-force Approach

The testing for `own12` is failed because the RAM can't hold it.
That is because the generated route permutation about N factorial.

#### Time Comparison

| Dataset | Serial Time (in seconds) | Parallel Time (in seconds) | Speedup      |
|---------|--------------------------|----------------------------|--------------|
| own5    | 0.000214                 | 0.000485                   | 0.4412 times |
| own7    | 0.008978                 | 0.008220                   | 1.0922 times |
| own10   | 6.317628                 | 5.145314                   | 1.2278 times |
| own12   | -                        | -                          | -            |

#### Thread Distribution Comparison

<table>
  <thead>
    <tr>
      <th rowspan="2">Dataset</th>
      <th colspan="4"><center>Serial Computation</center></th>
      <th colspan="4"><center>Parallel Computation</center></th>
    </tr>
    <tr>
      <th>#0</th>
      <th>#1</th>
      <th>#2</th>
      <th>#3</th>
      <th>#0</th>
      <th>#1</th>
      <th>#2</th>
      <th>#3</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>own5</td>
      <td>240 times</td>
      <td>0 times</td>
      <td>0 times</td>
      <td>0 times</td>
      <td>62 times</td>
      <td>66 times</td>
      <td>55 times</td>
      <td>57 times</td>
    </tr>
    <tr>
      <td>own7</td>
      <td>10080 times</td>
      <td>0 times</td>
      <td>0 times</td>
      <td>0 times</td>
      <td>2532 times</td>
      <td>2518 times</td>
      <td>2511 times</td>
      <td>2519 times</td>
    </tr>
    <tr>
      <td>own10</td>
      <td>7257600 times</td>
      <td>0 times</td>
      <td>0 times</td>
      <td>0 times</td>
      <td>1825016 times</td>
      <td>1821672 times</td>
      <td>1791897 times</td>
      <td>1819015 times</td>
    </tr>
    <tr>
      <td>own12</td>
      <td>-</td>
      <td>-</td>
      <td>-</td>
      <td>-</td>
      <td>-</td>
      <td>-</td>
      <td>-</td>
      <td>-</td>
    </tr>
  </tbody>
</table>

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
