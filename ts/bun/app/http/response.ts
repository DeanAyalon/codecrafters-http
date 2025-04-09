import { Socket } from 'net'
import { STATUS_CODES } from 'http'
import { Buffer } from 'buffer'

const version = 1.1,
    contentType = {
        text: 'text/plain',
        other: 'application/octet-stream'
    }

export class Response {
    private _contentType: string
    get contentType() { return this._contentType }

    constructor(private code: number | string, private payload?) {
        if (typeof payload === 'string') this._contentType = contentType.text
        else this._contentType = contentType.other
    }

    /** The Content-Length of the response message, in bytes */
    length() { return Buffer.byteLength(this.payload) }

    headers() {
        return [`HTTP/${version} ${this.code} ${STATUS_CODES[this.code]}`,
            'Content-Type: ' + this._contentType,
            'Content-Length: ' + this.length(),
            '', // header-body separator
            ''].join('\r\n')
    }
    payloadBuffer() { 
        return Buffer.isBuffer(this.payload) ? this.payload : Buffer.from(this.payload)
    }
}

export type Respond = (code: number | string, payload?) => void
export function respond(socket: Socket, code: number | string, payload?) {
    console.log('->', socket.remoteAddress, code, payload)
    // console.log('->', socket.remoteAddress, code, (typeof payload === 'string' ? payload : '<binary>'))
    const response = new Response(code, payload),
        headers = response.headers(), body = response.payloadBuffer()
    console.log('  ', response.contentType, response.length(), 'bytes')

    socket.write(headers)
    socket.write(new Uint8Array(body))
    socket.end()
}
