#include <Arduino.h>

const char MAIN_page[] PROGMEM = R"=====(
<html>
<meta name="viewport" content="width=device-width, initial-scale=1">
<head>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/2.1.3/jquery.min.js"></script>
<script src="https://code.jquery.com/ui/1.12.1/jquery-ui.js"></script>
<link rel="stylesheet" href="https://code.jquery.com/ui/1.12.1/themes/base/jquery-ui.css">
<script src="https://cdn.jsdelivr.net/npm/@jaames/iro/dist/iro.min.js"></script>
</head>

<script>
$( function() {
    $( "#tabs" ).tabs();
} );
</script>
       
      
<body>
<div id="tabs">
<ul>
    <li><a href="#tabs-1" id="tab-rainbow">Rainbow</a></li>
    <li><a href="#tabs-2" id="tab-two-colours">Two colours</a></li>
    <li><a href="#tabs-3" id="tab-imu">IMU</a></li>
</ul>
<div id="tabs-1">
<!-- TAB rainbow -->
<!-- TAB rainbow -->
</div>
<div id="tabs-2">
<!-- TAB two colours -->
<div>
    <table><tr>
    <td><span id="color-picker-container1" class="col-md-6" style="width:40%;"></span></td>
    <td><span id="color-picker-container2" class="col-md-6" style="width:40%;"></span></td>
    </tr></table>
</div>
</div>
<!-- TAB two colours -->
<div id="tabs-3">
<!-- TAB IMU -->
<!-- TAB IMU -->
</div>
</div>

<div>
    <label for="delay_spinner">Delay (msec)</label>
    <input id="delay_spinner" class="delay_spinner" name="delay_spinner" value="10">
</div>
<div>
    <label for="waves_spinner">n</label>
    <input id="waves_spinner" class="waves_spinner" name="waves_spinner" value="1">
</div>

<script>
var colorPicker1 = new iro.ColorPicker('#color-picker-container1', {
  width: 160,
  display: "inline"
});
var colorPicker2 = new iro.ColorPicker('#color-picker-container2', {
  width: 160,
  display: "inline"
});
colorPicker1.on('color:change', sendRGB1);
colorPicker2.on('color:change', sendRGB2);

function sendDelay() {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "setTimedelay?n="+$("#delay_spinner").val(), true);
  xhttp.send();
}

function senddTheta() {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "setdTheta?n="+$("#dtheta_spinner").val(), true);
  xhttp.send();
}

function sendRGB1(color, changes) {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "setColour1?r="+color.rgb.r+"&g="+color.rgb.g+"&b="+color.rgb.b, true);
    xhttp.send();
  console.log(color.hexString);
}

function sendRGB2(color, changes) {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "setColour2?r="+color.rgb.r+"&g="+color.rgb.g+"&b="+color.rgb.b, true);
    xhttp.send();
  console.log(color.hexString);
}


$( function() {
$( "#delay_spinner" ).spinner({
    min: 1,
    max: 1000,
    step: 10,
    change: sendDelay,
    stop: sendDelay
});

$( "#dtheta_spinner" ).spinner({
    min: -10,
    max: 10,
    change: senddTheta,
    stop: senddTheta
});

$("#wave_selector").change(function() {
    $.ajax({
      url: "setHarmonicFactor?n=" + $("#wave_selector").val()
    });
  });

$("#tab-rainbow").click(function() {
    $.ajax({
    url: "setAnimationMode?mode=rainbow"
    });
});

$("#tab-two-colours").click(function() {
    $.ajax({
    url: "setAnimationMode?mode=two_colours"
    });
});
        
$("#tab-imu").click(function() {
    $.ajax({
    url: "setAnimationMode?mode=imu"
    });
});
});

</script>


</body>
</html>
)=====";
