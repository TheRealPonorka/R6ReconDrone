#include "esp_http_server.h"
#include "Arduino.h"

extern int gpLb;
extern int gpLf;
extern int gpRb;
extern int gpRf;
extern int gpLed;
extern byte RED;
extern byte GREEN;
extern byte BLUE;

void WheelAct(int nLf, int nLb, int nRf, int nRb);

esp_err_t index_handler(httpd_req_t *req){
    httpd_resp_set_type(req, "text/html");
    String page = R"=="==(
<!DOCTYPE html>
<html>
<head>
	<title>R6 Siege Drone WebUI</title>
	<meta http-equiv="content-type" content="text/html; charset=utf-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0">
	<script>var xhttp = new XMLHttpRequest();</script><script>function getsend(arg) { xhttp.open('GET', arg +'?' + new Date().getTime(), true); xhttp.send() } </script>
</head>

<body>
  <div class="main">
	<div class="left">
		<button class="buttonGo" onmousedown="getsend('go')" onmouseup="getsend('stop')" ontouchstart="getsend('go')" ontouchend="getsend('stop')">W</button>
		<button class="buttonStop" onmousedown="getsend('stop')" onmouseup="getsend('stop')">-</button>
		<button class="buttonBack" onmousedown="getsend('back')" onmouseup="getsend('stop')" ontouchstart="getsend('back')" ontouchend="getsend('stop')">S</button>
	</div>
	
	<div class="right">
		<button class="buttonLeft" onmousedown="getsend('left')" onmouseup="getsend('stop')" ontouchstart="getsend('left')" ontouchend="getsend('stop')">A</button>
		<button class="buttonRight" onmousedown="getsend('right')" onmouseup="getsend('stop')" ontouchstart="getsend('right')" ontouchend="getsend('stop')">D</button>
	</div>
	
	<div class="stream">
		<img class="video" src="http://192.168.6.6:81/stream">
	</div>
	
			<div class="flashlight">
			<button class="buttonLedOn" onmousedown="getsend('ledon')"><b>Flash ON</b></button>
			<button class="buttonLedOff" onmousedown="getsend('ledoff')">Flash OFF</button>
		</div>
    <div class="bottom">

	
		<div class="RGB1">
			<p class="RGBheding">LED bar color</p>
			<button class="buttonRgbDefault" onmousedown="getsend('ledDEFAULT')">DEFAULT</button>
			<button class="buttonRgbRed" onmousedown="getsend('ledRED')">RED</button>
			<button class="buttonRgbYellow" onmousedown="getsend('ledYELLOW')">YELLOW</button>
		</div>
		<div class="RGB2">
			<button class="buttonRgbBlue" onmousedown="getsend('ledBLUE')">BLUE</button>
			<button class="buttonRgbPurple" onmousedown="getsend('ledPURPLE')">PURPLE</button> 
			<button class="buttonRgbOrange" onmousedown="getsend('ledORANGE')">YELLOW</button>
		</div>
		<div class="RGB3">
			<button class="buttonRgbOff" onmousedown="getsend('rgbOFF')">OFF</button>
		</div>
		
		<div class="settings">
			<button class="buttonSettings" onclick="window.open('/cp')" ><b>&#9881;</b></button>
		</div>
	</div>
  </div>


<footer>
	<p class="inspiration"> Insipired by:<a href="https://youtu.be/NfWd9Kn30i0" target="_blank">HemRobotics</a><p>
	<p class="project"> Project details available on <a href="https://github.com/TheRealPonorka/R6ReconDrone" target="_blank">TheRealPonorka GitHub</a></p>
	<p class="fullBuild"> Designed and built by <a href="https://www.facebook.com/sweeetsquad" target="_blank">Ponorka</a></p>
</footer>



  <style>	
	body {
		background-color:rgb(70,70,70,0.6);
	}
	.left {
		float:left;
		height:100%;
		margin-top:14%;
		margin-left:4%;
	
	}
	
	.buttonGo, .buttonStop, .buttonBack{
		margin: 10px;
		font-size: 35px;
		display:block;
		height: 50px;
		width: 50px;
		border-radius: 50%;
		border: 0px;
	}
	
	.right {
		float:right;
		height:100%;
		margin-top:18%;
		margin-right:4%;
		padding:auto;
	}

	.buttonLeft, .buttonRight{
		margin: 10px;
		font-size: 35px;
		height: 50px;
		width: 50px;
		border-radius: 50%;
		border: 0px;
	}	
	
	.stream {
		display:block;
		margin-left:auto;
		margin-right:auto;
	
	}
	
	.video, .src, .img{
		object-fit: fill;
		display: block;
		position: fixed;
		left: 30%;
		width: 768px;
		top:7%;		
	}
	
	element.style {
		object-fit: fill;
		display: block;
		position: fixed;
		left: 30%;
		width: 768px;
	}
	
	.bottom {
		position:fixed;
		bottom:5%;
		float:left;
		display:block;
		border-style: hidden;
		border-radius: 0.4em;
		background-color: rgb(181,181,181,0.4);
	}
	
	.flashlight{
		position:fixed;
		bottom:5%;
		left:47%;
		float:right;
		border-style: hidden;
		border-radius: 0.4em;
		text-align:center;
		background-color:rgb(181,181,181,0.4);
	}

	.buttonLedOff, .buttonLedOn{
		margin: 2%;
	}
	
	.buttonLedOn{
		font-size:0.9em;
	}
	
	.buttonLedOff{
		font-size:0.9em;
	}	

	.RGB1, .RGB2, .RGB3{
		margin:10px;
	}
	
	.RGBheding{
		text-align:center;
	}
	
	.buttonRgbDefault, .buttonRgbRed, .buttonRgbYellow, .buttonRgbBlue, .buttonRgbPurple, .buttonRgbOrange{
		width:80px;
	}
	
	.buttonRgbDefault{
		background-color:green;
		color:white;
	}
	
	.buttonRgbRed{
		background-color:red;
		color:white;
	}
	
	.buttonRgbYellow{
		background-color:yellow;
		color:black;
	}

	.buttonRgbBlue{
		background-color:blue;
		color:white;
	}

	.buttonRgbPurple{
		background-color:purple;
		color:white;
	}
	
	.buttonRgbOrange{
		background-color:Orange;
		color:black;
	}

	.buttonRgbOff{
		width:250px;
		background-color:black;
		color:white;
	}
	
	.settings{
		position:fixed;
		float:right;
		right:2%;
		margin-right:auto;
		display:block;
		bottom:5%;
	}
	.buttonSettings {
		font-size: 1.9em;
		opacity: 0.7;
	}
	
	footer{
		position: fixed;
		bottom: 0px;
		height: 30px;
		width: 100%;
		display: block;
		border-style: hidden;
		background-color: rgb(181,181,181,0.4);
		left: 0px;
	}
	
	.inspiration, .fullBuild, .project {
		position:fixed;
		diplay:block;
		margin:auto;
		text-align:center;
		font-size:0.8em;
	} 
	
	.inspiration{
		float:left;
		bottom:1%;
		left:2%;
	}
	
	.fullBuild{
		float:right;
		right:2%;
		bottom:1%;
	}

	.project{
		float:left;
		bottom:1%;
		left:42%;
	}
	
    /* Code block from https://stackoverflow.com/a/4407335/6202405 is
	licensed under CC BY-SA 4.0 */
    .noselect {
      -webkit-touch-callout: none; /* iOS Safari */
	-webkit-user-select: none; /* Safari */
	 -khtml-user-select: none; /* Konqueror HTML */
	   -moz-user-select: none; /* Old versions of Firefox */
	    -ms-user-select: none; /* Internet Explorer/Edge */
		user-select: none; /* Non-prefixed version, currently
				      supported by Chrome, Edge, Opera and Firefox */
    }
    /* End of code block */
  </style>
  </body>
</html>
)=="==";

    return httpd_resp_send(req, &page[0], strlen(&page[0]));
}

