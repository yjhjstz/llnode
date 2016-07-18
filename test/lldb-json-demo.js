const ll = require('../build/Release/llnode_module')

console.log(JSON.stringify(core(process.argv[2])))

function core(dump, opts) {
  opts = opts || {}

  ll.loadDump(dump, opts.bin || process.env._)

  if (opts.all) {
    return Array(ll.getThreadCount())
      .fill()
      .map(Number.call, Number)
      .map(stack)
  }
  // normal behaviour is to get stack for
  // the main thread, where all the JS happens
  return stack(opts.thread || 0)
}

function stack(thread) {
  return Array(ll.getFrameCount(thread))
    .fill()
    .map(Number.call, Number)
    .map(index => ll.getFrame(thread, index))
    .filter(frame => frame.substring(0, 3) !== '???')
}





