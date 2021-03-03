import os.path
import gzip

folder = ''

#periods_resets = [[25,4000],[50,2000],[100,1000],[200,500],[400,250],[1000,100]]
periods = ["25","50","100","200","400","1000"]
seeds = range(10, 21)
header = "uid treatment rep update coop count hist_0\n"

outputFileName = "munged_basic.dat"

outFile = open(outputFileName, 'w')
outFile.write(header)

for t in periods:
    for r in seeds:
        fname = folder + str(r) + "periods" + t + ".dat"
        uid = t + "_" + str(r)
        curFile = open(fname, 'r')
        for line in curFile:
            if (line[0] != "u"):
                splitline = line.split(',')
                outstring1 = "{} {} {} {} {} {} {} {} {} {}\n".format(uid, t, r, splitline[0], splitline[1], splitline[2], \
                splitline[3],splitline[4], splitline[5], \
                splitline[6])
                outFile.write(outstring1)
        curFile.close()
outFile.close()

