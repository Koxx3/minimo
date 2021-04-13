$.getJSON('/getversion', function (data) {
    $('#ACE_OTA_current_version').append(data.version)
});

$.getJSON('https://raw.githubusercontent.com/Koxx3/SmartController_SmartDisplay_ESP32/master/ota_updates/smartcontroller_minimo/firmware.json', function (data) {
        var items = [];

        $.each(data.versions, function (id, item) {
            $('#versionslist').append('<p><input type="radio" name="version_selected" id="' + item.version + '" value="' + item.version + '"><label>' + item.version + " - " + EpochToDate(item.date) + '</label></input></p>')
        });

        $('#versions_not_available').hide();
        $('label[for="ACE_OTA_version_manual"]').hide();
        $('#ACE_OTA_version_manual').hide();
    }
);

//Epoch To Date
function EpochToDate(epoch) {
    if (epoch < 10000000000) {
        // Convert to milliseconds (Epoch is usually expressed in seconds, but Javascript uses Milliseconds)
        epoch *= 1000;
    }
    var epoch = epoch + (new Date().getTimezoneOffset() * -1); // For timeZone
    var dat = new Date(epoch);

    return dat.toLocaleString();
}
