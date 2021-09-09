// web server in express
const express = require('express');
const crypto = require("crypto")
const { exec } = require("child_process");
const fs = require('fs');
const app = express();
const port = 80;

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


// get doors
app.get('/doors', (_req, res) => {
    exec("gpio-client r 4", (err, stdout, stderr) => {
        if (err) console.error(err);
        if (stderr) console.error(stderr);
        res.send(stdout.trim().replace(/\D/g, '').split('').map((m) => m == "1"));

    });
});

// post leds
app.post('/leds', (req, res) => {
    // return 400 unless leds
    if (Object.keys(req.body).length === 0) {
        res.status(400).send('No leds JSON in body');
        return;
    }
    // extract leds from body
    const leds = req.body;
    leds.forEach((led, index) => {
        exec(`gpio-client ${led.on ? "n" : "f"} ${led.id}`);
    });
    res.sendStatus(200);
});


app.get('/camera', (_req, res) => {
    exec("fswebcam /opt/house-smart-web/dist/assets/camera.jpg", (err, stdout, stderr) => {
        if (err) console.error(err);
        res.redirect('/assets/camera.jpg')
    });
});


app.get('/hello', (req, res) => {
    res.send('Hello World!');
})

app.listen(port, () => {
    console.log(`Example app listening at http://0.0.0.0:${port}`);
})