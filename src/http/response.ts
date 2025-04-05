import { STATUS_CODES } from 'http'
import { Buffer } from 'buffer'

import { encode } from '../encode.js'

const version = 1.1,
    contentType = {
        text: 'text/plain',
        other: 'application/octet-stream'
    }

export class Response {
    constructor(private code: number | string, private payload?) {
        if (typeof payload === 'string') this._contentType = contentType.text
        else this._contentType = contentType.other
    }

    private _contentType: string
    get contentType() { return this._contentType }

    private _encoding: string
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
        let buffer = Buffer.isBuffer(this.payload) ? this.payload : Buffer.from(this.payload)
        if (this._encoding) buffer = encode(buffer, this._encoding)
        return buffer    
    }
}
