# Test performance of natural sort ci with random and descending orders
# 10000: all correct natural sort ciing algorithms are expected pass
# 50000: natural sort ciing algorithms with O(n^2) time complexity are expected failed
# 100000: natural sort ciing algorithms with O(nlogn) time complexity are expected pass
option fail 0
option malloc 0
new
ih RAND 10000
nsort ci
reverse
nsort ci
free
new
ih RAND 50000
nsort ci
reverse
nsort ci
free
new
ih RAND 100000
nsort ci
reverse
nsort ci
free
