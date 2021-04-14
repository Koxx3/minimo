/**
 * @param int value
 * @param string selector
 */
function setDataFromSelector(value, selector) {
    $(selector).html(value);
}

/**
 * @param int value
 * @param string selector
 */
function setDataValueAttrFromSelector(value, selector) {
    $(selector).attr('data-value', value);
}

/**
 * @param int value
 * @param string selector
 */
function setListSelected(value, selector) {
    // Firstly we reset selected option
    $(selector).find('.selected').removeClass('selected');
    // Secondly we set new mode value to selected
    $(selector).find('.select-mode').each(function() {
        if ($(this).attr('data-value') == value) {
            $(this).addClass('selected');
            return;
        }
    });
}

/**
 * @param int voltageFilterMean
 * @returns Number
 */
function getVoltage(voltageFilterMean) {
    return Number((voltageFilterMean / 1000).toFixed(1));
}

/**
 * @param int percentBattery
 * @param int voltageFilterMean
 */
function showBatteryLevel(percentBattery, voltageFilterMean) {
    var width = (100 - percentBattery) + '%';
    $('#battery-level').css('width', width);

    // Add square mask
    var batteryLevelWidth = $('#battery-gauge').width();
    var batteryLevelHeight = $('#battery-gauge').height();
    var batteryLevelNbSquareMask = 10;
    var maskBorderWidth = (batteryLevelWidth - (batteryLevelHeight * batteryLevelNbSquareMask)) / (batteryLevelNbSquareMask - 1);

    $('#battery-mask-container').css({'width': batteryLevelWidth + 'px'});
    $('#battery-mask-container td').css({'border-rigth': maskBorderWidth + 'px'});
    $('#battery-gauge').css({'width': (batteryLevelWidth - maskBorderWidth) + 'px'});

    setDataFromSelector(percentBattery, '#batteryLevel');
    setDataFromSelector(getVoltage(voltageFilterMean), '#voltageFilterMean');
}

/**
 * @param int time
 */
function setTimeToHumanTime(time, selector) {
    var date = new Date(time).toISOString().substr(12, 7);
    setDataFromSelector(date, selector);
}

/**
 * @param current
 * @param voltageFilterMean
 */
function setCurrentPower(current, voltageFilterMean) {
    var power = getVoltage(voltageFilterMean) * current;
    setDataFromSelector(power, '#currentPower');
}

/**
 * @param int value
 * @param string selector
 */
function setOnOff(value, selector) {
    var label = 'OFF';
    if (value == 1) {
        label = 'ON';
    }
    setDataFromSelector(label, selector);
    setDataValueAttrFromSelector(value, selector);
}

/**
 * @param int value
 * @param string selector
 */
function setModeFromList(datas, selector) {
    var value;

    if (selector == '#modeOrder') {
        value = datas.modeOrder;
    } else if (selector == '#brakeSentOrder') {
        value = datas.brakeSentOrder;
        $(selector).attr('data-ebrake-progresive-mode', datas.Ebrake_progressive_mode);
    }

    setDataValueAttrFromSelector(value, selector);
    setListSelected(value, selector);
}

/**
 * @param int accel
 */
function setEco(eco) {
    var selector = '#ecoOrder';
    var ecoModList = {
        0: 'NONE',
        1: 'MED',
        2: 'MAX'
    };
    setDataFromSelector(ecoModList[eco], selector);
    setDataValueAttrFromSelector(eco, selector);
}

/**
 * @param int accel
 */
function setAccel(accel) {
    var selector = '#accelOrder';
    var accelModeList = {
        0: 'MAX',
        1: 'V.FAST',
        2: 'FAST',
        3: 'MED',
        4: 'SLOW',
        5: 'V.SLOW'
    };
    setDataFromSelector(accelModeList[accel], selector);
    setDataValueAttrFromSelector(accel, selector);
}

