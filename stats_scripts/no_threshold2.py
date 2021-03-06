#a script to run several replicates of several treatments locally

directory = "NoThreshold/"
seeds = range(10, 21)
periods_resets = [[50,12000],[100,6000],[125,4800],[160,3750],[192,3125],[200,3000],[400,1500]]

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