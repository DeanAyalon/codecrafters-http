import net from 'net'
import { argv } from 'process'

import { Request } from './http/request.js'
import { respond as _respond, Respond } from './http/response.js'
import { synthasizeArguments } from './args.js'

import { files } from './files.js'

const server = net.createServer((socket) => socket.on('close', () => socket.end()))
server.listen(4221, '0.0.0.0')
console.log('Listening on port 4221')

const options = synthasizeArguments(argv)

server.on('connection', socket => {
    socket.on('data', data => {
        const request = data.toString(),
            { method, url, path } = Request.title(request), // Parse first request line
            respond: Respond = (code: number | string, payload?) => _respond(socket, code, payload)

        console.log(socket.remoteAddress, method, url)

        switch(path[0]) {
            case undefined: respond(200, 'Welcome!'); break
            case 'echo': respond(200, path[1]); break
            case 'user-agent': respond(200, Request.headers(request)['User-Agent']); break
            case 'files': files(options.directory, request, respond); break
            default: respond(404, url + ' not found')
        }
    })
})