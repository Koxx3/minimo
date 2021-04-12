$(function() {
    // Websocket instance
    var socket = new WebSocket('ws://koxx3.fr.eu.org:81/');

    // Get values from ESP
    socket.onmessage = function(event) {
        var datas = JSON.parse(event.data);

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
    };
});
