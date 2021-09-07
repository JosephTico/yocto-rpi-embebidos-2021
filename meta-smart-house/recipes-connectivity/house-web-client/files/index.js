// web server in express
const express = require('express');
const { exec } = require("child_process");
const fs = require('fs');
const app = express();
const port = 80;

app.use(express.json()); //Used to parse JSON bodies
app.use(express.static('dist'))


// get doors
app.get('/doors', (_req, res) => {
    exec("gpio-client r 4", (err, stdout, stderr) => {
        if (err) throw err;
        if (stderr) throw stderr;
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


app.get('/hello', (req, res) => {
    res.send('Hello World!');
})

app.listen(port, () => {
    console.log(`Example app listening at http://localhost:${port}`);
})