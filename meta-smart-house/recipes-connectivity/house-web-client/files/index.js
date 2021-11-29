// web server in express
const express = require('express');
const crypto = require("crypto")
const { exec } = require("child_process");
const fs = require('fs');
const app = express();
const port = 80;

const CAMERA_DIR = "/opt/smart-house-web/dist/assets/cam";

app.use(express.json()); //Used to parse JSON bodies
app.use(express.static('dist'))

const users = [
    { "username": "joseph", "password_sha256": "03ac674216f3e15c761ee1a5e255f067953623c8b388b4459e13f978d7c846f4" }
];

function verifyUserPassword(login, password) {
    let userData = users.find(u => u.username === login.trim());
    if (userData) {
        return userData.password_sha256 === crypto.createHash("sha256").update(password).digest("hex");
    }
    return false;
}

// authentication middleware
app.use((req, res, next) => {

    // parse login and password from headers
    const b64auth = (req.headers.authorization || '').split(' ')[1] || ''
    const [login, password] = Buffer.from(b64auth, 'base64').toString().split(':')

    // Verify login and password are set and correct
    if (login && password && verifyUserPassword(login, password)) {
        // Access granted...
        return next()
    }

    // Access denied...
    res.set('WWW-Authenticate', 'Basic realm="401"') // change this
    res.status(401).send('Authentication required.') // custom message
})



app.get('/gpio', (_req, res) => {
    exec("cat /dev/gpio-sensor", (err, stdout, stderr) => {
        if (err) console.error(err);
        if (stderr) console.error(stderr);
        res.send(stdout.trim());
    });
});

// convert unix timestamp to human readable date
function timestampToDate(timestamp) {
    let date = new Date(timestamp * 1000);
    return date.toLocaleString();
}

// convert filename (unix timestamp).jpg to human readable date
function filenameToDate(filename) {
    let timestamp = filename.split('.')[0];
    return timestampToDate(timestamp);
}

// list all files in /opt/smart-house-web/assets/cam directory as json
app.get('/cam', (_req, res) => {
    if (!fs.existsSync(CAMERA_DIR)) {
        fs.mkdirSync(CAMERA_DIR, { recursive: true });
    }
    fs.readdir(CAMERA_DIR, (err, files) => {
        if (err) {
            console.error(err);
            res.status(500).send(err);
        } else {
            // exclude .keep from files list
            files = files.filter(f => f.indexOf('.keep') === -1);
            // filter files created in the last 3 seconds
            files = files.filter(f => (Date.now() - fs.statSync(CAMERA_DIR + '/' + f).mtimeMs) > 3000);
            res.send(files.map((f) => {
                return {
                    name: f,
                    time: filenameToDate(f),
                    url: `/assets/cam/${f}`
                }
            }));
        }
    });
});



app.get('/hello', (req, res) => {
    res.send('Hello World!');
})

app.listen(port, () => {
    console.log(`Example app listening at http://0.0.0.0:${port}`);
})