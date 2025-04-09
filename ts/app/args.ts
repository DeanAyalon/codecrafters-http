import fs from 'fs'

function crash(...err) {
    console.error(...err)
    process.exit(1)
}

export function synthasizeArguments(argv: string[]) {
    // --directory
    const index = argv.indexOf('--directory')
    let directory: string
    if (index > -1) {
        directory = argv[index + 1]
        if (directory) {
            if (!fs.existsSync(directory)) crash('Directory does not exist:', directory)
            console.log('Directory set to', directory)
        } else crash('No directory specified after --directory')
    } else {
        console.log('No directory specified, using ./files')
        directory = './files'
    }

    return { directory }
}