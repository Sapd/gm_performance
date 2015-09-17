# gm_performance
## Usage
```
require("performance")
performance.getFPS() -- returns fps as a double
performance.getDiff() -- returns time between one frame in microseconds
performance.getDiffMS() -- in milliseconds
```

## Benchmarking:

```
-- Starts measuring performance every 200ms and writes it into a csv file in garrysmod root folder
performance.startBenchmark()
-- Stops it
performance.stopBenchmark()
```
You can open the csv later with Microsoft Excel or Libreoffice Calc and generate Diagrams out of it.

![Image](http://i.imgur.com/LA0iS3b.png)