esp_err_t go_handler(httpd_req_t *req){
    WheelAct(1.0f, 0f, 1.0f, 0f);
    Serial.println("Go with float");
    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, "OK", 2);
}

esp_err_t back_handler(httpd_req_t *req){
    WheelAct(LOW, HIGH, LOW, HIGH);
    Serial.println("Back");
    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, "OK", 2);
}

esp_err_t left_handler(httpd_req_t *req){
    WheelAct(HIGH, LOW, LOW, HIGH);
    Serial.println("Left");
    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, "OK", 2);
}

esp_err_t right_handler(httpd_req_t *req){
    WheelAct(LOW, HIGH, HIGH, LOW);
    Serial.println("Right");
    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, "OK", 2);
}

esp_err_t stop_handler(httpd_req_t *req){
    WheelAct(LOW, LOW, LOW, LOW);
    Serial.println("Stop");
    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, "OK", 2);
}

esp_err_t ledon_handler(httpd_req_t *req){
    digitalWrite(gpLed, HIGH);
    Serial.println("LED ON");
    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, "OK", 2);
}

esp_err_t ledoff_handler(httpd_req_t *req){
    digitalWrite(gpLed, LOW);
    Serial.println("LED OFF");
    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, "OK", 2);
}

esp_err_t ledDEFAULT_handler(httpd_req_t *req){ 
    RED = 0;
    GREEN = 255;
    BLUE = 0;
    Serial.println("ledDEFAULT");               
    httpd_resp_set_type(req, "text/html");  
    return httpd_resp_send(req, "OK", 2);   
}

