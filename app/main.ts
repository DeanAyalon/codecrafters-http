import net from 'net'

const server = net.createServer((socket) => 
    socket.on('close', () => socket.end())
)

server.listen(4221, '0.0.0.0')
console.log('Listening on port 4221')

server.on('connection', (socket) => {
    console.log('Connection established', socket.remoteAddress)
    socket.write('HTTP/1.1 200 OK\r\n')
    socket.write('Content-Type: text/plain\r\n')
    socket.write('\r\n') // Headers end
    socket.write('Successfully returned 200!\r\n')
    socket.end()
})