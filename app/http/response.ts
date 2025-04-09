import { Socket } from 'net'
import { STATUS_CODES } from 'http'
import { Buffer } from 'buffer'

const version = 1.1,
    contentType = {
        text: 'text/plain',
        other: 'application/octet-stream'
    }

export class Response {
    constructor(private code: number | string, private payload?) {
        this._contentType = typeof payload === 'string' ? contentType.text : contentType.other
    }

    private _contentType: string
    get contentType() { return this._contentType }

    private _encoding?: string
    set encoding(encoding: string) { this._encoding = encoding }

    /** The Content-Length of the response message, in bytes */
    length() { return Buffer.byteLength(this.payload) }

    headers() {
        const headers = [`HTTP/${version} ${this.code} ${STATUS_CODES[this.code]}`,
            'Content-Type: ' + this._contentType,
            'Content-Length: ' + this.length()]
        if (this._encoding) headers.push('Content-Encoding: ' + this._encoding)

        headers.push('', '') // Header-body separator
        return headers.join('\r\n')
    }
    payloadBuffer() { 
        return Buffer.isBuffer(this.payload) ? this.payload : Buffer.from(this.payload)
    }
}

export type ResponseOptions = Partial<{ encoding: string }>
export type Respond = (code: number | string, payload?) => void
export function respond(socket: Socket, code: number | string, payload?, options?: ResponseOptions) {
    console.log('->', socket.remoteAddress, code, payload)
    const response = new Response(code, payload)
    if (options?.encoding) response.encoding = options.encoding
    
    const headers = response.headers(), body = response.payloadBuffer()
    console.log('  ', response.contentType, response.length(), 'bytes')

    socket.write(headers)
    socket.write(new Uint8Array(body))
    socket.end()
}
