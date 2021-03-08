import json
import re
import jinja2


template = """
package org.koxx.smartcntrl.settings;

import android.content.Context;
import android.text.InputType;
import com.hotmail.or_dvir.easysettings.pojos.CheckBoxSettingsObject;
import com.hotmail.or_dvir.easysettings.pojos.EasySettings;
import com.hotmail.or_dvir.easysettings.pojos.HeaderSettingsObject;
import com.hotmail.or_dvir.easysettings.pojos.SeekBarSettingsObject;
import com.hotmail.or_dvir.easysettings.pojos.SettingsObject;
import com.hotmail.or_dvir.easysettings_dialogs.pojos.EditTextSettingsObject;
import com.hotmail.or_dvir.easysettings_dialogs.pojos.ListSettingsObject;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.ArrayList;

public class SmartElecSettings2 {

{#- run 1 - description strings #}
{%- for key, value in parameters.items() %}


    {%- for key2, value2 in value.items() %}

    public static final String {{ key2 | replace(" ", "_") | title }} = "{{ key2 }}";

        {%- for  item in value2.settings %}
            {%- set var_name = item.display_name | lower  | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") %}
    public static final String {{ var_name | title }} = "{{ item.display_name }}";

            {%- if item.smartphone_display_type | lower == "list" %}
                {%- set list1 = item.list_strings.split('\\n') %}
                {%- for item4 in list1 %}
    public static final String {{ var_name |title}}_LIST_{{ item4 | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") |title}} = "{{ item4 }}";
                {%- endfor %}
            {%- endif %}

        {%- endfor %}

    {%- endfor %}
{% endfor %}

    public static ArrayList<SettingsObject> initialize() {

{#- run 2 - array list init #}
{%- for key, value in parameters.items() %}
    {%- for key2, value2 in value.items() %}

        {%- for  item in value2.settings %}
            {%- set var_name = item.display_name | lower  | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") %}
                {%- if item.smartphone_display_type | lower == "list" %}
                {%- set list1 = item.list_strings.split('\n')  %}

        ArrayList<String> {{ item.display_name | replace(" ", "_") |title}}_LIST = new ArrayList<>();;
                {%- for item4 in list1 %}
                    {%- set var_elem_name = item4 | lower  | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") %}
        {{ item.display_name | replace(" ", "_") |title}}_LIST.add({{ item.display_name | replace(" ", "_") |title}}_LIST_{{ var_elem_name | replace(" ", "_") |title}} );
                {%- endfor %}
            {% endif %}

        {%- endfor %}

    {%- endfor %}
{%- endfor %}


{#- run 3 - menu builder #}
        ArrayList<SettingsObject> settings = EasySettings.createSettingsArray(
{%- for key, value in parameters.items() %}
    {%- for key2, value2 in value.items() %}

// ----------------------

                new HeaderSettingsObject.Builder({{ key2 | replace(" ", "_") | title }})
                        .build(),

        {%- for  item in value2.settings %}
            {%- set var_name = item.display_name | lower  | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") | title %}
            {%- if item.smartphone_display_type | lower == "list" %}
            
                {%- set list1 = item.list_strings.split('\n')  %}
                {%- set list_default = list1[item.default] | lower  | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") | title %}
                new ListSettingsObject.Builder({{ var_name }}, {{ var_name }}, {{ var_name }}_LIST_{{ list_default }}, {{ var_name | title }}_LIST , "save")
                        .setUseValueAsSummary()
                        .setNegativeBtnText("cancel")
                        .build(),

                {%- set list1 = item.list_strings.split('\n')  %}
                {%- for item4 in list1 %}
                    {%- set var_elem_name = item4 | lower  | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") %}
                    
                {%- endfor %}
            {% endif %}

            {%- if item.smartphone_display_type | lower == "edit_text_number_float" %}
                new EditTextSettingsObject.Builder({{ var_name }}, {{ var_name }}, "{{ item.default }}", "save")
                        .setDialogTitle("{{ item.display_unit }}")
                        .setUseValueAsPrefillText()
                        .setNegativeBtnText("cancel")
                        .setInputType(InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_FLAG_DECIMAL)
                        .setUseValueAsSummary()
                        .build(),
            {% endif %}
            {%- if item.smartphone_display_type | lower == "edit_text_number_integer" %}
                new EditTextSettingsObject.Builder({{ var_name }}, {{ var_name }}, "{{ item.default }}", "save")
                        .setDialogTitle("{{ item.display_unit }}")
                        .setUseValueAsPrefillText()
                        .setNegativeBtnText("cancel")
                        .setInputType(InputType.TYPE_CLASS_NUMBER)
                        .setUseValueAsSummary()
                        .build(),
            {% endif %}
            {%- if item.smartphone_display_type | lower == "edit_text_number_integer_signed" %}
                new EditTextSettingsObject.Builder({{ var_name }}, {{ var_name }}, "{{ item.default }}", "save")
                        .setDialogTitle("{{ item.display_unit }}")
                        .setUseValueAsPrefillText()
                        .setNegativeBtnText("cancel")
                        .setInputType(InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_FLAG_SIGNED)
                        .setUseValueAsSummary()
                        .build(),
            {% endif %}
            {%- if item.smartphone_display_type | lower == "edit_text_string" %}            
                new EditTextSettingsObject.Builder({{ var_name }}, {{ var_name }}, "{{ item.default }}", "save")
                        .setDialogTitle("{{ item.display_hint }}")
                        .setUseValueAsPrefillText()
                        .setNegativeBtnText("cancel")
                        .setRegex("{{ item.regex_valid }}")
                        .setInputType(InputType.TYPE_CLASS_TEXT)
                        .setUseValueAsSummary()
                        .build(),
            {% endif %}
            {%- if item.smartphone_display_type | lower == "seek_bar" %}
                new SeekBarSettingsObject.Builder({{ var_name }}, {{ var_name }}, {{ item.default }}, {{ item.min }}, {{ item.max }}, {{ item.fast_increment }})
                        .setUseValueAsSummary()
                        .build(),
            {% endif %}
            {%- if item.smartphone_display_type | lower == "checkbox" %}
                new CheckBoxSettingsObject.Builder({{ var_name }}, {{ var_name }}, {{ "true" if item.default else "false" }})
                        .setOffText("off")
                        .setOnText("on")
                        .build(),
            {% endif %}
            


        {%- endfor %}

                new HeaderSettingsObject.Builder("")
                        .addDivider()
                        .build(),

    {%- endfor %}
{%- endfor %}
                new HeaderSettingsObject.Builder("")
                        .build()
        );
        return settings;
    }

}
"""
# Custom filter method
def regex_replace(s, find, replace):
    """A non-optimal implementation of a regex filter"""
    return re.sub(find, replace, s)

env = jinja2.Environment()
env.filters['regex_replace'] = regex_replace

tmpl = env.from_string(template)

# load json from file
jsonConfigName = "params_list.json"
print ("jsonConfigName: " + jsonConfigName)
with open(jsonConfigName) as json_file:
    json_data = json.load(json_file)
    print(json_data)

    print("================================================")

    # merge template with data
    result = tmpl.render(parameters=json_data)

    # get template name, output file name
    outputFileName = "C:\\Users\\Francois\\StudioProjects\\minimo_android\\app\\src\\main\\java\\org\\koxx\\smartcntrl\\settings\\SmartElecSettings2.java"
    print("outputFileName: " + outputFileName)
    print (result)

    # write output to file
    outFile = open(outputFileName,"w")
    outFile.write(result)
    outFile.close()