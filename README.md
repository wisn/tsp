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

This is tested on Intel Core i3-5005U 2.0 GHz (4 cores) with 12 GB RAM.

### Brute-force Approach

The testing for `own12` is failed because the RAM can't hold it.
That is because the generated route permutation about N factorial.

#### Time Comparison

| Dataset | Nodes | Serial Time (in seconds) | Parallel Time (in seconds) | Speedup      |
|---------|-------|--------------------------|----------------------------|--------------|
| own5    | 5     | 0.000214                 | 0.000485                   | 0.4412 times |
| own7    | 7     | 0.008978                 | 0.008220                   | 1.0922 times |
| own10   | 10    | 6.317628                 | 5.145314                   | 1.2278 times |
| own12   | 12    | -                        | -                          | -            |
| p01     | 15    | -                        | -                          | -            |
| gr17    | 17    | -                        | -                          | -            |
| fri26   | 26    | -                        | -                          | -            |

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
    <tr>
      <td>p01</td>
      <td>-</td>
      <td>-</td>
      <td>-</td>
      <td>-</td>
      <td>-</td>
      <td>-</td>
      <td>-</td>
      <td>-</td>
    </tr>
    <tr>
      <td>gr17</td>
      <td>-</td>
      <td>-</td>
      <td>-</td>
      <td>-</td>
      <td>-</td>
      <td>-</td>
      <td>-</td>
      <td>-</td>
    </tr>
    <tr>
      <td>fri26</td>
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

### Branch and Bound Approach

The testing for `fri26` is failed because the RAM can't hold it.

#### Time Comparison

| Dataset | Nodes | Serial Time (in seconds) | Parallel Time (in seconds) | Speedup      |
|---------|-------|--------------------------|----------------------------|--------------|
| own5    | 5     | 0.000071                 | 0.000273                   | 0.2600 times |
| own7    | 7     | 0.000154                 | 0.001048                   | 0.1469 times |
| own10   | 10    | 0.000379                 | 0.000519                   | 0.7302 times |
| own12   | 12    | 0.000959                 | 0.000751                   | 1.2769 times |
| p01     | 15    | 0.017047                 | 0.012871                   | 1.3244 times |
| gr17    | 17    | 34.846037                | 28.207000                  | 1.2353 times |
| fri26   | 26    | -                        | -                          | -            |

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
      <td>11 times</td>
      <td>0 times</td>
      <td>0 times</td>
      <td>0 times</td>
      <td>2 times</td>
      <td>2 times</td>
      <td>4 times</td>
      <td>3 times</td>
    </tr>
    <tr>
      <td>own7</td>
      <td>22 times</td>
      <td>0 times</td>
      <td>0 times</td>
      <td>0 times</td>
      <td>4 times</td>
      <td>8 times</td>
      <td>6 times</td>
      <td>4 times</td>
    </tr>
    <tr>
      <td>own10</td>
      <td>46 times</td>
      <td>0 times</td>
      <td>0 times</td>
      <td>0 times</td>
      <td>4 times</td>
      <td>12 times</td>
      <td>13 times</td>
      <td>17 times</td>
    </tr>
    <tr>
      <td>own12</td>
      <td>67 times</td>
      <td>0 times</td>
      <td>0 times</td>
      <td>0 times</td>
      <td>4 times</td>
      <td>12 times</td>
      <td>21 times</td>
      <td>30 times</td>
    </tr>
    <tr>
      <td>p01</td>
      <td>1831 times</td>
      <td>0 times</td>
      <td>0 times</td>
      <td>0 times</td>
      <td>381 times</td>
      <td>503 times</td>
      <td>534 times</td>
      <td>395 times</td>
    </tr>
    <tr>
      <td>gr17</td>
      <td>3195180 times</td>
      <td>0 times</td>
      <td>0 times</td>
      <td>0 times</td>
      <td>862218 times</td>
      <td>629183 times</td>
      <td>1027272 times</td>
      <td>676628 times</td>
    </tr>
    <tr>
      <td>fri26</td>
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

## Conclusion

Based on this experiments, parallel computation time is better than the serial one when the
number of data is quite many.
The parallel computation time is much worse otherwise.
That's mean we need to use parallel programming when we have to process a large data.
Also, from this result, it is clear that branch and bound approach is much better than the
brute-force approach.
The reason is quite simple. It is because the brute-force approach need to generate all
(possible) route permutation.
As for the branch and bound approach, we don't need to do so.
This is also make sense for the brute-force approach to failed in the `own12` dataset and so
on.
It is simply because the size of the generated permutation is N factorial which is too huge to
handle by the RAM.

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
