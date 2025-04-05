export function selectEncoding(encoding: string | string[]) {
    if (!encoding) return null
    if (typeof encoding == 'string') encoding = [encoding]

    const encodings = encoding.map(e => e.toLowerCase())
    if (encodings.includes('gzip')) return 'gzip'
    // if (encodings.includes('deflate')) return 'deflate'
    // if (encodings.includes('br')) return 'br'
    // if (encodings.includes('identity')) return 'identity'
    
    return null
}