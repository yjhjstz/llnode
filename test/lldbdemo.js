// Test/demo for prototype llnode Javascript API
var http = require("http");
const llnode_module = require('../build/Release/llnode_module');
const fs = require('fs');

function my_listener(request, response) {

    switch (request.url) {
    case '/':
        console.log('lldbdemo.js: core dump list request');
        response.writeHead(200, "OK",{'Content-Type': 'text/html'});
        response.write('<html><head><title>Diagnostic Javascript API - NPM Demo</title></head><body>');
        response.write('<h2>Diagnostic Javascript API - NPM Demo</h2>');

        // Locate and list any core dumps
        response.write('<p> Core dumps found:<br>');
        files = fs.readdirSync('.');
        for (var i = 0; i < files.length; i++) {
            if (files[i].substring(0,4) == 'core' ) {
                var stats = fs.lstatSync('./' + files[i]);
                response.write('<br>' + files[i] + '&emsp;' + stats.mtime);
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
            var core_file = inputData.split("=")[1];
            response.writeHead(200,{"Content-Type": "text/html"});
            response.write('<h2>Diagnostic Javascript API - NPM Demo</h2>');
            response.write('<p>Loading core dump: ' + core_file + '\n');
            process.env.LLNODE_RANGESFILE = core_file + '.ranges';
            llnode_module.loadDump(inputData.split("=")[1], process.env._);
            // Display thread stacks in a table
            var threads = llnode_module.getThreadCount();
            //console.log(llnode_module.loadPlugin('llnode.so'));
            //console.log(llnode_module.nodeinfo());
            console.log(llnode_module.findjsobjects());

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
                        response.write(count++ + ': ' + frame + '\n');
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
        console.log("lldbdemo.js: [404] " + request.method + " to " + request.url);
    };
}

var http_server = http.createServer(my_listener);
http_server.listen(2000);

console.log('Diagnostic Javascript API - NPM Demo');
console.log('Usage: node lldbdemo.js [-timeout=<timeout in minutes>]');
console.log('Recommended Linux OS settings:\n\tulimit -c unlimited\n\t/proc/sys/kernel/core_pattern core.%p');
console.log('To create a core dump, run: node --abort-on-uncaught-exception unknown.js');
console.log('\nDemo running, open URL http://<machine>:2000/ or http://localhost:2000/');

// Set timeout if requested
process.argv.forEach(function (val, index, array) {
  if (val.substring(0,8) == '-timeout') {
    console.log('lldbdemo.js: setting timeout to ' + (val.split("=")[1]) + ' minute(s)');
    setTimeout(function() {
      console.log('Diagnostic Javascript API NPM Demo closing down....');
      process.exit(0); 
    }, val.split("=")[1] * 1000 * 60);
  }
});

