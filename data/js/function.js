function setDataFromSelector(value, selector) {
    $(selector).html(value);
}

function setDataValueAttrFromSelector(value, selector) {
    $(selector).attr('data-value', value);
}

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

function getVoltage(voltageFilterMean) {
    return Number((voltageFilterMean / 1000).toFixed(1));
}

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

function setTimeToHumanTime(time) {
    var date = new Date(time).toISOString().substr(12, 7);
    setDataFromSelector(date, '#time');
}

function setCurrentPower(current, voltageFilterMean) {
    var power = getVoltage(voltageFilterMean) * current;
    setDataFromSelector(power, '#currentPower');
}

function setOnOff(value, selector) {
    var label = 'OFF';
    if (value == 1) {
        label = 'ON';
    }
    setDataFromSelector(label, selector);
    setDataValueAttrFromSelector(value, selector);
}

function setModeFromList(value, selector) {
    setDataValueAttrFromSelector(value, selector);
    setListSelected(value, selector);
}

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

function showDatasToDashboard(datas) {
    console.log(datas);
    // Current speed
    setDataFromSelector(datas.speedCurrent, '#speedCurrent');
    // Max speed
    setDataFromSelector(datas.speedMax, '#speedMax');
    // Mode
    setModeFromList(datas.modeOrder, '#modeOrder');
    // Brake
    setModeFromList(datas.brakeSentOrder, '#brakeSentOrder');
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
    // Distance trip
    setDataFromSelector(datas.distanceOdo, '#distanceOdo');
    // Time
    setTimeToHumanTime(datas.time);


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
