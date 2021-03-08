
package org.koxx.smartcntrl.settings;

import android.content.Context;
import android.text.InputType;

import com.hotmail.or_dvir.easysettings.pojos.CheckBoxSettingsObject;
import com.hotmail.or_dvir.easysettings.pojos.EasySettings;
import com.hotmail.or_dvir.easysettings.pojos.HeaderSettingsObject;
import com.hotmail.or_dvir.easysettings.pojos.SeekBarSettingsObject;
import com.hotmail.or_dvir.easysettings.pojos.SettingsObject;
import com.hotmail.or_dvir.easydialogs.pojos.EditTextSettingsObject;
import com.hotmail.or_dvir.easydialogs.pojos.ListSettingsObject;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.ArrayList;

public class SmartElecSettings2 {

    public static final String Escooter_characteristics = "Escooter characteristics";
    public static final String Wheel_size = "Wheel size";
    public static final String Number_of_poles_pairs = "Number of poles pairs";
    public static final String Battery_minimum_voltage = "Battery minimum voltage";
    public static final String Battery_maximum_voltage = "Battery maximum voltage";
    public static final String Battery_maximum_distance = "Battery maximum distance";

    public static final String Smart_display = "Smart display";
    public static final String Default_acceleration = "Default acceleration";
    public static final String Default_acceleration_LIST_Very_slow = "Very slow";
    public static final String Default_acceleration_LIST_Slow = "Slow";
    public static final String Default_acceleration_LIST_Medium = "Medium";
    public static final String Default_acceleration_LIST_Fast = "Fast";
    public static final String Default_acceleration_LIST_Very_fast = "Very fast";
    public static final String Default_acceleration_LIST_Maximum = "Maximum";
    public static final String Pas_enabled = "PAS enabled";

    public static final String General = "General";
    public static final String Beacon_mac_address = "Beacon Mac Address";

    public static final String Electric_brake = "Electric brake";
    public static final String Electric_brake_type = "Electric_brake_type";
    public static final String Electric_brake_type_LIST_From_controller = "From Controller";
    public static final String Electric_brake_type_LIST_From_smart__digital_brake_lever = "From Smart - Digital brake lever";
    public static final String Electric_brake_type_LIST_From_smart__analog_brake_lever = "From Smart - Analog brake lever";
    public static final String Electric_brake_type_LIST_From_smart__digital_brake_lever__throttle_experimental = "From Smart - Digital brake lever + Throttle (Experimental)";


    public static ArrayList<SettingsObject> initialize() {

        ArrayList<String> Default_acceleration_LIST = new ArrayList<>();;
        Default_acceleration_LIST.add(Default_acceleration_LIST_Very_slow );
        Default_acceleration_LIST.add(Default_acceleration_LIST_Slow );
        Default_acceleration_LIST.add(Default_acceleration_LIST_Medium );
        Default_acceleration_LIST.add(Default_acceleration_LIST_Fast );
        Default_acceleration_LIST.add(Default_acceleration_LIST_Very_fast );
        Default_acceleration_LIST.add(Default_acceleration_LIST_Maximum );
            

        ArrayList<String> Electric_brake_type_LIST = new ArrayList<>();;
        Electric_brake_type_LIST.add(Electric_brake_type_LIST_From_controller );
        Electric_brake_type_LIST.add(Electric_brake_type_LIST_From_smart__digital_brake_lever );
        Electric_brake_type_LIST.add(Electric_brake_type_LIST_From_smart__analog_brake_lever );
        Electric_brake_type_LIST.add(Electric_brake_type_LIST_From_smart__digital_brake_lever__throttle_experimental );
            
        ArrayList<SettingsObject> settings = EasySettings.createSettingsArray(

// ----------------------
                new HeaderSettingsObject.Builder(Escooter_characteristics)
                        .build(),
                new EditTextSettingsObject.Builder(Wheel_size, Wheel_size, 10.0, "save")
                        .setDialogTitle("In inches")
                        .setUseValueAsPrefillText()
                        .setNegativeBtnText("cancel")
                        .setInputType(InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_FLAG_DECIMAL)
                        .setUseValueAsSummary()
                        .build(),
            
                new SeekBarSettingsObject.Builder(Number_of_poles_pairs, Number_of_poles_pairs, 15, 10, 30, 1)
                        .setUseValueAsSummary()
                        .build(),
            
                new EditTextSettingsObject.Builder(Battery_minimum_voltage, Battery_minimum_voltage, 42.0, "save")
                        .setDialogTitle("In volts")
                        .setUseValueAsPrefillText()
                        .setNegativeBtnText("cancel")
                        .setInputType(InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_FLAG_DECIMAL)
                        .setUseValueAsSummary()
                        .build(),
            
                new EditTextSettingsObject.Builder(Battery_maximum_voltage, Battery_maximum_voltage, 58.8, "save")
                        .setDialogTitle("In volts")
                        .setUseValueAsPrefillText()
                        .setNegativeBtnText("cancel")
                        .setInputType(InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_FLAG_DECIMAL)
                        .setUseValueAsSummary()
                        .build(),
            
                new EditTextSettingsObject.Builder(Battery_maximum_distance, Battery_maximum_distance, 40, "save")
                        .setDialogTitle("In kilometers")
                        .setUseValueAsPrefillText()
                        .setNegativeBtnText("cancel")
                        .setInputType(InputType.TYPE_CLASS_NUMBER)
                        .setUseValueAsSummary()
                        .build(),
            

// ----------------------
                new HeaderSettingsObject.Builder(Smart_display)
                        .build(),
                new ListSettingsObject.Builder(Default_acceleration, Default_acceleration, Default_acceleration_LIST_Maximum, Default_acceleration_LIST , "save")
                        .setUseValueAsSummary()
                        .setNegativeBtnText("cancel")
                        .addDivider()
                        .build(),
            
                new CheckBoxSettingsObject.Builder(Pas_enabled, Pas_enabled, False)
                        .setOffText("off")
                        .setOnText("on")
                        .build(),
            

// ----------------------
                new HeaderSettingsObject.Builder(General)
                        .build(),            
                new EditTextSettingsObject.Builder(Beacon_mac_address, Beacon_mac_address, aa:bb:cc:dd:ee:ff, "save")
                        .setDialogTitle(MAC address)
                        .setUseValueAsPrefillText()
                        .setNegativeBtnText("cancel")
                        .setRegex(^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$)
                        .setInputType(InputType.TYPE_CLASS_TEXT)
                        .setUseValueAsSummary()
                        .build(),
            

// ----------------------
                new HeaderSettingsObject.Builder(Electric_brake)
                        .build(),
                new ListSettingsObject.Builder(Electric_brake_type, Electric_brake_type, Electric_brake_type_LIST_From_controller, Electric_brake_type_LIST , "save")
                        .setUseValueAsSummary()
                        .setNegativeBtnText("cancel")
                        .addDivider()
                        .build(),
            
                new HeaderSettingsObject.Builder("")
                        .build()
        );
        return settings;
    }

}