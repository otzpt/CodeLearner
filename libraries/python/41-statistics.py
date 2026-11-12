"""
TITLE: statistics: mathematical statistics functions
GROUP: Numbers and time
LIBRARY: Python standard library (statistics)
USES: import statistics
SUMMARY: Averages and spread of numeric data: mean, median, mode, variance,
  standard deviation and quantiles.
ENTRY: statistics.mean(data)
   Return the sample arithmetic mean of data.
ENTRY: statistics.fmean(data, weights=None)
   Convert data to floats and compute the arithmetic mean.
ENTRY: statistics.median(data)
   Return the median (middle value) of numeric data.
ENTRY: statistics.median_low(data)
   Return the low median of numeric data.
ENTRY: statistics.median_high(data)
   Return the high median of data.
ENTRY: statistics.mode(data)
   Return the most common data point from discrete or nominal data.
ENTRY: statistics.multimode(data)
   Return a list of the most frequently occurring values.
ENTRY: statistics.stdev(data, xbar=None)
   Return the square root of the sample variance.
ENTRY: statistics.pstdev(data, mu=None)
   Return the square root of the population variance.
ENTRY: statistics.variance(data, xbar=None)
   Return the sample variance of data.
ENTRY: statistics.pvariance(data, mu=None)
   Return the population variance of data.
ENTRY: statistics.quantiles(data, *, n=4, method='exclusive')
   Divide *data* into *n* continuous intervals with equal
   probability.
ENTRY: statistics.StatisticsError
   Inappropriate argument value (of correct type).
NOTES:
  - stdev() and variance() are for a sample (divide by n - 1);
    pstdev() and pvariance() for a whole population (divide by n).
  - fmean() is faster than mean() and always returns a float.
  - An empty list raises StatisticsError.
  - Signatures and descriptions are Python's own, read from the
    module's docstrings for the version that runs this page.
    Version notes (added, changed, deprecated) are in the online
    documentation.
SEE: https://docs.python.org/3/library/statistics.html, pydoc statistics
"""

import statistics

data = [3, 1, 4, 1, 5, 9, 2, 6]
print("mean, fmean:", statistics.mean(data), statistics.fmean(data))
print("median, low, high:", statistics.median(data), statistics.median_low(data), statistics.median_high(data))
print("mode, multimode:", statistics.mode(data), statistics.multimode([1, 1, 2, 2, 3]))
print("stdev (sample), pstdev (population):", round(statistics.stdev(data), 3), round(statistics.pstdev(data), 3))
print("variance, pvariance:", round(statistics.variance(data), 3), round(statistics.pvariance(data), 3))
print("quartiles:", statistics.quantiles(data, n=4))
try:
    statistics.mean([])
except statistics.StatisticsError as error:
    print("StatisticsError:", error)
