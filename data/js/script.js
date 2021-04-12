// https://dev.to/paprajith/create-a-jquery-like-library-in-vanilla-javascript-3m9d
/*$(() => {
    // Websocket
    var socket = new WebSocket('ws://koxx3.fr.eu.org:81/');
    socket.onmessage = function(event) {
        var datas = JSON.parse(event.data);

        setDatasToDashboard(datas);
    }
});*/

$(function() {
    // Websocket
    var socket = new WebSocket('ws://koxx3.fr.eu.org:81/');
    socket.onmessage = function(event) {
        var datas = JSON.parse(event.data);

        setDatasToDashboard(datas);
    }
});

/*
TODO :
Format d'envoie vers le serveur :
doc["modeOrder"] = WifiSettingsPortal_shrd->modeOrder;
doc["distanceTrip"] = WifiSettingsPortal_shrd->distanceTrip / 100;
doc["bleLockForced"] = WifiSettingsPortal_shrd->bleLockForced;
doc["speedLimiter"] = WifiSettingsPortal_shrd->speedLimiter;
doc["ecoOrder"] = WifiSettingsPortal_shrd->ecoOrder;
doc["accelOrder"] = WifiSettingsPortal_shrd->accelOrder;
doc["auxOrder"] = WifiSettingsPortal_shrd->auxOrder;
doc["brakeSentOrderFromBLE"] = WifiSettingsPortal_shrd->brakeSentOrderFromBLE;

Les valeurs max doivent être générées localement
*/
