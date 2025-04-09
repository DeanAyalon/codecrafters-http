import net from 'net'

import { Response } from './response'
import { encodingSelection } from '../encode'

const crlf = '\r\n'

export class Request {
    method: string  // GET/POST currently supported
    url: string
    path: string[]
    version: string // Only HTTP 1.1 implemented

    private header: string
    headers: { [header: string]: string | string[] } // Not many supported yet
    body: string    // Only strings supported currently

    constructor(data: Buffer, private socket: net.Socket) {
        // Title
        [this.header, this.body] = data.toString().split(crlf + crlf)
        const lines = this.header.toString().split(crlf);
        [this.method, this.url, this.version] = lines[0].split(' ')
        // this.method = lineOne[0], this.url = lineOne[1],
        this.path = this.url.split('/').filter(val => val.length > 0) // Sanitize path from empty sections

        // Headers
        const headers = lines.slice(1).map(line => line.split(': '))
        this.headers = {}
        for (const [key, value] of headers)
            if (this.headers[key])  // Value exists, convert/insert to array
                if (Array.isArray(this.headers[key])) this.headers[key].push(value)
                else this.headers[key] = [this.headers[key], value]
            else this.headers[key] = value
    }

    respond(code: number | string, payload) {
        console.log('->', this.socket.remoteAddress, code, payload)
        const response = new Response(code, payload)

        const encoding = encodingSelection(this.headers['Accept-Encoding'])
        if (encoding) response.encoding = encoding

        const headers = response.headers(), body = response.payloadBuffer()
        console.log('  ', response.contentType, response.length(), 'bytes')

        this.socket.write(headers)
        this.socket.write(new Uint8Array(body))
        this.socket.end()
    }
}