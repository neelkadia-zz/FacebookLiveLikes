/*
 Facebook Live Like BackEnd

 Created in 22 Feb 2014
 by Neel Kadia
 
 This code is in the public domain.
 
 */ 

var my_http = require("http");

var sys                 = require("sys");
var events              = require("events");
var url                 = require("url");
var path                = require("path");
var fs                  = require('fs');
var i= 0;
var facebook_client = my_http.createClient(80, "graph.facebook.com");
var facebook_emitter = new events.EventEmitter();	
function get_data() {
	var request = facebook_client.request("GET", "/20531316728", {"host": "graph.facebook.com"});
	request.addListener("response", function(response) {
		var body = "";
		response.addListener("data", function(data) {
			body += data;
		});

		response.addListener("end", function() {
			var data = JSON.parse(body);
			facebook_emitter.emit("data", String(data.likes));
		});
	});
	request.end();
}
my_http.createServer(function(request,response){
	var my_path = url.parse(request.url).pathname;
	    if(my_path === "/getdata") {
	    	sys.puts("Request came! "+i++);
			var listener = facebook_emitter.once("data", function(data) {
				response.writeHeader(200, { "Content-Type" : "text/plain" });
	    		response.write("<"+data+">");
	    		response.end();
			});
		}
}).listen(8080);
setInterval(get_data,1000);
sys.puts("Server Running on 8080");
