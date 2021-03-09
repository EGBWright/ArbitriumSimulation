#a script to run several replicates of several treatments locally

directory = "resets2/"
seeds = range(10, 21)
periods_resets = [[25,26400,0.0141,0.0081],[50,13200,0.0136,0.0068],[100,6600,0.0128,0.0059],[110,6000,0.0382,0.0331],[120,5500,0.1298,0.0222],[132,5000,0.1718,0.0234],[150,4400,0.2012,0.0293],[176,3750,0.2279,0.0295],[200,3300,0.2227,0.0291],[400,1650,0.2264,0.0392],[1000,660,0.1934,0.0403],[2000,330,0.1773,0.0196],[5000,132,0.1689,0.0347]]


import subprocess

def cmd(command):
    '''This wait causes all executions to run in sieries.                          
    For parralelization, remove .wait() and instead delay the                      
    R script calls unitl all neccesary data is created.'''
    return subprocess.Popen(command, shell=True).wait()

def silent_cmd(command):
    '''This wait causes all executions to run in sieries.                          
    For parralelization, remove .wait() and instead delay the                      
    R script calls unitl all neccesary data is created.'''
    return subprocess.Popen(command, shell=True, stdout=subprocess.PIPE).wait()

print("Copying Settings.cfg to "+directory)
cmd("cp Settings.cfg "+directory)

for a in seeds:
    for b in periods_resets:
        command_str = './evo-algo -SEED '+str(a)+' -PERIODS '+str(b[0]) +' -RESETS '+str(b[1]*3)+' -FILE_PATH '+directory+' -FILE_NAME periods'+str(b[0])+'.dat' + ' -LYSOGENY_RATE_MEAN '+ str(b[2])+ ' -LYSOGENY_RATE_DEVIATION '+ str(b[3])+' -THRESHOLD_EVOLVERS 1.0 -TIMING_REPEAT 500'
        
        print(command_str)
        cmd(command_str)