// Variables compute
var speedAverage = 0;
var lastSpeedIsRun = 0;
var lastTimeSpeedInActivity;
var timeInActivityTrip = 0;
var timeCurrentTrip;

function computeAverageSpeed(speedCurrent, distanceTrip) {
    if (speedCurrent > 0) {// If we run
        if (lastSpeedIsRun == 0) {// .. and after we are stopped
            lastTimeSpeedInActivity = new Date().getMilliseconds();
            lastSpeedIsRun = 1;
        } else {// .. and we are already running
            var currentTime = new Date().getMilliseconds();
            var diffTime = currentTime - lastTimeSpeedInActivity;
            timeInActivityTrip = timeInActivityTrip + diffTime;

            lastTimeSpeedInActivity = currentTime;
        }
    } else {// If we are stopped
        if (lastSpeedIsRun == 1) {
            var currentTime = new Date().getMilliseconds();
            var diffTime = currentTime - lastTimeSpeedInActivity;
            timeInActivityTrip = timeInActivityTrip + diffTime;

            lastSpeedIsRun = 0;
        }
    }

    if (timeInActivityTrip > 0) {
        timeCurrentTrip = new Date().getMilliseconds();
        // https://run-motion.com/calcul-vitesse-moyenne-allure/
        // Exemple de calcul manuel :
        // 10km en 40min, distance = 10000m, temps = 40×60 = 2400 secondes
        // vitesse = 10000 / 2400 x 3,6 = 15 km/h.
        // spdAvg = (x km * 1000 (=> m)) / (y ms / 1000 (=> s)) * 3.6 (=> convertion m/s vers km/h)
        speedAverage = (distanceTrip * 1000) / (timeInActivityTrip / 1000) * 3.6;
        speedAverage = Number((speedAverage).toFixed(1));
    }
}

/**
 * @param array datas
 */
function showDatasToDashboard(datas) {
    //console.log(datas);
    // Current speed
    setDataFromSelector(datas.speedCurrent, '#speedCurrent');
    // Average speed
    setDataFromSelector(speedAverage, '#speedAvg');
    // Max speed
    setDataFromSelector(datas.speedMax, '#speedMax');
    // Mode
    setModeFromList(datas, '#modeOrder');
    // Brake
    setModeFromList(datas, '#brakeSentOrder');
    // Eco
    setEco(datas.ecoOrder);
    // Accel
    setAccel(datas.accelOrder);
    // Lock
    setOnOff(datas.isLocked, '#isLocked');
    // Speed limiter
    setOnOff(datas.speedLimiter, '#speedLimiter');
    // Aux
    setOnOff(datas.auxOrder, '#auxOrder');
    // Distance trip
    setDataFromSelector(datas.distanceTrip, '#distanceTrip');
    // Distance ODO
    setDataFromSelector(datas.distanceOdo, '#distanceOdo');
    // Time from startup
    setTimeToHumanTime(datas.time, '#time');
    // Time in activity during Vcurrent > 0 km/h
    setTimeToHumanTime((timeInActivityTrip / 1000), '#timeInActivity');

    // Battery level
    showBatteryLevel(datas.batteryLevel, datas.voltageFilterMean);
    // Autonomy
    setDataFromSelector(datas.autonomyLeft, '#autonomyLeft');
    // Current
    setDataFromSelector(datas.currentActual, '#currentActual');
    // Power
    setCurrentPower(datas.currentActual, datas.voltageFilterMean);
    // Temperature
    setDataFromSelector(datas.currentTemperature, '#currentTemperature');
    // Humidity
    setDataFromSelector(datas.currentHumidity, '#currentHumidity');
}

/**
 *
 * @param int currentValue
 * @param int valueMin
 * @param int valueMax
 * @returns int
 */
function getNextModeValue(currentValue, valueMin, valueMax) {
    var newValue;
    if (currentValue == valueMax) {
        newValue = valueMin;
    } else {
        newValue = currentValue + 1;
    }

    return newValue;
}
