#include "Arduino.h"

static const char JS_DASHBOARD_PAGE[] PROGMEM = R"=====(
<script type="text/javascript">

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

};

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
h1 {
    color: #ffffff;
    margin: 0 0 5px;
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
.block-info > .block-middle {
    width: 50%;
    float: left;
}
.block-info > .block-middle > .content > .data {
    font-size: 14pt;
    text-transform: uppercase;
}
.block-info .content {
    text-align: center;
    padding: 10px 8px;
}
.block-info .content > .legend {
    text-transform: uppercase;
    font-weight: bold;
    margin-bottom: 8px;
    font-size: 10pt;
    text-align: left;
}
.block-info .content > .data {
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

)=====";


static const char HTML_DASHBOARD_PAGE[] PROGMEM = R"=====(
	<div class="content-wrapper">
		<main class="wrap">
			<div class="app-title">
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

					<div class="block-info">
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
					<div class="block-info">
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
							<div class="data">Off</div>
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
							<div class="data">0.0 V</div>
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
            
			<div class="app-bottom">
				<h3 id="version">Smart-e8 sd_minimo_35 v38</h3>
			</div>
		</main>
	</div>
)=====";
