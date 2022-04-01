import sys
import subprocess
from pip import main
import pkg_resources
import argparse
import os


print("[INFO] Checking required python packages...")

required = {'numpy', 'scipy', 'matplotlib', 'memory-profiler'}
installed = {pkg.key for pkg in pkg_resources.working_set}
missing = required - installed

#For the packages
if missing:
    print("[INFO] Installing missing packages: {}".format(list(missing)))
    python = sys.executable
    subprocess.check_call([python, '-m', 'pip', 'install', *missing], stdout=subprocess.DEVNULL)
else:
    print("[INFO] Requirement satisfied.")

#for reading all the files inside. 
source_dic = {}
query_dic = {}
sourceidentifier = "_source.txt"
queryidentifier = "_queries.txt"
files = os.listdir(".")
if not os.path.exists('.\\results'):
    os.makedirs('.\\results')
#Need to reverse the order of the files to ensure source ALWAYS appears first before queries. 
files.sort(reverse = True)

#Checking for duplicates in the source and the query
for file in os.listdir("."):
    if file.endswith(sourceidentifier):
    	if file[:-len(sourceidentifier)] in source_dic:
    		print("duplicate file -> " + file)
    	source_dic[file[:-len(sourceidentifier)]] = file
    if file.endswith(queryidentifier):
    	if file[:-len(queryidentifier)] in query_dic:
    		print("duplicate file -> " + file)
    	query_dic[file[:-len(queryidentifier)]] = file

parser = argparse.ArgumentParser()
parser.add_argument('-c', '--command', type=str, nargs=1, required=True, help="Command to run autotester")
parser.add_argument('-t', '--interval', default=0.01, help="Sampling rate of capturing memory usage")
args = parser.parse_args()

def runAutotester(src, qry):	    
	if args.command:
	    command = list(args.command)
	    command.append(".\\" + src)
	    command.append( ".\\" + qry)
	    command.append(".\\results\\" + qry + "_for_" + src + "_out.xml")
	    result = subprocess.run(['mprof', 'run', '-T', str(args.interval), '-C'] + command, stdout=subprocess.DEVNULL)
	    peak_memory_log = subprocess.run(['mprof', 'peak'], capture_output=True, text=True)
	    peak_memory = ' '.join(peak_memory_log.stdout.split()[-2:])
	    print("[MEM USAGE] Peak Memory Usage {}: {}".format(qry + "_for_" + src + " ", peak_memory))
	    #Saving the memory log. 
	    peak_memory_log = subprocess.run(['mprof', 'plot','-o ' + qry + "_for_" + src + "_memoryUsageGraph.png"], capture_output=True, text=True)


# Running the autotester. 
for key in source_dic:
	if key not in query_dic:
		print("corresponding query file not found -> " + key)
		continue
	else:
		runAutotester(source_dic[key], query_dic[key])






