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
            headers = lines.slice(1, lines.length - 2)
                .map(line => line.split(': '))
                .reduce((acc, [key, value]) => ({ ...acc, [key]: value }), {})
        return headers
    }
}