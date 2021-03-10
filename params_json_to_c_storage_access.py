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

#include "Arduino.h"
#include <Preferences.h>

#ifndef _Settings2_h
#define _Settings2_h

class Settings2
{
private : 
    Preferences prefs;

public:

    Settings2();

    void restore();
    void save();

    void unpack_setting_packet(uint8_t* packet, uint8_t length);
    void pack_setting_packet(uint16_t settingId, uint16_t packetNumber, uint8_t* packet, int32_t* length);

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
    void save_{{ var_name }}({{ var_type }} value);

        {%- endfor %}
    {%- endfor %}
{%- endfor %}

};

#endif
"""

template_cpp = """
//////------------------------------------
//////------------------------------------
//////------------------------------------
////// GENERATED FILE - DO NOT EDIT MANUALLY
//////------------------------------------
//////------------------------------------
//////------------------------------------

////// Manage settings exchanged in BLE and stored in EEPOM

#include "Arduino.h"
#include "Settings2.h"
#include "tools/buffer.h"

#define SETTINGS_STORAGE "SETTINGS"

Settings2::Settings2()
{
}

void Settings2::restore() {

    Serial.println(" > restore settings");
    prefs.begin(SETTINGS_STORAGE, false);

{%- for key, value in parameters.items() %}
    {%- for key2, value2 in value.items() %}
        {%- for  item in value2.settings %}
            {%- set var_name = item.display_name | lower  | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") %}
            {%- if item.type | lower == "string" %}
    {{ var_name }} = prefs.getString(SETTINGS_{{ var_name | upper }}_ID_STR, "{{ item.default }}");
            {%- elif item.type | lower == "float" %}
    {{ var_name }} = prefs.getFloat(SETTINGS_{{ var_name | upper }}_ID_STR, {{ item.default }});
            {%- else %}
    {{ var_name }} = prefs.getInt(SETTINGS_{{ var_name | upper }}_ID_STR, {{ item.default  | int}});
            {%- endif %}
    Serial.println("  >> {{ var_name }} = " + (String){{ var_name }});
        {%- endfor %}
    {%- endfor %}
{%- endfor %}
    prefs.end();
    Serial.println(" < restore settings");
}

void Settings2::save() {

    prefs.begin(SETTINGS_STORAGE, false);
{%- for key, value in parameters.items() %}
    {%- for key2, value2 in value.items() %}
        {%- for  item in value2.settings %}
            {%- set var_name = item.display_name | lower  | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") %}
            {%- if item.type | lower == "string" %}
                {%- set var_type = "String" %}
            {%- else %}
                {%- set var_type = item.type %}
            {%- endif %}
            {%- if item.type | lower == "string" %}
    prefs.putString(SETTINGS_{{ var_name | upper }}_ID_STR, {{ var_name }});
            {%- elif item.type | lower == "float" %}
    prefs.putFloat(SETTINGS_{{ var_name | upper }}_ID_STR, {{ var_name }});
            {%- else %}
    prefs.putInt(SETTINGS_{{ var_name | upper }}_ID_STR, {{ var_name }});
            {%- endif %}
        {%- endfor %}
    {%- endfor %}
{%- endfor %}

    prefs.end();

}

void Settings2::unpack_setting_packet(uint8_t* packet, uint8_t length) {
    
    int32_t ind = 0;

    uint16_t settingId = buffer_get_uint16(packet, &ind);
    uint16_t packetNumber = buffer_get_uint16(packet, &ind);

    //Serial.println("settingId = " + (String)settingId);
    //Serial.println("packetNumber = " + (String)packetNumber);

    switch(settingId) {
{%- for key, value in parameters.items() %}
{%- for key2, value2 in value.items() %}
    {%- for  item in value2.settings %}
        {%- set var_name = item.display_name | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") | title %}
    case SETTINGS_{{ var_name | upper }}_ID :
            {%- if item.type == "uint8_t" %}
        set_{{ var_name | lower }}(buffer_get_uint8(packet, &ind));
            {%- elif item.type == "int8_t" %}
        set_{{ var_name | lower }}(buffer_get_int8(packet, &ind));
            {%- elif item.type == "uint16_t" %}
        set_{{ var_name | lower }}(buffer_get_uint16(packet, &ind));
            {%- elif item.type == "int16_t" %}
        set_{{ var_name | lower }}(buffer_get_int16(packet, &ind));
            {%- elif item.type == "uint32_t" %}
        set_{{ var_name | lower }}(buffer_get_uint32(packet, &ind));
            {%- elif item.type == "int32_t" %}
        set_{{ var_name | lower }}(buffer_get_int32(packet, &ind));
            {%- elif item.type == "float" %}
        set_{{ var_name | lower }}(buffer_get_float32_auto(packet, &ind));
            {%- elif item.type == "string" %}
        char {{ var_name | lower }}_part[17];
        memset({{ var_name | lower }}_part, 0, 17 );
        if (packetNumber == 0) {
            {{ var_name | lower }} = "";
        }
        memcpy({{ var_name | lower }}_part, &packet[ind], length  - 4);
        {{ var_name | lower }} =  {{ var_name | lower }} + {{ var_name | lower }}_part;
        set_{{ var_name | lower }}( {{ var_name | lower }});
            {%- endif %}
        Serial.print("unpack_setting_packet - {{ var_name | lower }} : " + (String) {{ var_name | lower }} + " / ");
        buffer_display("", packet, length);
        break;
    {%- endfor %}
{%- endfor %}
{%- endfor %}
    default:
        Serial.println("unpack_setting_packet : ID error");
        break;
    }
}

void Settings2::pack_setting_packet(uint16_t settingId, uint16_t packetNumber, uint8_t* packet, int32_t* ind) {
    
    buffer_append_uint16(packet, settingId, ind);
    buffer_append_uint16(packet, packetNumber, ind);;

    //Serial.println("settingId = " + (String)settingId);
    //Serial.println("packetNumber = " + (String)packetNumber);

    switch(settingId) {
{%- for key, value in parameters.items() %}
{%- for key2, value2 in value.items() %}
    {%- for  item in value2.settings %}
        {%- set var_name = item.display_name | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") | title %}
    case SETTINGS_{{ var_name | upper }}_ID :
            {%- if item.type == "uint8_t" %}
        buffer_append_uint8(packet, {{ var_name | lower }}, ind);
            {%- elif item.type == "int8_t" %}
        buffer_append_int8(packet, {{ var_name | lower }}, ind);
            {%- elif item.type == "uint16_t" %}
        buffer_append_uint16(packet, {{ var_name | lower }}, ind);
            {%- elif item.type == "int16_t" %}
        buffer_append_int16(packet, {{ var_name | lower }}, ind);
            {%- elif item.type == "uint32_t" %}
        buffer_append_uint32(packet, {{ var_name | lower }}, ind);
            {%- elif item.type == "int32_t" %}
        buffer_append_int32(packet, {{ var_name | lower }}, ind);
            {%- elif item.type == "float" %}
        buffer_append_float32_auto(packet, {{ var_name | lower }}, ind);
            {%- elif item.type == "string" %}
/*
        char {{ var_name | lower }}_part[17];
        memset({{ var_name | lower }}_part, 0, 17 );
        if (packetNumber == 0) {
            {{ var_name | lower }} = "";
        }
        memcpy({{ var_name | lower }}_part, &packet[ind], length  - 4);
        {{ var_name | lower }} =  {{ var_name | lower }} + {{ var_name | lower }}_part;
        set_{{ var_name | lower }}( {{ var_name | lower }});
*/
            {%- endif %}
        Serial.print("pack_setting_packet - {{ var_name | lower }} : " + (String) {{ var_name | lower }} + " / ");
        buffer_display("", packet, *ind);
        break;
    {%- endfor %}
{%- endfor %}
{%- endfor %}
    default:
        Serial.println("pack_setting_packet : ID error");
        break;
    }
}


{%- for key, value in parameters.items() %}
    {%- for key2, value2 in value.items() %}
        {%- for  item in value2.settings %}
            {%- set var_name = item.display_name | lower  | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") %}
        
            {%- if item.type | lower == "string" %}
                {% set var_type = "String" %}
            {%- else %}
                {% set var_type = item.type %}
            {%- endif %}

/*-------------------------------------------------------*/

void Settings2::set_{{ var_name }}({{ var_type }} value) {
    {{ var_name }} = value;
}

{{ var_type }} Settings2::get_{{ var_name }}() {
    return {{ var_name }} ;
}

void Settings2::display_{{ var_name }}() {
    Serial.println("  {{ var_name }} = " + (String) {{ var_name }});
}

void Settings2::save_{{ var_name }}({{ var_type }} value) {
    prefs.begin(SETTINGS_STORAGE, false);
            {%- if item.type | lower == "string" %}
    prefs.putString(SETTINGS_{{ var_name | upper }}_ID_STR, {{ var_name }});
            {%- elif item.type | lower == "float" %}
    prefs.putFloat(SETTINGS_{{ var_name | upper }}_ID_STR, {{ var_name }});
            {%- else %}
    prefs.putInt(SETTINGS_{{ var_name | upper }}_ID_STR, {{ var_name }});
            {%- endif %}
    prefs.end();
}
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

tmpl_cpp = env.from_string(template_cpp)
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
    outputFileName = ".\\src\\Settings2.cpp"
    print("outputFileName CPP : " + outputFileName)
    print (result_cpp)

    # write output to file
    outFile = open(outputFileName,"w")
    outFile.write(result_cpp)
    outFile.close()
    
    print("================================================")

    # merge template with data
    result_h = tmpl_h.render(parameters=json_data)

    # get template name, output file name
    outputFileName = ".\\src\\Settings2.h"
    print("outputFileName H : " + outputFileName)
    print (result_h)

    # write output to file
    outFile = open(outputFileName,"w")
    outFile.write(result_h)
    outFile.close()