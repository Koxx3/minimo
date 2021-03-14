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
ACCheckbox(ACE_{{ item.var_name }}, "ACE_{{ item.var_name }}", "{{ item.tft_display_name }}", false, AC_Infront);   
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


/* save */
void saveConfig(AutoConnectAux &aux)
{

{%- for key, value in parameters.items() %}
    {%- for key2, value2 in value.items() %}
        {%- for  item in value2.settings %}

                {%- if item.smartphone_display_type == "edit_text_number_float" %}
    WifiSettingsPortal_settings->set_{{ item.var_name }}((elementsAux["ACE_{{ item.var_name }}"].as<AutoConnectInput>()).value.toFloat());
                {%- elif item.smartphone_display_type == "edit_text_number_integer" %}
    WifiSettingsPortal_settings->set_{{ item.var_name }}((elementsAux["ACE_{{ item.var_name }}"].as<AutoConnectInput>()).value.toInt());
                {%- elif item.smartphone_display_type == "edit_text_number_integer_signed" %}
    WifiSettingsPortal_settings->set_{{ item.var_name }}((elementsAux["ACE_{{ item.var_name }}"].as<AutoConnectInput>()).value.toInt());
                {%- elif item.smartphone_display_type == "edit_text_string" %}
    WifiSettingsPortal_settings->set_{{ item.var_name }}((elementsAux["ACE_{{ item.var_name }}"].as<AutoConnectInput>()).value);
                {%- elif item.smartphone_display_type == "edit_text_password" %}
    WifiSettingsPortal_settings->set_{{ item.var_name }}((elementsAux["ACE_{{ item.var_name }}"].as<AutoConnectInput>()).value);
                {%- elif item.smartphone_display_type == "seek_bar" %}
    WifiSettingsPortal_settings->set_{{ item.var_name }}((elementsAux["ACE_{{ item.var_name }}"].as<AutoConnectInput>()).value.toInt());
                {%- elif item.smartphone_display_type == "checkbox" %}
    WifiSettingsPortal_settings->set_{{ item.var_name }}((elementsAux["ACE_{{ item.var_name }}"].as<AutoConnectCheckbox>()).checked ? 1 : 0);
    Serial.println("{{ item.var_name }} = " + (String) (elementsAux["ACE_{{ item.var_name }}"].as<AutoConnectCheckbox>()).checked);
                {%- elif item.smartphone_display_type | lower == "list" %}
    WifiSettingsPortal_settings->set_{{ item.var_name }}((elementsAux["ACE_{{ item.var_name }}"].as<AutoConnectSelect>()).selected - 1);
                {%- endif %}
        {%- endfor %}
    {%- endfor %}
{%- endfor %}
    WifiSettingsPortal_settings->save();
    WifiSettingsPortal_settings->restore();
    
}

/* load */

void loadConfig(AutoConnectAux &aux)
{


{%- for key, value in parameters.items() %}
    {%- for key2, value2 in value.items() %}
        {%- for  item in value2.settings %}



            {%- if item.smartphone_display_type == "edit_text_number_float" %}
    aux.setElementValue("ACE_{{ item.var_name }}", (String)WifiSettingsPortal_settings->get_{{ item.var_name }}());
            {%- elif item.smartphone_display_type == "edit_text_number_integer" %}
    aux.setElementValue("ACE_{{ item.var_name }}", (String)WifiSettingsPortal_settings->get_{{ item.var_name }}());
            {%- elif item.smartphone_display_type == "edit_text_number_integer_signed" %}
    aux.setElementValue("ACE_{{ item.var_name }}", (String)WifiSettingsPortal_settings->get_{{ item.var_name }}());
            {%- elif item.smartphone_display_type == "edit_text_string" %}
    aux.setElementValue("ACE_{{ item.var_name }}", (String)WifiSettingsPortal_settings->get_{{ item.var_name }}());
            {%- elif item.smartphone_display_type == "edit_text_password" %}
    aux.setElementValue("ACE_{{ item.var_name }}", (String)WifiSettingsPortal_settings->get_{{ item.var_name }}());
            {%- elif item.smartphone_display_type == "seek_bar" %}
    aux.setElementValue("ACE_{{ item.var_name }}", (String)WifiSettingsPortal_settings->get_{{ item.var_name }}());
            {%- elif item.smartphone_display_type == "checkbox" %}
    aux.setElementValue("ACE_{{ item.var_name }}", WifiSettingsPortal_settings->get_{{ item.var_name }}() ? "checked" : "");
            {%- elif item.smartphone_display_type | lower == "list" %}
                {%- set list1 = item.list_strings.split('\\n') %}
    uint8_t val_{{ item.var_name }} = WifiSettingsPortal_settings->get_{{ item.var_name }}();
                {%- for item4 in list1 %}
    if (val_{{ item.var_name }} == {{ loop.index - 1 }})
        aux.setElementValue("ACE_{{ item.var_name }}", "{{ item4 }}");
                {%- endfor %}
            {%- endif %}
        {%- endfor %}
    {%- endfor %}
{%- endfor %}

      /*
      aux.setElementValue("uniqueid", "checked");
      aux.setElementValue("channelid", "15");
      aux.setElementValue("select", "1");
      */
}

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
    