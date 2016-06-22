// Test/demo for prototype llnode Javascript API
var http = require("http");
const llnode_module = require('../build/Release/llnode_module');
const fs = require('fs');

function my_listener(request, response) {

    switch (request.url) {
    case '/':
        console.log('lldbdemo.js: core dump list request');
        response.writeHead(200, "OK",{'Content-Type': 'text/html'});
        response.write('<html><head><title>Diagnostic NPM Demo</title></head><body>');
        response.write('<h2>Node Post-Mortem Diagnostics Server</h2>');

        // Locate and list any core dumps
        response.write('<p> Core dump repository:<p>');
        files = fs.readdirSync('.');
        for (var i = 0; i < files.length; i++) {
            if (files[i].substring(0,4) == 'core' ) {
                var stats = fs.lstatSync('./' + files[i]);
                response.write('<br>' + files[i]);
                response.write('<br>' + stats.mtime);
            }
        }
        response.write('<p>');
        response.write('<form enctype="application/x-www-form-urlencoded" action="/formhandler" method="post">');
        response.write('Select core dump for analysis: <input type="text" name="filename" value="" /><br />');
        response.write('<input type="submit" value="Submit"/>');
        response.write('</form></body></html');
        response.end();
        break;

    case '/formhandler':
        console.log('lldbdemo.js: form response');
        var inputData = 'null'
        request.on('data', function(input){
            inputData = input.toString();
        });
        request.on('end', function(){
            response.writeHead(200,{"Content-Type": "text/html"});
            response.write('<h2>Node Post-Mortem Diagnostics Server</h2>');
            response.write('<p>Loading core dump: ' + inputData.split("=")[1] + '\n');
            llnode_module.loadDump(inputData.split("=")[1]);
            // Display thread stacks in a table
            var threads = llnode_module.getThreadCount();
            response.write('<p><table border="1" style="width:100%">');
            response.write('<tr><th><pre>Thread number</pre></th><th align=left><pre>Thread stack frames</pre></th></tr>');
            for (var j = 0; j < threads; j++){
                var frames = llnode_module.getFrameCount(j);
                response.write('<tr valign=top><td><pre>Thread #' + j + '</pre></td><td><pre>');
                for (var k = 0, count = 0; k < frames; k++){
                    var frame = llnode_module.getFrame(j,k);
                    if (frame.substring(0,3) == '???'){
                        // skip unknown frame
                    } else {
                        response.write(count++ + ': ' + frame);
                    }
                }
                response.write('</pre></td></tr>');
            }
            response.write('</table>');
            response.end();
        });
        break;
    default:
        response.writeHead(404, "Not found",{'Content-Type': 'text/html'});
        response.end('<html><head><title>404 - Not found</title></head><body><h1>Not found.</h1></body></html>');
        console.log("lldbdemo.js: [404] " + req.method + " to " + req.url);
    };
}

var http_server = http.createServer(my_listener);
http_server.listen(2000);

console.log('Node Post-Mortem Diagnostic Server running, go to http://<machine>:2000/ or http://localhost:2000/');

setTimeout(function(){
    console.log('Node Post-Mortem Diagnostics Server closing down....');
    process.exit(0); 
}, 120000);

