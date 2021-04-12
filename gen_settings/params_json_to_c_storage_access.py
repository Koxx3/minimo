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

#ifndef _Settings_h
#define _Settings_h

class Settings
{
private : 
    Preferences prefs;

public:

    Settings();

    void restore();
    void save();

    void unpack_setting_packet(uint8_t* packet, uint8_t length);
    bool pack_setting_packet(uint16_t settingId, uint16_t packetNumber, uint8_t* packet, int32_t* length);

{% for key, value in parameters.items() %}
    {% for key2, value2 in value.items() %}
        {%- for  item in value2.settings %}
        
        {%- if item.type | lower == "string" %}
            {% set var_type = "String" %}
        {%- else %}
            {% set var_type = item.type %}
        {%- endif %}
    /*-------------------------------------------------------*/

    #define SETTINGS_{{ item.var_name | upper }}_BLE_ID {{ item.ble_id }}
    #define SETTINGS_{{ item.var_name | upper }}_STORAGE_KEY "{{ item.var_name_hash }}"
    #define SETTINGS_{{ item.var_name | upper }}_NAME "{{ item.var_name }}"

    {{ var_type }} {{ item.var_name }};

        {%- if item.smartphone_display_type | lower == "list" %}
    typedef enum LIST_{{ item.var_name }} {
            {%- set list1 = item.list_strings.split('\n') %}
            {%- for item4 in list1 %}
        LIST_{{ item.var_name }}_{{ item4 | replace(" ", "_") | replace("/", "_") | regex_replace("[^A-Za-z0-9_]","") |title}},
            {%- endfor %} 
    } t{{ item.var_name }};
        {%- else %}
        {%- endif %}

