import json
from jinja2 import Template

template = """{% for key, value in parameters.items() %}
    {% for key2, value2 in value.items() %}
        {% for  item in value2.settings %}

        {% set var_name = item.display_name | lower  | replace(" ", "_") %}

/*-------------------------------------------------------*/

#define SETTINGS_{{ item.display_name | upper | replace(" ", "_")}}_ID {{ item.id }}
#define SETTINGS_{{ item.display_name | upper | replace(" ", "_")}}_NAME "{{ item.display_name }}"

{{ item.type }} {{ item.display_name | lower  | replace(" ", "_")}};

{%- if item.smartphone_display_type | lower == "list" %}
typedef enum LIST_{{ item.display_name | replace(" ", "_") |title}} {
    {%- set list1 = item.list_strings.split('\n') %}
    {%- for item4 in list1 %}
    LIST_{{ item.display_name | replace(" ", "_") |title}}_{{ item4 | replace(" ", "_") |title}},
    {%- endfor %} 
} t{{ item.display_name | replace(" ", "_") |title}};
{%- else %}
{%- endif %}

void set_{{ item.display_name | lower  | replace(" ", "_")}} ({{ item.type }} value) {
    {{ var_name }} = value;
}

{{ item.type }} get_{{ item.display_name | lower  | replace(" ", "_")}} () {
    return {{ var_name }} ;
}

void display_{{ item.display_name | lower  | replace(" ", "_")}} () {
    Serial.println("  {{ item.display_name | lower  | replace(" ", "_")}} = " + (String) {{ item.display_name | lower  | replace(" ", "_")}});
}

        {%- endfor %}

    {%- endfor %}
{%- endfor %}"""


tm = Template(template)

# load json from file
jsonConfigName = "params_list.json"
print ("jsonConfigName: " + jsonConfigName)
with open(jsonConfigName) as json_file:
    json_data = json.load(json_file)
    print(json_data)

    print("================================================")

    # merge template with data
    result = tm.render(parameters=json_data)

    # get template name, output file name
    outputFileName = "test.c"
    print("outputFileName: " + outputFileName)
    print (result)

    # write output to file
    outFile = open(outputFileName,"w")
    outFile.write(result)
    outFile.close()