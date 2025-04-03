import net from 'net'
import { HttpResponse } from './http.js'

const server = net.createServer((socket) => 
    socket.on('close', () => socket.end())
)

server.listen(4221, '0.0.0.0')
console.log('Listening on port 4221')

server.on('connection', socket => {
    function respond(code: number | string, message: string) {
        console.log('->', socket.remoteAddress, code, message)
        socket.write(new HttpResponse(code, message).toString())
        socket.end()
    }

    // console.log('Connection established', socket.remoteAddress)
    socket.on('data', data => {
        // console.log('Received data:', data.toString())
        const request = data.toString(),
            reqWords = request.split(' '),
            method = reqWords[0], url = reqWords[1],
            path = url.split('/').filter(val => val.length > 0) // Sanitize path from empty sections
        console.log(socket.remoteAddress, method, url)

        respond(path.length ? 404 : 200, url)
    })
})