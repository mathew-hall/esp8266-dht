<html><head><title>DHT</title>
<link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
<div id="main">
<h1>DHT 22 temperature/humidity sensor</h1>
<p>
	It looks like there %sensor_present% a sensor hooked up to GPIO2
</p>
<p>
If there's one connected, its temperature reading is %temperature%*C, humidity is %humidity%.
</p>
<br/>
<p>
	If there's a DS18B20 connected on GPIO port 0, its reading is %ds_temperature%.
</p>
<button onclick="location.href = '/';" class="float-left submit-button" >Home</button>
</div>
</body></html>
