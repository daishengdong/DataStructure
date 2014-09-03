# -*- coding: UTF-8 -*-

import os 
import shutil 

rm_suffixs = ['.dsp', '.dsw', '.ncb', 'opt', 'plg']
edit_suffixs = ['.cpp', '.c']

def clear_format(file_name):
    bak_name = file_name + '_bak'
    bak_file = open(bak_name, 'w+')
    for line in open(file_name):
        line = line.replace('( ', '(')
        line = line.replace(' )', ')')
        line = line.replace('[ ', '[')
        line = line.replace(' ]', ']')
        line = line.replace('int *', 'int* ')
        line = line.replace('char *', 'char* ')
        line = line.replace('float *', 'float* ')
        line = line.replace('double *', 'double* ')
        bak_file.write(line)
    bak_file.close()
    os.remove(file_name)
    os.rename(bak_name, file_name)

def clean(path): 
    for i in os.listdir(path): 
        cur_path = os.path.join(path, i)
        if os.path.isdir(cur_path):
            if (i == 'Debug'):
                shutil.rmtree(cur_path)
            else:
                clean(cur_path) 
        else:
            for s in rm_suffixs:
                if i.endswith(s):
                    os.remove(cur_path)
            for e in edit_suffixs:
                if i.endswith(e):
                    clear_format(cur_path)
           
rootpath = os.path.abspath('.') 
clean(rootpath)
