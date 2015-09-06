Performance module

Usage:

require("performance")

performance.getFPS()

-- returns time between one frame in microseconds

performance.getDiff()

-- in milliseconds

performance.getDiffMS()



Benchmarking:

-- Starts measuring performance every 200ms and writes it into a csv file in garrysmod root folder

performance.startBenchmark()

-- Stops it

performance.stopBenchmark()
