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

/*

float Wheel_size = 8.5;
int Motor_number_of_magents_pairs=15;
float Battery_minimum_voltage =42.0;
float Battery_maximum_voltage=59.0;
int Battery_distance=40;
int thottleRegen=0;

TOGGLE(thottleRegen,setThottleRegen,"  Throttle signal regeneration: ",doNothing,noEvent,noStyle //
  ,VALUE("On",1,doNothing,noEvent) //
  ,VALUE("Off",0,doNothing,noEvent) //
);

MENU(escooterCharac,"  E-scooter characteristics",doNothing,noEvent,noStyle //
  ,altFIELD(decPlaces<1>::menuField, Wheel_size, "  Wheel size (inches)","",8.5,13.0,0.5,0.1,doNothing,anyEvent,wrapStyle) //
  ,FIELD(Motor_number_of_magents_pairs,"  Motor number of magents pairs","",5,20,1,0,doNothing,noEvent,wrapStyle) //
  ,altFIELD(decPlaces<1>::menuField, Battery_minimum_voltage,"  Battery minimum voltage (volts)","",30, 100,1,0.1,doNothing,noEvent,wrapStyle) //
  ,altFIELD(decPlaces<1>::menuField, Battery_maximum_voltage,"  Battery maximum voltage (volts)","",30, 100,1,0.1,doNothing,noEvent,wrapStyle) //
  ,FIELD(Battery_distance,"  Battery distance (km)","",30, 100,5,1,doNothing,noEvent,wrapStyle) //
  ,SUBMENU(setThottleRegen) //
  ,EXIT("< Back")
);

MENU(escooterSettings,"  E-scooter settings",doNothing,noEvent,noStyle //
  ,altFIELD(decPlaces<1>::menuField, dummy, "  Dummy","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle) //
  ,EXIT("< Back") //
);

MENU(escooterAccessories,"  E-scooter accessories",doNothing,noEvent,noStyle //
  ,altFIELD(decPlaces<1>::menuField, dummy, "  Dummy","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle) //
  ,EXIT("< Back") //
);

MENU(electricBrake,"  Electric brake",doNothing,noEvent,noStyle //
  ,altFIELD(decPlaces<1>::menuField, dummy, "  Dummy","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle) //
  ,EXIT("< Back") //
);

MENU(electricThrottle,"  Electric throttle",doNothing,noEvent,noStyle // 
  ,altFIELD(decPlaces<1>::menuField, dummy, "  Dummy","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle) //
  ,EXIT("< Back") //
);

MENU(displaySettings,"  Display",doNothing,noEvent,noStyle //
  ,altFIELD(decPlaces<1>::menuField, dummy, "  Dummy","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle) //
  ,EXIT("< Back") //
);


*/


{% for key, value in parameters.items() %}
    {% for key2, value2 in value.items() %}
        {%- for  item in value2.settings %}
            {%- set var_name = item.display_name | lower  | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") %}
            {%- if item.smartdisplay_tft_menu_visible %}
    {{ item.type }} {{ var_name }};
            {%- endif %}
        {%- endfor %}
    {%- endfor %}
{%- endfor %}


{% for key, value in parameters.items() %}
    {% for key2, value2 in value.items() %}
        {% set vars = {'foo': False} %}
        {%- for  item in value2.settings %}
            {%- if item.smartdisplay_tft_menu_visible | int == 1 %}
                {%- if vars.update({'foo': True}) %} {% endif %}
            {%- endif %}
        {%- endfor %}

            {%- if vars.foo == True %}
MENU(SUBMENU_{{ key2 | replace(" ", "_")}},"  {{ key2 }}",doNothing,noEvent,noStyle //

            {%- for  item in value2.settings %}
                {%- set var_name = item.display_name | lower  | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") %}
                
                {%- if item.smartdisplay_tft_menu_visible %}
                    {%- if item.type | lower == "float" %}
    ,altFIELD(decPlaces<1>::menuField, {{ var_name }}, "  {{ item.display_name }}","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle) //
                    {%- elif 'int' in item.type %}
    ,FIELD({{ var_name }},"  {{ item.display_name }}","",5,20,1,0,doNothing,noEvent,wrapStyle) //
                    {%- else %}
                    {%- endif %}
                {%- endif %}
            {%- endfor %}
);        
        {%- endif %}
    {%- endfor %}

{%- endfor %}

MENU(mainMenu,"  Main menu",doNothing,noEvent,wrapStyle //
    ,SUBMENU(SUBMENU_MANUAL_moreInfos) //
{%- for key, value in parameters.items() %}
    {%- for key2, value2 in value.items() %}
        {%- set vars = {'foo': False} %}
        {%- for  item in value2.settings %}
            {%- if item.smartdisplay_tft_menu_visible | int == 1 %}
                {%- if vars.update({'foo': True}) %} {% endif %}
            {%- endif %}
        {%- endfor %}
            {%- if vars.foo == True %}
    ,SUBMENU(SUBMENU_{{ key2 | replace(" ", "_")}}) //
        {%- endif %}
    {%- endfor %}
{%- endfor %}
    ,SUBMENU(SUBMENU_MANUAL_bluetooth) //
    ,SUBMENU(SUBMENU_MANUAL_firmware) //
    ,OP("< Discard & exit", discard_exit,enterEvent) //
    ,OP("< Save & exit", save_exit,enterEvent) //
);

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
    result_h = tmpl_h.render(parameters=json_data)

    # get template name, output file name
    outputFileName = ".\\src\\TFT\\tft_settings_menu_specs_gen.h"
    print("outputFileName H : " + outputFileName)
    print (result_h)

    # write output to file
    outFile = open(outputFileName,"w")
    outFile.write(result_h)
    outFile.close()
    