import json
import re
import jinja2

template_h = """
# Manual

{%- for key, value in parameters.items() %}
    {%- for key2, value2 in value.items() %}
        {%- set outer_loop = loop %}

## {{ key2 }}
        {%- for  item in value2.settings %}
            {%- if (item.valid_config != "old") %}

                {%- if (item.smartphone_display_name ==  item.tft_display_name) %}
### {{ item.smartphone_display_name }}
                {%- else %}
### {{ item.smartphone_display_name }} / {{ item.tft_display_name }} 
                {%- endif %}
<em>
                {%- if ((item.smartphone_display_type | lower) == "list") %}
Possible values : 
                    {%- set list1 = item.list_strings.split('\n')  %}
                    {%- for item4 in list1 %}
                        {%- set var_elem_name = item4 | lower  | replace(" ", "_") | replace("/", "_") | regex_replace("[^A-Za-z0-9_]","") %}
- {{ item4 }} (value = {{ loop.index - 1 }})

                {%- endfor %}
                {%- endif %}
Default value : {{ item.default }}

                {%- if (item.valid_config == "^sd_.*")  %}
Valid configuration : SmartDisplay
                {%- elif (item.valid_config == "^sc_.*") %}
Valid configuration : SmartController
                {%- else %}
Valid configuration : SmartController / SmartDisplay
                {%- endif %}
</em>
Usage : {{ item.help_en }}

            {%- endif %}
        {%- endfor %}
    {%- endfor %}
{%- endfor %}


"""


# Custom filter method
def regex_replace(s, find, replace):
    """A non-optimal implementation of a regex filter"""
    return re.sub(find, replace, s)

env = jinja2.Environment()
env.filters['regex_replace'] = regex_replace

tmpl_h = env.from_string(template_h)


# load json from file
jsonConfigName = "gen_settings\\params.json"
print ("jsonConfigName: " + jsonConfigName)

with open(jsonConfigName) as json_file:
    json_data = json.load(json_file)

    # merge template with data
    result_h = tmpl_h.render(parameters=json_data)

    # get template name, output file name
    outputFileName = "documents\\help_en.md"
    print("outputFileName H : " + outputFileName)

    # write output to file
    outFile = open(outputFileName,"w")
    outFile.write(result_h)
    outFile.close()
    
    print("done.")
    