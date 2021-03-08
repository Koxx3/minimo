#%%
file = 'parameters.xls'
import xlrd
from itertools import chain
from collections import OrderedDict
import simplejson as json
import hashlib

# Open the workbook and select the first worksheet
wb = xlrd.open_workbook(file)
sh = wb.sheet_by_index(0)

keys = sh.row_values(0)

# List to hold dictionaries
data_list = []
param_classes_sub_list = OrderedDict()
param_classes = OrderedDict()
i = 0

# Iterate through each row in worksheet and fetch values into dict
for rownum in range(1, sh.nrows):
    row_values = sh.row_values(rownum)

    # single param
    param_param_data_sub = {}
    param_param_data_sub[keys[0]] = int(row_values[0])
    param_param_data_sub[keys[2]] = int(row_values[2])

    for col_idx in range(3, sh.ncols):
        if keys[col_idx] == "default" and row_values[keys.index("type")] != "float" and row_values[keys.index("type")] != "string" and row_values[col_idx] != "":
            param_param_data_sub[keys[col_idx]] = int(row_values[col_idx])
        elif keys[col_idx] == "max" and row_values[keys.index("type")] != "float" and row_values[keys.index("type")] != "string" and row_values[col_idx] != "":
            param_param_data_sub[keys[col_idx]] = int(row_values[col_idx])
        elif keys[col_idx] == "min" and row_values[keys.index("type")] != "float" and row_values[keys.index("type")] != "string" and row_values[col_idx] != "":
            param_param_data_sub[keys[col_idx]] = int(row_values[col_idx])
        elif keys[col_idx] == "fast_increment" and row_values[keys.index("type")] != "float" and row_values[keys.index("type")] != "string" and row_values[col_idx] != "":
            param_param_data_sub[keys[col_idx]] = int(row_values[col_idx])
        elif keys[col_idx] == "slow_increment" and row_values[keys.index("type")] != "float" and row_values[keys.index("type")] != "string" and row_values[col_idx] != "":
            param_param_data_sub[keys[col_idx]] = int(row_values[col_idx])
        else :
            param_param_data_sub[keys[col_idx]] = row_values[col_idx]

    if row_values[1] in param_classes_sub_list:
        #print ("-- update")
        current_items = param_classes_sub_list[row_values[1]]
            
        array_items = []
        array_items.append(param_param_data_sub)
        merged_items = current_items + array_items

        param_classes_sub_list[row_values[1]] = merged_items
    else :
        #print ("-- create")
        array_items = []
        array_items.append(param_param_data_sub)
        param_classes_sub_list[row_values[1]] = array_items

    
print ("-----------")
data_list = {'parameters': param_classes_sub_list} # Added line

# Serialize the list of dicts to JSON
j = json.dumps(data_list, indent=2)

# Write to file
with open('params_list.json', 'w') as f:
    f.write(j)
    print (j)

