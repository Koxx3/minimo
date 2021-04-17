$(function() {
    // Websocket instance
    var socket = new WebSocket('ws://" + location.host +":81/');
    var datas;
    var freezeUpdateDatasDuringUpdate = false;
    var timeDuringFreeze = 0;

    // Get values from ESP
    socket.onmessage = function(event) {
        datas = JSON.parse(event.data);

        // Compute average speed and time trip in activity
        computeAverageSpeed(datas.speedCurrent, datas.distanceTrip)

        showDatasToDashboard(datas);
    }

    // Sent new value to ESP
    socket.onopen = function() {
        // Mode
        $('#modeOrder .select-mode').on('click', function() {
            var newValue = parseInt($(this).attr('data-value'));

            socket.send(JSON.stringify({modeOrder: newValue}));
            setModeFromList(newValue, '#modeOrder');
        });
        // Brake
        $('#brakeSentOrder .select-mode').on('click', function() {
            var newValue = parseInt($(this).attr('data-value'));

            socket.send(JSON.stringify({brakeSentOrder: newValue}));
            setModeFromList(newValue, '#brakeSentOrder');
        });
        // Eco
        $('#ecoOrder').on('click', function() {
            var currentValue = parseInt($(this).attr('data-value'));
            var newValue = getNextModeValue(currentValue, 0, 2);

            socket.send(JSON.stringify({ecoOrder: newValue}));
            setEco(newValue);
        });
        // Accel
        $('#accelOrder').on('click', function() {
            var currentValue = parseInt($(this).attr('data-value'));
            var newValue = getNextModeValue(currentValue, 0, 5);

            socket.send(JSON.stringify({accelOrder: newValue}));
            setAccel(newValue);
        });
        // Lock
        $('#isLocked').on('click', function() {
            var currentValue = $(this).attr('data-value');
            var newValue = (currentValue == 1) ? 0 : 1;

            socket.send(JSON.stringify({isLocked: newValue}));
            setOnOff(newValue, '#isLocked');
        });
        // Speed limiter
        $('#speedLimiter').on('click', function() {
            var currentValue = $(this).attr('data-value');
            var newValue = (currentValue == 1) ? 0 : 1;

            socket.send(JSON.stringify({speedLimiter: newValue}));
            setOnOff(newValue, '#speedLimiter');
        });
        // Aux
        $('#auxOrder').on('click', function() {
            var currentValue = $(this).attr('data-value');
            var newValue = (currentValue == 1) ? 0 : 1;

            socket.send(JSON.stringify({auxOrder: newValue}));
            setOnOff(newValue, '#auxOrder');
        });
        // DST
        $('#distanceTrip').parent('.data').on('click', function() {
            /*$('#dialog').empty().html('Are you sure to reset distance trip ?');
            $('#dialog').dialog({
                dialogClass: 'no-close',
                buttons: [{
                    text: 'OK',
                    click: function() {
                        $(this).dialog('close');
                        socket.send(JSON.stringify({distanceTrip: 0}));
                        setDataFromSelector(0, '#distanceTrip');
                    }
                }]
            });*/
        });
    };

    // Avertissment if connection with server is loose
    socket.onclose = function() {
        alert('ERROR : Connection is closed ...');
    };
});
/*
datas = {
    "speedCurrent": 1,
    "speedMax": 0,
    "voltageFilterMean": 55723,
    "currentActual": 0,
    "currentTemperature": 26,
    "currentHumidity": 0,
    "distanceTrip": 65,
    "distanceOdo": 567,
    "batteryLevel": 86,
    "autonomyLeft": 34,
    "isLocked": 0,
    "bleBeaconVisible": 1,
    "beaconRSSI": -1,
    "bleLockForced": 0,
    "modeOrder": 3,
    "speedLimiter": 0,
    "ecoOrder": 2,
    "accelOrder": 0,
    "auxOrder": 0,
    "brakeSentOrder": 3,
    "brakeSentOrderFromBLE": 3,
    "brakePressedStatus": 0,
    "brakeFordidenHighVoltage": 0,
    "Ebrake_progressive_mode": 0,
    "time": 139912
};*/
/*
TODO

koxx / Francois Deslandes, [13.04.21 08:55]
1/ le frein ne peut être changé que si "Ebrake_progressive_mode" (nouvelle valeur) est à 0 (frein progressif non activé).
Si pas activé, le click est permis et seul la valeur 3 est affichée (et pas toute la liste de valeures).

koxx / Francois Deslandes, [13.04.21 08:55]
2/ la vmax ne fonctionne pas ;) -> OK

koxx / Francois Deslandes, [13.04.21 08:56]
3/ la vmoy est absente -> OK

koxx / Francois Deslandes, [13.04.21 08:56]
4/ quand on click sur une valeur, il y a un aller/retour sur la valeur car tu changes la valeur toi meme.
Il faudrait attendre le feedback pour changer la valeur je pense

koxx / Francois Deslandes, [13.04.21 08:57]
5/ le reset de distanceTrip ne fonctionne pas (envoi du distanceTrip à 0) -> EN COURS

koxx / Francois Deslandes, [13.04.21 08:58]
6/ pour les 2 times, le premier est le temps de fonctionnement, le 2eme est le temps de roulage (mais pas calculé par ESP,
donc a faire coté JS 😅 ... temps ou la vitesse est différente de 0) -> OK
*/
