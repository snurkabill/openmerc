"""
Mercore 0.1.0

Usage:
    merc core exit
    merc core run
    merc core status
    merc module add [-c <config_path> -P <permission> -n <module_name> -g <group_id>] <module_path>
    merc module set [-c <config_path> -P <permission> -g <group_id>]  (--mid <mid> | -n <module_name>)
    merc module run <mid>
    merc module stop <mid>
    merc module remove <mid>
    merc module status [<mid>]
    merc module connections
    merc module find ([--mid <mid> | -c <config_path> | -n <module_name> | -p <module_path> | -P <permission>])
    merc group add
    merc group set
    merc group remove
    merc group status

Options:
    -c <config_path>                Configuration file.
    -n <module_name>                Name of the module.
    --mid <mid>                     Module ID.
    -P <permission>                 Permission for every.
    -p <module_path>                path_to_module.
    -g <group_id>                   Group ID.
"""
import readline
import logging
import sys
from docopt import docopt, DocoptExit
import zmq
import json
import os
import subprocess


LOG_FILENAME = '/tmp/oms.log'
logging.basicConfig(filename=LOG_FILENAME,
                    level=logging.DEBUG)

#used commands for completion
COMMANDS = [ 'module', 'add', 'remove', 'help', 'blah', 'foo',
    'status', 'find', 'core', 'group', 'clear', 'exit']

variables = {}
variable_to_set = ""

def check_if_mercore_is_running():
    process = subprocess.Popen("ps cax | grep mercore.out",shell=True,stdout=subprocess.PIPE)
    stdout_list = process.communicate()[0].split('\n')
    print stdout_list
    print len(stdout_list)

# function sends message throught TCP to core
def send_socket(zprava, ip, port):
    context = zmq.Context()
    socket = context.socket(zmq.REQ)
    socket.connect("tcp://%s:%s" %(ip, port))
    socket.send(zprava)
    # u urrcitych prikazu nema cekat na odpoved...
    message = socket.recv() # casovy omezeni??
    return message

def create_json(arguments):
    m = "{"
    for item in arguments:
        if str(arguments[item]).startswith("[") and str(arguments[item]).endswith("]"):
            #print("hello %s" %(m))
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


def process_variables(command):
    global variables
    global variable_to_set

    tokens = command.split()

    if tokens[0][0] == '$': #there is a variable
        var = ' '.join(tokens[:1])
        if len(tokens) == 1:
            if var in variables:
                print(variables[var])
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

def process_command(command):
    command = command.strip()
    if command == 'exit':
        return
    elif command == "":
        return
    elif command == 'help':
        print(__doc__)
    elif command == "core status":
        check_if_mercore_is_running()
    elif command == 'clear':
        os.system('cls' if os.name == 'nt' else 'clear')
    elif command == "core run":
        ret = os.system("nohup /usr/local/bin/mercore.out > ~/.mercore.log 2>&1&");
        if ret == 0:
            print "core is up and running!"
        else:
            print "uups. some error occurred"
    else: # check if mercore is listening
        command = process_variables(command)
        if command == "continue":
            return
        try:
            arguments = docopt(__doc__, command.split(), version='mercore 0.0.1')
            json = create_json(arguments)
            return json
        except DocoptExit as e:
                # The DocoptExit is thrown when the args do not match.
                # We print a message to the user and the usage block.

                print('Invalid Command!')
                print(e)
                return

        except SystemExit:
            # The SystemExit exception prints the usage for --help
            # We do not need to do the print here.

            return



class SimpleCompleter(object):

    def __init__(self, options):
        self.options = sorted(options)
        return


    def complete(self, text, state):
        response = None
        if state == 0:
            # This is the first time for this text, so build a match list.
            if text:
                self.matches = [s
                                for s in self.options
                                if s and s.startswith(text)]
                logging.debug('%s matches: %s', repr(text), self.matches)
            else:
                self.matches = self.options[:]
                logging.debug('(empty input) matches: %s', self.matches)

        # Return the state'th item from the match list,
        # if we have that many.
        try:
            response = self.matches[state]
        except IndexError:
            response = None
        logging.debug('complete(%s, %s) => %s',
                      repr(text), state, repr(response))
        return response

def input_loop():
    line = ''
    while line != 'exit':
        line = raw_input('merc >_ ')
        #print 'Dispatch %s' % line
        message = process_command(line)
        if message != None:
            ret = send_socket(message, "localhost", 5555 )
            #ret_json = json.loads(ret)
            print "return_message=%s" %ret
            # process return message and save it to the variable


# Process args -> help/version



# Register our completer function
readline.set_completer(SimpleCompleter(COMMANDS).complete)

# Use the tab key for completion
readline.parse_and_bind('tab: complete')
readline.parse_and_bind('set editing-mode vi')

# Prompt the user for text
input_loop()


##
#   pokud mi vrati {return: X}, pak X ulozim do promenne a vypisu
#   pokud mi vrati {error: Y}, pak vypisu error message Y
#   pokud mi vrati neco jineho, ulozim a vypisu
#
