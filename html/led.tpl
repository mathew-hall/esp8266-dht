<html><head><title>LED</title>
<link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
<div id="main">
<h1>The LED</h1>
<p>
If there's a LED connected to GPIO13, it's now %ledstate%. You can change that using the buttons below.
</p>
<form method="get" action="led.cgi">
<input type="submit" name="led" value="1">
<input type="submit" name="led" value="0">
</form>
<button onclick="location.href = '/';" class="float-left submit-button" >Home</button>
</div>
</body></html>
