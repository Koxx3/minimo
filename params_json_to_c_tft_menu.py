import json
import re
import jinja2

template_h = """
//////------------------------------------
//////------------------------------------
//////------------------------------------
////// GENERATED FILE - DO NOT EDIT MANUALLY
//////------------------------------------
//////------------------------------------
//////------------------------------------

////// Manage settings exchanged in BLE and stored in EEPOM

{% for key, value in parameters.items() %}
    {% for key2, value2 in value.items() %}
        {%- for  item in value2.settings %}
        {%- set var_name = item.display_name | lower  | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") %}
        
        {%- if item.type | lower == "string" %}
            {% set var_type = "String" %}
        {%- else %}
            {% set var_type = item.type %}
        {%- endif %}
    /*-------------------------------------------------------*/

    #define SETTINGS_{{ var_name | upper }}_ID {{ item.id }}
    #define SETTINGS_{{ var_name | upper }}_ID_STR "{{ item.id }}"
    #define SETTINGS_{{ var_name | upper }}_NAME "{{ item.display_name }}"

    {{ var_type }} {{ var_name }};

        {%- if item.smartphone_display_type | lower == "list" %}
    typedef enum LIST_{{ item.display_name | replace(" ", "_") |title}} {
            {%- set list1 = item.list_strings.split('\n') %}
            {%- for item4 in list1 %}
        LIST_{{ var_name |title}}_{{ item4 | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") |title}},
            {%- endfor %} 
    } t{{ var_name |title}};
        {%- else %}
        {%- endif %}

    void set_{{ var_name }} ({{ var_type }} value);
    {{ var_type }} get_{{ var_name }}();
    void display_{{ var_name }}();

        {%- endfor %}
    {%- endfor %}
{%- endfor %}

};

#endif
"""


# Custom filter method
def regex_replace(s, find, replace):
    """A non-optimal implementation of a regex filter"""
    return re.sub(find, replace, s)

env = jinja2.Environment()
env.filters['regex_replace'] = regex_replace

tmpl_h = env.from_string(template_h)


# load json from file
jsonConfigName = "params.json"
print ("jsonConfigName: " + jsonConfigName)
with open(jsonConfigName) as json_file:
    json_data = json.load(json_file)
    print(json_data)

    print("================================================")

    # merge template with data
    result_cpp = tmpl_cpp.render(parameters=json_data)

    # get template name, output file name
    outputFileName = ".\\src\\TFT\\tft_settings_menu_specs_gen.h"
    print("outputFileName H : " + outputFileName)
    print (result_cpp)

    # write output to file
    outFile = open(outputFileName,"w")
    outFile.write(result_cpp)
    outFile.close()
    