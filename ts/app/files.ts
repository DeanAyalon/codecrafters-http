import fs from 'fs'

import { Request } from './http/request'

export function files(directory: string, request: Request) {
    switch (request.method) {
        case 'GET':
            const fileName = request.path.slice(1), filePath = [directory, fileName].join('/')
            fs.existsSync(filePath) ?
                request.respond(200, fs.readFileSync(filePath))
                : request.respond(404, fileName + ' not found')
            break
        case 'POST': // Request body must be a string
            try {
                fs.writeFileSync([directory, request.path[1]].join('/'), request.body)
                request.respond(201, 'Created')
            } catch (e) {
                request.respond(500, (e as Error).message || 'An unknown error has occured')
            }
            break
        default: request.respond(401, request.method + ' requests are not supported')
    }
}

