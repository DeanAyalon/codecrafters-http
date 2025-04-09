import { Socket } from 'net'
import { STATUS_CODES } from 'http'
import { Buffer } from 'buffer'

const version = 1.1

export class Response {
    constructor(private code: number | string, private message?: string) {}
    /** The response message */
    toString() {
        return [`HTTP/${version} ${this.code} ${STATUS_CODES[this.code]}`,
            'Content-Type: text/plain', 
            'Content-Length: ' + this.length(),
            // Headers end
            , this.message].join('\r\n')
    }

    /** The Content-Length of the response message, in bytes */
    length() { return this.message ? Buffer.byteLength(this.message) : 0 }
}

export function  respond(socket: Socket, code: number | string, message?: string) {
    console.log('->', socket.remoteAddress, code, message)
    const response = new Response(code, message)
    console.log('  ', response.length(), 'bytes')
    socket.write(response.toString())
    socket.end()
}
