import net from 'net'
import fs from 'fs'
import { argv } from 'process'

import { Request } from './http/request.js'
import { respond as _respond } from './http/response.js'
import { synthasizeArguments } from './args.js'

const server = net.createServer((socket) => socket.on('close', () => socket.end()))
server.listen(4221, '0.0.0.0')
console.log('Listening on port 4221')

const options = synthasizeArguments(argv)

server.on('connection', socket => {
    // console.log('Connection established', socket.remoteAddress)
    socket.on('data', data => {
        // console.log('Received data:', data.toString())
        const request = data.toString(),
            { method, url, path } = Request.title(request), // Parse first request line
            respond = (code: number | string, payload?) => _respond(socket, code, payload)

        console.log(socket.remoteAddress, method, url)

        switch(path[0]) {
            case undefined: respond(200, 'Welcome!'); break
            case 'echo': respond(200, path[1]); break
            case 'user-agent': respond(200, Request.headers(request)['User-Agent']); break
            case 'files': 
                const fileName = path.slice(1), filePath = [options.directory, fileName].join('/')
                if (fs.existsSync(filePath)) respond(200, fs.readFileSync(filePath))
                else respond(404, fileName + ' file not found')
                break
            default: respond(404, url + ' not found')
        }
    })
})