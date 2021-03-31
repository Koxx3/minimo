#include "Arduino.h"

static const char JS_DASHBOARD_PAGE[] PROGMEM = R"=====(
<script type="text/javascript">
/*
var ws = new WebSocket("ws://" + location.host +":81");

function SendText() {
  ws.send('Hello, world');
}

ws.onmessage = function(event) {
  var msg = JSON.parse(event.data);

  switch(msg.id) {
    case 1:
      document.getElementById("ACE_DASHBOARD_text1").innerHTML = 'Init' + msg.value;
      break;
    case 2:
      document.getElementById("ACE_DASHBOARD_text1").innerHTML = 'Reply : ' + msg.value;
      break;
    case 3:
      document.getElementById("ACE_DASHBOARD_text1").innerHTML = 'Temperature : ' + msg.value;
      break;
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
    $('#battery-gauge').css({'width': (batteryLevelWidth - maskBorderWidth) + 'px'})
}
*/

const getArgumentType = option => typeof option;
const createObjectFromArray = arr => Object.fromEntries([arr]);
const addListener = (elements, action, listener) => {
    elements.forEach(ele => ele.addEventListener(action, listener));
};

const invokeCallbackForElements = (elements, callback) => {
    elements.forEach((ele, itx) => {
        const fn = callback.bind(ele);
        fn(itx);
    });
};

const setPropertyCustomLogicForElements = (elements, prop, options, customFn) => {
    elements.forEach((ele) => {
        for (const key in options) {
            customFn(ele, prop, key, options[key]);
        }
    });
};

const setPropertyCustomLogic = (elements, prop, data, customFn) => {
    const dataType = getArgumentType(data[0]);
    if (dataType === 'string') {
        setPropertyCustomLogicForElements(
            elements,
            prop,
            createObjectFromArray([data[0], data[1]]),
            customFn
        );
    } else if (dataType === 'object') {
        setPropertyCustomLogicForElements(elements, prop, data[0], customFn);
    }
};

const addEvents = (ele, prop, callback) => {
    ele[prop] = callback;
};

const addEventToElements = (elements) => {
    addEvents(elements, "css", (...opts) =>
        setPropertyCustomLogic(elements, 'style', opts, (ele, prop, key, value) => {
            ele[prop][key] = value;
        })
    );

    addEvents(elements, 'html', () => elements[0].innerHTML);
    addEvents(elements, 'text', () => elements[0].textContent);
    addEvents(elements, 'width', () => elements[0].offsetWidth);
    addEvents(elements, 'height', () => elements[0].offsetHeight);

    addEvents(elements, 'on', (...opts) =>
        addListener(elements, opts[0], opts[1])
    );

    addEvents(elements, 'each', (opts) => {
        invokeCallbackForElements(elements, opts);
    });

};

const $ = (argument) => {
    const argumentType = getArgumentType(argument);
    if (argumentType === 'function') {
        window.addEventListener('DOMContentLoaded', argument);
        return;
    }
    if (argumentType === 'string') {
        const elements = document.querySelectorAll(argument);
        addEventToElements(elements);
        return elements;
    } else if (argumentType === 'object' && argument instanceof HTMLElement) {
        const elements = [argument];
        addEventToElements(elements);
        return elements;
    }
};

// https://dev.to/paprajith/create-a-jquery-like-library-in-vanilla-javascript-3m9d
$(() => {
    // Battery level
    showBatteryLevel(45);
});

</script>


)=====";


static const char CSS_DASHBOARD_PAGE[] PROGMEM = R"=====(
* {
    box-sizing: border-box;
}
body {
    margin: 0;
    padding: 0;
    background-color: #00013D;
    font-family: Sans-Serif;
}
.app-title {
    padding: 15px;
}
h3 {
    color: #b0b0d4;
    margin: 0;
    font-weight: normal;
    font-size: 12pt;
}
.clearfix {
    clear: both;
}
.base-panel {
    margin: 0 !important;
}
.noorder {
    display: block !important;
}
.container {
    width: 100%;
}
.blocks-left,
.blocks-right {
    width: 50%;
    float: left;
}
.block-info {
    margin: 10px;
    color: #ffffff;
    background-color: #7643ea;
}
.block-info-double {
    margin: 10px;
    color: #ffffff;
}
.block-info-double > .block-middle {
    width: 50%;
    float: left;
    background-color: #7643ea;
}
.block-info-double > .block-middle:nth-child(1) {
    border-right: 3px solid #00013d
}
.block-info-double > .block-middle:nth-child(2) {
    border-left: 3px solid #00013d
}
.block-info > .block-middle > .content > .data,
.block-info-double > .block-middle > .content > .data {
    font-size: 14pt;
    text-transform: uppercase;
}
.block-info .content,
.block-info-double .content {
    text-align: center;
    padding: 10px 8px;
    position: relative;
}
.block-info .content > .legend,
.block-info-double .content > .legend {
    text-transform: uppercase;
    font-weight: bold;
    margin-bottom: 8px;
    font-size: 9pt;
    text-align: left;
}
.block-info .content > .legend-data {
    font-size: 16pt;
    font-weight: bold;
    position: absolute;
    top: 10px;
    right: 5px;
}
.block-info .content > .data,
.block-info-double .content > .data {
    font-size: 22pt;
    font-weight: bold;
}
.block-info .content > .data > .select-mode {
    color: #9796a4;
}
.block-info .content > .data > .select-mode.selected {
    color: #ffffff;
}
.block-info .content > .sub-data {
    font-size: 16pt;
    font-weight: bold;
}
/* Battery level */
#battery-gauge {
    background: linear-gradient(90deg, #ff0000 0%, #fcfd1d 50%, #64fc45 100%);
    /*background-color: #ffffff;*/
    height: 10px;
    width: 80%;
    margin: 8px auto 0 auto;
    position: relative;
}
#battery-level {
    background-color: #969a9b;
    float: right;
    width: 50%;
    margin: 0;
    height: 100%;
}
#battery-mask-container {
    position: absolute;
    height: 100%;
    /*width: 100%;*/
    position: absolute;
    top: -2px;
}
#battery-mask-container td {
    height: 10px;
    /*border-right: 2.33px solid #7643ea;*/
    border-top: 0;
    border-bottom: 0;
    border-left: 0;
    border-style: solid;
    border-color: #7643ea;
    width: 10%;
}
#battery-mask-container td:nth-child(10) {
    border-right: 0;
}

)=====";


