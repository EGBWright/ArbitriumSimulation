#a script to run several replicates of several treatments locally

directory = "NoThreshold4/"
seeds = range(10, 21)
periods_resets = [[25,26400],[50,13200],[100,6600],[110,6000],[120,5500],[132,5000],[150,4400],[176,3750],[200,3300],[400,1650],[1000,660],[2000,330],[5000,132]]


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
        command_str = './evo-algo -SEED '+str(a)+' -PERIODS '+str(b[0]) +' -RESETS '+str(b[1])+' -FILE_PATH '+directory+' -FILE_NAME periods'+str(b[0])+'.dat'
        
        print(command_str)
        cmd(command_str)