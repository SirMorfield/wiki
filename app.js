const readline = require('linebyline')
const rl = readline('./wiki.xml', { maxLineLength: 2 ** 14 })
const fs = require('fs').promises

const start = process.hrtime()
rl.on('line', async (line, lineCount, byteCount) => {
	line = line.trim()

	if (line[0] == '{' || line[0] == '<' || line[0] == '|' || line[0] == '*') return
	await fs.appendFile('./cleanWiki.txt', line)

	if (lineCount % 100000 == 0) {
		console.log(`${lineCount}: ${lineCount / process.hrtime(start)[0]} lines/sec`)
	}
})
rl.on('error', (err) => {
	console.error(err)
})
rl.on('end', () => {
	console.log(data)
	fs.writeFile('./result.json', JSON.stringify(data))
})
