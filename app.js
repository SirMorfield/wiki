const readline = require('linebyline')
const rl = readline('./wiki.xml', { maxLineLength: 2 ** 14 })
const fs = require('fs').promises

const start = process.hrtime()
rl.on('line', async (line, lineCount, byteCount) => {
	line = line.trim()

	if (line.match(/^\s*(\{|\}|\<|\||\*|\;|\[|File\:|bar\:)/)) return
	line = line.replace(/\&.*\;/ig, ' ')
	line = line.replace(/(\(|\{).*(\)|\}|\n)/g, '')
	line = line.replace(/'''/g, '')
	line = line.replace(/\=*/g, '')
	line = line.toLowerCase()
	if (line.length < 3) return

	await fs.appendFile('./cleanWiki.txt', line)

	if (lineCount % 100000 == 0) {
		console.log(`${lineCount}: ${lineCount / process.hrtime(start)[0]} lines/sec`)
	}
})

rl.on('error', console.error)
