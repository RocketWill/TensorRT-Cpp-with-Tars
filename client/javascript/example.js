var fs = require('fs');
var path = require('path');
var request = require('request');


function base64Encode(file) {
    const bitmap = fs.readFileSync(file);
    return bitmap.toString('base64');
}

function sendReqest(url, data, method, callback) {
    return new Promise((res, rej) => {
        request({
            url,
            method,
            json: true,
            body: data
        }, (err, resp, body) => {
            if (err) {
                rej(err);
                return console.error(err);
            }
            callback && callback(body);
            res(body);
        });
    })
}

function main() {
    const URL = 'http://127.0.0.1:30002/';
    const currentDir = path.dirname(__dirname);
    const imageFile = path.join(currentDir, 'test-image.jpeg');
    const base64str = base64Encode(imageFile);

    sendReqest(URL, {data: base64str}, 'POST')
        .then(respBody => console.log(respBody))
}

if (require.main === module) {
    main();
}