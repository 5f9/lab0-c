# Demonstration of queue testing framework
# Use help command to see list of commands and options
# Copy output to file
log misc.txt
# Initial queue is NULL.
show
# Create empty queue
new
# Fill it with some values.  First at the head
ih dolphin
ih bear
# Reverse it
reverse
ih gerbil
# Now at the tail
it meerkat
it bear
# Reverse it
reverse
# See how long it is
size
# Sort
sort
# Natural sort
nsort
# Sort case insensitive order
sort ci
# Natural sort case insensitive descending order
nsort ci desc
# Delete queue.  Goes back to a NULL queue.
free
# Read commands from source file
source traces/trace-01-ops.cmd
# Exit program
quit
