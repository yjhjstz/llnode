const http = require('http');
const buf0 = new Buffer([0])
const fs = require('fs');
const spawn = require('child_process').spawn;
const spawnSync = require('child_process').spawnSync;
const readline = require('readline');

if (process.argv.length < 3) {
  console.log('usage: node lldb-repl.js corefile');
  process.exit(1);
}

var env = Object.assign({}, process.env);

const core = process.argv[2];
const corerange = core + '.ranges';

var range = spawnSync('readelf2segments.py', [core]);
fs.writeFileSync(corerange, range.stdout);
// set env
env.LLNODE_RANGESFILE = corerange;

const lldb = spawn('lldb', ['-c', core], {
    cwd: process.cwd(),
    env: env });

var server = http.createServer(function (req, res) {
  res.setHeader('content-type', 'multipart/octet-stream')

  res.write('analyse coredump \r\n');
  req.pipe(lldb.stdin);
  
  var rl = readline.createInterface({
   input: req, output: res
  });
   rl.setPrompt('> ');
   rl.prompt();
   rl.on('line', (input) => {
      rl.prompt();
   })
   
   lldb.stdout.pipe(res);
  // log
  console.log(req.headers['user-agent'])

  // hack to thread stdin and stdout
  // simultaneously in curl's single thread
  var iv = setInterval(function () {
    res.write(buf0)
  }, 100)

  res.connection.on('end', function () {
    clearInterval(iv)
  })
})
server.listen(8000);
