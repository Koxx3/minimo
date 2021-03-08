
    
        

        

/*-------------------------------------------------------*/

#define SETTINGS_WHEEL_SIZE_ID 1
#define SETTINGS_WHEEL_SIZE_NAME "Wheel size"

float wheel_size;

void set_wheel_size (float value) {
    wheel_size = value;
}

float get_wheel_size () {
    return wheel_size ;
}

void display_wheel_size () {
    Serial.println("  wheel_size = " + (String) wheel_size);
}

        

/*-------------------------------------------------------*/

#define SETTINGS_NUMBER_OF_POLES_PAIRS_ID 2
#define SETTINGS_NUMBER_OF_POLES_PAIRS_NAME "Number of poles pairs"

uint8_t number_of_poles_pairs;

void set_number_of_poles_pairs (uint8_t value) {
    number_of_poles_pairs = value;
}

uint8_t get_number_of_poles_pairs () {
    return number_of_poles_pairs ;
}

void display_number_of_poles_pairs () {
    Serial.println("  number_of_poles_pairs = " + (String) number_of_poles_pairs);
}

        

/*-------------------------------------------------------*/

#define SETTINGS_BATTERY_MINIMUM_VOLTAGE_ID 3
#define SETTINGS_BATTERY_MINIMUM_VOLTAGE_NAME "Battery minimum voltage"

float battery_minimum_voltage;

void set_battery_minimum_voltage (float value) {
    battery_minimum_voltage = value;
}

float get_battery_minimum_voltage () {
    return battery_minimum_voltage ;
}

void display_battery_minimum_voltage () {
    Serial.println("  battery_minimum_voltage = " + (String) battery_minimum_voltage);
}

        

/*-------------------------------------------------------*/

#define SETTINGS_BATTERY_MAXIMUM_VOLTAGE_ID 4
#define SETTINGS_BATTERY_MAXIMUM_VOLTAGE_NAME "Battery maximum voltage"

float battery_maximum_voltage;

void set_battery_maximum_voltage (float value) {
    battery_maximum_voltage = value;
}

float get_battery_maximum_voltage () {
    return battery_maximum_voltage ;
}

void display_battery_maximum_voltage () {
    Serial.println("  battery_maximum_voltage = " + (String) battery_maximum_voltage);
}

        

/*-------------------------------------------------------*/

#define SETTINGS_BATTERY_MAXIMUM_DISTANCE_ID 5
#define SETTINGS_BATTERY_MAXIMUM_DISTANCE_NAME "Battery maximum distance"

uint8_t battery_maximum_distance;

void set_battery_maximum_distance (uint8_t value) {
    battery_maximum_distance = value;
}

uint8_t get_battery_maximum_distance () {
    return battery_maximum_distance ;
}

void display_battery_maximum_distance () {
    Serial.println("  battery_maximum_distance = " + (String) battery_maximum_distance);
}
        

        

/*-------------------------------------------------------*/

#define SETTINGS_DEFAULT_ACCELERATION_ID 6
#define SETTINGS_DEFAULT_ACCELERATION_NAME "Default acceleration"

uint8_t default_acceleration;
typedef enum LIST_Default_acceleration {
    LIST_Default_acceleration_Very_slow,
    LIST_Default_acceleration_Slow,
    LIST_Default_acceleration_Medium,
    LIST_Default_acceleration_Fast,
    LIST_Default_acceleration_Very_fast,
    LIST_Default_acceleration_Maximum, 
} tDefault_acceleration;

void set_default_acceleration (uint8_t value) {
    default_acceleration = value;
}

uint8_t get_default_acceleration () {
    return default_acceleration ;
}

void display_default_acceleration () {
    Serial.println("  default_acceleration = " + (String) default_acceleration);
}

        

/*-------------------------------------------------------*/

#define SETTINGS_PAS_ENABLED_ID 7
#define SETTINGS_PAS_ENABLED_NAME "PAS enabled"

uint8_t pas_enabled;

void set_pas_enabled (uint8_t value) {
    pas_enabled = value;
}

uint8_t get_pas_enabled () {
    return pas_enabled ;
}

void display_pas_enabled () {
    Serial.println("  pas_enabled = " + (String) pas_enabled);
}
        

        

/*-------------------------------------------------------*/

#define SETTINGS_BEACON_MAC_ADDRESS_ID 9
#define SETTINGS_BEACON_MAC_ADDRESS_NAME "Beacon Mac Address"

string beacon_mac_address;

void set_beacon_mac_address (string value) {
    beacon_mac_address = value;
}

string get_beacon_mac_address () {
    return beacon_mac_address ;
}

void display_beacon_mac_address () {
    Serial.println("  beacon_mac_address = " + (String) beacon_mac_address);
}
        

        

/*-------------------------------------------------------*/

#define SETTINGS_ELECTRIC_BRAKE_TYPE_ID 8
#define SETTINGS_ELECTRIC_BRAKE_TYPE_NAME "Electric_brake_type"

uint8_t electric_brake_type;
typedef enum LIST_Electric_brake_type {
    LIST_Electric_brake_type_From_controller,
    LIST_Electric_brake_type_From_smart_-_digital_brake_lever,
    LIST_Electric_brake_type_From_smart_-_analog_brake_lever,
    LIST_Electric_brake_type_From_smart_-_digital_brake_lever_+_throttle_(Experimental), 
} tElectric_brake_type;

void set_electric_brake_type (uint8_t value) {
    electric_brake_type = value;
}

uint8_t get_electric_brake_type () {
    return electric_brake_type ;
}

void display_electric_brake_type () {
    Serial.println("  electric_brake_type = " + (String) electric_brake_type);
}