    void set_{{ item.var_name }} ({{ var_type }} value);
    {{ var_type }} get_{{ item.var_name }}();
    void display_{{ item.var_name }}();
    void save_{{ item.var_name }}({{ var_type }} value);

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
#include "Settings.h"
#include "tools/buffer.h"

#define SETTINGS_STORAGE "SETTINGS"

Settings::Settings()
{
}

void Settings::restore() {

    Serial.println(" > restore settings");
    prefs.begin(SETTINGS_STORAGE, false);

{%- for key, value in parameters.items() %}
    {%- for key2, value2 in value.items() %}
        {%- for  item in value2.settings %}
            {%- if item.type | lower == "string" %}
    {{ item.var_name }} = prefs.getString(SETTINGS_{{ item.var_name | upper }}_STORAGE_KEY, "{{ item.default }}");
            {%- elif item.type | lower == "float" %}
    {{ item.var_name }} = prefs.getFloat(SETTINGS_{{ item.var_name | upper }}_STORAGE_KEY, {{ item.default }});
            {%- else %}
    {{ item.var_name }} = prefs.getInt(SETTINGS_{{ item.var_name | upper }}_STORAGE_KEY, {{ item.default  | int}});
            {%- endif %}
    Serial.println("  >> {{ item.var_name }} = " + (String){{ item.var_name }});
        {%- endfor %}
    {%- endfor %}
{%- endfor %}
    prefs.end();
    Serial.println(" < restore settings");
}

void Settings::save() {

    prefs.begin(SETTINGS_STORAGE, false);
{%- for key, value in parameters.items() %}
    {%- for key2, value2 in value.items() %}
        {%- for  item in value2.settings %}
            {%- if item.type | lower == "string" %}
                {%- set var_type = "String" %}
            {%- else %}
                {%- set var_type = item.type %}
            {%- endif %}
            {%- if item.type | lower == "string" %}
    prefs.putString(SETTINGS_{{ item.var_name | upper }}_STORAGE_KEY, {{ item.var_name }});
            {%- elif item.type | lower == "float" %}
    prefs.putFloat(SETTINGS_{{ item.var_name | upper }}_STORAGE_KEY, {{ item.var_name }});
            {%- else %}
    prefs.putInt(SETTINGS_{{ item.var_name | upper }}_STORAGE_KEY, {{ item.var_name }});
            {%- endif %}
        {%- endfor %}
    {%- endfor %}
{%- endfor %}
    prefs.end();

}

void Settings::unpack_setting_packet(uint8_t* packet, uint8_t length) {
    
    int32_t ind = 0;

    uint16_t settingId = buffer_get_uint16(packet, &ind);
    uint16_t packetNumber = buffer_get_uint16(packet, &ind);

    //Serial.println("settingId = " + (String)settingId);
    //Serial.println("packetNumber = " + (String)packetNumber);

    switch(settingId) {
{%- for key, value in parameters.items() %}
{%- for key2, value2 in value.items() %}
    {%- for  item in value2.settings %}
    case SETTINGS_{{ item.var_name | upper }}_BLE_ID :
            {%- if item.type == "uint8_t" %}
        set_{{ item.var_name  }}(buffer_get_uint8(packet, &ind));
            {%- elif item.type == "int8_t" %}
        set_{{ item.var_name  }}(buffer_get_int8(packet, &ind));
            {%- elif item.type == "uint16_t" %}
        set_{{ item.var_name  }}(buffer_get_uint16(packet, &ind));
            {%- elif item.type == "int16_t" %}
        set_{{ item.var_name  }}(buffer_get_int16(packet, &ind));
            {%- elif item.type == "uint32_t" %}
        set_{{ item.var_name  }}(buffer_get_uint32(packet, &ind));
            {%- elif item.type == "int32_t" %}
        set_{{ item.var_name  }}(buffer_get_int32(packet, &ind));
            {%- elif item.type == "float" %}
        set_{{ item.var_name  }}(buffer_get_float32_auto(packet, &ind));
            {%- elif item.type == "string" %}
        char {{ item.var_name  }}_part[17];
        memset({{ item.var_name  }}_part, 0, 17 );
        if (packetNumber == 0) {
            {{ item.var_name  }} = "";
        }
        memcpy({{ item.var_name  }}_part, &packet[ind], length  - 4);
        {{ item.var_name  }} =  {{ item.var_name  }} + {{ item.var_name  }}_part;
        set_{{ item.var_name  }}( {{ item.var_name  }});
            {%- endif %}
        //Serial.print("unpack_setting_packet - {{ item.var_name  }} : " + (String) {{ item.var_name  }} + " / ");
        //buffer_display("", packet, length);
        break;
    {%- endfor %}
{%- endfor %}
{%- endfor %}
    default:
        Serial.println("unpack_setting_packet : ID error");
        break;
    }
}

bool Settings::pack_setting_packet(uint16_t settingId, uint16_t packetNumber, uint8_t* packet, int32_t* ind) {
    
    bool hasNextPacket = false;

    buffer_append_uint16(packet, settingId, ind);
    buffer_append_uint16(packet, packetNumber, ind);;

    //Serial.println("settingId = " + (String)settingId);
    //Serial.println("packetNumber = " + (String)packetNumber);

    switch(settingId) {
{%- for key, value in parameters.items() %}
{%- for key2, value2 in value.items() %}
    {%- for  item in value2.settings %}
    case SETTINGS_{{ item.var_name | upper }}_BLE_ID :
            {%- if item.type == "uint8_t" %}
        buffer_append_uint8(packet, {{ item.var_name  }}, ind);
            {%- elif item.type == "int8_t" %}
        buffer_append_int8(packet, {{ item.var_name  }}, ind);
            {%- elif item.type == "uint16_t" %}
        buffer_append_uint16(packet, {{ item.var_name  }}, ind);
            {%- elif item.type == "int16_t" %}
        buffer_append_int16(packet, {{ item.var_name  }}, ind);
            {%- elif item.type == "uint32_t" %}
        buffer_append_uint32(packet, {{ item.var_name  }}, ind);
            {%- elif item.type == "int32_t" %}
        buffer_append_int32(packet, {{ item.var_name  }}, ind);
            {%- elif item.type == "float" %}
        buffer_append_float32_auto(packet, {{ item.var_name  }}, ind);
            {%- elif item.type == "string" %}
        if (packetNumber == 0) {
            if ({{ item.var_name  }}.length() > 16) {
                hasNextPacket = true;
                memcpy(&packet[*ind], &{{ item.var_name  }}[0], 16);
                *ind = *ind + 16;
            }
            else {
                memcpy(&packet[*ind], &{{ item.var_name  }}[0], {{ item.var_name  }}.length());
                *ind = *ind + {{ item.var_name  }}.length();
            }
        }
        else if (packetNumber == 1) {
            memcpy(&packet[*ind], &{{ item.var_name  }}[16], {{ item.var_name  }}.length() - 16);
            *ind = *ind + {{ item.var_name  }}.length() - 16;
        }
            {%- endif %}
        //Serial.print("pack_setting_packet - {{ item.var_name  }} : " + (String) {{ item.var_name  }} + " / ");
        //buffer_display("", packet, *ind);
        break;
    {%- endfor %}
{%- endfor %}
{%- endfor %}
    default:
        Serial.println("pack_setting_packet : ID error");
        break;
    }
    
    return hasNextPacket;
}


{%- for key, value in parameters.items() %}
    {%- for key2, value2 in value.items() %}
        {%- for  item in value2.settings %}        
            {%- if item.type | lower == "string" %}
                {% set var_type = "String" %}
            {%- else %}
                {% set var_type = item.type %}
            {%- endif %}

/*-------------------------------------------------------*/

void Settings::set_{{ item.var_name }}({{ var_type }} value) {
    {{ item.var_name }} = value;
}

{{ var_type }} Settings::get_{{ item.var_name }}() {
    return {{ item.var_name }} ;
}

void Settings::display_{{ item.var_name }}() {
    Serial.println("  {{ item.var_name }} = " + (String) {{ item.var_name }});
}

void Settings::save_{{ item.var_name }}({{ var_type }} value) {
    prefs.begin(SETTINGS_STORAGE, false);
            {%- if item.type | lower == "string" %}
    prefs.putString(SETTINGS_{{ item.var_name | upper }}_STORAGE_KEY, {{ item.var_name }});
            {%- elif item.type | lower == "float" %}
    prefs.putFloat(SETTINGS_{{ item.var_name | upper }}_STORAGE_KEY, {{ item.var_name }});
            {%- else %}
    prefs.putInt(SETTINGS_{{ item.var_name | upper }}_STORAGE_KEY, {{ item.var_name }});
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
jsonConfigName = "gen_settings\\params.json"
print ("jsonConfigName: " + jsonConfigName)
with open(jsonConfigName) as json_file:
    json_data = json.load(json_file)

    # merge template with data
    result_cpp = tmpl_cpp.render(parameters=json_data)

    # get template name, output file name
    outputFileName = ".\\src\\Settings.cpp"
    print("outputFileName CPP : " + outputFileName)

    # write output to file
    outFile = open(outputFileName,"w")
    outFile.write(result_cpp)
    outFile.close()

    # merge template with data
    result_h = tmpl_h.render(parameters=json_data)

    # get template name, output file name
    outputFileName = ".\\src\\Settings.h"

    # write output to file
    outFile = open(outputFileName,"w")
    outFile.write(result_h)
    outFile.close()
    
    print("done.")