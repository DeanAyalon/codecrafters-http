export abstract class Request {
    static title(data: string) {
        const lines = data.toString().split('\r\n'),
            lineOne = lines[0].split(' '),
            method = lineOne[0], url = lineOne[1],
            path = url.split('/').filter(val => val.length > 0) // Sanitize path from empty sections
        return { method, url, path }
    }

    static headers(data: string) {
        const lines = data.toString().split('\r\n'),
            headerArray = lines.slice(1, lines.length - 2)
                .map(line => line.split(': '))
                // .reduce((acc, [key, value]) => ({ ...acc, [key]: value }), {})
        const headers = {}
        for (const [key, value] of headerArray) {
            if (headers[key]) {
                if (Array.isArray(headers[key])) headers[key].push(value)
                else headers[key] = [headers[key], value]
            } else headers[key] = value
        }
        return headers
    }
}