import json
import re
import jinja2

template_h = """<!doctype html>
<html lang="en">
  <head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

    <link href='https://fonts.googleapis.com/css?family=Roboto' rel='stylesheet'>
    <style>
    body {
        font-family: 'Roboto';font-size: 22px;
    }

    h1 {
        border-bottom: 3px solid #000000;
    }
    h2 {
        border-bottom: 1px solid #000000;
    }
    </style>

    <title>SmartElec - settings help</title>
  </head>
  <body>

<h1>Settings manual</h1>

{%- for key, value in parameters.items() %}
    {%- for key2, value2 in value.items() %}
        {%- set outer_loop = loop %}

<h2>{{ key2 }}</h2>
        {%- for  item in value2.settings %}
            {%- if (item.valid_config != "old") %}
<h3>{{ item.smartphone_display_name }}</h3>

                {%- if (item.valid_config == "^sd_.*")  %}
Valid configuration : SmartDisplay<br>
                {%- elif (item.valid_config == "^sc_.*") %}
Valid configuration : SmartController<br>
                {%- else %}
Valid configuration : SmartController / SmartDisplay<br>
                {%- endif %}
                
                {%- if ((item.smartphone_display_type | lower) == "list") %}
Possible values : <br>
                    {%- set list1 = item.list_strings.split('\n')  %}
                    {%- for item4 in list1 %}
                        {%- set var_elem_name = item4 | lower  | replace(" ", "_") | replace("/", "_") | regex_replace("[^A-Za-z0-9_]","") %}
    - {{ item4 }} (value = {{ loop.index - 1 }})<br>

                {%- endfor %}
                {%- endif %}
Default value : {{ item.default }} {%- if item.smartphone_display_unit != "" %} ({{ item.smartphone_display_unit }}) {%- endif %}<br>
<br>
Usage : {{ item.help_en | replace("\n- ", "<br>- ") }}<br>
                {%+ if True %}{% endif %}
            {%- endif %}
        {%- endfor %}
    {%- endfor %}
{%- endfor %}

  </body>
</html>

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
    outputFileName = "documents\\help_settings_en.html"
    print("outputFileName H : " + outputFileName)

    # write output to file
    outFile = open(outputFileName,"w")
    outFile.write(result_h)
    outFile.close()
    
    print("done.")
    