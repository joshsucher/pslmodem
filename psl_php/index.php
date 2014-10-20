<?php
session_start();
date_default_timezone_set('America/New_York');

if(empty($_SERVER['CONTENT_TYPE']))
{ 
  $_SERVER['CONTENT_TYPE'] = "application/x-www-form-urlencoded"; 
}

?>

<html>
<head>
<title>YOLO PSL YOLO</title>

<link type="text/css" href="psl.css" rel="stylesheet" />

<script src="//use.typekit.net/YOURTYPEKITID.js"></script>
<script>try{Typekit.load();}catch(e){}</script>

<script src="//code.jquery.com/jquery-1.11.0.min.js"></script>

<body>

<div class="container"><div class="innercontainer">

<div id="header">it's pumpkin spice latte season, motherfuckers.</div>

<div id="separator"></div>

<div id="tweet" class="content"></div>
<div id="author" class="content"></div>
<div id="timestamp" class="content"></div>
<div id="uid" class="content"></div>

<script>

$(document).ready(function(){ // added

setInterval(function() {

     $.ajax({

        dataType : 'json', //You need to declare data type
        url: "psl_json.php",
		error: function(xhr,textStatus,ErrorThrown){
		alert("error");
		},
        success: function(json){
           $( "#tweet" ).text("\"" + json.tweet + "\"" );
           $( "#timestamp" ).text( json.timestamp );
           $( "#author" ).text("- " + json.user );
           $( "#uid" ).text( json.id );
        }
      }); //$.ajax ends here
	return false 

}, 5000);

});

</script>

</div></div>

</body>
</html>