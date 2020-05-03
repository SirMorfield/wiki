const { spawn } = require('child_process')
const os = require('os')
const cpuCount = os.cpus().length

const fileName = 'cleanWiki.txt'
const fs = require('fs');
const wordListSize = (fs.statSync(fileName)).size

const blockSize = Math.floor(wordListSize / 8)
let start = 0
for (let i = 0; i < cpuCount; i++) {
	let end = start + blockSize
	if (i == cpuCount - 1) end += (wordListSize % blockSize)
	const c = spawn(`./main`, `${start} ${end} outputs/${i}.txt ${fileName} ${i}`.split(' '), { pwd: __dirname })
	// c.stdout.on('data', d => console.log(`${i}: ${d.toString().replace(/\n$/, '')}`))
	// c.stderr.on('data', d => console.error(`${i}: ${d.toString().replace(/\n$/, '')}`))
	c.on('close', code => console.log(`${i} close(${code})`))

	start += blockSize
}
