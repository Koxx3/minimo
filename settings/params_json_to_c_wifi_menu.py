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

#include <AutoConnect.h>

/*
// In the declaration,
// Declare AutoConnectElements for the page asf /mqtt_setting
ACText(header, "<h2>MQTT broker settings</h2>", "text-align:center;color:#2f4f4f;padding:10px;");
ACText(test1, "aaaaaaaaaaaaaaaaaaaaaaaaaaa", "text-align:center;color:#2f4f4f;padding:10px;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;");
ACText(test2, "aaaaaaaaaaaaaaaaaaaaaaaaaaa", "text-align:center;color:#2f4f4f;padding:10px;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;");
ACText(test3, "aaaaaaaaaaaaaaaaaaaaaaaaaaa", "text-align:center;color:#2f4f4f;padding:10px;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;");
ACText(test4, "aaaaaaaaaaaaaaaaaaaaaaaaaaa", "text-align:center;color:#2f4f4f;padding:10px;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;");
ACText(test5, "aaaaaaaaaaaaaaaaaaaaaaaaaaa", "text-align:center;color:#2f4f4f;padding:10px;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;text-align:center;color:#2f4f4f;padding:10px;text-align:center;");
ACInput(mqttserver, "", "Server", "^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\\-]*[A-Za-z0-9])$", "MQTT broker server");
ACInput(channelid, "", "Channel ID", "^[0-9]{6}$");
ACInput(userkey, "", "User Key");
ACInput(apikey, "", "API Key");
ACElement(newline, "<hr>");
ACCheckbox(uniqueid, "unique", "Use APID unique");
ACRadio(period, {"30 sec.", "60 sec.", "180 sec."}, "Update period", AC_Vertical, 1);
ACSubmit(save, "Save", "/save");
ACSubmit(discard, "Discard", "/elements");

// Declare the custom Web page as /mqtt_setting and contains the AutoConnectElements
AutoConnectAux elementsAux("/elements", "MQTT Setting", true, {header, test1, test2, test3, test4, test5, mqttserver, channelid, userkey, apikey, newline, uniqueid, period, newline, save, discard});

*/

//-----------------------
// variables and lists
//-----------------------

/* elements */
{%- for key, value in parameters.items() %}
    {%- for key2, value2 in value.items() %}
ACText(ACE_{{ key2 | replace(" ", "_") | title }}, "<h2>{{ key2 }}</h2>", "color:#303F9F;padding:10px;");
        {%- for  item in value2.settings %}

                {%- if item.smartphone_display_type == "edit_text_number_float" %}
ACInput(ACE_{{ item.var_name }}, "{{ item.default }}", "{{ item.tft_display_name }}" {%- if item.regex_valid != "" %}, "{{item.regex_valid}}"{%- endif %}, "edit_text_number_float", AC_Tag_BR, AC_Input_Text);   
                {%- endif %}
                {%- if item.smartphone_display_type == "edit_text_number_integer" %}
ACInput(ACE_{{ item.var_name }}, "{{ item.default }}", "{{ item.tft_display_name }}" {%- if item.regex_valid != "" %}, "{{item.regex_valid}}"{%- endif %}, "edit_text_number_integer", AC_Tag_BR, AC_Input_Text);   
                {%- endif %}
                {%- if item.smartphone_display_type == "edit_text_number_integer_signed" %}
ACInput(ACE_{{ item.var_name }}, "{{ item.default }}", "{{ item.tft_display_name }}" {%- if item.regex_valid != "" %}, "{{item.regex_valid}}"{%- endif %}, "edit_text_number_integer_signed", AC_Tag_BR, AC_Input_Text);        
                {%- endif %}
                {%- if item.smartphone_display_type == "edit_text_string" %}
ACInput(ACE_{{ item.var_name }}, "{{ item.default }}", "{{ item.tft_display_name }}" {%- if item.regex_valid != "" %}, "{{item.regex_valid}}"{%- endif %}, "edit_text_string", AC_Tag_BR, AC_Input_Text);     
                {%- endif %}
                {%- if item.smartphone_display_type == "edit_text_password" %}
ACInput(ACE_{{ item.var_name }}, "{{ item.default }}", "{{ item.tft_display_name }}" {%- if item.regex_valid != "" %}, "{{item.regex_valid}}"{%- endif %}, "edit_text_password", AC_Tag_BR, AC_Input_Password);        
                {%- endif %}
                {%- if item.smartphone_display_type == "seek_bar" %}
ACInput(ACE_{{ item.var_name }}, "{{ item.default }}", "{{ item.tft_display_name }}" {%- if item.regex_valid != "" %}, "{{item.regex_valid}}"{% else %}, "" {%- endif %}, "", AC_Tag_BR, AC_Input_Text);  
                {%- endif %}
                {%- if item.smartphone_display_type == "checkbox" %}
ACCheckbox(ACE_{{ item.var_name }}, "unique", "{{ item.tft_display_name }}", {%- if item.default | int == 1 %} "checked" {%- else %} "unchecked" {%- endif %}, AC_Infront);   
                {%- endif %}
                {%- if item.smartphone_display_type | lower == "list" %}
                    {%- set list1 = item.list_strings | replace('\\n', '","') %}
ACSelect(ACE_{{ item.var_name }}, {"{{ list1 }}"}, "{{ item.tft_display_name }}", {{ item.default }});
                {%- endif %}
        {%- endfor %}
    {%- endfor %}
{%- endfor %}

ACSubmit(ACE_Save, "Save", "/save");
ACSubmit(ACE_Discard, "Discard", "/elements");

/* menus */
AutoConnectAux elementsAux("/elements", "Setting", true, {
    ACE_Style1,
    ACE_Style2,
    ACE_Style3,
    ACE_Style4,
{%- for key, value in parameters.items() %}
    {%- for key2, value2 in value.items() %}
    ACE_{{ key2 | replace(" ", "_") | title }},
        {%- for  item in value2.settings %}
    ACE_{{ item.var_name }},
        {%- endfor %}
    {%- endfor %}
{%- endfor %}
    ACE_Discard, 
    ACE_Save,
    ACE_Js1 });        


"""


# Custom filter method
def regex_replace(s, find, replace):
    """A non-optimal implementation of a regex filter"""
    return re.sub(find, replace, s)

env = jinja2.Environment()
env.filters['regex_replace'] = regex_replace

tmpl_h = env.from_string(template_h)


# load json from file
jsonConfigName = "settings\\params.json"
print ("jsonConfigName: " + jsonConfigName)

with open(jsonConfigName) as json_file:
    json_data = json.load(json_file)

    # merge template with data
    result_h = tmpl_h.render(parameters=json_data)

    # get template name, output file name
    outputFileName = "src\\WifiSettingsPortal\\WifiSettingsPortalSpecs.h"
    print("outputFileName H : " + outputFileName)

    # write output to file
    outFile = open(outputFileName,"w")
    outFile.write(result_h)
    outFile.close()
    
    print("done.")
    