static const char HTML_DASHBOARD_PAGE[] PROGMEM = R"=====(
	<div class="content-wrapper">
		<main class="wrap">
			<div class="app-title">
				<h3 id="version">Smart-e8 sd_minimo_35 v38</h3>
			</div>

			<div class="container">
				<div class="blocks-left">
					<div class="block-info">
						<div class="content">
							<div class="legend">Speed (M)</div>
							<div class="data">0 km/h</div>
						</div>
					</div>
					<div class="block-info">
						<div class="content">
							<div class="legend">Mode</div>
							<div class="data">
								<span class="select-mode" data-value="1">1</span>
								<span class="select-mode" data-value="2">2</span>
								<span class="select-mode selected" data-value="3">3</span>
							</div>
						</div>
					</div>
					<div class="block-info">
						<div class="content">
							<div class="legend">Brake</div>
							<div class="data">
								<span class="select-mode selected" data-value="1">1</span>
								<span class="select-mode" data-value="2">2</span>
								<span class="select-mode" data-value="3">3</span>
								<span class="select-mode" data-value="4">4</span>
								<span class="select-mode" data-value="5">5</span>
							</div>
						</div>
					</div>
					<div class="block-info-double">
						<div class="block-middle">
							<div class="content">
								<div class="legend">Eco</div>
								<div class="data">None</div>
							</div>
						</div>
						<div class="block-middle">
							<div class="content">
								<div class="legend">Accel.</div>
								<div class="data">Max</div>
							</div>
						</div>
						<div class="clearfix"></div>
					</div>
					<div class="block-info-double">
						<div class="block-middle">
							<div class="content">
								<div class="legend">Lock</div>
								<div class="data">Off</div>
							</div>
						</div>
						<div class="block-middle">
							<div class="content">
								<div class="legend">Spd. Lmt.</div>
								<div class="data">Off</div>
							</div>
						</div>
						<div class="clearfix"></div>
					</div>
					<div class="block-info">
						<div class="content">
							<div class="legend">Aux</div>
							<div class="data">OFF</div>
						</div>
					</div>
					<div class="block-info">
						<div class="content">
							<div class="legend">Dst</div>
							<div class="data">0.0 km</div>
							<div class="sub-data">237.8 km</div>
						</div>
					</div>
					<div class="block-info">
						<div class="content">
							<div class="legend">Time</div>
							<div class="data">0:00:05</div>
							<div class="sub-data">0:00:00</div>
						</div>
					</div>
				</div>
				<div class="blocks-right">
					<div class="block-info">
						<div class="content">
							<div class="legend">Voltage</div>
							<div class="legend-data">45%</div>
							<div class="data">64.3 V</div>
							<div class="sub-data">
								<div id="battery-gauge">
									<div id="battery-level"></div>
									<table id="battery-mask-container">
										<tbody>
											<tr>
												<td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td>
											</tr>
										</tbody>
									</table>
								</div>
							</div>
						</div>
					</div>
					<div class="block-info">
						<div class="content">
							<div class="legend">Auton.</div>
							<div class="data">0 km</div>
						</div>
					</div>
					<div class="block-info">
						<div class="content">
							<div class="legend">Cur.</div>
							<div class="data">0.0 A</div>
							<div class="sub-data">0.0 A</div>
						</div>
					</div>
					<div class="block-info">
						<div class="content">
							<div class="legend">Power</div>
							<div class="data">0 W</div>
							<div class="sub-data">0 W</div>
						</div>
					</div>
					<div class="block-info">
						<div class="content">
							<div class="legend">Temp.</div>
							<div class="data">31.0 °</div>
							<div class="sub-data">31.0 °</div>
						</div>
					</div>
					<div class="block-info">
						<div class="content">
							<div class="legend">Hr</div>
							<div class="data">30.3 %</div>
						</div>
					</div>
				</div>
			</div>
		</main>
	</div>
)=====";
