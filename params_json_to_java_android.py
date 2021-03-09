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
import java.util.Arrays;

import timber.log.Timber;

public class SmartElecSettings2 {

{#- run 1 - description strings #}
{%- for key, value in parameters.items() %}


    {%- for key2, value2 in value.items() %}

    /* ------------------------------------------------------------------------------------*/
    /* ------------------------------------------------------------------------------------*/

    public static final String {{ key2 | replace(" ", "_") | title }} = "{{ key2 }}";

        {%- for  item in value2.settings %}
    /* ------------------------------------------------------------------------------------*/

            {%- set var_name = item.display_name | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") | title  %}
    public static final int {{ var_name | upper }}_ID = {{ item.id }};
    public static final String {{ var_name  }} = "{{ item.display_name }}";

            {%- if item.smartphone_display_type | lower == "list" %}
                {%- set list1 = item.list_strings.split('\\n') %}
                {%- for item4 in list1 %}
    public static final String {{ var_name |title }}_LIST_{{ item4 | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") |title}} = "{{ item4 }}";
                {%- endfor %}
            {%- endif %}

            {%- if item.smartphone_display_type | lower == "list" %}
    public static final String[] {{ var_name | title }}_LIST = {

                {%- set list1 = item.list_strings.split('\\n') %}
                {%- for item4 in list1 %}
        {{ var_name |title }}_LIST_{{ item4 | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") |title}},
                {%- endfor %}
    };
            {%- endif %}




    static public ArrayList<byte[]> {{ var_name }}_to_byte_array(Context ctx) {

        // First packet
        ArrayList<byte[]> result = new ArrayList<byte[]>();
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(bos);
        try {
            dos.writeShort({{ var_name | upper }}_ID);
            dos.writeShort(0);

                    {%- if item.smartphone_display_type | lower == "edit_text_number_float" %}
                        {%- set java_display_type = "String" %}
                        {%- set java_default = '"' + item.default |string + '"' %}
                    {%- elif item.smartphone_display_type | lower == "edit_text_number_integer" %}
                        {%- set java_display_type = "String" %}
                        {%- set java_default = '"' + item.default |string + '"' %}
                    {%- elif item.smartphone_display_type | lower == "edit_text_number_integer_signed" %}
                        {%- set java_display_type = "String" %}
                        {%- set java_default = '"' + item.default |string + '"' %}
                    {%- elif item.smartphone_display_type | lower == "edit_text_number_integer_signed" %}
                        {%- set java_display_type = "String" %}
                        {%- set java_default = '"' + item.default |string + '"' %}
                    {%- elif item.smartphone_display_type | lower == "edit_text_string" %}
                        {%- set java_display_type = "String" %}
                        {%- set java_default = '"' + item.default |string + '"' %}
                    {%- elif item.smartphone_display_type | lower == "checkbox" %}
                        {%- set java_display_type = "Boolean" %}
                        {%- set java_default = "true" if item.default else "false" %}
                        {%- set java_end_comparator = " ? 1 : 0" %}
                    {%- elif item.smartphone_display_type | lower == "slider" %}
                        {%- set java_display_type = "Int" %}
                        {%- set java_default = item.default %}
                    {%- elif item.smartphone_display_type | lower == "list" %}
                        {%- set java_display_type = "String" %}
                        {%- set java_default = '"' + item.default |string + '"' %}
                    {%- else %}
                        {%- set java_display_type = "Int" %}
                        {%- set java_default = item.default %}
                    {%- endif %}

                    {%- if item.type | lower == "uint8_t" %}
            dos.writeByte({{ "Integer.parseInt" if java_display_type == "String" }}(EasySettings.retrieveSettingsSharedPrefs(ctx).get{{ java_display_type }}({{ var_name }}, {{ java_default }} )) {{ java_end_comparator }});
                    {%- elif item.type | lower == "int8_t" %}
            dos.writeByte({{ "Integer.parseInt" if java_display_type == "String" }}(EasySettings.retrieveSettingsSharedPrefs(ctx).get{{ java_display_type }}({{ var_name }}, {{ java_default }} )) {{ java_end_comparator }});
                    {%- elif item.type | lower == "uint16_t" %}
            dos.writeShort({{ "Integer.parseInt" if java_display_type == "String" }}(EasySettings.retrieveSettingsSharedPrefs(ctx).get{{ java_display_type }}({{ var_name }}, {{ java_default }} )));
                    {%- elif item.type | lower == "int16_t" %}
            dos.writeShort({{ "Integer.parseInt" if java_display_type == "String" }}(EasySettings.retrieveSettingsSharedPrefs(ctx).get{{ java_display_type }}({{ var_name }}, {{ java_default }} )));
                    {%- elif item.type | lower == "uint32_t" %}
            dos.writeInt({{ "Integer.parseInt" if java_display_type == "String" }}(EasySettings.retrieveSettingsSharedPrefs(ctx).get{{ java_display_type }}({{ var_name }}, {{ java_default }} )));
                    {%- elif item.type | lower == "int32_t" %}
            dos.writeInt({{ "Integer.parseInt" if java_display_type == "String" }}(EasySettings.retrieveSettingsSharedPrefs(ctx).get{{ java_display_type }}({{ var_name }}, {{ java_default }} )));
                    {%- elif item.type | lower == "float" %}
            dos.writeFloat((Float.parseFloat(EasySettings.retrieveSettingsSharedPrefs(ctx).get{{ java_display_type }}({{ var_name }}, {{ java_default }}).replace(",", "."))));
                    {%- elif item.type | lower == "string" %}
            String value = EasySettings.retrieveSettingsSharedPrefs(ctx).get{{ java_display_type }}({{ var_name }}, {{ java_default }});
            int length = 1;
            if (value.length() > 16)
                length = 15;
            else
                length = value.length();
            dos.writeBytes(value.substring(0, length));
                    {%- else %}
            error
                    {%- endif %}
            dos.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
        result.add(bos.toByteArray());

                    {%- if item.type | lower == "string" %}

        // Second packet
        String value = EasySettings.retrieveSettingsSharedPrefs(ctx).getString({{ var_name }}, "{{ item.default }}");
        if (value.length() > 16) {
            bos = new ByteArrayOutputStream();
            dos = new DataOutputStream(bos);
            try {
                dos.writeShort({{ var_name | upper }}_ID);
                dos.writeShort(1);
                dos.writeBytes(EasySettings.retrieveSettingsSharedPrefs(ctx).getString({{ var_name }}, "{{ item.default }}" ).substring(16, value.length()));
                dos.flush();
            } catch (IOException e) {
                e.printStackTrace();
            }
            result.add(bos.toByteArray());
        }
                    {%- endif %}

        return result;
    }

        {% endfor %}

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
        ArrayList<String> {{ var_name |title}}_LIST_AL = new ArrayList<>();
        {{ var_name |title}}_LIST_AL.addAll(Arrays.asList({{ var_name | title }}_LIST));
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

    public static ArrayList<Integer> getAllSettingsIdList() {

        ArrayList<Integer> result = new ArrayList<Integer>();

{#- run 4 - settings list #}
{%- for key, value in parameters.items() %}
    {%- for key2, value2 in value.items() %}
        {%- for  item in value2.settings %}
            {%- set var_name = item.display_name | lower  | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") %}
        result.add({{ var_name | upper }}_ID);
        {%- endfor %}
    {%- endfor %}
{%- endfor %}
        return result;
    }
    
{#- run 5 - settings packer #}
    static public ArrayList<byte[]> pack_setting_packet(Context ctx, Integer settingId) {
        ArrayList<byte[]> result = null;
        
        switch(settingId) {
{%- for key, value in parameters.items() %}
    {%- for key2, value2 in value.items() %}
        {%- for  item in value2.settings %}
            {%- set var_name = item.display_name | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") | title %}
        case {{ var_name | upper }}_ID :
            result = {{ var_name }}_to_byte_array(ctx);
            break;
        {%- endfor %}
    {%- endfor %}
{%- endfor %}
        default:
            Timber.e("invalid parameter");
            break;
        }

        return result;
    }

    static public void unpack_setting_packet(Context ctx, byte[] packet) {

    }


    public static int listToValue_(Context ctx, String value, String[] list) {
        int intValue = 0;
        String valueStr = EasySettings.retrieveSettingsSharedPrefs(ctx).getString(value, "");

        int i = 0;
        for (String elem : list) {
            if (elem.equals(list[0])) {
                return i;
            }
        }

        return 0;
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