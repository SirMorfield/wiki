const { spawn, execSync } = require('child_process')
const os = require('os')

execSync('rm c; gcc dictionary.c -o c')
let cpuCount = os.cpus().length
// cpuCount = 1


const fileName = 'data/cleanWiki.txt'
const fs = require('fs');
const wikiSize = (fs.statSync(fileName)).size

const blockSize = Math.floor(wikiSize / cpuCount)
let start = 0
for (let i = 0; i < cpuCount; i++) {
	let end = start + blockSize
	if (i == cpuCount - 1) end += (wikiSize % blockSize)
	const args = `${i} ${start} ${end} outputs/${i}.txt ${fileName} data/en.txt`.split(' ')
	const c = spawn(`./c`, args, { pwd: __dirname })
	console.log('./c', args.join(' '))
	c.stdout.on('data', d => console.log(`${i}:\n ${d.toString().replace(/\n$/, '')}`))
	c.stderr.on('data', d => console.error(`${i}:\n ${d.toString().replace(/\n$/, '')}`))
	c.on('close', code => console.log(`${i} close(${code})`))

	start += blockSize
}
