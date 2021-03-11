import json
import re
import jinja2


template = """
//////------------------------------------
//////------------------------------------
//////------------------------------------
////// GENERATED FILE - DO NOT EDIT MANUALLY
//////------------------------------------
//////------------------------------------
//////------------------------------------

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

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.nio.ByteOrder;

import com.welie.blessed.BluetoothBytesParser;

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



    static public ArrayList<byte[]> {{ var_name }}_to_byte_array_write(Context ctx) {

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
                    {%- elif item.smartphone_display_type | lower == "seek_bar" %}
                        {%- set java_display_type = "Int" %}
                        {%- set java_default = item.default %}
                    {%- elif item.smartphone_display_type | lower == "list" %}
                        {%- set java_display_type = "String" %}
                        {%- set java_default = '"' + item.default |string + '"' %}
                    {%- else %}
                        {%- set java_display_type = "Int" %}
                        {%- set java_default = item.default %}
                    {%- endif %}

            Object sourceData = (EasySettings.retrieveSettingsSharedPrefs(ctx).get{{ java_display_type }}({{ var_name }}, {{ java_default }} ));
            
                    {%- if item.smartphone_display_type | lower == "list" %}
            sourceData = listToValue(ctx, (String) sourceData, {{ var_name }}_LIST);
                    {%- endif %}

            if (sourceData instanceof Boolean) {
                sourceData = new Integer((Boolean)sourceData ? 1 : 0);
            }

            Timber.e("sourceData = " + sourceData);

                    {%- if 'int8' in item.type %}
            if (sourceData instanceof Integer) {
                dos.writeByte((Integer)sourceData);
            }
            if (sourceData instanceof String) {
                dos.writeByte(Integer.parseInt((String)sourceData));
            }
                    {%- elif 'int16' in item.type %}
            if (sourceData instanceof Integer) {
                dos.writeShort((Integer)sourceData);
            }
            if (sourceData instanceof String) {
                dos.writeShort(Integer.parseInt((String)sourceData));
            }

                    {%- elif 'int32' in item.type %}
            if (sourceData instanceof Integer) {
                dos.writeInt((Integer)sourceData);
            }
            if (sourceData instanceof String) {
                dos.writeInt(Integer.parseInt((String)sourceData));
            }

                    {%- elif item.type | lower == "float" %}
            dos.writeFloat((Float.parseFloat(EasySettings.retrieveSettingsSharedPrefs(ctx).get{{ java_display_type }}({{ var_name }}, {{ java_default }}).replace(",", "."))));

                    {%- elif item.type | lower == "string" %}
            String value = EasySettings.retrieveSettingsSharedPrefs(ctx).get{{ java_display_type }}({{ var_name }}, {{ java_default }});
            int length = 1;
            if (value.length() > 16)
                length = 16;
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


    static public byte[] {{ var_name }}_to_byte_array_read(Context ctx) {

        // First packet
        ArrayList<byte[]> result = new ArrayList<byte[]>();
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(bos);
        try {
            dos.writeByte(0);
            dos.writeShort({{ var_name | upper }}_ID);
            dos.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return bos.toByteArray();
    }


    static public void {{ var_name }}_from_byte(Context ctx, BluetoothBytesParser parser) {

        int packetNumber = parser.getIntValue(BluetoothBytesParser.FORMAT_UINT16);

                    {%- if 'uint8' in item.type %}
        int value = parser.getIntValue(BluetoothBytesParser.FORMAT_UINT8);
                    {%- elif 'int8' in item.type %}
        int value = parser.getIntValue(BluetoothBytesParser.FORMAT_SINT8);
                    {%- elif 'uint16' in item.type %}
        int value = parser.getIntValue(BluetoothBytesParser.FORMAT_UINT16);
                    {%- elif 'int16' in item.type %}
        int value = parser.getIntValue(BluetoothBytesParser.FORMAT_SINT16);
                    {%- elif 'uint32' in item.type %}
        int value = parser.getIntValue(BluetoothBytesParser.FORMAT_UINT32);
                    {%- elif 'int32' in item.type %}
        int value = parser.getIntValue(BluetoothBytesParser.FORMAT_SINT32);
                    {%- elif item.type | lower == "float" %}
        float value = 0;
        try {
            DataInputStream dis = new DataInputStream(new ByteArrayInputStream(parser.getValue(), 4, parser.getValue().length));
            value = dis.readFloat();
        } catch (IOException e) {
            e.printStackTrace();
        }
                    {%- elif item.type | lower == "string" %}
        String value = parser.getStringValue();
                    {%- endif %}

        
                    {%- if item.smartphone_display_type | lower == "list" %}
        EasySettings.retrieveSettingsSharedPrefs(ctx).edit().putString({{ var_name }}, {{ var_name }}_LIST[value]).commit();
                    {%- elif item.smartphone_display_type | lower == "edit_text_number_float" %}
        EasySettings.retrieveSettingsSharedPrefs(ctx).edit().putString({{ var_name }}, "" + value).commit();
                    {%- elif item.smartphone_display_type | lower == "edit_text_number_integer" %}
        EasySettings.retrieveSettingsSharedPrefs(ctx).edit().putString({{ var_name }}, "" + value).commit();
                    {%- elif item.smartphone_display_type | lower == "edit_text_number_integer_signed" %}
        EasySettings.retrieveSettingsSharedPrefs(ctx).edit().putString({{ var_name }}, "" + value).commit();
                    {%- elif item.smartphone_display_type | lower == "edit_text_string" %}
        if (packetNumber == 1)
            EasySettings.retrieveSettingsSharedPrefs(ctx).edit().putString({{ var_name }}, EasySettings.retrieveSettingsSharedPrefs(ctx).getString({{ var_name }}, "") + value).commit();
        else if (packetNumber == 0)
            EasySettings.retrieveSettingsSharedPrefs(ctx).edit().putString({{ var_name }}, "" + value).commit();
                    {%- elif item.smartphone_display_type | lower == "seek_bar" %}
        EasySettings.retrieveSettingsSharedPrefs(ctx).edit().putInt({{ var_name }}, value).commit();
                    {%- elif item.smartphone_display_type | lower == "checkbox" %}
        EasySettings.retrieveSettingsSharedPrefs(ctx).edit().putBoolean({{ var_name }}, value == 1).commit();
                    {%- endif %}

        Timber.i("{{ var_name }} value = " + value);
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
        ArrayList<SettingsObject> settings = EasySettings.createSettingsArray();
{%- for key, value in parameters.items() %}
    {%- for key2, value2 in value.items() %}

        {% set outer_loop = loop %}
// ----------------------

        settings.add(new HeaderSettingsObject.Builder({{ key2 | replace(" ", "_") | title }})
                        .build());

        {%- for  item in value2.settings %}
        settings.add(
            {%- set var_name = item.display_name | lower  | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") | title %}
            {%- if item.smartphone_display_type | lower == "list" %}
                {%- set list1 = item.list_strings.split('\n')  %}
                {%- set list_default = list1[item.default] | lower  | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") | title %}
            new ListSettingsObject.Builder({{ var_name }}, {{ var_name }}, {{ var_name }}_LIST_{{ list_default }}, {{ var_name | title }}_LIST_AL , "save")
                .setUseValueAsSummary()
                .setNegativeBtnText("cancel")
                {%- set list1 = item.list_strings.split('\n')  %}
                {%- for item4 in list1 %}
                    {%- set var_elem_name = item4 | lower  | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") %}
                {%- endfor %}
            {%- endif %}
            {%- if item.smartphone_display_type | lower == "edit_text_number_float" %}
            new EditTextSettingsObject.Builder({{ var_name }}, {{ var_name }}, "{{ item.default }}", "save")
                .setDialogTitle("{{ item.display_unit }}")
                .setUseValueAsPrefillText()
                .setNegativeBtnText("cancel")
                .setInputType(InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_FLAG_DECIMAL)
                .setUseValueAsSummary()
            {%- endif %}
            {%- if item.smartphone_display_type | lower == "edit_text_number_integer" %}
            new EditTextSettingsObject.Builder({{ var_name }}, {{ var_name }}, "{{ item.default }}", "save")
                .setDialogTitle("{{ item.display_unit }}")
                .setUseValueAsPrefillText()
                .setNegativeBtnText("cancel")
                .setInputType(InputType.TYPE_CLASS_NUMBER)
                .setUseValueAsSummary()
            {%- endif %}
            {%- if item.smartphone_display_type | lower == "edit_text_number_integer_signed" %}
            new EditTextSettingsObject.Builder({{ var_name }}, {{ var_name }}, "{{ item.default }}", "save")
                .setDialogTitle("{{ item.display_unit }}")
                .setUseValueAsPrefillText()
                .setNegativeBtnText("cancel")
                .setInputType(InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_FLAG_SIGNED)
                .setUseValueAsSummary()
            {%- endif %}
            {%- if item.smartphone_display_type | lower == "edit_text_string" %}            
            new EditTextSettingsObject.Builder({{ var_name }}, {{ var_name }}, "{{ item.default }}", "save")
                .setDialogTitle("{{ item.display_hint }}")
                .setUseValueAsPrefillText()
                .setNegativeBtnText("cancel")
                .setRegex("{{ item.regex_valid }}")
                .setInputType(InputType.TYPE_CLASS_TEXT)
                .setUseValueAsSummary()
            {% endif %}
            {%- if item.smartphone_display_type | lower == "seek_bar" %}
            new SeekBarSettingsObject.Builder({{ var_name }}, {{ var_name }}, {{ item.default }}, {{ item.min }}, {{ item.max }}, {{ item.increment | int }})
                .setUseValueAsSummary()
            {%- endif %}
            {%- if item.smartphone_display_type | lower == "checkbox" %}
            new CheckBoxSettingsObject.Builder({{ var_name }}, {{ var_name }}, {{ "true" if item.default else "false" }})
                .setOffText("off")
                .setOnText("on")
            {%- endif %}
            {%- if outer_loop.last == False and loop.last == True %}
                .addDivider()
            {%- endif %}
                .build());

        {%- endfor %}

    {%- endfor %}
{%- endfor %}
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
    
{#- run 5 - settings packer write #}
    static public ArrayList<byte[]> pack_setting_packet_write(Context ctx, Integer settingId) {
        ArrayList<byte[]> result = null;
        
        switch(settingId) {
{%- for key, value in parameters.items() %}
    {%- for key2, value2 in value.items() %}
        {%- for  item in value2.settings %}
            {%- set var_name = item.display_name | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") | title %}
        case {{ var_name | upper }}_ID :
            result = {{ var_name }}_to_byte_array_write(ctx);
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



{#- run 7 - settings unpacker #}
    static public void unpack_setting_packet(Context ctx, byte[] value) {
        BluetoothBytesParser parser = new BluetoothBytesParser(value, ByteOrder.BIG_ENDIAN);
        
        int settingId = parser.getIntValue(BluetoothBytesParser.FORMAT_UINT16);
        
        switch(settingId) {
{%- for key, value in parameters.items() %}
    {%- for key2, value2 in value.items() %}
        {%- for  item in value2.settings %}
            {%- set var_name = item.display_name | replace(" ", "_") | regex_replace("[^A-Za-z0-9_]","") | title %}
        case {{ var_name | upper }}_ID :
            {{ var_name }}_from_byte(ctx, parser);
            break;
        {%- endfor %}
    {%- endfor %}
{%- endfor %}
        default:
            Timber.e("invalid parameter");
            break;
        }
    }

    static public byte[] pack_setting_packet_read(Context ctx, Integer settingId) {
                
        byte[] result = null;
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(bos);
        try {
            dos.writeByte(0);
            dos.writeShort(settingId);
            dos.flush();
            result = bos.toByteArray();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return result;
    }

    public static int listToValue(Context ctx, String value, String[] list) {

        int i = 0;
        for (String elem : list) {
            if (elem.equals(value)) {
                return i;
            }
            i++;
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
jsonConfigName = "params.json"
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