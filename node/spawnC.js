const { spawn, execSync } = require('child_process')

execSync('rm c; gcc letters.c -o c')
const os = require('os')
// const cpuCount = os.cpus().length
const cpuCount = 1


const fileName = 'cleanWiki.txt'
const fs = require('fs');
const wordListSize = (fs.statSync(fileName)).size
console.log('wordListSize', wordListSize)

const blockSize = Math.floor(wordListSize / cpuCount)
let start = 0
for (let i = 0; i < cpuCount; i++) {
	let end = start + blockSize
	if (i == cpuCount - 1) end += (wordListSize % blockSize)
	const args = `${i} ${start} ${end} outputs/${i}.txt ${fileName}`.split(' ')
	const c = spawn(`./c`, args, { pwd: __dirname })
	console.log('start', args)
	c.stdout.on('data', d => console.log(`${i}:\n ${d.toString().replace(/\n$/, '')}`))
	c.stderr.on('data', d => console.error(`${i}:\n ${d.toString().replace(/\n$/, '')}`))
	c.on('close', code => console.log(`${i} close(${code})`))

	start += blockSize
}
