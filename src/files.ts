import fs from 'fs'

import { Respond } from './http/response'
import { Request } from './http/request.js'

export function files(directory: string, request: string, respond: Respond) {
    const { method, path } = Request.title(request) // Parse first request line
    if (method === 'GET') {
        const fileName = path.slice(1), filePath = [directory, fileName].join('/')
        fs.existsSync(filePath) ? 
            respond(200, fs.readFileSync(filePath)) 
            : respond(404, fileName + ' not found')
    } else if (method === 'POST') {
        try { 
            fs.writeFileSync([directory, path[1]].join('/'), request.split('\r\n\r\n')[1]) 
            respond(201, 'Created')
        } catch (e) { respond(500, e.toString()) }
    }
}