esp_err_t ledRED_handler(httpd_req_t *req){ 
    RED = 255;
    GREEN = 0;
    BLUE = 0;
    Serial.println("ledRED");               
    httpd_resp_set_type(req, "text/html");  
    return httpd_resp_send(req, "OK", 2);   
}

esp_err_t ledYELLOW_handler(httpd_req_t *req){ 
    RED = 255;
    GREEN = 255;
    BLUE = 0;
    Serial.println("ledYELLOW");               
    httpd_resp_set_type(req, "text/html");  
    return httpd_resp_send(req, "OK", 2);   
}

esp_err_t ledBLUE_handler(httpd_req_t *req){ 
    RED = 0;
    GREEN = 0;
    BLUE = 255;
    Serial.println("ledBLUE");               
    httpd_resp_set_type(req, "text/html");  
    return httpd_resp_send(req, "OK", 2);   
}

esp_err_t ledPURPLE_handler(httpd_req_t *req){ 
    RED = 128;
    GREEN = 0;
    BLUE = 128;
    Serial.println("ledPURPLE");               
    httpd_resp_set_type(req, "text/html");  
    return httpd_resp_send(req, "OK", 2);   
}

esp_err_t ledORANGE_handler(httpd_req_t *req){ 
    RED = 255;
    GREEN = 140;
    BLUE = 0;
    Serial.println("ledORANGE");               
    httpd_resp_set_type(req, "text/html");  
    return httpd_resp_send(req, "OK", 2);   
}

esp_err_t rgbOFF_handler(httpd_req_t *req){ 
    RED = 0;
    GREEN = 0;
    BLUE = 0;
    Serial.println("rgbOFF");               
    httpd_resp_set_type(req, "text/html");  
    return httpd_resp_send(req, "OK", 2);   
}

void WheelAct(int nLf, int nLb, int nRf, int nRb) {

    digitalWrite(gpLf, nLf);
    digitalWrite(gpLb, nLb);
    digitalWrite(gpRf, nRf);
    digitalWrite(gpRb, nRb);
}
