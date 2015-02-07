#!/usr/bin/python

"""
Usage:
    mercore core stop
    mercore core exit
    mercore core run
    mercore core status
    mercore module add [-c <config_path> -P <permission> -n <module_name> -g <group_id>] <module_path>
    mercore module set [-c <config_path> -P <permission> -g <group_id>]  (--mid <mid> | -n <module_name>)
    mercore module run <mid>
    mercore module stop <mid>
    mercore module remove <mid>
    mercore module status <mid>
    mercore module find ([--mid <mid> | -c <config_path> | -n <module_name> | -p <module_path> | -P <permission>])
    mercore group add
    mercore group set
    mercore group remove
    mercore group status

Options:
    -c <config_path>                Configuration file.
    -n <module_name>                Name of the module.
    --mid <mid>                     Module ID.
    -P <permission>                 Permission for every.
    -p <module_path>                path_to_module.
    -g <group_id>                   Group ID.
"""

import zmq
from docopt import docopt
import json
import os   # run mercore with nohup

#########################################
##          Global variables           ##
#########################################

command = ""
variables = {}
variable_to_set = ""


#########################################
##              Functions              ##
#########################################


# function sends message throught TCP to core
def send_socket(zprava, ip, port):
    context = zmq.Context()
    socket = context.socket(zmq.REQ)
    socket.connect("tcp://%s:%s" %(ip, port))
    socket.send(zprava)
    message = socket.recv() # casovy omezeni??
    return message

def create_json(arguments):
    m = "{"
    for item in arguments:
        if str(arguments[item]).startswith("[") and str(arguments[item]).endswith("]"):
            print("hello %s" %(m))
            m = m + "'" + item + "':" + str(arguments[item]) + ","
        else:
            written = 0
            for var in variables:
                if var == str(arguments[item]):
                    m = m + "'" + item + "':" + variables[var] + ","
                    written = 1
                    break

            if written == 0 :
                m = m + "'" + item + "':'" + str(arguments[item]) + "',"
    m = m[:-1]
    m = m + "}"
    return m

# pokud neco zacina promennou, pak vystup ulozi do ni
def process_variables(command):
    global variables
    global variable_to_set

    tokens = command.split()

    if tokens[0][0] == '$': #there is a variable
        var = ' '.join(tokens[:1])
        if len(tokens) == 1:
            if var in variables:
                print("variable=%s" %(variables[var]))
            else:
                print("variable does not exist")
            return "continue"
        elif len(tokens) > 2 and tokens[1] == "=" : # and first token contains more than 1 character
            com = ' '.join(tokens[2:])
            variables[var] = "EMPTY"    # is it neccesery to set to EMPTY?
            variable_to_set = var
            return com
        else:
            return "continue"
    else:
        return command


#########################################
##                Main                 ##
#########################################


while command != "exit" :
    command = raw_input(">>> ")

    # preprocess command
    if command == "exit":
        break
    elif command == "mercore run":
        print("initializing program mercore")
        # run mercore with nohup
        continue
    else :
        command = process_variables(command)
        if command == "continue":
            continue

        print("processing command [%s] with docopt." %(command))
        if variable_to_set != "":
            print("alocated variable [%s]" %(variable_to_set))
        else:
            print("no variable alocated")
            print("alocated variable [%s]" %(variable_to_set))

    #docopt magic
    command_list = command.split()
    arguments = docopt(__doc__, command_list, version='mercore 0.0.1')

    # if possible, process command in kirk
    # else send it to the mercore

    # replace variables with their value

    m = create_json(arguments)
    print("sending message: %s" %(m))
    ret_m = send_socket(m, "localhost", 5555)

    #json.loads(ret_m)
    print(ret_m)
    if variable_to_set != "":
        variables[variable_to_set] = ret_m

# konec programu
# vypsat hlasku pokud bezi mercore, ze mercore pobezi dal i po ukonceni tohoto programu,
# a jestli chce klient priste zobrazit tuto zpravu

print("bye bye..")
