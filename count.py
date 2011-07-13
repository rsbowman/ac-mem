import sys

"""
This is a short program to work out the hash function and test 
to make sure it works.  It has been implemented in ord_tup.c
"""

def ordtup(i, j):
    """ number of ordered i-tuples from an alphabet of j letters
    """
    if i <= 1:
        return j
    else:
        return sum([ ordtup(i-1, k) for k in range(1, j+1)])

def cumtup(i, j):
    """ number of ordered tuples up to length i from an alphabet
    of j letters
    """
    return sum([ ordtup(k, j) for k in range(1, i+1)])

def cutoff(n, k):
    """ look at n-tuples, k = 1..4, find cutoff ...
    """
    if n <= 1:
        return k
    else:
        return sum([cutoff(n-1, x) for x in range(1, k+1)])

def number(tup):
    """ given an ordered tuple from alphabet {0,1,2,3}, find its index in
    the list of
    all ordered tuples, ordered lexicographically
    """
    if len(tup) == 1:
        return tup[0]
    else:
        a, b = tup[0], tup[1:]
        return cutoff(len(tup), a) + number(b) + ordtup(len(tup) - 1, 4)

def allOrderedTups(n, l):
    """
    all ordered i-tuples, in order, i=1..n
    from alphabet 0..l
    """
    for i in range(1, n+1):
        for tup in orderedTups(i, l):
            yield tup

# ordered i-tuples from alphabet 0..l
def orderedTups(i, l):
    if i == 1:
        for w in range(l):
            yield (w,)
    else:
        for j in range(l):
            for tup in orderedTups(i - 1, j+1):
                yield (j,)+tup
        
def main(argv):
    for j in range(1, 8):
        for i, tup in enumerate(allOrderedTups(j, 4)):
            assert number(tup) == i
    print 'oh yeah, everything worked'
    
if __name__ == '__main__':
    sys.exit(main(sys.argv))
