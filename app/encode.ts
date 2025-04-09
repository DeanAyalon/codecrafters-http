export function selectEncoding(acceptedEncodings: string | string[]) {
    // Synthesize Accepted-Encoding headers (may be multi-lined, or comma separated)
    if (!acceptedEncodings) return null
    if (typeof acceptedEncodings == 'string') acceptedEncodings = [acceptedEncodings] // Single string - transform to array
    const encodings: string[] = []
    // Each header line may be comma-separated; Remove spaces, lowercase
    acceptedEncodings.forEach(line => encodings.push(...(line.split(',').map(encoding => encoding.toLowerCase().replace(' ', '')))))

    // Supported encodings
    if (encodings.includes('gzip')) return 'gzip'

    return null
}