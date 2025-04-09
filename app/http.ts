import { STATUS_CODES } from 'http'

const version = 1.1

export class HttpResponse {
    constructor(private code: number | string, private message?: string) {}
    toString() {
        return [`HTTP/${version} ${this.code} ${STATUS_CODES[this.code]}`,
            'Content-Type: text/plain', 
            // Headers end
            , this.message, this.code].join('\r\n')
    }
}