import net from 'net'

import { Request } from './http/request.js'
import { respond as _respond } from './http/response.js'

const server = net.createServer((socket) => socket.on('close', () => socket.end()))
server.listen(4221, '0.0.0.0')
console.log('Listening on port 4221')

server.on('connection', socket => {
    // console.log('Connection established', socket.remoteAddress)
    socket.on('data', data => {
        // console.log('Received data:', data.toString())
        const request = data.toString(),
            { method, url, path } = Request.title(request), // Parse first request line
            respond = (code: number | string, message?: string) => _respond(socket, code, message)

        console.log(socket.remoteAddress, method, url)

        switch(path[0]) {
            case undefined: respond(200, 'Welcome!'); break
            case 'echo': respond(200, path[1]); break
            case 'user-agent': respond(200, Request.headers(request)['User-Agent']); break
            default: respond(404, url + ' not found')
        }
    })
})