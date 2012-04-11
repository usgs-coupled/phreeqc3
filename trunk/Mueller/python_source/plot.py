"""Plot the outflow."""

import matplotlib.pyplot as plt



def plot(ncells, outflow, specie_names):
    """Plot the results.
    """
    colors = {'Ca': 'r', 'Cl': 'b', 'K': 'g', 'N': 'y', 'Na': 'm'}
    x = [i / float(ncells) for i in xrange(1,
                                           len(outflow[specie_names[0]]) + 1)]
    args = []
    for name in specie_names:
        args.extend([x, outflow[name], colors[name]])
    # pylint: disable-msg=W0142
    # we do want *
    plt.plot(*args)
    plt.legend(specie_names, loc=(0.8, 0.5))
    plt.ylabel('MILLIMOLES PER KILOGRAM WATER')
    plt.xlabel('PORE VOLUME')
    plt.show()

def read_outflow(file_name):
    """Read outflow values.
    """
    fobj = open(file_name)
    header = fobj.next().split()
    outflow = {}
    for head in header:
        outflow[head] = []
    for line in fobj:
        line = line.split()
        for pos, head in enumerate(header):
            outflow[head].append(float(line[pos]))
    return outflow

if __name__ == '__main__':
    plot(40, read_outflow('out.txt'),
         specie_names = ('Ca', 'Cl', 'K', 'N', 'Na'))
