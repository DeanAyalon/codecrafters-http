import fs from 'fs'

type Options = {
    directory?: string;
}

function crash(...err) {
    console.error(...err)
    process.exit(1)
}

export function synthasizeArguments(argv: string[]) {
    const options: Options = {}
    // --directory
    const index = argv.indexOf('--directory')
    if (index > -1) {
        options.directory = argv[index + 1]
        if (options.directory) {
            if (!fs.existsSync(options.directory)) crash('Directory does not exist:', options.directory)
            console.log('Directory set to', options.directory)
            argv[2] = options.directory
        } else crash('No directory specified after --directory')
    } else {
        console.log('No directory specified, using ./files')
        options.directory = './files'
    }

    return options
}