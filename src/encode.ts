export function selectEncoding(acceptedEncodings: string | string[]) {
    // Synthesize Accepted-Encoding headers (may be multi-lined, or comma separated)
    if (!acceptedEncodings) return null
    if (typeof acceptedEncodings == 'string') acceptedEncodings = [acceptedEncodings] // Single string - transform to array
    const encodings = []
    // Each header line may be comma-separated; Remove spaces, lowercase
    acceptedEncodings.forEach(line => encodings.push(...(line.split(',').map(encoding => encoding.toLowerCase().replace(' ', '')))))

    console.log(encodings)

    // Supported encodings
    if (encodings.includes('gzip')) return 'gzip'
    // if (encodings.includes('deflate')) return 'deflate'
    // if (encodings.includes('br')) return 'br'
    // if (encodings.includes('identity')) return 'identity'

    return null
}