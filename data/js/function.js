function setDataFromSelector(value, selector) {
    $(selector).html(value);
}

function showBatteryLevel(percentBattery) {
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

    $('#batteryLevel').text(percentBattery + ' %')
}

function setDatasToDashboard(datas) {
    console.log(datas);
    // Current speed
    setDataFromSelector(datas.speedCurrent + ' km/h', '#speedCurrent');
    // Max speed
    setDataFromSelector(datas.speedMax + ' km/h', '#speedMax');
    // Distance trip
    setDataFromSelector(datas.distanceTrip + ' km', '#distanceTrip')


    // Current speed
    //setDataFromSelector(datas.speedCurrent + ' km/h', '#speedCurrent');
    // Battery level
    showBatteryLevel(datas.batteryLevel);
}