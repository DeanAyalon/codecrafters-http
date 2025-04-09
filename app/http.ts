import { STATUS_CODES } from 'http'
import { Buffer } from 'buffer'

const version = 1.1

export class HttpResponse {
    constructor(private code: number | string, private message?: string) {}
    toString() {
        return [`HTTP/${version} ${this.code} ${STATUS_CODES[this.code]}`,
            'Content-Type: text/plain', 
            'Content-Length: ' + this.length(),
            // Headers end
            , this.message].join('\r\n')
    }

    length() { return this.message ? Buffer.byteLength(this.message) : 0 }
}