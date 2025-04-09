import net from 'net'
import { argv } from 'process'

import { Request } from './http/request.js'
import { synthasizeArguments } from './args.js'

import { files } from './files.js'

const server = net.createServer((socket) => socket.on('close', () => socket.end()))
server.listen(4221, '0.0.0.0')
console.log('Listening on port 4221')

const options = synthasizeArguments(argv)

server.on('connection', socket => {
    socket.on('data', data => {
        const request = new Request(data, socket)
        console.log(socket.remoteAddress, request.method, request.url)

        switch(request.path[0]) {
            case undefined: request.respond(200, 'Welcome!'); break
            case 'echo': request.respond(200, request.path[1]); break
            case 'user-agent': request.respond(200, request.headers['User-Agent']); break
            case 'files': files(options.directory, request); break
            default: request.respond(404, request.url + ' not found')
        }
    })
})