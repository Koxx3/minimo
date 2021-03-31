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
body {
  margin: 0;
  padding: 0;
  background-color: #00013D;
}

* {
  font-family: Helvetica, sans-serif;
  color: #fff;
}

h1 {
  text-align: center;
  /*text-transform: uppercase;*/
  letter-spacing: 3px;
  color: #eee;
  position: relative;
  z-index: 100;
}
h1 a {
  letter-spacing: 1px;
  text-transform: lowercase;
  color: #3F51B5;
  font-size: 30px;
  transition: all 0.5s ease;
  -webkit-transition: all 0.5s ease;
  -moz-transition: all 0.5s ease;
  -o-transition: all 0.5s ease;
  -ms-transition: all 0.5s ease;
}
h1 a:hover {
  color: #F44336;
}

main.wrap {
  position: absolute;
  top: 0;
  /* height: 70%; */
  /* transform: translateY(-50%); */
  /* -webkit-transform: translateY(-50%); */
  /* -moz-transform: translateY(-50%); */
  /* -o-transform: translateY(-50%); */
  /* -ms-transform: translateY(-50%); */
  width: 100%;
  left: 5px;
}
main.wrap .tile-wrap {
  display: inline-block;
  margin: 0;
  max-width: 50%;
}
main.wrap .tile-wrap h3 {
  margin: 0;
  padding: 0;
  color: #aaa;
  text-align: center;
}

.tile {
  display: inline-block;
  width: 100%;
  height: 125px;
  background-color: #7E3FF3;
  outline: none;
  
  cursor: pointer;
  border: 5px solid #00013D;
  padding: 0;
  float: left;
  transition: all 0.5s ease;
  -webkit-transition: all 0.5s ease;
  -moz-transition: all 0.5s ease;
  -o-transition: all 0.5s ease;
  -ms-transition: all 0.5s ease;
}
.tile .icon {
  font-size: 60px;
}
.tile .name {
  text-align: center;
  opacity: 0;
  transform: translateY(20px);
  -webkit-transform: translateY(20px);
  -moz-transform: translateY(20px);
  -o-transform: translateY(20px);
  -ms-transform: translateY(20px);
  transition: all 0.5s ease;
  -webkit-transition: all 0.5s ease;
  -moz-transition: all 0.5s ease;
  -o-transition: all 0.5s ease;
  -ms-transition: all 0.5s ease;
}
.tile:hover {
  border-color: #fff;
}
.tile:hover > .name {
  transform: translateY(0px);
  -webkit-transform: translateY(0px);
  -moz-transform: translateY(0px);
  -o-transform: translateY(0px);
  -ms-transform: translateY(0px);
  opacity: 1;
}

.short {
  width: 50%;
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
